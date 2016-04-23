
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
    cv::Mat a = cv::imread("../../testdata/xx.png");

    cv::imshow("hola", a);
    cv::waitKey(0);

    return 0;
}
