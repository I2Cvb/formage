
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
    // Image load
    cv::Mat a = cv::imread("../../testdata/A/A05_38_gt_gray.png", CV_LOAD_IMAGE_GRAYSCALE);
    cv::resize(a, a, cv::Size(), 0.1, 0.1);

    std::cout << a.size() <<std::endl;
    cv::Mat b(a, cv::Rect(10,10,20,20));
    std::cout << b << std::endl << std::endl;

    cv::imshow("hola", b);
    cv::waitKey(0);

    return 0;
}
