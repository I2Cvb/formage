#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout <<   "no help"      << endl;
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
    void setImagePath(const string& _s) {imagePath = &_s;};
    string* getImagePath() {return &imagePath;};
private:
    void preprocessImage( const Mat& inImage, Mat& outImage );

    const string* winName;
    const string* imagePath;
    const Mat* image;
};


GCApplication gcapp;

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
    gcapp.setImagePath( parser.get<string>(0) );

    // Load the source image. HighGUI use.
    Mat image = imread( gcapp.getImagePath(), CV_LOAD_IMAGE_GRAYSCALE );
    if(image.empty())
    {
        std::cerr << "Cannot read image file: " << gcapp.getImagePath() << std::endl;
        return -1;
    }

    // TODO: check size and reduce it only if needed
    cv::resize(image, image, cv::Size(), 0.28, 0.28);
    help();

    /// Create the GUI
    const string winName = "image";
    namedWindow( winName, WINDOW_AUTOSIZE );

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

    std::cout << " nothing here" << std::endl;
}

void GCApplication::setImageAndWinName( const Mat& _image, const string& _winName  )
{
    if( _image.empty() || _winName.empty() )
        return;
    image = &_image;
    winName = &_winName;
}

void GCApplication::preprocessImage( const Mat& inImage, Mat& outImage )
// TODO: add image->copy(out)
{

}

void GCApplication::showImage()
{
    if( image->empty() || winName->empty() )
        return;

    // Treat the image
    Mat res;
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

    imshow( *winName, res );
}
