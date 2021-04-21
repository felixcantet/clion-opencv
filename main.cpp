#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "HandDetection.h"

/*  - Afficher la vidéo
 *  - Afficher le traçage sur les doigts/mains (skelette)
 *
 *  TODO Pour ce soir :
 *  - Pouvoir dire l'état de la main (nb doigt, fermé, ouvert, bouge, symbole ?)
 *      - Gauche ou droite
 *  - Nombre de mains
 *
 *  TODO :
 *  - Controler le cursor sans la souris
 *  - Créer des "inputs" pour utiliser la souris
 */

const float DELAY = (1.0f/60.0f) * 1000.0f;;
cv::Mat frame;
HandDetection handDetection;

void DisplayVideo(const char*);

int main()
{
    std::cout << "Delay is : " << DELAY << std::endl;

    handDetection.Initialize();

    DisplayVideo(nullptr);

    return 0;
}

void DisplayVideo(const char* videoname){

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
    float aspect_ratio = (float)frameWidth/(float)frameHeight;
    int inHeight = 368;
    int inWidth = int(aspect_ratio*inHeight);


    //recuperer une image depuis cap et la stocker dans frame
    cap.read(frame);

    while(!frame.empty())
    {
        double t = (double) cv::getTickCount();

        cv::flip(frame, frame, 1);

        handDetection.DetectHand(frame, inWidth, inHeight, frameWidth, frameHeight);
        handDetection.DrawHand(frame);

        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        //std::cout << "Time Taken for frame = " << t << std::endl;
        cv::putText(frame, cv::format("time taken = %.4f sec", t), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, .8, cv::Scalar(255, 50, 0), 2);

        cv::imshow(winName, frame);

        // - > attendre 10ms que l’utilisateur tape une touche, et quitter si il le fait
        int key = cv::waitKey(DELAY);
        if(key == 27)
            break;

        // - > recuperer une nouvelle image et la stocker dans frame
        cap.read(frame);
    }

    cap.release();
    cv::destroyAllWindows();
}