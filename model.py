# model.py
import os
import shutil
import subprocess
from flask import Flask, request, jsonify, send_file, render_template
from werkzeug.utils import secure_filename

UPLOAD_FOLDER = "uploads"
COMPILED_DIR = "compiled"
ALLOWED_EXTENSIONS = {"c"}

# Create required directories
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(COMPILED_DIR, exist_ok=True)

def allowed_file(filename):
    return "." in filename and filename.rsplit(".", 1)[1].lower() in ALLOWED_EXTENSIONS

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

        if os.name == "nt":  # Windows
            compile_command = f"gcc -shared -o {output_dll} -Wl,--out-implib,{COMPILED_DIR}/{output_name}.a -Wl,--export-all-symbols {c_filepath}"
            compiled_file = output_dll
        else:  # Linux/macOS
            compile_command = f"gcc -shared -o {output_so} -fPIC {c_filepath}"
            compiled_file = output_so

        result = subprocess.run(compile_command, shell=True, capture_output=True, text=True)

        if result.returncode == 0:
            return jsonify({"message": "Compilation successful", "output": compiled_file})
        else:
            return jsonify({"error": result.stderr}), 400

    return jsonify({"error": "Invalid file format"}), 400

def download_file_handler(filename):
    UPLOAD_FOLDER = "uploads"
    COMPILED_DIR = "compiled"

    clear_directory(UPLOAD_FOLDER)  # ✅ just use it, no import
    filepath = os.path.join(COMPILED_DIR, filename)

    if os.path.exists(filepath):
        return send_file(filepath, as_attachment=True)
    else:
        print(f"Directory '{filepath}' does not exist.")
        return jsonify({"error": "File not found"}), 404

    
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
