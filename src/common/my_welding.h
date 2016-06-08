#ifndef MY_WELDING_H
#define MY_WELDING_H

#include "my_core.h"
#include "my_img_pro.h"
#include "other.h"


class MyWelding : public MyOperationWindow
{
	enum OperationType { OPENING=2, CLOSING, GRADIENT, TOP_HAT, BLACK_HAT }; // morphology operation codes are 2-6
	enum ElementType { RECTANGLE, CROSS, ELIPSE};
	OperationType operation;
	ElementType element;
	int morph_size;
    const string winControls;

public:
	MyWelding( std::string const _winName, const cv::Mat& _image)
		: MyOperationWindow( _winName, _image ),
		  operation(OPENING),
		  element(RECTANGLE),
		  winControls(_winName+"_ctrols"),
		  morph_size(5) { _initWindow(); }
	static void morphologyCallback( int _th, void* ptr);
	void showImage();
private:
	string getDescription() override;
	void setControls();
	void procesImage();
	void _initWindow();
	static void _elementWorkaround( int _elem, void* ptr);
	static void _operationWorkaround( int _op, void* ptr);
};

#endif /* MY_WELDING_H */
