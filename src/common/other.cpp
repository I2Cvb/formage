#include "other.h"


void findBag::getBackgroundMask(cv::Mat& _outImage)
// TODO: add image->copy(out)
{
    const string bkgPath = "../../testdata/A/background.bmp";
    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
        throw std::runtime_error("Cannot read image file: "+bkgPath);

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    cv::threshold( background, _outImage, 250, 255, cv::THRESH_BINARY );
    myRegionGrowing(_outImage,_outImage,5,cv::MORPH_ERODE);
}

void findBag::remove_background(cv::Mat& image)// const Mat& _inImage, Mat& _outImage )
// TODO: add image->copy(out)
{
    const string _out_path = out_path+"background_removed/"+std::to_string(image_id)+".png";
    const string bkgPath = "../../testdata/A/background.bmp";
    Mat background = imread( bkgPath, CV_LOAD_IMAGE_GRAYSCALE );
    if(background.empty())
        throw std::runtime_error("Cannot read image file: "+bkgPath);

    cv::resize(background, background, cv::Size(), IMAGE_SCALE, IMAGE_SCALE);
    /* _inImage.copyTo( _outImage ); */
    /* _outImage -= background; */
    /* cv::absdiff(background, _outImage, _outImage); */
    cv::absdiff(background, image, image);
}

void findBag::binarize(cv::Mat &ioImage){
    std::string const OUT_DIR = "xx/binarize/";
    Mat _image, _img_th, _img_otsu, _img_adapt, _img_combination;
    ioImage.copyTo(_image);

    // sik wrapper
    thresholdImage (_image, _img_th, 5);
    // othsu
    // cv::threshold( _image, _img_otsu, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU );
    // cv::imwrite(getImagePath(OUT_DIR+"otsu/"), _img_otsu);
    // Adaptive threshold
    // cv::adaptiveThreshold(_image, _img_adapt, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37, 0 );
    // cv::imwrite(getImagePath(OUT_DIR+"adapt/"), _img_adapt);

    _img_th.copyTo(ioImage);
}

/*void clean(cv::Mat &_ioImage){
  Mat _image, background_mask;
  _ioImage.copyTo(_image);
  findBag::clean(_image, _ioImage);
  }*/

void findBag::clean(cv::Mat &_inImage, cv::Mat &_outImage){
    std::string const OUT_DIR = "xx/clean/";
    Mat _image, background_mask;

    // Eliminate the background borders
    //findBag::getBackgroundMask(background_mask);
    //_inImage.copyTo(_image, background_mask);

    _inImage.copyTo(_image);
    cv::Mat _img_close_a, _img_close_b, _img_close_c, _img_close_d;
    cv::Mat _img_open_a, _img_open_b, _img_open_c;
    // perform the morphology operation
    myRegionGrowing(_image, _img_close_a, 5, cv::MORPH_CLOSE);
    cv::imwrite(getImagePath(OUT_DIR+"close_a/"), _img_close_a);
    myRegionGrowing(_image, _img_close_b, 10, cv::MORPH_CLOSE);
    cv::imwrite(getImagePath(OUT_DIR+"close_b/"), _img_close_b);
    myRegionGrowing(_image, _img_close_c, 20, cv::MORPH_CLOSE);
    cv::imwrite(getImagePath(OUT_DIR+"close_c/"), _img_close_c);
    myRegionGrowing(_image, _img_close_d, 30, cv::MORPH_CLOSE);
    cv::imwrite(getImagePath(OUT_DIR+"close_d/"), _img_close_d);

    myRegionGrowing(_image, _img_open_a, 2, cv::MORPH_OPEN);
    cv::imwrite(getImagePath(OUT_DIR+"open_a/"), _img_open_a);
    myRegionGrowing(_image, _img_open_b, 5, cv::MORPH_OPEN);
    cv::imwrite(getImagePath(OUT_DIR+"open_b/"), _img_open_b);
    myRegionGrowing(_image, _img_open_c, 10, cv::MORPH_OPEN);
    cv::imwrite(getImagePath(OUT_DIR+"open_c/"), _img_open_c);

}

void findBag::findContours( cv::Mat &_inImage,
        std::vector<std::vector<cv::Point>> &bag_outline,
        std::vector<cv::Vec4i> &hierarchy,
        //cv::Mat &_image_to_decorate=nullptr)
        cv::Mat &_image_to_decorate)
{
    //CV_Assert( !mask.empty() ); //TODO: assert that the images are same size etc..
    //std::cout << "in: " << _inImage.size() << "out: " << _image_to_decorate.size() << std::endl;
    cv::Mat xx, kk;
    _inImage.copyTo(xx);
    _inImage.copyTo(kk);

    cv::findContours(xx, bag_outline, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::cvtColor(kk, kk, CV_GRAY2RGB); // TODO: this is unnecessary
    cv::drawContours( kk, bag_outline, -1, GREEN, 2, 8, hierarchy);
    kk.copyTo(_image_to_decorate);
}

int findBag::getBiggestOtulineIndx( std::vector<std::vector<cv::Point>> const &bag_outline )
{
    if (bag_outline.size() == 0 )
        throw std::runtime_error("no boundaries found in: "+image_id);

    auto it = std::max_element(bag_outline.begin(), bag_outline.end(),
            [](auto &a, auto &b){return cv::contourArea(a) < cv::contourArea(b);});
    auto idx_max = std::distance(bag_outline.begin(), it);

    return idx_max;
}

void findBag::smoothContours( cv::Mat &_inImage,
        std::vector<cv::Point> const &_inContour,
        std::vector<cv::Point> &_outContour,
        //cv::mat &_image_to_decorate=nullptr)
        cv::Mat &_image_to_decorate)
        // The function smoothes an outline and draws only the output outline into _image_to_decorate
{
    /* _outContour.clear(); */
    /* _outContour.insert(_outContour.end(), _inContour.begin(), _inContour.end()); */
    cv::approxPolyDP(_inContour, _outContour, 10, true);
    std::vector<std::vector<cv::Point>> cc = {_outContour};
    cv::drawContours( _image_to_decorate, cc, -1, RED, 1, 8);

}

void findBag::findHull( std::vector<std::vector<cv::Point>> const &contours,
        std::vector<cv::Point> &hull,
        //cv::Mat &_image_to_decorate=nullptr)
        cv::Mat &_image_to_decorate)
{
    std::vector<cv::Point> myPoints;
    for( auto& c : contours)
        myPoints.insert(myPoints.end(), c.begin(), c.end());

    cv::convexHull( Mat(myPoints), hull, false );

    std::vector<std::vector<cv::Point>> hhull = {hull};
    drawContours( _image_to_decorate, hhull, 0, RED, 2, 8, vector<Vec4i>(), 0, Point() );
}


void xx::decorate(cv::Mat &_image, vector<vector<Point>> &contour)
{
    cv::cvtColor(_image, _image, CV_GRAY2RGB);
    std::vector<std::vector<cv::Point>> _smooth;
    /* cv::drawContours(_image, contour, -1, GREEN, 2, 8); */
    for (auto const &c : contour)
    {
        std::vector<cv::Point> s;
        cv::approxPolyDP(c,s, 4, true);
        _smooth.push_back(s);
    }
    cv::drawContours(_image, _smooth, -1, RED, -1, 8);

    for (auto const &c : _smooth)
        for (auto const &p : c)
            cv::circle(_image, p, 4, GREEN);
    /* _smooth.clear(); */
    /* for (auto const &c : contour) */
    /* { */
    /*     std::vector<cv::Point> s; */
    /*     cv::approxPolyDP(c,s, 7, true); */
    /*     _smooth.push_back(s); */
    /* } */
    /* cv::drawContours(_image, _smooth, -1, GREEN, 1, 8); */
}

void xx::findClosings(cv::Mat const &_inImage, vector<vector<vector<Point>>> &levels, vector<vector<cv::Mat>> &structuring_elements, double angle=0.0)
{
    std::string const OUT_DIR = "morph/close";
    cv::Mat _img_morph;
    int morph_size[] = {5, 10, 20, 30};
    vector<cv::Mat> _structuring_elements;
    for (int ii=0; ii<4; ii++)
    {
        cv::Mat _img_xx;
        vector<vector<Point>> delineation;
        vector<Vec4i> h;

        display_iteration = "close"+std::to_string(ii);

        myRegionGrowing_rotated(_inImage, _img_morph, _structuring_elements, morph_size[ii], 0.2, cv::MORPH_CLOSE, angle);
        _img_morph.copyTo(_img_xx);
        cv::findContours(_img_xx, delineation, h, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
        levels.push_back(delineation);
        xx::decorate(_img_morph, delineation);
        cv::imwrite(getImagePath(OUT_DIR+std::to_string(ii)+"/"), _img_morph);
    }
    structuring_elements.push_back(_structuring_elements);
}

void xx::findOpenings(cv::Mat const &_inImage, vector<vector<vector<Point>>> &levels, vector<vector<cv::Mat>> &structuring_elements, double angle=0.0)
{
    std::string const OUT_DIR = "morph/open";
    cv::Mat _img_morph;
    int morph_size[] = {3, 5, 15, 30};
    vector<cv::Mat> _structuring_elements;
    for (int ii=0; ii<4; ii++)
    {
        cv::Mat _img_xx;
        vector<vector<Point>> delineation;
        vector<Vec4i> h;

        display_iteration = "open"+std::to_string(ii);

        myRegionGrowing_rotated(_inImage, _img_morph, _structuring_elements, morph_size[ii], 0.2, cv::MORPH_OPEN, angle);
        _img_morph.copyTo(_img_xx);
        cv::findContours(_img_xx, delineation, h, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
        levels.push_back(delineation);
        xx::decorate(_img_morph, delineation);
        cv::imwrite(getImagePath(OUT_DIR+std::to_string(ii)+"/"), _img_morph);
    }
    structuring_elements.push_back(_structuring_elements);
}

double xx::findRotationAngle(cv::Mat &_img, vector<RotatedRect> &boxes)
{
    cv::Mat _img_xx;
    _img.copyTo(_img_xx);
    vector<vector<Point>> delineation;
    vector<Vec4i> h;
    double angle=0.0;

    cv::findContours(_img_xx, delineation, h, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    if (!delineation.empty()){
        auto it = std::max_element(delineation.begin(), delineation.end(),
                [](auto &a, auto &b){return cv::contourArea(a) < cv::contourArea(b);});
        RotatedRect box = minAreaRect(cv::Mat(*it));
        boxes.push_back(box);
        angle = box.angle;
        if (box.size.width <= box.size.height) {
            angle -= 90;
        }
    }
    return angle;
}
}

void addBoundingBox(cv::Mat img, RotatedRect &box)
{
    Point2f vertices[4];
    box.points(vertices);
    for (int i = 0; i < 4; i++)
        line(img, vertices[i], vertices[(i+1)%4], RED);
}

void addAngle(cv::Mat img, double angle)
{
    auto s = img.size();
    int const length = 50;
    Point P1(s.width/2, s.height/2);
    Point P2;

    P2.x =  (int)round(P1.x + length * cos(angle * CV_PI / 180.0));
    P2.y =  (int)round(P1.y + length * sin(angle * CV_PI / 180.0));
    std::cout << P1 << "     " << P2 << std::endl;
    circle(img, P1, 4 , GREEN);
    circle(img, P2, 4 , GREEN);
    line(img, P1, P2, RED);
}