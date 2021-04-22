#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

//#include <Python.h>

#include "HandDetection.h"
#include "InputHandler.h"

/*  TODO :
 *  - Pouvoir dire l'état de la main (bouge, symbole ?)
 *      - Gauche ou droite
 *  - Nombre de mains
 *
 *  - Optimisation
 *
 *  - Controler le cursor sans la souris
 *  - Créer des "inputs" pour utiliser la souris
 */

const float DELAY = (1.0f / 60.0f) * 1000.0f;


void DisplayVideo(const char *, HandDetection& handDetection);

int main() {
    std::cout << "Delay is : " << DELAY << std::endl;

    //Py_Initialize();

    HandDetection handDetection;

    handDetection.Initialize();
    DisplayVideo(nullptr, handDetection);

    //Py_Finalize();

    return 0;
}

void DisplayVideo(const char *videoname, HandDetection& handDetection) {

    cv::VideoCapture cap;

    //si videoname n’est pas null, ouvrir la DisplayVideo dans cap, sinon ouvrir la camera 0
    if (videoname != nullptr)
        cap.open(videoname);
    else
        cap.open(0);

    //si cap n’est pas ouvert, quitter la fonction
    if (!cap.isOpened())
        return;

    std::string winName = "Video";

    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    float aspect_ratio = (float) frameWidth / (float) frameHeight;
    int inHeight = 225;
    int inWidth = int(aspect_ratio * inHeight);

    cv::Mat frame;

    //recuperer une image depuis cap et la stocker dans frame
    cap.read(frame);

    //cv::namedWindow(winName, cv::WINDOW_NORMAL);
    //cv::setWindowProperty(winName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    while (!frame.empty()) {
        std::cout << "====================== \n" << std::endl;
        double t = (double) cv::getTickCount();

        cv::flip(frame, frame, 1);

        handDetection.DetectHand(frame , inWidth, inHeight, frameWidth, frameHeight);
        handDetection.DrawHand(frame);

        int fingerCount = handDetection.GetFingerCount();
        bool hand_open = handDetection.GetHandOpened();
        bool thumb_open = handDetection.GetFingerOpened(FingerType::THUMB);

        InputHandler::PerformInput(handDetection);

        std::cout << "There is : " << fingerCount << " open\n" <<
                  "The hand is " << (hand_open ? "open" : "close") << "\n" <<
                  "The thumb is " << (thumb_open ? "open" : "close") << std::endl;

        t = ((double) cv::getTickCount() - t) / cv::getTickFrequency();
        cv::putText(frame, cv::format("time taken = %.4f sec", t), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, .8,
                    cv::Scalar(255, 50, 0), 2);

        cv::imshow(winName, frame);

        int key = cv::waitKey(DELAY);
        if (key == 27) // 27 == echap
        {
            cv::putText(frame, "ON FERME", cv::Point(1.0f, frameHeight * 0.5f), cv::FONT_HERSHEY_COMPLEX, 2.0f,
                        cv::Scalar(255, 50, 0), 2);
            cv::imshow(winName, frame);
            cv::waitKey(2000);
            break;
        }

        // - > recuperer une nouvelle image et la stocker dans frame
        cap.read(frame);
    }

    cap.release();
    cv::destroyAllWindows();
}