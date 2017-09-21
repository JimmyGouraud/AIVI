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

#include <opencv2/highgui/highgui.hpp> //VideoCapture, imshow, imwrite, ...
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/core/core.hpp>

#include "utils.hpp" // compute fonction

int
main(int argc, char **argv)
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

  unsigned long frameNumber = 0;

  /*
  // Part 1.1)
  for ( ; ; ) {
    // les images OpenCV sont stockés dans des cv::Mat
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

  // Part 1.2)
  cv::Mat frameBGR, framePrec, frameCur, frameErr;
  cap >> frameBGR;
  cvtColor(frameBGR, framePrec, CV_BGR2GRAY);
  cvtColor(frameBGR, frameErr, CV_BGR2GRAY);

  for ( ; ; ) {
    cap >> frameBGR;
    if (frameBGR.empty()) { break; }
    if (cv::waitKey(30) >= 0) { break; } // Permet l'affichage des images

    cvtColor(frameBGR, frameCur, CV_BGR2GRAY);
    //double MSE = computeMSE(framePrec, frameCur);
    //double PSNR = computePSNR(framePrec, frameCur);
    //double entropy = computeEntropy(framePrec);
    //computeErrorImage(framePrec, frameCur, frameErr);
    computeDisplayableErrorImage(framePrec, frameCur, frameErr);
    imshow("frameErr", frameErr);

    framePrec = frameCur.clone();
    ++frameNumber;
  }

  return EXIT_SUCCESS;
}
