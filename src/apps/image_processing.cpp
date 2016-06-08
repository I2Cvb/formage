#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <tuple>
#include <stdexcept>

#include "common/my_core.h"
#include "common/my_img_pro.h"
#include "common/my_welding.h"
#include "common/load_files.h"

using namespace cv;
using namespace std;

class MyAppXX : public MyApp
{
public:
	MyAppXX ( const std::string& _winName, const cv::Mat&  _image)
		: MyApp( _winName, _image) {}
	char option(char _option);
private:
	void removeBackground();
};

void MyAppXX::removeBackground()
{
	MyBgRemoval bgr;
	bgr.operate(d.image, d.image);
	process.push_back( bgr.getDescription() );
}

char MyAppXX::option( char _option )
{

  switch(_option)
          {
          case 'b': /// remove background
                  removeBackground();
                  break;
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
                                  delete(current_operation); current_operation = nullptr;
                          }
                  break;
          case 'h':
                  if (current_operation != nullptr)
                          {
                                  delete(current_operation); current_operation = nullptr;
                          }
                  break;
          case 'w':
                  /// create the threshold window
                  if (current_operation == nullptr)
                          current_operation = new MyWelding("Welding", d.image);
                  else
                          cout << "there's already an operation being done" << endl;
                  break;
          //case 'j':

          //case 'k':
          }
  showImage();
  return _option;
}

const char* keys =
{
	"{help h||}{@image|../../testdata/A/A05_38.bmp|input image file}"
};


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





static void help();

int main( int argc, const char** argv )
{
    vector<vector<std::string>> image_configuration(num_images);

    std::string current_img_path;
    HasDefect xx;
    WeldingFoldingType yy;
    std::tie(current_img_path, xx, yy) = cheese_imgs[0];

    cv::Mat current_img = loadTestImage("../../testdata/"+current_img_path);
    help();

    /// Create the GUI
    std::string winName = "main window";
    MyAppXX appHandle = MyAppXX(winName, current_img);
    appHandle.option('w');

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
       << "b - remove background\n"
       << "m - activate morphology window\n"
       << "r (or f) - reset the operation window\n"
       << "(enter) (or g) - accept changes and kill the operation window\n"
       << "(backspace) (or h)- ignore changes and kill the operation window\n"
       << "j, k - to change the current image\n"
       << endl;
}
