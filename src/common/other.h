#ifndef MY_OTHER_H
#define MY_OTHER_H

#include "my_core.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#include <iostream>

using namespace cv;
using namespace std;

namespace findBag{
    getBackgroundMask(cv::Mat& _outImage);
    remove_background(cv::Mat& image);
    binarize(cv::Mat &ioImage);
    clean(cv::Mat &_inImage, cv::Mat &_outImage);
    findContours( cv::Mat &_inImage, std::vector<std::vector<cv::Point>> &bag_outline, std::vector<cv::Vec4i> &hierarchy, cv::Mat &_image_to_decorate);
    getBiggestOtulineIndx( std::vector<std::vector<cv::Point>> const &bag_outline );
    smoothContours( cv::Mat &_inImage, std::vector<cv::Point> const &_inContour, std::vector<cv::Point> &_outContour, cv::Mat &_image_to_decorate);
    findHull( std::vector<std::vector<cv::Point>> const &contours, std::vector<cv::Point> &hull, cv::Mat &_image_to_decorate);
}
namespace xx{

    decorate(cv::Mat &_image, vector<vector<Point>> &contour);
    findClosings(cv::Mat const &_inImage, vector<vector<vector<Point>>> &levels, vector<vector<cv::Mat>> &structuring_elements, double angle=0.0);
    findOpenings(cv::Mat const &_inImage, vector<vector<vector<Point>>> &levels, vector<vector<cv::Mat>> &structuring_elements, double angle=0.0);
    findRotationAngle(cv::Mat &_img, vector<RotatedRect> &boxes);
}
addBoundingBox(cv::Mat img, RotatedRect &box);
addAngle(cv::Mat img, double angle);

#endif /* MY_OTHER_H */
