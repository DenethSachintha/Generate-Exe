#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
void press(BOOL shift, BOOL alt, BOOL ctrl, WORD key,BOOL backspace) {
    INPUT input[8] = {0};
    int index = 0;

    if (backspace) {
        // Press Backspace key
        input[index].type = INPUT_KEYBOARD;
        input[index].ki.wVk = VK_BACK;
        index++;

        // Release Backspace key
        input[index].type = INPUT_KEYBOARD;
        input[index].ki.wVk = VK_BACK;
        input[index].ki.dwFlags = KEYEVENTF_KEYUP;
        index++;
    }
    // Set up inputs for modifier keys and the main key
    if (ctrl) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_CONTROL};
    }
    if (alt) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_MENU};
    }
    if (shift) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_SHIFT};
    }

    // Press and release the main key
    input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = key};
    printf("shift :%d\n",GetAsyncKeyState(VK_SHIFT) & 0x8000);
    printf("win :%d\n",GetAsyncKeyState(VK_LWIN) & 0x8000);
    printf("F :%d\n",GetAsyncKeyState('F') & 0x8000);
    input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = key, .ki.dwFlags = KEYEVENTF_KEYUP};

    // Release modifier keys
    if (shift) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_SHIFT, .ki.dwFlags = KEYEVENTF_KEYUP};
    }
    if (alt) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_MENU, .ki.dwFlags = KEYEVENTF_KEYUP};
    }
    if (ctrl) {
        input[index++] = (INPUT){INPUT_KEYBOARD, .ki.wVk = VK_CONTROL, .ki.dwFlags = KEYEVENTF_KEYUP};
    }
    // Send the inputs
    SendInput(index, input, sizeof(INPUT));
}

int main() {
    printf("worked \n");

    BOOL shiftPressed, altPressed, ctrlPressed, winPressed;

    while (1) {
        //ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
        shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x8000;
        //altPressed = GetAsyncKeyState(VK_MENU) & 0x8000;
        winPressed = GetAsyncKeyState(VK_LWIN) & 0x8000; // Left Windows key


        if (winPressed && shiftPressed && GetAsyncKeyState('F') & 0x8000) {
            while ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_LWIN) & 0x8000)) {
                Sleep(50);
            }
                press(false, false, false, 'S', true);

                //simple s  =  win+shift+f
                //capital s =  win+shift+f+shift or win+ shift+ f then release win
                //shift + s =  win+shift+f then release win
                //win + s   =  win+shift+f then release shift
                Sleep(400);
        }
        Sleep(20);
    }
    return 0;
}
