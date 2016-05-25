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
