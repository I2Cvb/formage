#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <tuple>
#include <stdexcept>

using namespace cv;
using namespace std;

static void help()
{
    cout << "r - reset\n"
         << "b - remove background\n"
         << "t - threshold (not working)\n"
         << "f - find contours (not working)\n"
         << endl;
}

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
std::string const out_path("/tmp/folding_out/");


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

/* int num_images = 4;
   static my_images const cheese_imgs[] = {
   std::make_tuple("./B/08_03_2016  09_11_43,667.bmp", OK    , FOLDED),
   std::make_tuple("./B/08_03_2016  09_11_18,165.bmp", OK    , UNFOLDED),
   std::make_tuple("./K/08_03_2016  09_09_45,273.bmp", OK    , UNFOLDED),
   std::make_tuple("./K/08_03_2016  09_10_16,829.bmp", OK    , FOLDED),
   };*/



void thresholdImage( const Mat& _inImage, Mat& _outImage, int th)
// TODO: add image->copy(out)
{
    enum ThresholdType {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                         THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
    ThresholdType const threshold_type = BINARY;
    int const max_BINARY_value = 255;

    threshold( _inImage, _outImage, th, max_BINARY_value, threshold_type );
}

string getImagePath(std::string s, int i)
{
    std::string num = std::to_string(i);
    std::string dest = std::string( 5 - num.length(), '0').append( num); 
    return (out_path+s+dest+"_"+std::to_string(image_id)+".png");
}

string getImagePath(std::string s)
{
    return (out_path+s+std::to_string(image_id)+".png");
}

void getBackgroundMask(cv::Mat& _outImage)
// TODO: add image->copy(out)
{
    const string _out_path = out_path+"background_removed/"+std::to_string(image_id)+".png";
    const string bkgPath = "../../testdata/A/background.bmp";
    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
    {
        throw std::runtime_error("Cannot read image file: "+bkgPath);
    }

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
    {
        throw std::runtime_error("Cannot read image file: "+bkgPath);
    }

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    /* _inImage.copyTo( _outImage ); */
    /* _outImage -= background; */
    /* cv::absdiff(background, _outImage, _outImage); */
    cv::absdiff(background, image, image);
    cv::imwrite(_out_path, image);
}

void getBag(const cv::Mat& _inImage, cv::Mat& _outImage)
{
    //Extract the contours so that
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cv::Mat bwImage;
    cv::cvtColor( _inImage, bwImage, CV_RGB2GRAY);

    /// Find contours
    findContours( bwImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /* std::cout << "num of contours: " << contours.size() << "->"; */
    // eliminate small contours
    contours.erase(
            std::remove_if(
                contours.begin(), contours.end(),
                [](const vector<Point>& c)
                { const int min_size = 200; return c.size() < min_size; }
                // or a functor/plain function/Boost.Lambda expression
                ), contours.end()
            );
    /* std::cout << contours.size() << std::endl; */

    vector<vector<Point>> polyAprox(contours);

    for( size_t i = 0; i< contours.size(); i++ )
    {
        cout << polyAprox[i] << endl;
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        approxPolyDP(Mat(contours[i]), polyAprox[i], 3, true);
        drawContours( _outImage, contours, (int)i, color, 2, 8, vector<Vec4i>(), 0, Point() );
        drawContours( _outImage, polyAprox, (int)i, color, 2, 8, vector<Vec4i>(), 0, Point() );

    }

//    for( size_t i = 0; i< contours.size(); i++ )
//    {
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        approxPolyDP(Mat(contours[i]), polyAprox[i], 3, true);
//        drawContours( _outImage, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
//        /* drawContours( _outImage, polyAprox, (int)i, color, 2, 8, hierarchy, 0, Point() ); */
//    }
    /* findContours( img, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); */

    /* contours.resize(contours0.size()); */
    /* for( size_t k = 0; k < contours0.size(); k++ ) */
    /*     approxPolyDP(Mat(contours0[k]), contours[k], 3, true); */

}

void getBag2(const cv::Mat& _inImage, vector<vector<Point>>& contours2, vector<vector<Point>>& bagOutline)
{
    //Extract the contours so that
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cv::Mat bwImage;
    cv::cvtColor( _inImage, bwImage, CV_RGB2GRAY);

    /// Find contours
    findContours( bwImage, contours2, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours( bwImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<Point> myPoints;
    for( auto& c : contours)
        myPoints.insert(myPoints.end(), c.begin(), c.end());

    vector<Point> hull;
    convexHull( Mat(myPoints), hull, false );
    bagOutline.push_back(hull);

    /* drawContours( _outImage, hhh, 0, RED, 2, 8, vector<Vec4i>(), 0, Point() ); */
}

void decorateImage(cv::Mat& image, vector<vector<cv::Point>>& outline,
		   vector<vector<cv::Point>>& delineations, vector<RotatedRect> welding_roi)
{
	/// Draw the bag's outline
	drawContours( image, outline, 0, GREEN, 2, 8, vector<Vec4i>(), 0, Point() );

	/// Draw the Welding area
	for (size_t i = 0; i < delineations.size(); i++)
	    drawContours( image, delineations, i, GREEN, 1, 8, vector<Vec4i>(), 0, Point() );

	for( auto& r : welding_roi )
	  {
	     // rotated rectangle
	     Point2f rect_points[4];
	     r.points( rect_points );
	     for( int j = 0; j < 4; j++ )
		line( image, rect_points[j], rect_points[(j+1)%4], RED, 1, 8 );
	   }
}

void getWeldingRoi( std::vector<vector<cv::Point>>& delineations, vector<RotatedRect>& _outRoi )
{
  for ( auto& d : delineations)
     _outRoi.push_back(minAreaRect( Mat(d) ));
}

Mat loadTestImage(const std::string& inputImage)
{
    // TODO:[to-UNDERSTAND] loading and image and making a copy constructor
    // the image points to the same object
    //
    // Mat image_orig = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    // Mat image(image_orig);
    // image_orig.copyTo(image);

    // Load the source image. HighGUI use.
   //cout<< inputImage << endl;
    Mat image_orig = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image_orig.empty())
    {
        throw std::runtime_error("Cannot read image file: "+inputImage);
    }

    cv::resize(image_orig, image_orig, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    return image_orig;
}

void myRegionGrowing(const cv::Mat& _inImage, cv::Mat& _outImage, int morph_size=10,
                     cv::MorphTypes morph_type=cv::MORPH_OPEN,
                     cv::MorphShapes morph_shape=cv::MORPH_RECT
                     )
{
    cv::Mat element = getStructuringElement( morph_shape, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    cv::Mat _midleImage;
    morphologyEx(  _inImage, _midleImage, morph_type, element );
}

void findWelding(const cv::Mat& _inImage, cv::Mat& _outImage,
                         vector<vector<Point>>& _outWeldingOutlines,
                         vector<Vec4i>& _outHierancy)
{
    /// Do some Color morpholy opeartion
    enum OperationType { OPENING=2, CLOSING, GRADIENT, TOP_HAT, BLACK_HAT }; // morphology operation codes are 2-6
    enum ElementType { RECTANGLE, CROSS, ELIPSE};
    int morph_size = 10;

    cv::Mat element = getStructuringElement( RECTANGLE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    cv::Mat _midleImage;

    /// Apply the specified morphology operation
    morphologyEx(  _inImage, _midleImage, TOP_HAT, element );
    cv::imwrite(getImagePath("morph/"), _midleImage);

    /// Threshold and clean the output
    enum ThresholdType {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                         THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
    int const max_BINARY_value = 255;

    const unsigned char th = 100;
    cv::Mat bwImage;
    threshold( _midleImage, bwImage, th, max_BINARY_value, BINARY );
    morph_size = 4;
    cv::Mat element2 = getStructuringElement( RECTANGLE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

    /// Apply the specified morphology operation
    morphologyEx(  bwImage, bwImage, CLOSING, element );

    bwImage.copyTo(_outImage);

    /// Find contours
    /// CAREFUL: [unexpected library behavior] bwImage gets modified by findContours
    findContours( bwImage, _outWeldingOutlines, _outHierancy,
                  RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Keep only the two biggest regions, the rest are considered to be noise.
    /// take care that you are using perimeter to determine which areas are bigger.
    std::sort( _outWeldingOutlines.begin(), _outWeldingOutlines.end(),
                  [] (vector<Point>& a, vector<Point>& b) {return a.size()>b.size();}
                 );
    if ( _outWeldingOutlines.size()>2)
        _outWeldingOutlines.erase(_outWeldingOutlines.begin()+2, _outWeldingOutlines.end());

}

double folding_detector(cv::Mat const & _inImage, cv::Mat &_outImage)
{
    enum OperationType { OPENING=2, CLOSING, GRADIENT, TOP_HAT, BLACK_HAT }; // morphology operation codes are 2-6
    enum ElementType { RECTANGLE, CROSS, ELIPSE};
    int morph_size = 10;

    cv::Mat element = getStructuringElement( RECTANGLE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

    cv::Mat _midleImage;
    cv::Mat bwImage;

    /// Apply the specified morphology operation
    morphologyEx(  _inImage, _midleImage, CLOSING, element );

    _midleImage.copyTo(bwImage);
    //Extract the contours so that
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Find contours
    findContours( bwImage, contours, hierarchy,
                  CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
                  //RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0) );

    /* std::cout << "num of contours: " << contours.size() << "->"; */
    std::sort(contours.begin(), contours.end(),
              [](auto const &a, auto const &b) {return a.size() > b.size();});


    /* std::cout << "contour lengths are: "; */
    /* for ( auto const &c : contours ) */
    /*     std::cout << c.size() << ", " << std::endl; */

    vector<vector<Point>> bag_outline;
    vector<vector<Point>> bag_hull;

    bwImage.copyTo(_outImage);

    cv::imwrite(getImagePath("fold/"), _midleImage);
    cvtColor(_midleImage, _midleImage, CV_GRAY2RGB);
	drawContours( _midleImage, contours, 0, GREEN, 2, 8, hierarchy, 0, Point() );
    
    if (contours.empty()){ throw std::runtime_error("Empty contours"); }
    RotatedRect my_rectangle (minAreaRect(Mat(contours[0])));
   
    /* double folded_magnitude; */
    int bag_area = cv::contourArea(contours[0]);
    /* auto min_rectangle_area = cv::area(my_rectangle); */ //TODO: suputamadre !!
    /* auto puta[] (my_rectangle.size); */
    Point2f rect_points[4];
    my_rectangle.points( rect_points );
    vector<Point> my_rectangle_points;
    for (auto const &p : rect_points)
        my_rectangle_points.push_back(Point(p));

    double min_rectangle_area = (double) cv::contourArea(my_rectangle_points);
    double relation =  (min_rectangle_area-bag_area)/min_rectangle_area;

    /* std::cout << "A1 : " << bag_area << std::endl; */
    /* std::cout << "A2 : " << min_rectangle_area << std::endl; */
    /* std::cout << "Relation :" << (int)(relation * 1000)<< std::endl; */


    //////// TRY TO BREAK THE RECTANGLE INTO SHAPE AND COMPARE THE SHAPES
//    Point2f rect_points[4];
//    my_rectangle.points( rect_points );
//    for( int j = 0; j < 4; j++ )
//        line( _midleImage, rect_points[j], rect_points[(j+1)%4], RED, 1, 8 );
//
//    vector<Point> my_rectangle_points;
//    for (auto const &p : rect_points)
//        my_rectangle_points.push_back(Point(p));
//
//    std::cout << "Points size: " << my_rectangle_points.size() << std::endl;
//    int i =0;
//    for (auto const &p : my_rectangle_points)
//        std::cout << "Point " << i++ << ":" << p << std::endl;



    /* enum ShapeComparisonMethod {CV_CONTOURS_MATCH_I1, */
    /*     CV_CONTOURS_MATCH_I2, CV_CONTOURS_MATCH_I3 }; */
    /* double xx = matchShapes(contours[0], my_rectangle_points, CV_CONTOURS_MATCH_I1,0); */
    /* std::cout << "shape Match " << xx << std::endl; */

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    /* int idx = 0; */
    /* for( ; idx >= 0; idx = hierarchy[idx][0] ) */
    /* { */
    /*     Scalar color( rand()&255, rand()&255, rand()&255 ); */
    /*     drawContours( _midleImage, contours, idx, color, CV_FILLED, 8, hierarchy ); */
    /* } */
    cv::imwrite(getImagePath("fold2/", (int) (relation *1000)), _midleImage);

    return relation;
}

void processsImage(const std::string& _img_path, double &folded_index) {

    Mat image = loadTestImage(_img_path);
    cv::imwrite(getImagePath("original/"), image);

    Mat image_find_folding;
    // Find bag;
    remove_background(image);
    // sik wrapper
    /* thresholdImage(image, image, 100); */
    // othsu
    /* cv::threshold( image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU ); */
    cv::adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37, 0 );
    myRegionGrowing(image,image,100,cv::MORPH_CLOSE);
    Mat background_mask;
    getBackgroundMask(background_mask);

    Mat xx;
    Mat yy;
    Mat kk;
    image.copyTo(xx, background_mask);
    image.copyTo(yy, background_mask);
    image.copyTo(kk, background_mask);

    cv::imwrite(getImagePath("threshold1/"), image);


    vector<vector<Point>> bag_outline;
    vector<Vec4i> hierarchy;
    /* bitwise_not (xx,xx); */
    cv::findContours(xx, bag_outline, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cvtColor(kk, kk, CV_GRAY2RGB); // TODO: this is unnecessary
	drawContours( kk, bag_outline, -1, GREEN, 1, 8, hierarchy);
    cv::imwrite(getImagePath("puta2/"), kk);

    bag_outline.clear();
    hierarchy.clear();
    cvtColor(kk, kk, CV_RGB2GRAY); // TODO: this is unnecessary
    cv::findContours(kk, bag_outline, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    auto it = std::max_element(bag_outline.begin(), bag_outline.end(),
                     [](auto &a, auto &b){return cv::contourArea(a) < cv::contourArea(b);});
    auto idx_max = std::distance(bag_outline.begin(), it);
    cvtColor(yy, yy, CV_GRAY2RGB); // TODO: this is unnecessary
	/* drawContours( yy, bag_outline, idx_max, GREEN, CV_FILLED, 8, vector<Vec4i>(), 0, Point() ); */
	drawContours( yy, bag_outline, -1, GREEN, 1, 8, hierarchy);
    
    vector<Point> myPoints;
    for( auto& c : bag_outline)
        myPoints.insert(myPoints.end(), c.begin(), c.end());

    vector<Point> hull;
    convexHull( Mat(myPoints), hull, false );
    
    vector<vector<Point>> hhull = {hull};
	drawContours( yy, hhull, 0, RED, 2, 8, vector<Vec4i>(), 0, Point() );

    cv::imwrite(getImagePath("puta/"), yy);
    /*
     * folded_index = folding_detector(xx, image_find_folding );
     * cvtColor(image, image, CV_GRAY2RGB); // TODO: this is unnecessary
     * vector<vector<Point>> bag_outline;
     * vector<vector<Point>> bag_hull;
     * getBag2(image,bag_outline, bag_hull);
     */

    // Find welding
    /* Mat image2 = loadTestImage(_img_path); */
    /* vector<vector<Point>> welding; */
    /* /1* vector<Vec4i> hierarchy; *1/ */
    /* vector<RotatedRect> welding_roi; */
    /* findWelding(image2, image2, welding, hierarchy); */
    /* getWeldingRoi(welding, welding_roi); */

    /* /// Decorate the image */
    /* /1* Mat imge3 = loadTestImage(_img_path); *1/ */
    /* Mat imge3(image_find_folding); */
    /* cvtColor(imge3, imge3, CV_GRAY2RGB); */
    /* decorateImage(imge3, bag_outline, welding, welding_roi); */
/* //    decorateImage(imge3, bag_outline); */
    /* cv::imwrite(getImagePath("decorated/"), imge3); */


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

        /* std::cout << "ok" << std::endl; */
        std::cout << folded_index << std::endl;
    }
}
