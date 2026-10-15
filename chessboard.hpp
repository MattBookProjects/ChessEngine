#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <cinttypes>
//#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>
#include "utils.hpp"
#define WHITE_PAWN 0
#define WHITE_KNIGHT 1
#define WHITE_BISHOP 2
#define WHITE_ROOK 3
#define WHITE_QUEEN 4
#define WHITE_KING 5
#define BLACK_PAWN 6
#define BLACK_KNIGHT 7
#define BLACK_BISHOP 8
#define BLACK_ROOK 9
#define BLACK_QUEEN 10
#define BLACK_KING 11
#define WHITE 0
#define BLACK 1
#define SHORT_CASTLE 0
#define LONG_CASTLE 1

enum GameState{
    UNDECIDED = 0,
    WHITE_WIN = 1,
    BLACK_WIN = 2,
    DRAW = 3
};
/*

enum Piece{
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
};

enum Player {
    WHITE = 0,
    BLACK = 1
};

enum CastleSide{
    SHORT = 0,
    LONG = 1
};
*/

class Chessboard{
   /* private:
        uint8_t squares[64];
        enum Player player_to_move;
        bool canWhiteCastle;
        bool canBlackCastle;
     */   

    public:
        /*GameState getGameState();
        std::vector<Move> getLegalMoves();
        static bool isPlayerInCheck(uint8_t *squares, enum Player player);
        static uint8_t getKingPosition(uint8_t *squares, enum Player player);
        static bool isSquareAttackedByPlayer(uint8_t *board, uint8_t square);
        static bool doesPieceBelongToPlayer(enum Player player, uint8_t piece);
        static bool isPawnAttackingSquare(uint8_t *squares, uint8_t pawn_square, uint8_t attacked_square);  */
        static uint8_t *initChessboard(); 
        static void initPawns(uint8_t *chessboard);
        static void initKnights(uint8_t *chessboard);
        static void initBishops(uint8_t *chessboard);
        static void initRooks(uint8_t *chessboard);
        static void initQueens(uint8_t *chessboard);
        static void initKings(uint8_t *chessboad);
        static void initFlags(uint8_t *chessboad);
};

class Move{
    public:
        //virtual uint8_t *getMoveBitboard() = 0; 
        uint8_t *makeMove(uint8_t *chessboad);
        virtual uint8_t *constructMoveBitboard(uint8_t *chessboard) = 0;
};

class BasicMove:public Move{
    private:
        uint8_t old_square;
        uint8_t new_square;
        uint8_t piece;
    
    public:
        BasicMove(uint8_t piece, uint8_t old_square, uint8_t new_square);
        //uint8_t *makeMove(uint8_t *chessboad) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;
};

class TakeMove:public Move{
    private:
        uint8_t attacking_piece;
        uint8_t attacker_square;
        uint8_t target_piece;
        uint8_t target_square;

    public:
        TakeMove(uint8_t attacking_piece, uint8_t attacker_square, uint8_t target_piece, uint8_t target_square);
        //uint8_t *makeMove(uint8_t *chessboad) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;
};

class TakeWithPromotionMove:public Move{
    private:
        uint8_t attacking_pawn;
        uint8_t attacker_file;
        uint8_t target_piece;
        uint8_t target_file;
        uint8_t promoted_piece;
    public:
        TakeWithPromotionMove(uint8_t attacking_pawn, uint8_t attacker_file, uint8_t target_piece, uint8_t target_file, uint8_t promoted_piece);
        //uint8_t *makeMove(uint8_t *chessboad) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;

    };


class CastleMove:public Move{
    private: 
        uint8_t player;
        uint8_t side;
        
    public:
        CastleMove(uint8_t player, uint8_t side);
        //uint8_t *makeMove(uint8_t *chessboad) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;
};



class EnpassantMove:public Move{
    private:
        uint8_t player;
        uint8_t attacker_file;
        uint8_t target_file;
        
    
    public:
        EnpassantMove(uint8_t player, uint8_t attacker_file, uint8_t target_file);
        //uint8_t *makeMove(uint8_t *chessboard) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;
};

class PromotionMove:public Move{
    private:
        uint8_t pawn;
        uint8_t file;
        uint8_t promoted_piece;
    public:
        PromotionMove(uint8_t pawn, uint8_t file, uint8_t promoted_piece);
        //uint8_t *makeMove(uint8_t *chessboad) override;
        uint8_t *constructMoveBitboard(uint8_t *chessboard) override;
};


/*
class MoveFactory{
    static Move createBasicMove(uint8_t piece, uint8_t old_square, uint8_t new_square);
    static Move createTakeMove(uint8_t attacking_piece, uint8_t target_piece, uint8_t attacking_piece_square, uint8_t target_square);
    static Move createCastleMove(uint8_t side, uint8_t castle_type);


};
*/

void printChessboard(uint8_t *chessboard);

void printChessboardFancy(uint8_t *chessboard);

bool isPlayerInCheck(uint8_t *chessboard, uint8_t player);

bool isSquareAttackedByPawn(uint8_t *chessboard, uint8_t pawn, uint8_t square);

bool isSquareAttackedByKnight(uint8_t *chessboard, uint8_t knight, uint8_t square);

bool isSquareAttackedByBishop(uint8_t *chessboard, uint8_t bishop, uint8_t square);

bool isSquareAttackedByRook(uint8_t *chessboard, uint8_t rook, uint8_t square);

bool isSquareAttackedByQueen(uint8_t *chessboard, uint8_t queen, uint8_t square);

bool isSquareAttackedByKing(uint8_t *chessboard, uint8_t king, uint8_t square);

uint8_t *getAllPiecesBitboard(uint8_t *chessboard);

uint8_t *getPlayerPiecesBitboard(uint8_t *chessboard, uint8_t player);

std::vector<*Move> getPlayersLegalPawnMoves(uint8_t *chessboard, uint8_t player);

std::vector<*Move> getPawnLegalMoves(uint8_t *chessboard, uint8_t player, uint8_t pawn, uint8_t pawn_square, uint8_t oppoent_piece_index_start, uint8_t oppoenet_piece_index_end);



#endif