#include "my_core.h"
#include "my_img_pro.h"
#include "my_welding.h"

/********************************************************************************
 * MY MORPHOLOGY
 *******************************************************************************/

string MyWelding::getDescription()
{
  return "morph, op:" + std::to_string(operation) + ", " +
            "elem: " + std::to_string(element) + ", " +
            "size: " + std::to_string(morph_size);
}

void MyWelding::procesImage()
{
    Mat original_image = d.original_image;
    Mat image;
    original_image.copyTo(image);

    Mat image_find_folding;
    Mat bag_outline_image;
    vector<vector<Point>> bag_outline;
    vector<Vec4i> hierarchy;
    vector<Point> hull;
    vector<Point> smooth_outline;

    /*
     * *********************************************************************
     *  Find bag
     * *********************************************************************
     */

    findBag::remove_background(image);

    Mat _image, background_mask;
    findBag::getBackgroundMask(background_mask);
    image.copyTo(_image, background_mask);

    findBag::binarize(_image);
    cv::imwrite(getImagePath("binarization/"), _image);

    vector<RotatedRect> boxes;
    double angle = xx::findRotationAngle(_image, boxes);

    // perform the morphology operation
    vector<cv::Mat> closing_structure;
    cv::Mat _img_close;
    myRegionGrowing_rotated(_image, _img_close, closing_structure, 15, 0.3, cv::MORPH_CLOSE, angle);

    vector<cv::Mat> opening_structure;
    cv::Mat _img_open;
    myRegionGrowing_rotated(_image, _img_open, opening_structure, 10, 0.2, cv::MORPH_OPEN, angle);

    // find bag outlines
    vector<vector<Point>> out_bag, in_bag;
    vector<Vec4i> h, hh;
    cv::findContours(_img_open, in_bag, h, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::findContours(_img_close, out_bag, hh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    auto out_idx = findBag::getBiggestOtulineIndx(out_bag);
    auto in_idx = findBag::getBiggestOtulineIndx(in_bag);

    cv::Mat welding_area;
    cv::Mat out_mask (original_image.size(), CV_8U);
    cv::drawContours( out_mask, out_bag, out_idx,255, -1);
    cv::Mat in_mask (original_image.size(), CV_8U);
    cv::drawContours( in_mask, in_bag, in_idx, 255, -1);
    std::cout << "out mask size: " << out_mask.size() << "in mask size: " << in_mask.size() << std::endl;
    cv::bitwise_xor(out_mask, in_mask, welding_area, out_mask);

    // find bag outlines
    vector<vector<Point>> welding;
    vector<Vec4i> hwelding;
    cv::findContours(welding_area, welding, hwelding, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::Mat original_img_copy;
    original_image.copyTo(original_img_copy);
    cv::cvtColor(original_img_copy, original_img_copy, CV_GRAY2RGB);
    cv::drawContours( original_img_copy, welding, -1, BLUE_B, 2, 8, hwelding);
    cv::drawContours( original_img_copy, in_bag, -1, PINK_C, 2, 8, h);
    cv::imwrite(getImagePath("welding/"), original_img_copy);

    // find morphological operators outlines
    // ATENTION: here's a potential segfault by assessing structure[0] I'm abusing that theres always an element
    vector<vector<Point>> out_struct, in_struct;
    vector<Vec4i> hhh, hhhh;
    cv::findContours(opening_structure[0], in_struct, hhh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::findContours(closing_structure[0], out_struct, hhhh, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    cv::cvtColor(original_image, original_image, CV_GRAY2RGB); // TODO: this is unnecessary
    if (!boxes.empty())
    {
        addBoundingBox(original_image, *boxes.rbegin());
        addAngle(original_image, angle);
    }
    cv::drawContours( original_image, in_bag, -1, PINK_C, 2, 8, h);
    cv::drawContours( original_image, in_struct, -1, PINK_C, 2, 8, hhh, 0, Point(0,0));
    cv::drawContours( original_image, out_bag, -1, BLUE_B, 2, 8, hh);
    cv::drawContours( original_image, out_struct, -1, BLUE_B, 2, 8, hhhh, 0, Point(0,50));

    d.update(_midleImage);
}


void MyWelding::_initWindow()
{
  cv::namedWindow( winControls, WINDOW_AUTOSIZE );
  setControls();
  procesImage();
  showImage();
}

void MyWelding::_operationWorkaround( int _op, void* ptr)
{
  MyWelding* that = (MyWelding*) (ptr);
        switch (_op)
        {
        case 0:
                that->operation = OPENING;
                break;
        case 1:
                that->operation = CLOSING;
                break;
        case 2:
                that->operation = GRADIENT;
                break;
        case 3:
                that->operation = TOP_HAT;
                break;
        case 4:
                that->operation = BLACK_HAT;
                break;
        default:
                that->operation = OPENING;
        }

        that->procesImage();
        that->showImage();
}

void MyWelding::_elementWorkaround( int _elem, void* ptr)
{
  MyWelding* that = (MyWelding*) (ptr);
        switch (_elem)
        {
        case 0:
                that->element = RECTANGLE;
                break;
        case 1:
                that->element = CROSS;
                break;
        case 2:
                that->element = ELIPSE;
                break;
        default:
                that->element = RECTANGLE;
        }

        that->procesImage();
        that->showImage();
}

void MyWelding::setControls()
/// TODO: error when operating the Trackbar. It produces a SegFault.
/// 		- the element bar is the one producing the error
///  		- the work around is to duplicate that bar.
/// 		- if the inital bar (element_bar) is removed, then the SegFault is produced by operation_bar
{

  std::string operation_bar = winName + "_morphOperationBar";
  std::string element_bar = winName + "_thElementBar";
  std::string element_bar2 = winName + "_thElementBar";
  std::string size_bar = winName + "_thSizeBar";
  int op;
  int elem;
  const int max_size = 30;

  cv::createTrackbar( operation_bar, winName, &op, 5, MyWelding::_operationWorkaround, this);
  cv::createTrackbar( element_bar, winName, &elem, 3, MyWelding::_elementWorkaround, this);
  cv::createTrackbar( element_bar2, winName, &elem, 3, MyWelding::_elementWorkaround, this);
  cv::createTrackbar( size_bar, winName, &morph_size, max_size, MyWelding::morphologyCallback,this);

  operation_bar = winControls + "_morphOperationBar";
  element_bar = winControls + "_thElementBar";
  element_bar2 = winControls + "_thElementBar";
  size_bar = winControls + "_thSizeBar";

  cv::createTrackbar( operation_bar, winControls, &op, 5, MyWelding::_operationWorkaround, this);
  cv::createTrackbar( element_bar, winControls, &elem, 3, MyWelding::_elementWorkaround, this);
  cv::createTrackbar( element_bar2, winControls, &elem, 3, MyWelding::_elementWorkaround, this);
  cv::createTrackbar( size_bar, winControls, &morph_size, max_size, MyWelding::morphologyCallback,this);

}

void MyWelding::morphologyCallback( int _th, void* ptr)
{
	MyWelding* that = (MyWelding*) (ptr);
	that->procesImage();
	that->showImage();
}

void MyWelding::showImage()
{
    MyWindow::showImage();
    cv::imshow(winControls, d.image);
}
