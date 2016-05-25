
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <gtest/gtest.h>
#include <tuple>

using param_struct_t = std::pair<std::string, bool>;

class IntegrationTestFixture : public ::testing::TestWithParam<param_struct_t>
{
     protected:
        virtual void setUp()
        {}
        virtual void tearDown()
        {}
};


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

TEST_P(IntegrationTestFixture, testImage)
{
    std::string path;
    bool expectedOutput;
    auto param = GetParam();
    std::tie<std::string,bool>(path, expectedOutput) = param;

    cv::Mat img;
    ASSERT_NO_THROW(img = cv::imread(getTestDataPath() + path)) << "Image reading exception";

    EXPECT_TRUE(img.cols > 0) << "Empty image";
    EXPECT_TRUE(img.rows > 0) << "Empty image";
    EXPECT_EQ(expectedOutput, isNicelySealed(img));

}

//param_struct_t foo{std::string("xx.png"), false};
static const std::vector<param_struct_t> param_data{ {"xx.png", false},
                                                     {"yy.png", true} };


INSTANTIATE_TEST_CASE_P(InstaName,
                        IntegrationTestFixture,
                        ::testing::Values(param_struct_t{"xx.png", false},
                                          param_struct_t{"yy.png", true}));

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}

