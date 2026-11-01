#ifndef PIECE_H
#define PIECE_H
#include <cstdint>
#include <vector>
#include <memory>
#include "move.hpp"
#include "common.hpp"
#include "chessboard.hpp"

class Move;
class Chessboard;
enum class Player: uint8_t;

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

inline const std::array<Piece, 6> getPlayerPieces(Player player){
    return player == Player::White ? WHITE_PIECES : BLACK_PIECES;
}
inline Piece getPlayerPawn(Player player){
    return player == Player::White ? Piece::WhitePawn : Piece::BlackPawn;
}

inline Piece getPlayerKnight(Player player){
    return player == Player::White ? Piece::WhiteKnight : Piece::BlackKnight;
}

inline Piece getPlayerBishop(Player player){
    return player == Player::White ? Piece::WhiteBishop : Piece::BlackBishop;
}

inline Piece getPlayerRook(Player player){
    return player == Player::White ? Piece::WhiteRook : Piece::BlackRook;
}

inline Piece getPlayerQueen(Player player){
    return player == Player::White ? Piece::WhiteQueen : Piece::BlackQueen;
}

inline Piece getPlayerKing(Player player){
    return player == Player::White ? Piece::WhiteKing : Piece::BlackKing;
}

class PieceMoveBehaviour{
    public:
        virtual std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,
            uint8_t square, 
            Chessboard &cheessboard
        ) const = 0;
        virtual bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const = 0;
        virtual std::unique_ptr<Move> getLegalPieceToSquareMove
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard,
            uint8_t target_square
        ) const;
};



class DiagonalMoveBehaviour{
    public:
        std::vector<std::unique_ptr<Move>> getLegalMovesOnDiagonal(
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &chessboard,
            int8_t file_step,
            int8_t rank_step,
            const PieceMoveBehaviour &piece_move_behaviour
        ) const;

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
            const PieceMoveBehaviour &piece_move_behaviour
        ) const;

};



class PawnMoveBehaviour: public PieceMoveBehaviour{
    private:
        uint8_t penultimate_rank;
        uint8_t starting_rank;
        uint8_t enpassant_rank;
        int8_t forward_move_step;


    public:
        PawnMoveBehaviour(uint8_t starting_rank, uint8_t penultimate_rank, uint8_t enpassant_rank, int8_t forward_move_step);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,
            uint8_t square,
            Chessboard &cheessboard 
        ) const override;
        inline bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
    
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
        ) const override;
        inline bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
};

class BishopMoveBehaviour: public PieceMoveBehaviour{
    private:
        const DiagonalMoveBehaviour &diagonal_move_behaviour;
    public:
        BishopMoveBehaviour(const DiagonalMoveBehaviour &diagonal_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            Chessboard &cheessboard
        ) const override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
        /*std::vector<std::unique_ptr<Move>> getLegalMovesFromDiagonal
        (
            Player player,
            Piece piece,  
            uint8_t square, 
            Chessboard &cheessboard,
            uint8_t file_step,
            uint8_t rank_step
        ) const;*/
};


class RookMoveBehaviour: public PieceMoveBehaviour{
    private:
        const StraightMoveBehaviour &straight_move_behaviour;
    public:
        RookMoveBehaviour(const StraightMoveBehaviour &straight_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard
        ) const override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
};


class QueenMoveBehaviour: public PieceMoveBehaviour{
    private:
        const StraightMoveBehaviour &straight_move_behaviour;
        const DiagonalMoveBehaviour &diagonal_move_behaviour;
    public:
        QueenMoveBehaviour(const StraightMoveBehaviour &straight_move_behaviour, const DiagonalMoveBehaviour &diagonal_move_behaviour);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece, 
            uint8_t square, 
            Chessboard &cheessboard
        ) const override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
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
        ) const override;
        bool doesPieceAttackSquare(Chessboard &cheessboard, uint8_t piece_square, uint8_t target_square) const override;
       
};



extern const DiagonalMoveBehaviour diagonal_move_behaviour;
extern const StraightMoveBehaviour straight_move_behaviour;
extern const PawnMoveBehaviour white_pawn_move_behaviour;
extern const PawnMoveBehaviour black_pawn_move_behaviour;
extern const KnightMoveBehaviour knight_move_behaviour;
extern const BishopMoveBehaviour bishop_move_behaviour;
extern const RookMoveBehaviour rook_move_behaviour;
extern const QueenMoveBehaviour queen_move_behaviour;
extern const KingMoveBehaviour king_move_behaviour;

/*
std::unique_ptr<PieceMoveBehaviour> white_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> black_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> knight_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> bishop_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> rook_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> queen_pawn_move_behaviour;
std::unique_ptr<PieceMoveBehaviour> king_move_behaviour;
*/
const PieceMoveBehaviour &getPieceMoveBehaviour(Piece piece);




#endif