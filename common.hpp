#ifndef COMMON_H
#define COMMON_H
#include <cinttypes>
#include <cstdint>
#include <vector>

enum class Player: uint8_t{
    White = 0,
    Black = 1
};


uint64_t squareIndexToBitboard(uint8_t);

std::vector<uint8_t> getSquareIndexesFromBitboard(uint64_t bitboard);

inline uint8_t getSquareRank(uint8_t square){
    return square >> 3;
}

inline uint8_t getSquareFile(uint8_t square){
    return square % 8;
}

inline Player getOpponentPlayer(Player player){
    return player == Player::White ? Player::Black : Player::White;
}

#endif