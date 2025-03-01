#include <windows.h>

__declspec(dllexport) void CALLBACK hello(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    MessageBox(NULL, "Hello from DLL!", "DLL Message", MB_OK);
}