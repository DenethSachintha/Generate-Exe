from flask import Flask, request, jsonify, send_file, render_template
import subprocess
import os
import uuid
import shutil

app = Flask(__name__)

# Temporary directory to store scripts and executables
TEMP_DIR = "temp_files"
os.makedirs(TEMP_DIR, exist_ok=True)

def clear_temp_dir():
    """Clear all files in the TEMP_DIR."""
    for filename in os.listdir(TEMP_DIR):
        file_path = os.path.join(TEMP_DIR, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            print(f"Failed to delete {file_path}. Reason: {e}")

@app.route('/generate_exe', methods=['POST'])
def generate_exe():
    # Clear the TEMP_DIR before generating a new executable
    clear_temp_dir()

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

@app.route('/')
def home():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)