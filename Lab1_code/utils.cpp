#include "utils.hpp"

void computeErrorImage(const cv::Mat &im, const cv::Mat &imC, cv::Mat &imErr)
{
    assert(im.size() == imC.size());
    assert(im.size() == imErr.size());

    cv::MatConstIterator_<uchar> itPrec = im.begin<uchar>();
    cv::MatConstIterator_<uchar> itCur = imC.begin<uchar>();
    cv::MatIterator_<uchar> itErr = imErr.begin<uchar>();
    for( ; itCur != im.end<uchar>(); ++itCur, ++itPrec, ++itErr) {
        *itErr = *itCur - *itPrec;
    }
}

void computeDisplayableErrorImage(const cv::Mat &im, const cv::Mat &imC, cv::Mat &imErr)
{
    assert(im.size() == imC.size());
    assert(im.size() == imErr.size());

    cv::MatConstIterator_<uchar> itPrec = im.begin<uchar>();
    cv::MatConstIterator_<uchar> itCur = imC.begin<uchar>();
    cv::MatIterator_<uchar> itErr = imErr.begin<uchar>();
    for( ; itCur != imC.end<uchar>(); ++itCur, ++itPrec, ++itErr) {
        *itErr = std::min(255, std::max(0, int(*itCur) - int(*itPrec) + 128));
    }
}

double computeMSE(const cv::Mat &im, const cv::Mat &imC)
{
    assert(im.size() == imC.size());
    double MSE = 0.0;

    cv::MatConstIterator_<uchar> it = im.begin<uchar>();
    cv::MatConstIterator_<uchar> it2 = imC.begin<uchar>();
    for( ; it != imC.end<uchar>(); ++it, ++it2) {
        double value = (*it - *it2);
        MSE += value * value;
    }

    return (MSE / (im.cols*im.rows));
}

double computePSNR(const cv::Mat &im, const cv::Mat &imC)
{
    assert(im.size() == imC.size());
    return computePSNR(computeMSE(im, imC));
}

double computePSNR(double MSE)
{
    return 10.0 * log10((255*255)/MSE);
}

double computeEntropy(const cv::Mat &im)
{
    double hist[256] = {0};

    cv::MatConstIterator_<uchar> it;
    for(it = im.begin<uchar>(); it != im.end<uchar>(); ++it) {
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
