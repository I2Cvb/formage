#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;


const auto RED = cv::Scalar(0,0,255);
const auto PINK = cv::Scalar(230,130,255);
const auto BLUE = cv::Scalar(255,0,0);
const auto LIGHTBLUE = cv::Scalar(255,255,160);
const auto GREEN = cv::Scalar(0,255,0);
const auto IMAGE_SCALE = .25;

class MyData
{
  public:
    const cv::Mat original_image;
    cv::Mat image;
    MyData( auto _image ) : original_image(_image)
    {
        original_image.copyTo(image);
    }
    void reset() { original_image.copyTo(image); };
    void update(auto _newImage) { _newImage.copyTo(image); };

};

class MyWindow
{
 protected:
    const string winName;
    MyData d;

 public:
    /* enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 }; */
    MyWindow( auto _winName, auto _image ):
        winName( _winName ), d( _image )
    {
        cv::namedWindow( winName, WINDOW_AUTOSIZE );
    }
    virtual ~MyWindow(void);
    virtual void showImage();
    virtual void reset();
};

MyWindow::~MyWindow()
{
    cv::destroyWindow( winName );
}

void MyWindow::reset()
{
    d.reset();
    showImage();
}

void MyWindow::showImage()
{
    cv::imshow( winName, d.image );
}

class MyOperationWindow : public MyWindow
{
  protected:
    virtual void setControls(){};
    virtual string getDescription() = 0;
  public:
    void apply( MyData& _d, std::vector<string>& _process_pile )
    {
        _d.update(d.image);
        _process_pile.push_back(getDescription());
    }
};

class MyThreshold : public MyOperationWindow
{
    int th;
  public:
    MyThreshold( auto _winName, auto _image)
        : MyWindow( _winName, _image ), th(125) { setControls(); };
    MyThreshold( auto _winName, auto _image, auto _th )
        : MyWindow( _winName, _image ), th(_th) { setControls(); };
  private:
    string getDescription() override;
    void setControls() override; //
    void thresholdImage();
    static void thresholdCallback( int _th, void* ); //
};

string MyThreshold::getDescription()
{
    return "th " + std::to_string(th);
}

void MyThreshold::setControls() //
{
    const std::string bar_name = winName + "_thBar";
    cv::createTrackbar( bar_name, winName, &th, 255, thresholdCallback );
}

void MyThreshold::thresholdCallback( int _th, void* ) //
{
    th = _th;
    thresholdImage();
    showImage();
}

void MyThreshold::thresholdImage()
{
    enum ThresholdType {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                         THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
    ThresholdType const threshold_type = BINARY;
    int const max_BINARY_value = 255;

    threshold( d.original_image, d.image, th, max_BINARY_value, threshold_type );
}


class MyApp : public MyWindow
{
    vector<string> process;
    MyOperationWindow* current_operation;
public:
    void showImage();
    char option(auto _option);
};

static void help()
{
    cout << "L (or a) - list image transformations\n"
         << "R (or s) - restore original image\n"
         << "t (or d) - activate threshold window\n"
         << "r (or f) - reset the operation window\n"
         << "(enter) (or g) - accept changes and kill the operation window\n"
         << "(backspace) (or h)- ignore changes and kill the operation window\n"
         << endl;
}

MyApp::option( auto _option )
{
    switch(_option)
    {
    case 't': case 'd':
        /// create the threshold window
        if (!current_operation)
            current_operation = new MyThreshold("theshold", d.image);
        break;
    case 'R': case 's':
        /// reset the whole application
        reset();
    case 'r': case 'f':
        /// reset the threshold window
        if (!current_operation)
            current_operation->reset();
        break;
    case 'L': case 'a':
        /// print the processes applied to the image until now
        for (const auto& p : process)
            std::cout << p << std::endl;
    case 'g':
        if (!current_operation){
            current_operation->apply(d.image, process);
            delete(current_operation);
        }
        break;
    case 'h':
        if (!current_operation)
            delete(current_operation);
        break;
    }
    return _option;
}


const char* keys =
{
    "{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
};


int main( int argc, const char** argv )
{
    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    std::string inputImage = parser.get<string>(0);

    // Load the source image. HighGUI use.
    cv::Mat image = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        return -1;
    }

    // TODO: check size and reduce it only if needed
    cv::resize(image, image, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    help();

    /// Create the GUI
    std::string winName = "main window"
    MyApp appHandle = MyApp(winName, image);

    /// Loop until the user kills the program
    const auto ESC_KEY = '\x1b';
    for (;;){
        // TODO: why the fuck appHandle is not declared???
        // TODO: Why this does not work? if ( appHandle.option((char) waitKey(0)) == ESC_KEY )
        int c = waitKey(0);
        if ( (char) c == ESC_KEY )
            goto exit_main;
        else
            appHandle.option((char) c);
    }

exit_main:
    std::cout << "Exit" << std::endl;
    destroyWindow(winName);
    return 0;
}


