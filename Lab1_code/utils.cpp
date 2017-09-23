#include "utils.hpp"

void computeErrorImage(const cv::Mat &im, const cv::Mat &imC, cv::Mat &imErr)
{
    assert((im.size() == imC.size()) && (im.size() == imErr.size()));
    assert((im.type() == CV_8UC1) && (imC.type() == CV_8UC1) && (imErr.type() == CV_8UC1));

    cv::MatConstIterator_<uchar> itPrec = im.begin<uchar>();
    cv::MatConstIterator_<uchar> itCur = imC.begin<uchar>();
    cv::MatIterator_<uchar> itErr = imErr.begin<uchar>();
    for( ; itPrec != im.end<uchar>(); ++itCur, ++itPrec, ++itErr) {
        *itErr = int(*itCur) - int(*itPrec);
    }
}

void computeDisplayableErrorImage(const cv::Mat &im, const cv::Mat &imC, cv::Mat &imErr)
{
    assert((im.size() == imC.size()) && (im.size() == imErr.size()));
    assert((im.type() == CV_8UC1) && (imC.type() == CV_8UC1) && (imErr.type() == CV_8UC1));

    cv::MatConstIterator_<uchar> itPrec = im.begin<uchar>();
    cv::MatConstIterator_<uchar> itCur = imC.begin<uchar>();
    cv::MatIterator_<uchar> itErr = imErr.begin<uchar>();
    for( ; itPrec != im.end<uchar>(); ++itCur, ++itPrec, ++itErr) {
        *itErr = std::min(255, std::max(0, int(*itCur) - int(*itPrec) + 128));
    }
}

double computeMSE(const cv::Mat &im, const cv::Mat &imC)
{
    assert(im.size() == imC.size());
    assert((im.type() == CV_8UC1) && (imC.type() == CV_8UC1));
    double MSE = 0.0;

    cv::MatConstIterator_<uchar> itPrec = im.begin<uchar>();
    cv::MatConstIterator_<uchar> itCur = imC.begin<uchar>();
    for( ; itPrec != im.end<uchar>(); ++itPrec, ++itCur) {
        double value = (*itPrec - *itCur);
        MSE += value * value;
    }

    return (MSE / (im.cols*im.rows));
}

double computePSNR(const cv::Mat &im, const cv::Mat &imC)
{
    assert(im.size() == imC.size());
    assert((im.type() == CV_8UC1) && (imC.type() == CV_8UC1));

    return computePSNR(computeMSE(im, imC));
}

double computePSNR(double MSE)
{
    return 10.0 * log10((255*255)/MSE);
}

double computeEntropy(const cv::Mat &im)
{
    assert(im.type() == CV_8UC1);
    double hist[256] = {0};

    cv::MatConstIterator_<uchar> it = im.begin<uchar>();
    for( ; it != im.end<uchar>(); ++it) {
        hist[int(*it)] += 1;
    }

    double entropy = 0;
    double size = im.cols * im.rows;
    for (int i = 0; i < 256; i++) {
        if (hist[i] == 0) { continue; }

        hist[i] /= size;
        entropy += hist[i] * log2(hist[i]);
    }

    return - entropy;
}
