#ifndef MOVE_H
#define MOVE_H
#include <cstdint>
#include <array>
#include <memory>
#include "piece.hpp"

class Chessboard;
enum class Player: uint8_t;
enum class Piece: int8_t;

enum class CastleSide: uint8_t{
    SHORT_CASTLE = 0,
    LONG_CASTLE = 1
};


class Move{
    public:
        virtual Chessboard applyMove(Chessboard &chessboard);
        virtual std::array<uint64_t, 12> constructMoveBitboard() = 0;
};

class BasicMove:public Move{
    private:
        uint8_t old_square;
        uint8_t new_square;
        Piece piece;
    
    public:
        BasicMove(Piece piece, uint8_t old_square, uint8_t new_square);
        Chessboard applyMove(Chessboard &chessboard) override;
        std::array<uint64_t, 12> constructMoveBitboard() override;
};

class TakeMove:public Move{
    private:
        Piece attacking_piece;
        uint8_t attacker_square;
        Piece target_piece;
        uint8_t target_square;

    public:
        TakeMove(Piece attacking_piece, uint8_t attacker_square, Piece target_piece, uint8_t target_square);
        std::array<uint64_t, 12> constructMoveBitboard() override;
};

class TakeWithPromotionMove:public Move{
    private:
        Piece pawn;
        uint8_t attacker_square;
        Piece target_piece;
        uint8_t target_square;
        Piece promoted_piece;
    public:
        TakeWithPromotionMove(
            Piece pawn, 
            uint8_t pawn_square, 
            Piece target_piece, 
            uint8_t target_square, 
            Piece promoted_piece
        );
        std::array<uint64_t, 12> constructMoveBitboard() override;

};


class CastleMove:public Move{
    private: 
        Player player;
        CastleSide side;
        
    public:
        CastleMove(uint8_t player, uint8_t side);
        Chessboard applyMove(Chessboard &chessboard) override;
        std::array<uint64_t, 12> constructMoveBitboard() override;
};



class EnpassantMove:public Move{
    private:
        Piece pawn;
        uint8_t attacker_old_square;
        uint8_t attacker_new_square;
        Piece target_pawn;
        uint8_t target_square;
        
    
    public:
        EnpassantMove(Piece pawn, uint8_t old_square, uint8_t new_square, Piece target_pawn, uint8_t target_square);
        std::array<uint64_t, 12> constructMoveBitboard() override;
};

class PromotionMove:public Move{
    private:
        Piece pawn;
        uint8_t old_square;
        uint8_t new_square;
        Piece promoted_piece;
    public:
        PromotionMove(Piece pawn, uint8_t old_square, uint8_t new_square, Piece promoted_piece);
        std::array<uint64_t, 12> constructMoveBitboard() override;
};


std::array<std::unique_ptr<Move>, 4> getPromotionMoves
(
    Player player, 
    Piece pawn, 
    uint8_t old_square, 
    uint8_t new_square
);

std::array<std::unique_ptr<Move>, 4> getTakeWithPromotionMoves
(
    Player player, 
    Piece pawn, 
    uint8_t pawn_square, 
    Piece target_piece, 
    uint8_t target_square
);

#endif
