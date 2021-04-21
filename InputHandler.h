#ifndef COMPUTERVISION_INPUTHANDLER_H
#define COMPUTERVISION_INPUTHANDLER_H

#include <opencv2/core/types.hpp>

class HandDetection;

enum InputType{
    NONE,
    MOVE,
    LEFT_CLICK,
    RIGHT_CLICK,
    QUIT
};

struct InputHandler {

    static void MoveCursor(const cv::Point& dir);
    static void PerformInput(const HandDetection& hd);//const bool* previous_finger, const bool* current_finger);

private:
    static void LeftClick();
    static void RightClick();

    static void EscapeKey();

    static InputType GetInputTypeFromHand(const HandDetection& hd);
};


#endif //COMPUTERVISION_INPUTHANDLER_H
