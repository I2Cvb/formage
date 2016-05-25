#ifndef MY_CORE_H
#define MY_CORE_H

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

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
  void reset() { original_image.copyTo(image); }
  void update( const cv::Mat& _newImage) { _newImage.copyTo(image); }

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

class MyOperationWindow : public MyWindow
{
protected:
  virtual string getDescription()=0;
public:
  MyOperationWindow( const std::string _winName, const cv::Mat& _image) : MyWindow( _winName, _image ) {}
  void apply( MyData& _d, std::vector<string>& _process_pile )
  {
    _d.update(d.image);
    _process_pile.push_back(getDescription());
  }
};

class MyOperation
{
protected:
  virtual string getDescription()=0;
  virtual void operate(const cv::Mat& _input, cv::Mat& _output)=0;
};

class MyApp : public MyWindow
{
protected:
  vector<string> process;
  // std::unique_ptr<MyOperationWindow> current_operation;
  MyOperationWindow* current_operation;
public:
  MyApp(const std::string& _winName, const cv::Mat&  _image);
  void reset();
  virtual ~MyApp();
};

#endif /* MY_CORE_H */
