#ifndef PIECE_H
#define PIECE_H
#include <cstdint>
#include <vector>
#include <memory>
#include "move.hpp"
#include "common.hpp"


class Chessboard;

enum class Piece: int8_t{
    Empty = -1,
    
    WhitePawn   = 0,
    WhiteKnight = 1,
    WhiteBishop = 2,
    WhiteRook   = 3,
    WhiteQueen  = 4,
    WhiteKing   = 5,

    BlackPawn   = 6,
    BlackKnight = 7,
    BlackBishop = 8,
    BlackRook   = 9,
    BlackQueen  = 10,
    BlackKing   = 11
};

constexpr std::array<Piece, 6> WHITE_PIECES;
constexpr std::array<Piece, 6> BLACK_PIECES;

inline std::array<Piece, 6> getPlayerPieces(Player player);

class PieceMoveBehaviour{
    public:
        virtual std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) = 0;
        virtual bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) = 0;
  
};

inline Piece getPlayerPawn(Player player);
inline Piece getPlayerKnight(Player player);
inline Piece getPlayerBishop(Player player);
inline Piece getPlayerRook(Player player);
inline Piece getPlayerQueen(Player player);
inline Piece getPlayerKing(Player player);

class PawnMoveBehaviour: public PieceMoveBehaviour{
    private:
        uint8_t penultimate_rank;
        uint8_t starting_rank;
        uint8_t enpassant_rank;
        int8_t forward_move_step;


    public:
        PawnMoveBehaviour(uint8_t starting_rank, uint8_t penultimate_rank, uint8_t enpassant_rank, int8_t forward_move_step);
        std::vector<std::unique_ptr<Move>> getLegalMoves(
            Player player,
            Piece piece,
            uint8_t square,
            std::unique_ptr<Chessboard> cheessboard 
        ) override;
        inline bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
    
};

class KnightMoveBehaviour: public PieceMoveBehaviour{
    public:
        KnightMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        inline bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
};

class BishopMoveBehaviour: public PieceMoveBehaviour{
    public:
        BishopMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
};


class RookMoveBehaviour: public PieceMoveBehaviour{
    public:
        RookMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
};


class QueenMoveBehaviour: public PieceMoveBehaviour{
    public:
        QueenMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
};

class KingMoveBehaviour: public PieceMoveBehaviour{
    public:
        KingMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(std::unique_ptr<Chessboard> cheessboard, uint8_t piece_square, uint8_t target_square) override;
};

const PawnMoveBehaviour white_pawn_move_behaviour;
const PawnMoveBehaviour black_pawn_move_behaviour;
const KnightMoveBehaviour knight_move_behaviour;
const BishopMoveBehaviour bishop_move_behaviour;
const RookMoveBehaviour rook_move_behaviour;
const QueenMoveBehaviour queen_move_behaviour;
const KingMoveBehaviour king_move_behaviour;

/*
std::unique_ptr<PieceMoveBehaviour> white_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> black_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> knight_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> bishop_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> rook_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> queen_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> king_move_behaviour;
*/
std::unique_ptr<PieceMoveBehaviour> getPieceMoveBehaviour(Piece piece);

#endif