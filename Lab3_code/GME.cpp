#include "GME.hpp"

#include <opencv2/calib3d/calib3d.hpp> //findHomography

#include <iostream>

void
computeGME(const cv::Mat &motionVectors,
	   cv::Mat &motionVectorsGlobal)
{
  //TODO: compute global motion estimated vectors
  // use cv::findHomeography and cv::perspectiveTransform

  //See http://docs.opencv.org/2.4.9/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html?highlight=findhomography#findhomography
  // and https://docs.opencv.org/2.4.9/modules/core/doc/operations_on_arrays.html?highlight=perspectivetransform#cv2.perspectiveTransform
  //According to the documentation, cv::findHomography & cv::perspectiveTransform take InputArrays & OutputArrays
  //See here http://docs.opencv.org/2.4.13/modules/core/doc/intro.html#inputarray-and-outputarray
  //They are just proxies around a cv::Mat or a std::vector
  //For example, for cv::findHomography(), for 'src',  we can pass a cv::Mat of type CV_32FC2 or a std::vector<cv::Point2f>. It will work the same.
  //
  //**WARNING**
  //If you pass cv::Mats to cv::findHomography, they have to have 1 column or 1 row !
  //Otherwise, there is an assert that will fail in OpenCV implementation.
  //You can use cv::reshape to just change the shape of you matrix :
  // pass from a matrix of size rows x cols to a matrix of size 1 x rows*cols.
  //Here you would call: reshape(0, 1);

  // /!\ i et j défini par rapport au centre ! donc faire i - center; j - center
  motionVectorsGlobal.create(motionVectors.rows, motionVectors.cols, CV_32FC2);
  motionVectorsGlobal.reshape(0,1);
  cv::findHomography(motionVectors, motionVectorsGlobal, CV_RANSAC);
  cv::perspectiveTransform();


  assert(motionVectorsGlobal.type() == CV_32FC2);
}

void
computeGlobalMotionError(const cv::Mat &motionVectors,
			 const cv::Mat &motionVectorsGlobal,
			 cv::Mat &motionError)
{
  assert(motionVectors.type() == CV_32FC2);
  assert(motionVectorsGlobal.type() == CV_32FC2);

  motionError.create(motionVectors.rows, motionVectors.cols, CV_32F);

  //TODO: compute the error between actual motion vectors and estimated global motion vectors


  assert(motionError.type() == CV_32F);
}
