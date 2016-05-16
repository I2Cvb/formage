#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout <<   "r - reset\n"
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

class GCApplication
{
public:
    enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
    static const int radius = 2;
    static const int thickness = -1;

    void reset();
    void setImageAndWinName( const Mat& _image, const string& _winName );
    void showImage();
    void setThreshold( int const th ) { thresholdValue = th; };
private:
    int preprocessImage( const Mat& _inImage, Mat& _outImage );
    void thresholdImage( const Mat& _inImage, Mat& _outImage );

    const string* winName;
    const Mat* image;
    Mat* outImage;
    int thresholdValue = -1;
};


GCApplication gcapp;

const char* keys =
{
    "{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
};

void Threshold_Demo( int th, void* )
{
    //std::cout << "threshold demo: < " << gcapp.getThreshold() << ", " << th;
    gcapp.setThreshold( th );
    gcapp.showImage();
    //std::cout << ", " << gcapp.getThreshold() << ">" << std::endl;
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

    // Load the source image. HighGUI use.
    Mat image = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        return -1;
    }

    // TODO: check size and reduce it only if needed
    cv::resize(image, image, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    help();

    /// Create the GUI
    const string winName = "image";
    const string threshold_bar_name = "image";
    int threshold_value = 0;
    namedWindow( winName, WINDOW_AUTOSIZE );
    createTrackbar( threshold_bar_name,
                    winName, &threshold_value,
                    255, Threshold_Demo );

    gcapp.setImageAndWinName( image, winName );
    gcapp.showImage();


    int iterCount_ = 0;

    for(;;)
    {
        int c = waitKey(0);
        switch( (char) c )
        {
        case '\x1b':
            cout << "Exiting ..." << endl;
            goto exit_main;
        case 'r':
            cout << endl;
            gcapp.reset();
            gcapp.showImage();
            break;
        case 'b':
            help();
            break;
        case 't':
            help();
            break;
        case 'f':
            help();
            break;
        }
    }

exit_main:
    destroyWindow( winName );
    return 0;
}

void GCApplication::reset()
{
    /* if( !mask.empty() ) */
    /*     mask.setTo(Scalar::all(GC_BGD)); */
    /* bgdPxls.clear(); fgdPxls.clear(); */
    /* prBgdPxls.clear();  prFgdPxls.clear(); */

    /* image->copyTo( outImage ); */
}

void GCApplication::setImageAndWinName( const Mat& _image, const string& _winName  )
{
    if( _image.empty() || _winName.empty() )
        return;
    image = &_image;
    winName = &_winName;
}

int GCApplication::preprocessImage( const Mat& _inImage, Mat& _outImage )
// TODO: add image->copy(out)
{
    const string bkgPath = "../../testdata/A/background.bmp";
    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
    {
        std::cerr << "Cannot read image file: " << bkgPath << std::endl;
        return -1;
    }

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    /* _inImage.copyTo( _outImage ); */
    /* _outImage -= background; */
    cv::absdiff(background, _outImage, _outImage);
    return 0;
 }

void GCApplication::thresholdImage( const Mat& _inImage, Mat& _outImage )
// TODO: add image->copy(out)
{
    enum ThresholdType {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                         THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
    ThresholdType const threshold_type = BINARY;
    int const max_BINARY_value = 255;

    threshold( _inImage, _outImage, thresholdValue, max_BINARY_value, threshold_type );
}

void GCApplication::showImage()
{
    if( image->empty() || winName->empty() )
        return;

    // Treat the image
    Mat res;
    image->copyTo( res );
    gcapp.preprocessImage(*image, res);
    gcapp.thresholdImage(res, res);

    /*if( !isInitialized )
        image->copyTo( res );
    else
    {
        getBinMask( mask, binMask );
        image->copyTo( res, binMask );
    }*/

    /// Set a contour and its rotated bounding box
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /* if( isInitialized ){ */
    /*     findContours( binMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE ); */
    /* } */
    /* if( isInitialized ) */
    /* { */
    /*     std::cout << "num of contours: " << contours.size() << "->"; */
    /*     // eliminate small contours */
    /*     contours.erase( */
    /*             std::remove_if( */
    /*                 contours.begin(), contours.end(), */
    /*                 [](const vector<Point>& c) */
    /*                 { const int min_size = 200; return c.size() < min_size; } */
    /*                 // or a functor/plain function/Boost.Lambda expression */
    /*                 ), contours.end() */
    /*             ); */
    /*     std::cout << contours.size() << " ..."; */
    /* } */

    /// Find the rotated rectangles for each contour
    /* vector<RotatedRect> minRect( contours.size() ); */
    /* for( size_t i = 0; i < contours.size(); i++ ) */
    /*     minRect[i] = minAreaRect( Mat(contours[i]) ); */

    /* for( size_t i = 0; i< contours.size(); i++ ) */
    /*     { */
    /*     Scalar color( rand()&255, rand()&255, rand()&255 ); */
    /*     // contour */
    /*     drawContours( res, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() ); */
    /*     // rotated rectangle */
    /*     Point2f rect_points[4]; minRect[i].points( rect_points ); */
    /*     for( int j = 0; j < 4; j++ ) */
    /*         line( res, rect_points[j], rect_points[(j+1)%4], color, 1, 8 ); */
    /*     } */

    imshow( *winName, res );
}

