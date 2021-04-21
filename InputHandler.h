#ifndef COMPUTERVISION_INPUTHANDLER_H
#define COMPUTERVISION_INPUTHANDLER_H

#include <Windows.h>
#include <opencv2/core/types.hpp>

class HandDetection;
struct InputHandler {

    static void MoveCursor(const cv::Point& dir);

    static void PerformInput(const HandDetection& hd);//const bool* previous_finger, const bool* current_finger);

private:
    static void LeftClick();
    static void RightClick();
};


#endif //COMPUTERVISION_INPUTHANDLER_H
