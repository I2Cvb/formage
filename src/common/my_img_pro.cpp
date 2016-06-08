#include "my_core.h"
#include "my_img_pro.h"

/********************************************************************************
 * MY THRESHOLD
 *******************************************************************************/

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
  return "threshold, type: " + std::to_string(threshold_type) + ", val: " + std::to_string(th);
}

void MyThreshold::thresholdImage()
{
  int const max_BINARY_value = 255;
  threshold( d.original_image, d.image, th, max_BINARY_value, threshold_type );
}


/********************************************************************************
 * MY MORPHOLOGY
 *******************************************************************************/

string MyMorphology::getDescription()
{
  return "morph, op:" + std::to_string(operation) + ", " +
            "elem: " + std::to_string(element) + ", " +
            "size: " + std::to_string(morph_size);
}

void MyMorphology::morphImage()
{
    /// The morph_size is considered to be the length between the center of the structural element and its border
    /// morph_size = (structural_element size / 2) - 1
    const auto size = cv::Size( 2*morph_size + 1, 2*morph_size+1 );
    const auto p = Point (morph_size, morph_size);

    cv::Mat structure_element = getStructuringElement(element, size, p );
    cv::Mat _midleImage;

    morphologyEx(  d.original_image, _midleImage, operation, structure_element );
    d.update(_midleImage);
}


void MyMorphology::_initWindow()
{
  setControls();
  morphImage();
  showImage();
}

void MyMorphology::_operationWorkaround( int _op, void* ptr)
{
  MyMorphology* that = (MyMorphology*) (ptr);
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

        that->morphImage();
        that->showImage();
}

void MyMorphology::_elementWorkaround( int _elem, void* ptr)
{
  MyMorphology* that = (MyMorphology*) (ptr);
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

        that->morphImage();
        that->showImage();
}

void MyMorphology::setControls()
/// TODO: error when operating the Trackbar. It produces a SegFault.
/// 		- the element bar is the one producing the error
///  		- the work around is to duplicate that bar.
/// 		- if the inital bar (element_bar) is removed, then the SegFault is produced by operation_bar
{

  const std::string operation_bar = winName + "_morphOperationBar";
  const std::string element_bar = winName + "_thElementBar";
  const std::string element_bar2 = winName + "_thElementBar";
  const std::string size_bar = winName + "_thSizeBar";
  int op;
  int elem;
  const int max_size = 30;

  cv::createTrackbar( operation_bar, winName, &op, 5, MyMorphology::_operationWorkaround, this);
  cv::createTrackbar( element_bar, winName, &elem, 3, MyMorphology::_elementWorkaround, this);
  cv::createTrackbar( element_bar2, winName, &elem, 3, MyMorphology::_elementWorkaround, this);
  cv::createTrackbar( size_bar, winName, &morph_size, max_size, MyMorphology::morphologyCallback,this);
}

void MyMorphology::morphologyCallback( int _th, void* ptr)
{
	MyMorphology* that = (MyMorphology*) (ptr);
	that->morphImage();
	that->showImage();
}

std::string MyBgRemoval::getDescription()
{
  return "remove bacground using \"" + bg_path +"\"";
}

void MyBgRemoval::operate(const cv::Mat &_input, cv::Mat& _output)
{
    Mat background = cv::imread( bg_path, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
    {
        std::cerr << "Cannot read image file: " << bg_path << std::endl;
        // TODO: how I through an error? shall I do an assert?
    }

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    cv::absdiff(background, _input, _output);
}


void myRegionGrowing_rotated(
        const cv::Mat& _inImage,
        cv::Mat& _outImage,
        std::vector<cv::Mat> &structuring_elements,
        int morph_size,
        double aspect_ratio,
        cv::MorphTypes morph_type,
        double angle
        )
{
    //TODO: fix for angles where the element would get out of the bounding square
    std::string const OUT_DIR = "xx/";
    int morph_height = std::floor(morph_size*aspect_ratio);
    cv::Point element_center( morph_size, morph_height );
    cv::Mat element = getStructuringElement( MORPH_RECT,
                                             Size( 2*morph_size + 1, 2*morph_height+1 ),
                                             element_center
                                           );
    cv::Mat element_square( Size(2*morph_size + 1,2*morph_size + 1), element.type());
    element_square.setTo(cv::Scalar::all(0));
    element.copyTo(element_square.rowRange(morph_size-morph_height, morph_size+morph_height+1));

    cv::Mat rotated_element(Size(2*morph_size + 1,2*morph_size + 1), element.type());

    cv::Mat rotation = cv::getRotationMatrix2D( Point(morph_size, morph_size), 90 - angle, 1.0);
    cv::warpAffine(element_square, rotated_element, rotation, rotated_element.size());

    structuring_elements.push_back(rotated_element);
    morphologyEx(  _inImage, _outImage, morph_type, rotated_element );
}

void myRegionGrowing(const cv::Mat& _inImage, cv::Mat& _outImage, int morph_size,
                     cv::MorphTypes morph_type,
                     cv::MorphShapes morph_shape,
                     double angle
                     )
{
    cv::Mat element = getStructuringElement( morph_shape, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(  _inImage, _outImage, morph_type, element );
}

