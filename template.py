#template.py
import os
import sys
import shutil
import ctypes

def is_admin():
    """Check if the script is running as administrator."""
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False

def extract_files():
    """Extract files bundled with the executable to C:\Windows\System32\test."""
    try:
        # Path to the destination directory
        dest_dir = r"C:\Windows\System32\test"
        os.makedirs(dest_dir, exist_ok=True)

        # Path to the compiled directory (bundled with the executable)
        compiled_dir = os.path.join(sys._MEIPASS, "compiled")

        # Copy files from compiled_dir to dest_dir
        for filename in os.listdir(compiled_dir):
            src_file = os.path.join(compiled_dir, filename)
            dest_file = os.path.join(dest_dir, filename)
            shutil.copy(src_file, dest_file)
            print(f"Copied {filename} to {dest_dir}")

    except Exception as e:
        print(f"Error extracting files: {e}")

def main():
    # Ensure the script runs as administrator
    if not is_admin():
        print("This script requires administrator privileges. Restarting with elevated permissions...")
        ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
        sys.exit()

    # Extract files to C:\Windows\System32\test
    extract_files()

if __name__ == "__main__":
    main()