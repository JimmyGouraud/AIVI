/*

//OpenCV documentation is available here: http://docs.opencv.org/2.4.9/

//Block matching in mono and multi-resolution

*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <queue>
#include <sstream>
#include <fstream>


#include <opencv2/highgui/highgui.hpp> //VideoCapture, imshow, imwrite, ...
#include <opencv2/imgproc/imgproc.hpp> //cvtColor

#include "utils.hpp"
#include "blockmatching.hpp"

int
main(int argc, char **argv)
{
    if(argc != 6) {
        std::cerr << "Usage: " << argv[0] << " video-filename distance-between-two-frames-for-prediction nbLevels blockSize windowSize" << std::endl;
        return EXIT_FAILURE;
    }

    const char *videoFilename = argv[1];

    const int interFramesDistance = atoi(argv[2]);
    if (interFramesDistance <= 0) {
        std::cerr<<"Error: distance-between-two-frames-for-prediction must be a strictly positive integer"<<std::endl;
        return EXIT_FAILURE;
    }

    const int nbLevels = atoi(argv[3]);
    if (nbLevels <= 0 || nbLevels>4) {
        std::cerr<<"Error: nbLevels must be between [0;4]"<<std::endl;
        return EXIT_FAILURE;
    }

    const int blockSize = atoi(argv[4]);
    if (blockSize <= 0) {
        std::cerr << "Error: blockSize must be a strictly positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    const int windowSize = atoi(argv[5]);
    if(windowSize <= 0) {
        std::cerr << "Error: windowSize must be a strictly positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    cv::VideoCapture cap;
    cap.open(videoFilename);
    if ( ! cap.isOpened()) {
        std::cerr << "Error : Unable to open video file " << argv[1] << std::endl;
        return -1;
    }

    const size_t deltaT = interFramesDistance;
    std::queue<cv::Mat> previousFrames;


    std::ofstream file_mse("../gnuplot/stats_mse.txt", std::ios::out | std::ios::trunc);
    std::ofstream file_psnr("../gnuplot/stats_psnr.txt", std::ios::out | std::ios::trunc);
    std::ofstream file_entropy("../gnuplot/stats_entropy.txt", std::ios::out | std::ios::trunc);
    unsigned long frameNumber = 0;
    for ( ; ; ) {

        cv::Mat frameBGR;
        cap >> frameBGR;
        if (frameBGR.empty()) {
            break;
        }

        //convert from BGR to Y
        cv::Mat frameY;
        cv::cvtColor(frameBGR, frameY, CV_BGR2GRAY);

        if (previousFrames.size() >= deltaT) {
            cv::Mat prevY = previousFrames.front();
            previousFrames.pop();

            if (cv::waitKey(30) >= 0) { break; } // Permet l'affichage des images

            double MSE, PSNR, entropyCur, entropyErr;
            std::vector<cv::Mat> levelsY;
            std::vector<cv::Mat> levelsPrevY;
            std::vector<cv::Mat> levelsCompY(nbLevels);
            std::vector<cv::Mat> levelsErrY(nbLevels);
            std::vector<cv::Mat> motionVectorsP;

            blockMatchingMulti(frameY, prevY, blockSize, windowSize, nbLevels, levelsY, levelsPrevY, motionVectorsP);

            std::cout << frameNumber << "\n";

            entropyCur = computeEntropy(frameY);
            file_mse     << frameNumber;
            file_psnr    << frameNumber;
            file_entropy << frameNumber << " " << entropyCur;
            for (int i = nbLevels-1; i >= 0; --i) {
                computeCompensatedImage(motionVectorsP[i], prevY, levelsCompY[i]);
                computeErrorImage(frameY, levelsCompY[i], levelsErrY[i]);

                // Create file for gnuplot
                MSE = computeMSE(levelsCompY[i], frameY);
                PSNR = computePSNR(levelsCompY[i], frameY);
                entropyErr = computeEntropy(levelsErrY[i]);
                file_mse     << " " << MSE;
                file_psnr    << " " << PSNR;
                file_entropy << " " << entropyErr;

                // Display results
                //cv::Mat mv = frameBGR.clone();
                //drawMVi(mv, motionVectorsP[i]);
                //imshow("motionVectors"+std::to_string(i), mv);
                //imshow("compY"+std::to_string(i), levelsCompY[i]);
                //imshow("errY"+std::to_string(i), levelsErrY[i]);
            }

            file_mse     << '\n';
            file_psnr    << '\n';
            file_entropy << '\n';
        }

        previousFrames.push(frameY);
        ++frameNumber;
    }

    file_mse.close();
    file_psnr.close();
    file_entropy.close();

    return EXIT_SUCCESS;
}
