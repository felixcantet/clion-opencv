#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

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

float DELAY;
const int POSE_PAIRS[20][2] =
        {
                {0,1}, {1,2}, {2,3}, {3,4},         // thumb
                {0,5}, {5,6}, {6,7}, {7,8},         // index
                {0,9}, {9,10}, {10,11}, {11,12},    // middle
                {0,13}, {13,14}, {14,15}, {15,16},  // ring
                {0,17}, {17,18}, {18,19}, {19,20}   // small
        };

cv::Mat frame;

std::string protoFile = "../Hand/pose_deploy.prototxt";
std::string weightsFile = "../Hand/pose_iter_102000.caffemodel";

void DisplayVideo(const char*, cv::dnn::Net& net);

int main()
{
    DELAY = (1.0f/60.0f) * 1000.0f;
    std::cout << "Delay is : " << DELAY << std::endl;

    cv::dnn::Net net = cv::dnn::readNetFromCaffe(protoFile, weightsFile);

    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);

    //CaptureHand(net);

    DisplayVideo(nullptr, net);

    return 0;
}

void DisplayVideo(const char* videoname, cv::dnn::Net& net){

    cv::VideoCapture cap;
    int nPoints = 22;

    int everyFrame = 0;

    //si videoname n’est pas null, ouvrir la DisplayVideo dans cap, sinon ouvrir la camera 0
    if (videoname != nullptr)
        cap.open(videoname);
    else
        cap.open(0);

    //si cap n’est pas ouvert, quitter la fonction
    if (!cap.isOpened())
        return;

    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    float aspect_ratio = (float)frameWidth/(float)frameHeight;
    int inHeight = 240;
    int inWidth = int(aspect_ratio*inHeight);

    float scaleFactor = 1.0f / 255.0f;
    int nPairs = sizeof(POSE_PAIRS)/sizeof(POSE_PAIRS[0]);
    std::vector<cv::Point> points(nPoints);

    //recuperer une image depuis cap et la stocker dans frame
    cap.read(frame);
    cv::Mat output;
    //tant que nextinput n’est pas vide
    while(!frame.empty())
    {
        double t = (double) cv::getTickCount();

        if(everyFrame % 2 == 0) {
            cv::Mat inpBlob = cv::dnn::blobFromImage(frame, scaleFactor, cv::Size(inWidth, inHeight), cv::Scalar(0, 0, 0), false, false);
            net.setInput(inpBlob);
            everyFrame = 0;
            output = net.forward();
        }
        everyFrame++;

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

        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        std::cout << "Time Taken for frame = " << t << std::endl;
        cv::putText(frame, cv::format("time taken = %.2f sec", t), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, .8, cv::Scalar(255, 50, 0), 2);

        cv::imshow("Video", frame);

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