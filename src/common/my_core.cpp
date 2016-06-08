#include "my_core.h"

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

void MyApp::reset()
{
  process.clear();
  MyWindow::reset();
}

cv::Mat loadTestImage(const std::string& inputImage)
{
    Mat image_orig = imread( inputImage, CV_LOAD_IMAGE_GRAYSCALE );
    if(image_orig.empty())
        throw std::runtime_error("Cannot read image file: "+inputImage);

    cv::resize(image_orig, image_orig, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    return image_orig;
}
