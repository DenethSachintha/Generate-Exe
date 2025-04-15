#app.py
from flask import Flask, request, jsonify, send_file, render_template
from model import clear_directory, upload_and_compile, download_file_handler
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
LAYOUT_DATA_DIR = "layouts"
ALLOWED_EXTENSIONS = {"c"}
os.makedirs(TEMP_DIR, exist_ok=True)
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(COMPILED_DIR, exist_ok=True)
os.makedirs(LAYOUT_DATA_DIR, exist_ok=True)
TEMPLATE_PATH = "template.py"

@app.route("/upload", methods=["POST"])
def handle_upload():
    return upload_and_compile()

@app.route("/download/<filename>", methods=["GET"])
def handle_download(filename):
    return download_file_handler(filename)

#fetch data from database and stores in json file
@app.route('/fetch-data/<_id>', methods=['GET'])
def fetch_data(_id):
    api_url = f"http://localhost:8081/api/v1/customers/find-by-id/{_id}"
    
    try:
        response = requests.get(api_url)
        response.raise_for_status()  # Raise error for bad status codes

        data = response.json()

        # Define the file path inside 'layouts' directory
        file_path = os.path.join(LAYOUT_DATA_DIR, f"layout_data-{_id}.json")

        # Save API response to the file
        with open(file_path, "w") as file:
            file.write(response.text)

        return jsonify({"message": f"Data saved in {file_path}", "data": data})

    except requests.exceptions.RequestException as e:
        return jsonify({"error": str(e)}), 500

#use templet.py for get code 
def generate_script_from_template(user_inputs):
    """Generate a Python script from the template and user inputs."""
    with open(TEMPLATE_PATH, 'r') as f:
        template_content = f.read()

    # Replace placeholders with user inputs
    script_content = template_content.replace("{{ user_inputs }}", user_inputs)
    return script_content
#generate EXE file 
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
        return jsonify({"error": f"Failed to generate exe=cutable: {e}"}), 500

    # Step 3: Provide the executable as a download link
    return send_file(exe_path, as_attachment=True)
@app.route('/')
def home():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)