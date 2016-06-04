#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <tuple>
#include <stdexcept>

using namespace cv;
using namespace std;

auto const RED = cv::Scalar(0,0,255);
auto const PINK = cv::Scalar(230,130,255);
auto const BLUE = cv::Scalar(255,0,0);
auto const LIGHTBLUE = cv::Scalar(255,255,160);
auto const GREEN = cv::Scalar(0,255,0);
auto const IMAGE_SCALE = .25;
RNG rng(12345);

enum WeldingFoldingType { FOLDED, UNFOLDED };
enum HasDefect { OK, DEFECT };

using my_images = std::tuple<std::string, HasDefect, WeldingFoldingType >;

/// TODO:CREEPY GLOBAL VARIABLES
int image_id;
std::string const out_path("/tmp/work/");

// mkdir /tmp/work/;
// mkdir /tmp/work/original/;
// mkdir /tmp/work/binarization/;
// mkdir /tmp/work/rm_background/;
// mkdir /tmp/work/clean/;
// mkdir /tmp/work/bag/;
// mkdir /tmp/work/smooth_contour/;
// mkdir /tmp/work/decorated/;

int num_images = 42;
static my_images const cheese_imgs[] = {
    std::make_tuple("./A/08_03_2016  09_05_38,207.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_05_27,885.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_05_39,622.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_06_06,897.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_05_47,611.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_06_25,349.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_43,667.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_28,497.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_08,151.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_18,165.bmp", OK, FOLDED),
    std::make_tuple("./C/08_03_2016  09_07_46,137.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_31,997.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_20,991.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_22,148.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_19,30.bmp" , OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_59,487.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_34,660.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_23,563.bmp", OK , FOLDED ),
    std::make_tuple("./D/08_03_2016  09_13_07,35.bmp" , OK, FOLDED),
    std::make_tuple("./D/08_03_2016  09_13_31,67.bmp" , OK, FOLDED),
    std::make_tuple("./D/08_03_2016  09_13_18,67.bmp" , OK, FOLDED),
    std::make_tuple("./E/08_03_2016  08_56_30,507.bmp", OK , FOLDED ),
    std::make_tuple("./E/08_03_2016  08_56_04,428.bmp", OK , FOLDED ),
    std::make_tuple("./E/08_03_2016  08_56_17,67.bmp" , OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_53_06,100.bmp", OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_52_39,828.bmp", OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_53_26,751.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_22,154.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_32,248.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_33,663.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_55,274.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_43,665.bmp", OK , FOLDED ),
    std::make_tuple("./H/08_03_2016  08_58_02,182.bmp", OK, FOLDED),
    std::make_tuple("./H/08_03_2016  08_57_50,442.bmp", OK, FOLDED),
    std::make_tuple("./H/08_03_2016  08_58_13,658.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_01_04,419.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_01_14,406.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_25,699.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_52,155.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_53,570.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_03,723.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_22,953.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_13,824.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_11,974.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_10_16,829.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_09_45,273.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_09_55,857.bmp", OK, FOLDED)
}; 

void thresholdImage( const Mat& _inImage, Mat& _outImage, int th)
// TODO: add image->copy(out)
{
    enum ThresholdType {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                         THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
    ThresholdType const threshold_type = BINARY;
    int const max_BINARY_value = 255;

    threshold( _inImage, _outImage, th, max_BINARY_value, threshold_type );
}

boost::format image_fmter("%03i.png");
boost::format folding_fmter("%05i");
string getImagePath(std::string s)
{
    std::string image_name = boost::str(image_fmter % image_id);
    return out_path+s+image_name;
}
string getImagePath(std::string s, int i)
{
    std::string image_name = boost::str(image_fmter % image_id) +
                             boost::str(folding_fmter % i);
    return out_path+s+image_name;
}

Mat loadTestImage(const std::string& inputImage)
{
    Mat image_orig = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image_orig.empty())
        throw std::runtime_error("Cannot read image file: "+inputImage);

    cv::resize(image_orig, image_orig, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    return image_orig;
}

void myRegionGrowing(const cv::Mat& _inImage, cv::Mat& _outImage, int morph_size=10,
                     cv::MorphTypes morph_type=cv::MORPH_OPEN,
                     cv::MorphShapes morph_shape=cv::MORPH_RECT
                     )
{
    cv::Mat element = getStructuringElement( morph_shape, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(  _inImage, _outImage, morph_type, element );
}

namespace findBag {
    std::string getOutPath( std::string const &s ){
        return getImagePath("find_bag/"+s);
    }

    void getBackgroundMask(cv::Mat& _outImage)
        // TODO: add image->copy(out)
    {
        const string bkgPath = "../../testdata/A/background.bmp";
        Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
        if(background.empty())
            throw std::runtime_error("Cannot read image file: "+bkgPath);

        cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
        cv::threshold( background, _outImage, 250, 255, cv::THRESH_BINARY );
    }

    void remove_background(cv::Mat& image)// const Mat& _inImage, Mat& _outImage )
        // TODO: add image->copy(out)
    {
        const string _out_path = out_path+"background_removed/"+std::to_string(image_id)+".png";
        const string bkgPath = "../../testdata/A/background.bmp";
        Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
        if(background.empty())
            throw std::runtime_error("Cannot read image file: "+bkgPath);

        cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
        /* _inImage.copyTo( _outImage ); */
        /* _outImage -= background; */
        /* cv::absdiff(background, _outImage, _outImage); */
        cv::absdiff(background, image, image);
    }

    void binarize(cv::Mat &ioImage){
        std::string const OUT_DIR = "xx/binarize/";
        Mat _image, _img_th, _img_otsu, _img_adapt, _img_combination;
        ioImage.copyTo(_image);

        // sik wrapper
        thresholdImage (_image, _img_th, 1);
        cv::imwrite(getImagePath(OUT_DIR+"th1/"), _img_th);
        thresholdImage (_image, _img_th, 10);
        cv::imwrite(getImagePath(OUT_DIR+"th10/"), _img_th);
        thresholdImage (_image, _img_th, 50);
        cv::imwrite(getImagePath(OUT_DIR+"th50/"), _img_th);
        thresholdImage (_image, _img_th, 100);
        cv::imwrite(getImagePath(OUT_DIR+"th100/"), _img_th);
        thresholdImage (_image, _img_th, 150);
        cv::imwrite(getImagePath(OUT_DIR+"th150/"), _img_th);
        thresholdImage (_image, _img_th, 200);
        cv::imwrite(getImagePath(OUT_DIR+"th200/"), _img_th);
        thresholdImage (_image, _img_th, 240);
        cv::imwrite(getImagePath(OUT_DIR+"th240/"), _img_th);
        // othsu
        cv::threshold( _image, _img_otsu, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU );
        cv::imwrite(getImagePath(OUT_DIR+"otsu/"), _img_otsu);
        // Adaptive threshold
        cv::adaptiveThreshold(_image, _img_adapt, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37, 0 );
        cv::imwrite(getImagePath(OUT_DIR+"adapt/"), _img_adapt);

        _img_adapt.copyTo(ioImage);
    }

    /*void clean(cv::Mat &_ioImage){
        Mat _image, background_mask;
        _ioImage.copyTo(_image);
        findBag::clean(_image, _ioImage);
    }*/

    void clean(cv::Mat &_inImage, cv::Mat &_outImage){
        std::string const OUT_DIR = "xx/clean/";
        Mat _image, background_mask;

        // Eliminate the background borders
        findBag::getBackgroundMask(background_mask);
        myRegionGrowing(background_mask,background_mask,5,cv::MORPH_ERODE);
        _inImage.copyTo(_image, background_mask);

        // perform the morphology operation
        myRegionGrowing(_image, _outImage, 5, cv::MORPH_CLOSE);
        cv::imwrite(getImagePath(OUT_DIR+"in/"), _image);
        cv::imwrite(getImagePath(OUT_DIR+"out/"), _outImage);
    }

    void findContours( cv::Mat &_inImage,
                       std::vector<std::vector<cv::Point>> &bag_outline,
                       std::vector<cv::Vec4i> &hierarchy,
                       //cv::Mat &_image_to_decorate=nullptr)
                       cv::Mat &_image_to_decorate)
    {
        //CV_Assert( !mask.empty() ); //TODO: assert that the images are same size etc..
        //std::cout << "in: " << _inImage.size() << "out: " << _image_to_decorate.size() << std::endl;
        cv::Mat xx, kk;
        _inImage.copyTo(xx);
        _inImage.copyTo(kk);

        cv::findContours(xx, bag_outline, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
        cv::cvtColor(kk, kk, CV_GRAY2RGB); // TODO: this is unnecessary
        cv::drawContours( kk, bag_outline, -1, GREEN, 2, 8, hierarchy);
        kk.copyTo(_image_to_decorate);
    }

    int getBiggestOtulineIndx( std::vector<std::vector<cv::Point>> const &bag_outline )
    {
        if (bag_outline.size() == 0 )
            throw std::runtime_error("no boundaries found in: "+image_id);

        auto it = std::max_element(bag_outline.begin(), bag_outline.end(),
                [](auto &a, auto &b){return cv::contourArea(a) < cv::contourArea(b);});
        auto idx_max = std::distance(bag_outline.begin(), it);

        return idx_max;
    }

    void smoothContours( cv::Mat &_inImage,
                         std::vector<cv::Point> const &_inContour,
                         std::vector<cv::Point> &_outContour,
                         //cv::mat &_image_to_decorate=nullptr)
                         cv::Mat &_image_to_decorate)
    // The function smoothes an outline and draws only the output outline into _image_to_decorate
    {
        /* _outContour.clear(); */
        /* _outContour.insert(_outContour.end(), _inContour.begin(), _inContour.end()); */
        cv::approxPolyDP(_inContour, _outContour, 10, true);
        std::vector<std::vector<cv::Point>> cc = {_outContour};
        cv::drawContours( _image_to_decorate, cc, -1, RED, 1, 8);

    }

    void findHull( std::vector<std::vector<cv::Point>> const &contours,
                   std::vector<cv::Point> &hull,
                   //cv::Mat &_image_to_decorate=nullptr)
                   cv::Mat &_image_to_decorate)
    {
        std::vector<cv::Point> myPoints;
        for( auto& c : contours)
            myPoints.insert(myPoints.end(), c.begin(), c.end());

        cv::convexHull( Mat(myPoints), hull, false );

        std::vector<std::vector<cv::Point>> hhull = {hull};
        drawContours( _image_to_decorate, hhull, 0, RED, 2, 8, vector<Vec4i>(), 0, Point() );
    }

}

void processsImage(const std::string& _img_path, double &folded_index) {

    Mat original_image = loadTestImage(_img_path);
    Mat image;
    original_image.copyTo(image);
    cv::imwrite(getImagePath("original/"), image);

    Mat image_find_folding;
    Mat bag_outline_image;
    vector<vector<Point>> bag_outline;
    vector<Vec4i> hierarchy;
    vector<Point> hull;
    vector<Point> smooth_outline;

    /*
     * *********************************************************************
     *  Find bag
     * *********************************************************************
     */

    findBag::remove_background(image);
    cv::imwrite(getImagePath("rm_background/"), image);

    findBag::binarize(image);
    cv::imwrite(getImagePath("binarization/"), image);

    cv::Mat xx; image.copyTo(xx);
    findBag::clean(xx, image);
    cv::imwrite(getImagePath("clean/"), image);

    findBag::findContours(image, bag_outline, hierarchy, bag_outline_image);
    cv::imwrite(getImagePath("bag/"), bag_outline_image);

    auto idx = findBag::getBiggestOtulineIndx(bag_outline);
    findBag::smoothContours(image, bag_outline[idx], smooth_outline, bag_outline_image);
    cv::imwrite(getImagePath("smooth_contour/"), bag_outline_image);

    cvtColor(original_image, original_image, CV_GRAY2RGB); // TODO: this is unnecessary
    findBag::findHull(bag_outline, hull, original_image);

    cv::resize(original_image, original_image, cv::Size(), 0.7, 0.7);
    cv::imwrite(getImagePath("decorated/"), original_image);
}


int main( int argc, const char** argv )
{
    /* for ( image_id = 0; image_id < cheese_imgs.size(); image_id++) */
    /// TODO: how to assess size(cheese_imgs)
    /* for ( image_id = 0; image_id < 47; image_id++) */

    for ( image_id = 0; image_id < num_images; image_id++)
    {
        std::string     current_img_path;
        HasDefect xx;
        WeldingFoldingType yy;
        std::tie(current_img_path, xx, yy) = cheese_imgs[image_id];
        std::cout << "\"" << current_img_path << "\", ";
        double folded_index;
        try{
        processsImage("../../testdata/"+current_img_path, folded_index);
        }
        catch ( const std::exception& e ){
            folded_index=-1.0;
        }

        std::cout << folded_index << std::endl;
    }

}
