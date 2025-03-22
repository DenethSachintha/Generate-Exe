from flask import Flask, request, jsonify, send_file, render_template
import subprocess
import os
import uuid
import shutil
from werkzeug.utils import secure_filename
import ctypes
import requests

app = Flask(__name__)

# Temporary directory to store scripts and executables
TEMP_DIR = "temp_files"
UPLOAD_FOLDER = "uploads"
COMPILED_DIR = "compiled"
ALLOWED_EXTENSIONS = {"c"}
os.makedirs(TEMP_DIR, exist_ok=True)
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(COMPILED_DIR, exist_ok=True)
TEMPLATE_PATH = "template.py"

def allowed_file(filename):
    return "." in filename and filename.rsplit(".", 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route("/upload", methods=["POST"])
def upload_and_compile():
    clear_directory(COMPILED_DIR)
    if "file" not in request.files:
        return jsonify({"error": "No file uploaded"}), 400

    file = request.files["file"]
    
    if file.filename == "":
        return jsonify({"error": "No selected file"}), 400

    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        c_filepath = os.path.join(UPLOAD_FOLDER, filename)
        file.save(c_filepath)

        output_name = os.path.splitext(filename)[0]  # Remove .c extension
        output_dll = os.path.join(COMPILED_DIR, f"{output_name}.dll")
        output_so = os.path.join(COMPILED_DIR, f"{output_name}.so")

        # Determine platform and compile accordingly
        if os.name == "nt":  # Windows
            compile_command = f"gcc -shared -o {output_dll} -Wl,--out-implib,{COMPILED_DIR}/{output_name}.a -Wl,--export-all-symbols {c_filepath}"
            compiled_file = output_dll
        else:  # Linux/macOS
            compile_command = f"gcc -shared -o {output_so} -fPIC {c_filepath}"
            compiled_file = output_so

        # Run the compilation command
        result = subprocess.run(compile_command, shell=True, capture_output=True, text=True)

        if result.returncode == 0:
            return jsonify({"message": "Compilation successful", "output": compiled_file})
        else:
            return jsonify({"error": result.stderr}), 400

    return jsonify({"error": "Invalid file format"}), 400

@app.route("/download/<filename>", methods=["GET"])
def download_file(filename):
    clear_directory(UPLOAD_FOLDER)
    filepath = os.path.join(COMPILED_DIR, filename)
    if os.path.exists(filepath):
        return send_file(filepath, as_attachment=True)
    else:
        return jsonify({"error": "File not found"}), 404


def generate_script_from_template(user_inputs):
    """Generate a Python script from the template and user inputs."""
    with open(TEMPLATE_PATH, 'r') as f:
        template_content = f.read()

    # Replace placeholders with user inputs
    script_content = template_content.replace("{{ user_inputs }}", user_inputs)
    return script_content
@app.route('/generate_exe', methods=['POST'])
def generate_exe():
    # Clear the TEMP_DIR before generating a new executable
    clear_directory(TEMP_DIR)
    clear_directory(UPLOAD_FOLDER)

    # Get user inputs from the request
    user_inputs = request.json.get('inputs')
    if not user_inputs:
        return jsonify({"error": "No inputs provided"}), 400

    # Generate a unique ID for this request
    request_id = str(uuid.uuid4())
    script_path = os.path.join(TEMP_DIR, f"{request_id}.py")
    exe_path = os.path.join(TEMP_DIR, f"{request_id}.exe")

    # Step 1: Generate a Python script from the template
    script_content = generate_script_from_template(user_inputs)
    with open(script_path, 'w') as f:
        f.write(script_content)

    # Step 2: Use PyInstaller to generate the executable
    try:
        subprocess.run([
            'pyinstaller',
            '--onefile',
            '--add-data', f"{os.pardir}{os.sep}{COMPILED_DIR}{os.pathsep}compiled",  # Correctly specify source and destination
            '--distpath', TEMP_DIR,
            '--workpath', TEMP_DIR,
            '--specpath', TEMP_DIR,
            script_path
        ], check=True)
    except subprocess.CalledProcessError as e:
        return jsonify({"error": f"Failed to generate executable: {e}"}), 500

    # Step 3: Provide the executable as a download link
    return send_file(exe_path, as_attachment=True)
def clear_directory(dir_name): 
    """Clear all files and subdirectories in the specified directory."""
    if not os.path.exists(dir_name):
        print(f"Directory '{dir_name}' does not exist.")
        return
    for filename in os.listdir(dir_name):
        file_path = os.path.join(dir_name, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)  # Delete file or symbolic link
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)  # Delete subdirectory and contents
        except Exception as e:
            print(f"Failed to delete {file_path}. Reason: {e}")

@app.route('/fetch-data', methods=['GET'])
def fetch_data():
    api_url = "http://localhost:8081/api/v1/customers/find-by-id/986494c8-0c3f-40be-9a83-e85c59100d2a"
    
    try:
        response = requests.get(api_url)
        response.raise_for_status()  # Raise an error for bad status codes (4xx, 5xx)

        data = response.json()

        # Store API response in a file
        with open("customer_data.json", "w") as file:
            file.write(response.text)

        return jsonify({"message": "Data fetched and stored successfully!", "data": data})

    except requests.exceptions.RequestException as e:
        return jsonify({"error": str(e)}), 500
@app.route('/')
def home():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)