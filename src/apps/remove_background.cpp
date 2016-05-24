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



class MyAppXX : public MyApp
{
    public:
        char option(char _option);
}


char MyAppXX::option( char _option )
{

  switch(_option)
{
  case 't': case 'd':
      /// create the threshold window
      if (current_operation == nullptr)
          current_operation = new MyThreshold("theshold", d.image);
      else
        cout << "there's already an operation being done" << endl;
      break;
  case 'm':
      /// create the morpholy window
      if (current_operation == nullptr)
          current_operation = new MyMorphology("morphology", d.image);
      else
        cout << "there's already an operation being done" << endl;
      break;
  case 'R': case 's':
      /// reset the whole application
      reset();
  case 'r': case 'f':
      /// reset the operation window
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


//void preprocessImage(cv::Mat& image)// const Mat& _inImage, Mat& _outImage )
//// TODO: add image->copy(out)
//{
//    const string bkgPath = "../../testdata/A/background.bmp";
//    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
//    if(background.empty())
//    {
//        std::cerr << "Cannot read image file: " << bkgPath << std::endl;
//    }

//    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
//    /* _inImage.copyTo( _outImage ); */
//    /* _outImage -= background; */
//    /* cv::absdiff(background, _outImage, _outImage); */
//    cv::absdiff(background, image, image);
//}




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
  MyAppXX appHandle = MyAppXX(winName, image);
  appHandle.option('m');

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
       << "m - activate morphology window\n"
       << "r (or f) - reset the operation window\n"
       << "(enter) (or g) - accept changes and kill the operation window\n"
       << "(backspace) (or h)- ignore changes and kill the operation window\n"
       << endl;
}
