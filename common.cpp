#include "common.hpp"

uint64_t squareIndexToU64(uint8_t square_index){
    return (uint64_t)1 << square_index;
}

std::vector<uint8_t> getSquareIndexesFromBitboard(uint64_t bitboard){
    std::vector<uint8_t> result{};
    uint8_t current_index;
    for(current_index=0; current_index<64; ++current_index){
        if(bitboard & (uint64_t)1 << current_index){
            result.push_back(current_index);
        }
    }
    return result;
}

inline uint8_t getSquareRank(uint8_t square){
    return square >> 3;
}

inline uint8_t getSquareFile(uint8_t square){
    return square % 8;
}

inline Player getOpponentPlayer(Player player){
    return player == WHITE ? BLACK : WHITE;
}
