
#include "HandDetection.h"

HandDetection::HandDetection() {
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

        cv::Point prev_hand_pos = points[0];

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

        offset = points[0] - prev_hand_pos;
        //cv::norm(offset);

        UpdateOpenedFingers();

        current_frame = 0;
    }

    current_frame++;
}

void HandDetection::DrawHand(cv::Mat &frame) const{
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
    current_frame = 0;

    net = cv::dnn::readNetFromCaffe(protoFile, weightsFile);

    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
}

int HandDetection::GetFingerCount() const {
    int finger_count = 0;

    for (int i = 0; i < 5; ++i) {
        if(opened_fingers[i])
            finger_count++;
    }

    return finger_count;
}

void HandDetection::UpdateOpenedFingers() {

    for (int i = 0; i < 5; ++i) {
        previous_opened_fingers[i] = opened_fingers[i];
    }

    cv::Point2f partA;
    cv::Point2f partB;

    // Thumb
    opened_fingers[0] = false;
    bool thumb_is_rigth_to_center = false;
    partA = points[0];
    partB = points[1];

    if(partA.x < partB.x)
        thumb_is_rigth_to_center = true;

    //Tips
    partA = points[4];
    //Middle of the Finger
    partB = points[3];
    if (partA.x>0 && partA.y>0 && partB.x>0 && partB.y>0)
    {
        if(partA.x > partB.x && thumb_is_rigth_to_center) {
            //std::cout << "Finger open " << 4 << std::endl;
            opened_fingers[0] = true;
        }
        else if(partA.x < partB.x && !thumb_is_rigth_to_center) {
            //std::cout << "Finger open REVERSE" << 4 << std::endl;
            opened_fingers[0] = true;
        }
    }

    // Other Fingers
    for(int i = 0; i < 4; ++i)
    {
        opened_fingers[i + 1] = false;
        //Tips
        partA = points[fingerTipsIds[i]];
        //Middle of the Finger
        partB = points[fingerTipsIds[i] - 2];

        if (partA.x<=0 || partA.y<=0 || partB.x<=0 || partB.y<=0)
            continue;

        if(partA.y < partB.y) {
            //std::cout << "Finger open " << fingerTipsIds[i] << std::endl;
            opened_fingers[i + 1] = true;
        }
    }
}

bool HandDetection::GetHandOpened() const {
    bool isOpen = true;

    for (int i = 0; i < 5; ++i) {
        if(!opened_fingers[i])
        {
            isOpen = false;
            break;
        }
    }

    return isOpen;
}

bool HandDetection::GetFingerOpened(FingerType type) const {
    return opened_fingers[(int)type];
}
