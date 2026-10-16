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

typedef uint8_t Player;
typedef uint8_t Piece;
typedef int8_t Result;

const Player WHITE;
const Player BLACK;

const Piece = WHITE_PAWN;
const Piece = WHITE_KNIGHT;
const Piece = WHITE_BISHOP;
const Piece = WHITE_ROOK;
const Piece = WHITE_QUEEN;
const Piece = WHITE_KING
const Piece = BLACK_PAWN;
const Piece = BLACK_KNIGHT;
const Piece = BLACK_BISHOP;
const Piece = BLACK_ROOK;
const Piece = BLACK_QUEEN
const Piece = BLACK_KING

const Result WHITE_WIN;
const Result DRAW;
const Result BLACK_WIN;
const std::array<Piece, 6> WHITE_PIECES;
const std::array<Piece, 6> BLACK_PIECES;




class Chessboard{
    private:
        std::array<uint8_t, 96> bitboard{};
        enum Player player_to_move;
        bool canWhiteCastle;
        bool canBlackCastle;
        int8_t white_enpassant;
        int8_t black_enpassant;
        bool is_finished;
        Result result;

    public:
        Chessboard(
            std::array<uint64_t, 12> bitboard = {},
            Player player_to_move = WHITE, 
            bool can_white_castle = true, 
            bool can_black_castle = true, 
            int8_t white_enpassant = -1, 
            int8_t black_enpassant = -1
        );
        static Chessboard initChessboard();
        //std::array<uint8_t, 96> getBitboard(); 
        void initPawns();
        void initKnights();
        void initBishops();
        void initRooks();
        void initQueens();
        void initKings();
        //void initFlags();

        uint64_t getPieceBitboard(Piece piece);
        uint64_t getPlayerPiecesBitboard(Player player);
        uint64_t getAllPiecesBitboard()

        bool isGameFinished();
       
        Result getGameResult();

        uint8_t getPlayersKingSquare(Player player);
       
        bool isSquareAttackedByPlayer(Player player, uint8_t square);
        bool isSquareAttackedBySomePawn(Piece pawn, uint8_t square);
        bool isSquareAttackedBySomeKnight(Piece knight, uint8_t square);
        bool isSquareAttackedBySomeBishop(Piece bishop, uint8_t square);t
        bool isSquareAttackedBySomeRook(Piece rook, uint8_t square);
        bool isSquareAttackedBySomeQueen(Piece queen, uint8_t square);
        bool isSquareAttackedByKing(Piece king, uint8_t square);

        bool doesPawnAttackSquare(Player player, uint8_t pawn_square, uint8_t target_square);
        bool doesKnightAttackSquare(Piece knight, uint8_t knight_square, uint8_t target_square);
        bool doesBishopAttackSquare(Piece bishop, uint8_t bishop_square, uint8_t target_square);
        bool doesRookAttackSquare(Piece rook, uint8_t rook_square, uint8_t target_square);
        bool doesQueenAttackSquare(Piece queen, uint8_t queen_square, uint8_t target_square);
        bool doesKingAttackSquare(Piece king, uint8_t king_square, uint8_t target_square);

        unique_ptr<Chessboard> applyMove(Move *move);

        std::vector<std::unique_ptr<Move>> getPlayerLegalMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalPawnMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKnightMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalBishopMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalRookMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalQueenMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKingMoves(Player player);

        std::vector<std::unique_ptr<Move>> getPawnLegalMoves(Player player, uint8_t pawn_square);
        std::vector<std::unique_ptr<Move>> getKnightLegalMoves(Player player, uint8_t knight_square);
        std::vector<std::unique_ptr<Move>> getBishopLegalMoves(Player player, uint8_t bishop_square);
        std::vector<std::unique_ptr<Move>> getRookLegalMoves(Player player, uint8_t rook_square);
        std::vector<std::unique_ptr<Move>> getQueenLegalMoves(Player player, uint8_t queen_square);
        std::vector<std::unique_ptr<Move>> getKingLegalMoves(Player player, uint8_t king_square);


    
        void printChessboard();
        void printChessboardFancy();
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
class ChessboardBuilder{
    public:
        ChessboardBuilder();
        void initPawns();
        void initKnights();
        void initBishops();
        void initRooks();
        void initQueens();
        void initKings();
        void initFlags();
    }


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

uint8_t u64ToSqureIndex(uint64_t);

uint64_t squreIndexToU64(uint8_t);


std::vector<uint8_t> getSquareIndexesFromBitboard(uint64_t bitboard);


#endif