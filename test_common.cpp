#include "test_common.hpp"
#include <cstdio>
#include <iostream>


bool testSquareIndexToBitboard(){
    const auto expected_1 = squareIndexToBitboard(0);
    const auto expected_2 = squareIndexToBitboard(1);
    const auto expected_4 = squareIndexToBitboard(2);
    const auto expected_8 = squareIndexToBitboard(3);
    const auto expected_16 = squareIndexToBitboard(4);
    const auto expected_2to63 = squareIndexToBitboard(63);
    std::printf("expected 2: %u\n", expected_2);
    std::printf("expected 4: %u\n", expected_4);
    std::printf("expected 8: %u\n", expected_8);
    std::printf("expected 16: %u\n", expected_16);
    std::cout<<"expected 2to63: "<<expected_2to63<<std::endl;
    //std::printf("expected 2to63: %PRIu64\n", expected_2to63);
    return true;
}