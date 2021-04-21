
#ifndef COMPUTERVISION_HANDDETECTION_H
#define COMPUTERVISION_HANDDETECTION_H

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

class HandDetection {
private:
    const int POSE_PAIRS[20][2] =
            {
                    {0,1}, {1,2}, {2,3}, {3,4},         // thumb
                    {0,5}, {5,6}, {6,7}, {7,8},         // index
                    {0,9}, {9,10}, {10,11}, {11,12},    // middle
                    {0,13}, {13,14}, {14,15}, {15,16},  // ring
                    {0,17}, {17,18}, {18,19}, {19,20}   // small
            };

    const std::string protoFile = "../Hand/pose_deploy.prototxt";
    const std::string weightsFile = "../Hand/pose_iter_102000.caffemodel";

    const int forward_every_n_frame = 2;
    int current_frame = 0;

    cv::dnn::Net net;

    int nPairs = sizeof(POSE_PAIRS)/sizeof(POSE_PAIRS[0]);

    const int nPoints = 22;
    std::vector<cv::Point> points;

    cv::Mat output;

public:
    HandDetection();

    void Initialize();
    void DetectHand(const cv::Mat& frame, int inWidth, int inHeight, int width, int heigth);
    void DrawHand(cv::Mat& frame);

};


#endif //COMPUTERVISION_HANDDETECTION_H
