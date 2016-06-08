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
auto const PINK_A = cv::Scalar(255,0,255);
auto const PINK_B = cv::Scalar(175,82,255);
auto const PINK_C = cv::Scalar(115,140,255);
auto const PINK_D = cv::Scalar(77,177,255);
auto const PINK_E = cv::Scalar(22,230,255);
auto const BLUE_A = cv::Scalar(255,0,0);
auto const BLUE_B = cv::Scalar(210,85,0);
auto const BLUE_C = cv::Scalar(195, 120, 0);
auto const BLUE_D = cv::Scalar(163, 183,0);
auto const BLUE_E = cv::Scalar(130,255,0);
RNG rng(12345);

enum WeldingFoldingType { FOLDED, UNFOLDED };
enum HasDefect { OK, DEFECT };

using my_images = std::tuple<std::string, HasDefect, WeldingFoldingType >;

/// TODO:CREEPY GLOBAL VARIABLES
int image_id;
std::string display_iteration;
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







namespace findCheese{
    void profOfConcept(cv::Mat const _inImg){

        Mat original_image;
        Mat image;
        _inImg.copyTo(image);
        _inImg.copyTo(original_image);

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

        Mat _image, background_mask;
        findBag::getBackgroundMask(background_mask);
        image.copyTo(_image, background_mask);

        findBag::binarize(_image);
        cv::imwrite(getImagePath("binarization/"), _image);

        vector<RotatedRect> boxes;
        double angle = xx::findRotationAngle(_image, boxes);

        // perform the morphology operation
        vector<vector<vector<Point>>> closing_levels;
        vector<vector<cv::Mat>> closing_structures;
        xx::findClosings(_image, closing_levels, closing_structures, angle);

        vector<vector<vector<Point>>> opening_levels;
        vector<vector<cv::Mat>> opening_structures;
        xx::findOpenings(_image, opening_levels, opening_structures, angle);

        cvtColor(original_image, original_image, CV_GRAY2RGB); // TODO: this is unnecessary
        if (!boxes.empty())
        {
            addBoundingBox(original_image, *boxes.rbegin());
            addAngle(original_image, angle);
        }
        int jj=0;
        std::array<cv::Scalar,5> color = { PINK_A, PINK_B, PINK_C, PINK_D, PINK_E };
        for (auto it=closing_levels.rbegin(); it!=closing_levels.rend(); ++it)
        {
            cv::Mat xx;
            vector<vector<Point>> structure_contour;
            vector<Vec4i> hh;
            auto structures_last_element = opening_structures.rbegin();
            (*structures_last_element)[jj].copyTo(xx); // last in opening structures are the structures applied to the current image. the second vector contains each opearation structure
            cv::findContours(xx, structure_contour, hh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
            cv::drawContours( original_image, structure_contour, -1, color[jj], 2, 8, hh);
            cv::drawContours( original_image, *it, -1, color[jj++], 2, 8, hierarchy);
        }

        jj=0;
        std::array<cv::Scalar,5> color2 = { BLUE_A, BLUE_B, BLUE_C, BLUE_D, BLUE_E, };
        for (auto it=opening_levels.rbegin(); it!=opening_levels.rend(); ++it)
        {
            cv::drawContours( original_image, *it, -1, color2[jj++], 2, 8, hierarchy);
        }

        cv::imwrite(getImagePath("decorated/"), original_image);
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

    Mat _image, background_mask;
    findBag::getBackgroundMask(background_mask);
    image.copyTo(_image, background_mask);

    findBag::binarize(_image);
    cv::imwrite(getImagePath("binarization/"), _image);

    vector<RotatedRect> boxes;
    double angle = xx::findRotationAngle(_image, boxes);

    // perform the morphology operation
    vector<cv::Mat> closing_structure;
    cv::Mat _img_close;
    myRegionGrowing_rotated(_image, _img_close, closing_structure, 15, 0.3, cv::MORPH_CLOSE, angle);

    vector<cv::Mat> opening_structure;
    cv::Mat _img_open;
    myRegionGrowing_rotated(_image, _img_open, opening_structure, 10, 0.2, cv::MORPH_OPEN, angle);

    // find bag outlines
    vector<vector<Point>> out_bag, in_bag;
    vector<Vec4i> h, hh;
    cv::findContours(_img_open, in_bag, h, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::findContours(_img_close, out_bag, hh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    auto out_idx = findBag::getBiggestOtulineIndx(out_bag);
    auto in_idx = findBag::getBiggestOtulineIndx(in_bag);

    cv::Mat welding_area;
    cv::Mat out_mask (original_image.size(), CV_8U);
    cv::drawContours( out_mask, out_bag, out_idx,255, -1);
    cv::Mat in_mask (original_image.size(), CV_8U);
    cv::drawContours( in_mask, in_bag, in_idx, 255, -1);
    std::cout << "out mask size: " << out_mask.size() << "in mask size: " << in_mask.size() << std::endl;
    cv::bitwise_xor(out_mask, in_mask, welding_area, out_mask);

    // find bag outlines
    vector<vector<Point>> welding;
    vector<Vec4i> hwelding;
    cv::findContours(welding_area, welding, hwelding, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::Mat original_img_copy;
    original_image.copyTo(original_img_copy);
    cv::cvtColor(original_img_copy, original_img_copy, CV_GRAY2RGB);
    cv::drawContours( original_img_copy, welding, -1, BLUE_B, 2, 8, hwelding);
    cv::drawContours( original_img_copy, in_bag, -1, PINK_C, 2, 8, h);
    cv::imwrite(getImagePath("welding/"), original_img_copy);

    // find morphological operators outlines
    // ATENTION: here's a potential segfault by assessing structure[0] I'm abusing that theres always an element
    vector<vector<Point>> out_struct, in_struct;
    vector<Vec4i> hhh, hhhh;
    cv::findContours(opening_structure[0], in_struct, hhh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::findContours(closing_structure[0], out_struct, hhhh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    cv::cvtColor(original_image, original_image, CV_GRAY2RGB); // TODO: this is unnecessary
    if (!boxes.empty())
    {
        addBoundingBox(original_image, *boxes.rbegin());
        addAngle(original_image, angle);
    }
    cv::drawContours( original_image, in_bag, -1, PINK_C, 2, 8, h);
    cv::drawContours( original_image, in_struct, -1, PINK_C, 2, 8, hhh, 0, Point(0,0));
    cv::drawContours( original_image, out_bag, -1, BLUE_B, 2, 8, hh);
    cv::drawContours( original_image, out_struct, -1, BLUE_B, 2, 8, hhhh, 0, Point(0,50));

    cv::imwrite(getImagePath("decorated/"), original_image);
}


int main( int argc, const char** argv )
{
    /* for ( image_id = 0; image_id < cheese_imgs.size(); image_id++) */
    /// TODO: how to assess size(cheese_imgs)
    /* for ( image_id = 0; image_id < 47; image_id++) */

    std::string     current_img_path;
    HasDefect xx;
    WeldingFoldingType yy;
    std::tie(current_img_path, xx, yy) = cheese_imgs[0];

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
