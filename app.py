from flask import Flask, request, jsonify, send_file, render_template
import subprocess
import os
import uuid
import shutil
from werkzeug.utils import secure_filename
import ctypes


app = Flask(__name__)

# Temporary directory to store scripts and executables
TEMP_DIR = "temp_files"
UPLOAD_FOLDER = "uploads"
COMPILED_DIR = "compiled"
ALLOWED_EXTENSIONS = {"c"}
os.makedirs(TEMP_DIR, exist_ok=True)
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(COMPILED_DIR, exist_ok=True)

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
    filepath = os.path.join(COMPILED_DIR, filename)
    if os.path.exists(filepath):
        return send_file(filepath, as_attachment=True)
    else:
        return jsonify({"error": "File not found"}), 404

@app.route('/generate_exe', methods=['POST'])
def generate_exe():
    # Clear the TEMP_DIR before generating a new executable
    clear_directory(TEMP_DIR)

    # Get user inputs from the request
    user_inputs = request.json.get('inputs')
    if not user_inputs:
        return jsonify({"error": "No inputs provided"}), 400

    # Generate a unique ID for this request
    request_id = str(uuid.uuid4())
    script_path = os.path.join(TEMP_DIR, f"{request_id}.py")
    exe_path = os.path.join(TEMP_DIR, f"{request_id}.exe")

    # Step 1: Generate a Python script based on user inputs
    script_content = f"""
# This is a dynamically generated script
print("User inputs: {user_inputs}")
# Add more logic here based on user inputs
"""
    with open(script_path, 'w') as f:
        f.write(script_content)

    # Step 2: Use PyInstaller to generate the executable
    try:
        subprocess.run(['pyinstaller', '--onefile', '--distpath', TEMP_DIR, '--workpath', TEMP_DIR, '--specpath', TEMP_DIR, script_path], check=True)
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
@app.route('/')
def home():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)