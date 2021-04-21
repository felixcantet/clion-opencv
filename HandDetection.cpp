
#include "HandDetection.h"

HandDetection::HandDetection() {
    points = std::vector<cv::Point>(nPoints);
    current_frame = 0;

    net = cv::dnn::readNetFromCaffe(protoFile, weightsFile);

    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
}

void HandDetection::DetectHand(const cv::Mat &frame, int inWidth, int inHeight, int frameWidth, int frameHeight) {
    if(current_frame % forward_every_n_frame == 0) {

        cv::Mat frameCopy = frame.clone();

        cv::Mat inpBlob = cv::dnn::blobFromImage(frameCopy, 1.0f/255.0f, cv::Size(inWidth, inHeight), cv::Scalar(0, 0, 0), false, false);
        net.setInput(inpBlob);
        output = net.forward();

        int H = output.size[2];
        int W = output.size[3];


        // find the position of the body parts
        for (int n=0; n < nPoints; ++n)
        {
            // Probability map of corresponding body's part.
            cv::Mat probMap(H, W, CV_32F, output.ptr(0,n));
            cv::resize(probMap, probMap,  cv::Size(frameWidth, frameHeight));

            cv::Point maxLoc;
            double prob;
            cv::minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
            points[n] = maxLoc;
        }

        current_frame = 0;
    }

    current_frame++;
}

void HandDetection::DrawHand(cv::Mat &frame) {
    for (int n = 0; n < nPairs; ++n)
    {
        // lookup 2 connected body/hand parts
        cv::Point2f partA = points[POSE_PAIRS[n][0]];
        cv::Point2f partB = points[POSE_PAIRS[n][1]];

        if (partA.x<=0 || partA.y<=0 || partB.x<=0 || partB.y<=0)
            continue;

        cv::line(frame, partA, partB,  cv::Scalar(0,255,255), 4);
        cv::circle(frame, partA, 8,  cv::Scalar(0,0,255), -1);
        cv::circle(frame, partB, 8,  cv::Scalar(0,0,255), -1);
    }
}

void HandDetection::Initialize() {
    points = std::vector<cv::Point>(nPoints);
    current_frame = 0;

    net = cv::dnn::readNetFromCaffe(protoFile, weightsFile);

    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
}
