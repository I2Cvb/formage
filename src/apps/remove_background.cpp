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
  MyData( cv::Mat _image ) : original_image(_image)
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
  MyWindow( const string&  _winName, const cv::Mat&  _image ):
    winName( _winName ), d( _image )
  {
    cv::namedWindow( winName, WINDOW_AUTOSIZE );
  }
  virtual ~MyWindow();
  void showImage();
  void reset();
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
  /* virtual void setControls(){}; */
  virtual string getDescription() {}
public:
  MyOperationWindow( auto _winName, auto _image) : MyWindow( _winName, _image ) {}
  void apply( MyData& _d, std::vector<string>& _process_pile )
  {
    _d.update(d.image);
    _process_pile.push_back(getDescription());
  }
};

class MyThreshold : public MyOperationWindow
{
  enum ThresholdType : int {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                      THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
  int th;
  ThresholdType threshold_type;
public:
  MyThreshold( auto _winName, auto _image)
    : MyOperationWindow( _winName, _image ), th(125), threshold_type(BINARY_INVERTED) { _initWindow(); }
  MyThreshold( auto _winName, auto _image, auto _th )
    : MyOperationWindow( _winName, _image ), th(_th), threshold_type(BINARY) { _initWindow(); }
  static void thresholdCallback( int _th, void* ptr);
private:
  string getDescription() override;
  void setControls(); //
  void thresholdImage();
  void _initWindow();
  static void _typeWorkaround( int _type, void* ptr);
};

void MyThreshold::_initWindow()
{
  setControls();
  thresholdImage();
  showImage();
}

void MyThreshold::_typeWorkaround( int _type, void* ptr)
{
  MyThreshold* that = (MyThreshold*) (ptr);
        switch (_type)
        {
        case 0:
                that->threshold_type = BINARY;
                break;
        case 1:
                that->threshold_type = BINARY_INVERTED;
                break;
        case 2:
                that->threshold_type = THRESHOLD_TRUNCATED;
                break;
        case 3:
                that->threshold_type = THRESHOLD_TO_ZERO;
                break;
        case 4:
                that->threshold_type = THRESHOLD_TO_ZERO_INVERTED;
                break;
        default:
                that->threshold_type = BINARY;
        }

        that->thresholdImage();
        that->showImage();
}

void MyThreshold::setControls()
{

  const std::string val_bar = winName + "_thValueBar";
  const std::string type_bar = winName + "_thTypeBar";
  int puta = BINARY;

  cv::createTrackbar( type_bar, winName, &puta, 5, MyThreshold::_typeWorkaround,this);
  cv::createTrackbar( val_bar, winName, &th, 255, MyThreshold::thresholdCallback,this);
}

void MyThreshold::thresholdCallback( int _th, void* ptr)
{
  MyThreshold* that = (MyThreshold*) (ptr);
  // that->th = _th; /// there's no need for this assignation since the trackbar was initialized with a reference to th
  that->thresholdImage();
  that->showImage();
}

string MyThreshold::getDescription()
{
  return "th " + std::to_string(th);
}

void MyThreshold::thresholdImage()
{
  int const max_BINARY_value = 255;
  threshold( d.original_image, d.image, th, max_BINARY_value, threshold_type );
}


class MyApp : public MyWindow
{
  vector<string> process;
  // std::unique_ptr<MyOperationWindow> current_operation;
  MyOperationWindow* current_operation;
public:
  MyApp(const std::string& _winName, const cv::Mat&  _image);

  char option(char _option);
  ~MyApp();
};

MyApp::~MyApp()
{
  if (current_operation != nullptr)
    delete(current_operation);
  //cv::destroyWindow( winName );
}

MyApp::MyApp(const string &_winName, const Mat &_image) :
  MyWindow( _winName, _image ),
  current_operation(nullptr)
{
  showImage();
}

char MyApp::option( char _option )
{

  switch(_option)
  {
  case 't': case 'd':
      /// create the threshold window
      if (current_operation == nullptr)
          current_operation = new MyThreshold("theshold", d.image);
      break;
  case 'R': case 's':
      /// reset the whole application
      reset();
  case 'r': case 'f':
      /// reset the threshold window
      if (current_operation != nullptr)
          current_operation->reset();
      break;
  case 'L': case 'a':
      /// print the processes applied to the image until now
      for (const auto& p : process)
          std::cout << p << std::endl;
      break;
  case 'g':
      if (current_operation != nullptr)
      {
          current_operation->apply(d, process);
          delete(current_operation);
      }
      break;
  case 'h':
      if (current_operation != nullptr)
          delete(current_operation);
      break;
  }
  showImage();
  return _option;
}


const char* keys =
{
  "{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
};


static void help();

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
  std::string winName = "main window";
  MyApp appHandle = MyApp(winName, image);
  appHandle.option('t');

  /// Loop until the user kills the program
  const auto ESC_KEY = '\x1b';
  while ( appHandle.option((char) waitKey(0)) != ESC_KEY )
    {
      /* until ESC */
    }
  std::cout << "Exit" << std::endl;
  return 0;
}


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
