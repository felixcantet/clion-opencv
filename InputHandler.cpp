
#include "InputHandler.h"
#include "HandDetection.h"

void InputHandler::LeftClick() {
    INPUT Input = {0};
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void InputHandler::RightClick() {
    INPUT Input = {0};
    // right down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // right up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void InputHandler::MoveCursor(const cv::Point &dir) {
    POINT p;
    if (GetCursorPos(&p))
    {
        p.x += dir.x ;
        p.y += dir.y ;

        SetCursorPos(p.x, p.y);
    }
}

void InputHandler::PerformInput(const HandDetection& hd) {

    // Check si y'a eu un changement
    bool is_equal = true;
    for (int i = 0; i < 5; ++i) {
//        if(previous_finger[i] != current_finger[i])
//        {
//            is_equal = false;
//            break;
//        }
    }

    InputHandler::MoveCursor(hd.GetHandOffset());

    // Si equal ==> return
    if(is_equal)
        return;

    // Perform click
}
