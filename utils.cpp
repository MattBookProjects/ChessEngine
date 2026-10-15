#include "utils.hpp"

uint8_t pow_uint8(uint8_t base, uint8_t exp){
    uint8_t result = 1;
    for(int i = 0; i < exp; i++){
        result *= base;
    }
    return result;
}

uint8_t *array_xor(uint8_t *target, uint8_t *other, uint8_t size){
    for(int i = 0; i <size; i++){
        target[i] ^= other[i];
    }
    return target;
}