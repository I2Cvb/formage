
#include <gtest/gtest.h>

TEST(test_case_name, test_name)
{

}

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();

}
