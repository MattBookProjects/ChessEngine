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

constexpr std::array<Piece, 6> WHITE_PIECES = {
    Piece::WhitePawn,
    Piece::WhiteKnight,
    Piece::WhiteBishop,
    Piece::WhiteRook,
    Piece::WhiteQueen,
    Piece::WhiteKing
};
constexpr std::array<Piece, 6> BLACK_PIECES = {
    Piece::BlackPawn,
    Piece::BlackKnight,
    Piece::BlackBishop,
    Piece::BlackRook,
    Piece::BlackQueen,
    Piece::BlackKing
};;

inline std::array<Piece, 6> getPlayerPieces(Player player);

class PieceMoveBehaviour{
    public:
        virtual std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,
            uint8_t square, 
            Chessboard &cheessboard
        ) = 0;
        virtual bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) = 0;
        virtual std::unique_ptr<Move> getLegalPieceToSquareMove
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard,
            uint8_t target_square
        );
};

inline Piece getPlayerPawn(Player player);
inline Piece getPlayerKnight(Player player);
inline Piece getPlayerBishop(Player player);
inline Piece getPlayerRook(Player player);
inline Piece getPlayerQueen(Player player);
inline Piece getPlayerKing(Player player);


class DiagonalMoveBehaviour{
    public:
        std::vector<std::unique_ptr<Move>> getLegalMovesOnDiagonal(
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &chessboard,
            int8_t file_step,
            int8_t rank_step,
            PieceMoveBehaviour &piece_move_behaviour
        );

};


class StraightMoveBehaviour{
    public:
        std::vector<std::unique_ptr<Move>> getLegalMovesOnStraight(
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &chessboard,
            int8_t file_step,
            int8_t rank_step,
            PieceMoveBehaviour &piece_move_behaviour
        );

};

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
            Chessboard &cheessboard 
        ) override;
        inline bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
    
};

class KnightMoveBehaviour: public PieceMoveBehaviour{
    public:
        KnightMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            Chessboard &cheessboard
        ) override;
        inline bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
};

class BishopMoveBehaviour: public PieceMoveBehaviour{
    private:
        DiagonalMoveBehaviour &diagonal_move_behaviour;
    public:
        BishopMoveBehaviour(DiagonalMoveBehaviour &diagonal_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            Chessboard &cheessboard
        ) override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
        std::vector<std::unique_ptr<Move>> getLegalMovesFromDiagonal
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            Chessboard &cheessboard,
            uint8_t file_step,
            uint8_t rank_step
        );
};


class RookMoveBehaviour: public PieceMoveBehaviour{
    private:
        StraightMoveBehaviour &straight_move_behaviour;
    public:
        RookMoveBehaviour(StraightMoveBehaviour &straight_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard
        ) override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
};


class QueenMoveBehaviour: public PieceMoveBehaviour{
    private:
        StraightMoveBehaviour &straight_move_behaviour;
        DiagonalMoveBehaviour &diagonal_move_behaviour;
    public:
        QueenMoveBehaviour(StraightMoveBehaviour &straight_move_behaviour, DiagonalMoveBehaviour &diagonal_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard
        ) override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
};

class KingMoveBehaviour: public PieceMoveBehaviour{
    public:
        KingMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard
        ) override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) override;
       
};

const DiagonalMoveBehaviour diagonal_move_behaviour();
const StraightMoveBehaviour straight_move_behaviour();
const PawnMoveBehaviour white_pawn_move_behaviour(0, 6, 4, 1);
const PawnMoveBehaviour black_pawn_move_behaviour(7, 1, 3, -1);
const KnightMoveBehaviour knight_move_behaviour();
const BishopMoveBehaviour bishop_move_behaviour(&diagonal_move_behaviour);
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