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

        //save frame
        //std::stringstream ss;
        //ss<<"frame_"<<std::setfill('0')<<std::setw(6)<<frameNumber<<".png";
        //cv::imwrite(ss.str(), frameBGR);

        //convert from BGR to Y
        cv::Mat frameY;
        cv::cvtColor(frameBGR, frameY, CV_BGR2GRAY);

        if (previousFrames.size() >= deltaT) {
            cv::Mat prevY = previousFrames.front();
            previousFrames.pop();

            double MSE, PSNR, entropyCur, entropyErr;

            if (nbLevels == 1) {
                cv::Mat motionVectors, compY, errY, err2Y;

                if (cv::waitKey(30) >= 0) { break; } // Permet l'affichage des images

                blockMatchingMono(frameY, prevY, blockSize, windowSize, motionVectors);
                computeCompensatedImage(motionVectors, prevY, compY);
                computeErrorImage(frameY, compY, errY);
                computeErrorImage(frameY, prevY, err2Y);

                MSE = computeMSE(compY, frameY);
                PSNR = computePSNR(compY, frameY);
                entropyCur = computeEntropy(frameY);
                entropyErr = computeEntropy(errY);

                // Display results
                cv::Mat mv = frameBGR.clone();
                drawMVi(mv, motionVectors);
                imshow("motionVectors", mv);
                imshow("compY", compY);
                imshow("errY", errY);
                imshow("err2Y", err2Y);

                // Create file for gnuplot
                file_mse     << frameNumber << " " << MSE  << '\n';
                file_psnr    << frameNumber << " " << PSNR << '\n';
                file_entropy << frameNumber << " " << entropyCur  << " " << entropyErr << '\n';
            } else {
                if (cv::waitKey(30) >= 0) { break; } // Permet l'affichage des images

                std::vector<cv::Mat> levelsY;
                std::vector<cv::Mat> levelsPrevY;
                std::vector<cv::Mat> motionVectorsP;
                blockMatchingMulti(frameY, prevY, blockSize, windowSize, nbLevels, levelsY, levelsPrevY, motionVectorsP);

                std::cout<<frameNumber;
                for (int i = nbLevels-1; i >= 0; --i) {
                    /*
                    cv::Mat compY, errY;
                    computeCompensatedImage(motionVectorsP[i], levelsPrevY[], compY);
                    computeErrorImage(frameY, compY, errY);

                    // Display results
                    cv::Mat mv = frameBGR.clone();
                    drawMVi(mv, motionVectorsP[i]);
                    imshow("motionVectors", mv);
                    imshow("compY", compY);
                    imshow("errY", errY);
                    */
                }

                cv::Mat compY, errY;
                std::cerr << "motionVectorsP : " << motionVectorsP[0] << '\n';
                computeCompensatedImage(motionVectorsP[0], prevY, compY);

                // Display results
                cv::Mat mv = frameBGR.clone();
                drawMVi(mv, motionVectorsP[0]);
                imshow("motionVectors", mv);
                imshow("compY", compY);

                std::cout<<"\n";
            }
        }

        previousFrames.push(frameY);

        ++frameNumber;
    }

    file_mse.close();
    file_psnr.close();
    file_entropy.close();

    return EXIT_SUCCESS;
}
