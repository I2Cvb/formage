
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <gtest/gtest.h>

bool isNicelySealed(const cv::Mat &cheese_img)
{
    return false;
}

static std::string getTestDataPath()
{
    char * testDataPath_env = getenv ("FORMAGE_TESTDATA_PATH");
    std::string testDataPath;

    if (testDataPath_env)
    {
        testDataPath = testDataPath_env;
    }
    else
    {
        testDataPath = "../../testdata/";
    }

    return testDataPath;

}

TEST(test_case_name, test_name)
{
    cv::Mat img;
    getTestDataPath();
    ASSERT_NO_THROW(img = cv::imread(getTestDataPath() + "/xx.png")) << "Hi ha hagut una excepcio";

    EXPECT_TRUE(img.cols > 0) << "L'imatge probablement esta vuida!!!";
    EXPECT_TRUE(img.rows > 0) << "L'imatge probablement esta vuida!!!";
    EXPECT_FALSE(isNicelySealed(img));
}

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
