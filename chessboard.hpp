#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <cinttypes>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>
#include <optional>
#include "utils.hpp"
#include "common.hpp"


typedef uint8_t Player;
typedef uint8_t Piece;
typedef uint8_t CastleSide;
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
const Piece = BLACK_QUEEN;
const Piece = BLACK_KING;

const CastleSide SHORT_CASTLE;
const CastleSide LONG_CASTLE; 

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
        int8_t enpassant;
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

        uint8_t getPlayerKingSquare(Player player);
       
        bool isSquareAttackedByPlayer(Player player, uint8_t square);

        bool isSquareAttackByPiece(Piece piece, uint8_t square);
        bool isSquareAttackedBySomePlayerPawn(Player player, uint8_t square);
        bool isSquareAttackedBySomePlayerKnight(Player player, uint8_t square);
        bool isSquareAttackedBySomePlayerBishop(Player player, uint8_t square);t
        bool isSquareAttackedBySomePlayerRook(Player player, uint8_t square);
        bool isSquareAttackedBySomePlayerQueen(Player player, uint8_t square);
        bool isSquareAttackedByPlayerKing(Player player, uint8_t square);


        bool doesPieceAttackSquare(Piece piece, uint8_t piece_square, uint8_t target_square);
        bool doesPawnAttackSquare(Player pawn, uint8_t pawn_square, uint8_t target_square);
        bool doesKnightAttackSquare(uint8_t knight_square, uint8_t target_square);
        bool doesBishopAttackSquare(uint8_t bishop_square, uint8_t target_square);
        bool doesRookAttackSquare(uint8_t rook_square, uint8_t target_square);
        bool doesQueenAttackSquare(uint8_t queen_square, uint8_t target_square);
        bool doesKingAttackSquare(uint8_t king_square, uint8_t target_square);

        Chessboard makeMove(std::shared_ptr<Move> move);

        std::vector<std::unique_ptr<Move>> getPlayerLegalMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalPawnMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKnightMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalBishopMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalRookMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalQueenMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKingMoves(Player player);

        std::vector<std::unique_ptr<Move>> getPawnLegalMoves(Player player, Piece pawn, uint8_t pawn_square);
        std::vector<std::unique_ptr<Move>> getKnightLegalMoves(Player player, Piece knight, uint8_t knight_square);
        std::vector<std::unique_ptr<Move>> getBishopLegalMoves(Player player, Piece bishop, uint8_t bishop_square);
        std::vector<std::unique_ptr<Move>> getRookLegalMoves(Player player, Piece rook, uint8_t rook_square);
        std::vector<std::unique_ptr<Move>> getQueenLegalMoves(Player player, Piece queen, uint8_t queen_square);
        std::vector<std::unique_ptr<Move>> getKingLegalMoves(Player player, Piece queen, uint8_t king_square);


    
        void printChessboard();
        void printChessboardFancy();
};













#endif