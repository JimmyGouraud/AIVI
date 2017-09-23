/*

  //OpenCV documentation is available here: http://docs.opencv.org/2.4.9/

  //1.1a - Display & save individual frames of the video as RGB
  // You will need to use classes & methods from the highgui module:
  // VideoCapture, imwrite, imshow, ...
  // You have an example of use of VideoCapture here:
  // http://docs.opencv.org/2.4.9/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture

  //1.1b - Display & save individual frames of the video as Y from YCrCb
  // You will need to use:
  //  cvtColor from the imgproc module
  //  split from the core module

  //1.2 - Compute the measures: MSE, PSNR, Entropy, Error Image
  // You will need to code these measures in utils.hpp/utils.cpp

  //1.3 - Trace the curves with gnuplot
  // You have examples of gnuplot commands are available in files example*.gplot

*/

#include <cstdlib>
#include <iostream>
#include <queue>
#include <fstream>

#include <opencv2/highgui/highgui.hpp> //VideoCapture, imshow, imwrite, ...
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/core/core.hpp>

#include "utils.hpp" // compute fonction

int main(int argc, char **argv)
{
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " video-filename distance-between-two-frames-for-prediction" << std::endl;
        return EXIT_FAILURE;
    }

    const char *videoFilename = argv[1];

    const int interFramesDistance = atoi(argv[2]);
    if (interFramesDistance <= 0) {
        std::cerr << "Error: distance-between-two-frames-for-prediction must be a strictly positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    cv::VideoCapture cap;
    cap.open(videoFilename);
    if (!cap.isOpened()) {
        std::cerr << "Error : Unable to open video file " << argv[1] << std::endl;
        return -1;
    }


    /*
    // Part 1.1)
    unsigned long frameNumber = 0;
    for ( ; ; ) {
        // les images OpenCV sont stockées dans des cv::Mat
        cv::Mat frameBGR, frameYCRCB;

        cap >> frameBGR;
        cvtColor(frameBGR, frameYCRCB, CV_BGR2YCrCb); // On convertie frameBGR en YCrCb et on stock dans frameYCRCB

        if (frameBGR.empty()) { break; }
        if (cv::waitKey(30) >= 0) { break; }

        imshow("frameBGR", frameBGR);
        imshow("frameYCRCB", frameYCRCB);

        cv::imwrite("frameBGR_" + std::to_string(frameNumber) + ".png", frameBGR);
        cv::imwrite("frameYCRCB_" + std::to_string(frameNumber) + ".png", frameYCRCB);

        ++frameNumber;
    }
    */

    // Part 1.2) & 1.3)
    cv::Mat frameBGR;
    std::queue<cv::Mat> frames;
    for (int i = 0; i <= interFramesDistance; i++) {
        cv::Mat frame;
        cap >> frameBGR;
        cvtColor(frameBGR, frame, CV_BGR2GRAY);
        frames.push(frame);
    }

    std::ofstream file_mse("../gnuplot/stats_mse.txt", std::ios::out | std::ios::trunc);
    std::ofstream file_psnr("../gnuplot/stats_psnr.txt", std::ios::out | std::ios::trunc);
    std::ofstream file_entropy("../gnuplot/stats_entropy.txt", std::ios::out | std::ios::trunc);
    unsigned long frameNumber = 0;
    cv::Mat framePrec, frameCur, frameErr;
    cvtColor(frameBGR, frameErr, CV_BGR2GRAY);
    for ( ; ; ) {
        framePrec = frames.front();
        frameCur = frames.back();

        if (frameBGR.empty()) { break; }
        if (cv::waitKey(30) >= 0) { break; } // Permet l'affichage des images

        double MSE = computeMSE(framePrec, frameCur);
        double PSNR = computePSNR(framePrec, frameCur);

        double entropyCur = computeEntropy(frameCur);
        computeErrorImage(framePrec, frameCur, frameErr);
        double entropyErr = computeEntropy(frameErr);

        //computeDisplayableErrorImage(framePrec, frameCur, frameErr);

        /*
        // Afficher les frames
        imshow("framePrec", framePrec);
        imshow("frameCur", frameCur);
        imshow("frameErr", frameErr);
        */

        // Create file for gnuplot
        file_mse     << frameNumber << " " << MSE  << '\n';
        file_psnr    << frameNumber << " " << PSNR << '\n';
        file_entropy << frameNumber << " " << entropyCur  << " " << entropyErr << '\n';

        cap >> frameBGR;
        cv::Mat frame;
        cvtColor(frameBGR, frame, CV_BGR2GRAY);
        frames.push(frame);
        frames.pop();

        ++frameNumber;
    }

    file_mse.close();
    file_psnr.close();
    file_entropy.close();

    return EXIT_SUCCESS;
}
