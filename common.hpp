#ifndef COMMON_H
#define COMMON_H
#include <cstdint>


uint64_t squreIndexToU64(uint8_t);

std::vector<uint8_t> getSquareIndexesFromBitboard(uint64_t bitboard);

inline uint8_t getSquareRank(uint8_t square);

inline uint8_t getSquareFile(uint8_t square);

inline Player getOpponentPlayer(Player player);

#endif