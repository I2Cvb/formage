#ifndef MY_IMGPRO_H
#define MY_IMGPRO_H

#include "my_core.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#include <iostream>

using namespace cv;
using namespace std;

class MyThreshold : public MyOperationWindow
{
  enum ThresholdType : int {BINARY, BINARY_INVERTED, THRESHOLD_TRUNCATED,
                      THRESHOLD_TO_ZERO, THRESHOLD_TO_ZERO_INVERTED };
  int th;
  ThresholdType threshold_type;
public:
  MyThreshold( const std::string _winName, const cv::Mat& _image)
    : MyOperationWindow( _winName, _image ), th(125), threshold_type(BINARY_INVERTED) { _initWindow(); }
  MyThreshold( const std::string _winName, const cv::Mat& _image, int _th )
    : MyOperationWindow( _winName, _image ), th(_th), threshold_type(BINARY) { _initWindow(); }
  static void thresholdCallback( int _th, void* ptr);
private:
  string getDescription() override;
  void setControls(); //
  void thresholdImage();
  void _initWindow();
  static void _typeWorkaround( int _type, void* ptr);
};


class MyMorphology : public MyOperationWindow
{
	enum OperationType { OPENING=2, CLOSING, GRADIENT, TOP_HAT, BLACK_HAT }; // morphology operation codes are 2-6
	enum ElementType { RECTANGLE, CROSS, ELIPSE};
	OperationType operation;
	ElementType element;
	int morph_size;
public:
	MyMorphology( const std::string _winName, const cv::Mat& _image)
		: MyOperationWindow( _winName, _image ),
		  operation(OPENING),
		  element(RECTANGLE),
		  morph_size(5) { _initWindow(); }
	static void morphologyCallback( int _th, void* ptr);
private:
	string getDescription() override;
	void setControls();
	void morphImage();
	void _initWindow();
	static void _elementWorkaround( int _elem, void* ptr);
	static void _operationWorkaround( int _op, void* ptr);
};

class MyBgRemoval : MyOperation
{
        const string bg_path;
public:
        MyBgRemoval() : bg_path("../../testdata/A/background.bmp") {}
        MyBgRemoval(const string& _p) : bg_path(_p) {}
        std::string getDescription();
        void operate(const cv::Mat& _input, cv::Mat& _output);
        ~MyBgRemoval() = default;
};

#endif /* MY_IMGPRO_H */
