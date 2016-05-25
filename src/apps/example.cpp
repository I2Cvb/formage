
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

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

const auto RED = cv::Scalar(0,0,255);
const auto PINK = cv::Scalar(230,130,255);
const auto BLUE = cv::Scalar(255,0,0);
const auto LIGHTBLUE = cv::Scalar(255,255,160);
const auto GREEN = cv::Scalar(0,255,0);
const auto IMAGE_SCALE = .25;
RNG rng(12345);

const char* keys =
{
    "{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
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

void preprocessImage(cv::Mat& image)// const Mat& _inImage, Mat& _outImage )
// TODO: add image->copy(out)
{
    const string bkgPath = "../../testdata/A/background.bmp";
    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
    {
        std::cerr << "Cannot read image file: " << bkgPath << std::endl;
    }

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    /* _inImage.copyTo( _outImage ); */
    /* _outImage -= background; */
    /* cv::absdiff(background, _outImage, _outImage); */
    cv::absdiff(background, image, image);
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

    std::cout << "num of contours: " << contours.size() << "->";
    // eliminate small contours
    contours.erase(
            std::remove_if(
                contours.begin(), contours.end(),
                [](const vector<Point>& c)
                { const int min_size = 200; return c.size() < min_size; }
                // or a functor/plain function/Boost.Lambda expression
                ), contours.end()
            );
    std::cout << contours.size() << std::endl;

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

void getBag2(const cv::Mat& _inImage, vector<vector<Point>>& bagOutline)
{
    //Extract the contours so that
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cv::Mat bwImage;
    cv::cvtColor( _inImage, bwImage, CV_RGB2GRAY);

    /// Find contours
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
    Mat image_orig = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image_orig.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        //return -1; //TODO: THIS IS COMPLETELY WRONG TO BE DONE LIKE THAT
    }

    cv::resize(image_orig, image_orig, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    return image_orig;
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

//   cout << "findWelding : _midleImageType " << _midleImage.type() << endl;
//   cout << "writting image in temporal file" << endl;
//    cv::imwrite("/tmp/xx.png", _midleImage);
//   cout << _midleImage <<endl;

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
     _outWeldingOutlines.erase(_outWeldingOutlines.begin()+2, _outWeldingOutlines.end());

}

int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string inputImage = parser.get<string>(0);
    Mat image = loadTestImage(inputImage);

    /// Create the GUI
    const string winName = "image";
    namedWindow( winName, WINDOW_AUTOSIZE );

    std::cout << "Starting..." << std::endl;
    std::cout << "Press esc to exit .." << std::endl;
    // Find bag;
    preprocessImage(image);
    thresholdImage(image, image, 100);
    cvtColor(image, image, CV_GRAY2RGB); // TODO: this is unnecessary
    vector<vector<Point>> bag_outline;
    getBag2(image,bag_outline);

    // Find welding
    Mat image2 = loadTestImage(inputImage);
    vector<vector<Point>> welding;
    vector<Vec4i> hierarchy;
    vector<RotatedRect> welding_roi;
    findWelding(image2, image2, welding, hierarchy);
    getWeldingRoi(welding, welding_roi);

    /// Decorate the image
    Mat imge3 = loadTestImage(inputImage);
    cvtColor(imge3, imge3, CV_GRAY2RGB);
    decorateImage(imge3, bag_outline, welding, welding_roi);
//    decorateImage(imge3, bag_outline);

    /// Show the image
    imshow( winName, imge3);
    cv::imwrite("/tmp/xx.png", imge3);

    while ( (char) waitKey(0) != '\x1b')
    {}
    std::cout << "Exiting .." << std::endl;

}
