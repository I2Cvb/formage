#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates GrabCut segmentation -- select an object in a region\n"
            "and then grabcut will attempt to segment it out.\n"
            "Call:\n"
            "./p <image_name>\n"
        "\nSelect a rectangular area around the object you want to segment\n" <<
        "\nHot keys: \n"
        "\tESC - quit the program\n"
        "\tr - restore the original image\n"
        "\tn - next iteration\n"
        "\n"
        "\tleft mouse button - set rectangle\n"
        "\n"
        << endl;
}

const auto RED = cv::Scalar(0,0,255);
const auto PINK = cv::Scalar(230,130,255);
const auto BLUE = cv::Scalar(255,0,0);
const auto LIGHTBLUE = cv::Scalar(255,255,160);
const auto GREEN = cv::Scalar(0,255,0);

class GCApplication
{
public:
    enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
    static const int radius = 2;
    static const int thickness = -1;

    void reset();
    void setImageAndWinName( const Mat& _image, const string& _winName );
    void showImage() const;
    void mouseClick( int event, int x, int y, int flags, void* param );
private:
    void setRectInMask();

    const string* winName;
    const Mat* image;
    Mat mask;

    uchar rectState, lblsState, prLblsState;
    bool isInitialized;

    Rect rect;
    vector<Point> snake;

    int iterCount = 0;
};


GCApplication gcapp;
static void on_mouse( int event, int x, int y, int flags, void* param )
{
    gcapp.mouseClick( event, x, y, flags, param );
}

const char* keys =
{
    "{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
};

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
    Mat image = imread( inputImage, 0 );
    if(image.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        return -1;
    }

    // TODO: check size and reduce it only if needed
    cv::resize(image, image, cv::Size(), 0.28, 0.28);
    help();

    const string winName = "image";
    namedWindow( winName, WINDOW_AUTOSIZE );
    setMouseCallback( winName, on_mouse, 0 );

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
        case 'n':
            cout << "<" << iterCount_ << "... ";
            cout << "rect must be determined>" << endl;
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

    isInitialized = false;
    rectState = NOT_SET;
    lblsState = NOT_SET;
    prLblsState = NOT_SET;
    iterCount = 0;
}

void GCApplication::setImageAndWinName( const Mat& _image, const string& _winName  )
{
    if( _image.empty() || _winName.empty() )
        return;
    image = &_image;
    winName = &_winName;
    mask.create( image->size(), CV_8UC1);
    reset();
}

void GCApplication::showImage() const
{
    if( image->empty() || winName->empty() )
        return;

    // Treat the image
    Mat res;
    Mat binMask;
    image->copyTo( res );
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

    if( rectState == IN_PROCESS || rectState == SET )
        rectangle( res, Point( rect.x, rect.y ), Point(rect.x + rect.width, rect.y + rect.height ), GREEN, 2);

    imshow( *winName, res );
}


void GCApplication::mouseClick( int event, int x, int y, int flags, void* )
{
    std::cout << "mouse click" << endl;
}
