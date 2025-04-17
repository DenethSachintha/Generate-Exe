# template.py
import os
import sys
import shutil
import ctypes
import winreg

def is_admin():
    """Check if the script is running as administrator."""
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False

def extract_files():
    """Extract files bundled with the executable to C:\\Windows\\System32\\test."""
    try:
        dest_dir = r"C:\\Windows\\System32\\test"
        os.makedirs(dest_dir, exist_ok=True)
        compiled_dir = os.path.join(sys._MEIPASS, "compiled")
        for filename in os.listdir(compiled_dir):
            src_file = os.path.join(compiled_dir, filename)
            dest_file = os.path.join(dest_dir, filename)
            shutil.copy(src_file, dest_file)
            print(f"Copied {filename} to {dest_dir}")
    except Exception as e:
        print(f"Error extracting files: {e}")

def create_keyboard_layout_registry():
    """Create registry key for custom keyboard layout."""
    try:
        custom_layout_key = "{{ layout_key }}"
        full_key_path = fr"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\{custom_layout_key}"

        with winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, full_key_path) as key:
            winreg.SetValueEx(key, "Layout Display Name", 0, winreg.REG_SZ, r"{{ layout_display_name }}")
            winreg.SetValueEx(key, "Layout File", 0, winreg.REG_SZ, "{{ layout_file }}")
            winreg.SetValueEx(key, "Layout Id", 0, winreg.REG_SZ, "{{ layout_id }}")
            winreg.SetValueEx(key, "Layout Text", 0, winreg.REG_SZ, "{{ layout_text }}")

        print(f"Registry key {full_key_path} created successfully.")

    except Exception as e:
        print(f"Error creating registry key: {e}")

def main():
    if not is_admin():
        print("This script requires administrator privileges. Restarting with elevated permissions...")
        ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
        sys.exit()

    extract_files()
    create_keyboard_layout_registry()

if __name__ == "__main__":
    main()