
#include "InputHandler.h"
#include "HandDetection.h"
#include <Windows.h>


void InputHandler::MoveCursor(const cv::Point &dir) {
    POINT p;
    if (GetCursorPos(&p))
    {
        p.x += dir.x ;
        p.y += dir.y ;

        SetCursorPos(p.x, p.y);
    }
}

void InputHandler::SetCursorPosition(const cv::Point &pos) {
    SetCursorPos(pos.x, pos.y);
}

void InputHandler::PerformInput(const HandDetection& hd) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    float ratio = (float)x/(float)y;

    // Check si y'a eu un changement
    bool is_equal = hd.CheckFingerStateChanged();

    InputType type = GetInputTypeFromHand(hd);
    if(type == InputType::MOVE)
        //InputHandler::SetCursorPosition(hd.GetHandPosition() * ratio);
        InputHandler::MoveCursor(hd.GetHandOffset() * ratio);

    // Si equal ==> return
    if(is_equal) {
        return;
    }

    // Perform click
    switch (type) {
        case NONE:
        case MOVE:
            break;

        case LEFT_CLICK:
            //InputHandler::LeftClick();
            break;

        case RIGHT_CLICK:
            //InputHandler::RightClick();
            break;

        case QUIT:
            InputHandler::EscapeKey();
            break;
    }
}

InputType InputHandler::GetInputTypeFromHand(const HandDetection &hd) {

    int finger_count = hd.GetFingerCount();

    if(hd.GetFingerOpened(FingerType::INDEX) && finger_count == 2)
        return InputType::MOVE;

    if(finger_count >= 5)
        return InputType::LEFT_CLICK;

    if(finger_count == 0)
        return InputType::RIGHT_CLICK;

    //if(finger_count == 1 && hd.GetFingerOpened(FingerType::THUMB))
    //    return InputType::QUIT;

    return InputType::NONE;
}


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

void InputHandler::EscapeKey() {
    INPUT Input = {0};
    // right down
    Input.type = INPUT_KEYBOARD;
    Input.ki.wVk = 27;
    Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
    ::SendInput(1, &Input, sizeof(INPUT));

    // right up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_KEYBOARD;
    Input.ki.dwFlags = KEYEVENTF_KEYUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}


