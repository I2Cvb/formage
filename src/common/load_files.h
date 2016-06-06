#ifndef MY_LOAD_IMGS_H
#define MY_LOAD_IMGS_H

#include <iostream>
#include <tuple>

enum WeldingFoldingType { FOLDED, UNFOLDED };
enum HasDefect { OK, DEFECT };

using my_images = std::tuple<std::string, HasDefect, WeldingFoldingType >;

int num_images = 40;
static my_images const cheese_imgs[] = {
    std::make_tuple("./A/08_03_2016  09_05_38,207.bmp", OK, FOLDED),
    /* std::make_tuple("./A/08_03_2016  09_05_27,885.bmp", OK, FOLDED), */
    /* std::make_tuple("./A/08_03_2016  09_05_39,622.bmp", OK, FOLDED), */
    std::make_tuple("./A/08_03_2016  09_06_06,897.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_05_47,611.bmp", OK, FOLDED),
    std::make_tuple("./A/08_03_2016  09_06_25,349.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_43,667.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_28,497.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_08,151.bmp", OK, FOLDED),
    std::make_tuple("./B/08_03_2016  09_11_18,165.bmp", OK, FOLDED),
    std::make_tuple("./C/08_03_2016  09_07_46,137.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_31,997.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_20,991.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_22,148.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_19,30.bmp" , OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_07_59,487.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_34,660.bmp", OK , FOLDED ),
    std::make_tuple("./C/08_03_2016  09_08_23,563.bmp", OK , FOLDED ),
    std::make_tuple("./D/08_03_2016  09_13_07,35.bmp" , OK, FOLDED),
    std::make_tuple("./D/08_03_2016  09_13_31,67.bmp" , OK, FOLDED),
    std::make_tuple("./D/08_03_2016  09_13_18,67.bmp" , OK, FOLDED),
    std::make_tuple("./E/08_03_2016  08_56_30,507.bmp", OK , FOLDED ),
    std::make_tuple("./E/08_03_2016  08_56_04,428.bmp", OK , FOLDED ),
    std::make_tuple("./E/08_03_2016  08_56_17,67.bmp" , OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_53_06,100.bmp", OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_52_39,828.bmp", OK , FOLDED ),
    std::make_tuple("./F/08_03_2016  08_53_26,751.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_22,154.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_32,248.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_33,663.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_55,274.bmp", OK , FOLDED ),
    std::make_tuple("./G/08_03_2016  09_02_43,665.bmp", OK , FOLDED ),
    std::make_tuple("./H/08_03_2016  08_58_02,182.bmp", OK, FOLDED),
    std::make_tuple("./H/08_03_2016  08_57_50,442.bmp", OK, FOLDED),
    std::make_tuple("./H/08_03_2016  08_58_13,658.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_01_04,419.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_01_14,406.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_25,699.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_52,155.bmp", OK, FOLDED),
    std::make_tuple("./I/08_03_2016  09_00_53,570.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_03,723.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_22,953.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_13,824.bmp", OK, FOLDED),
    std::make_tuple("./J/08_03_2016  09_04_11,974.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_10_16,829.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_09_45,273.bmp", OK, FOLDED),
    std::make_tuple("./K/08_03_2016  09_09_55,857.bmp", OK, FOLDED)
}; 


#endif /* MY_LOAD_IMG_H */
