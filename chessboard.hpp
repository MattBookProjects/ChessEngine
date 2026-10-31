#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <cinttypes>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>
#include <optional>
#include <memory>
#include <algorithm>
#include "utils.hpp"
#include "common.hpp"
#include "move.hpp"
#include "piece.hpp"



enum class GameResult: int8_t{
    Ongoing = 10,
    WhiteWin = 1,
    Draw = 0,
    BlackWin = -1
};


enum class Piece: uint8_t;


class Chessboard{
    private:
        std::array<uint64_t, 12> bitboard;
        Player player_to_move;
        bool can_white_castle;
        bool can_black_castle;
        int8_t enpassant;
        //bool is_finished;
        GameResult game_result;
        PawnMoveBehaviour &white_pawn_move_behaviour;
        PawnMoveBehaviour &black_pawn_move_behaviour;
        KnightMoveBehaviour &knight_move_behaviour;
        BishopMoveBehaviour &bishop_move_behvaiour;
        RookMoveBehaviour &rook_move_behaviour;
        QueenMoveBehaviour &queen_move_behaviour;
        KingMoveBehaviour &king_move_behaviour;

    public:
        Chessboard(
            std::array<uint64_t, 12> bitboard = {},
            Player player_to_move = Player::White,
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

        std::array<uint64_t, 12> getBitboard();
        uint64_t getPieceBitboard(Piece piece);
        uint64_t getPlayerPiecesBitboard(Player player);
        uint64_t getAllPiecesBitboard();
        Piece getPlayerPieceFromSquare(Player player, uint8_t square);

        //bool isGameFinished();
       
        GameResult getGameResult();
        int8_t getEnpassant();

        uint8_t getPlayerKingSquare(Player player);

        bool isPlayerInCheck(Player player);
       
        bool isSquareAttackedByPlayerPieces(Player player, uint8_t square);

        bool isSquareAttackedByPiece(Piece piece, uint8_t square);
       // bool isSquareAttackedBySomePlayerPawn(Player player, uint8_t square);
        //bool isSquareAttackedBySomePlayerKnight(Player player, uint8_t square);
        //bool isSquareAttackedBySomePlayerBishop(Player player, uint8_t square);
        //bool isSquareAttackedBySomePlayerRook(Player player, uint8_t square);
       // bool isSquareAttackedBySomePlayerQueen(Player player, uint8_t square);
        //bool isSquareAttackedByPlayerKing(Player player, uint8_t square);
        
        bool isSquareOccupied(uint8_t square);

        bool isDiagonalBetweenSquaresEmpty(uint8_t start_square, uint8_t end_square, uint8_t step);
        bool isStraightBetweenSquaresEmpty(uint8_t start_square, uint8_t end_square, uint8_t step);

        //bool doesPieceAttackSquare(Piece piece, uint8_t piece_square, uint8_t target_square);
        /*bool doesPawnAttackSquare(Player pawn, uint8_t pawn_square, uint8_t target_square);
        bool doesKnightAttackSquare(uint8_t knight_square, uint8_t target_square);
        bool doesBishopAttackSquare(uint8_t bishop_square, uint8_t target_square);
        bool doesRookAttackSquare(uint8_t rook_square, uint8_t target_square);
        bool doesQueenAttackSquare(uint8_t queen_square, uint8_t target_square);
        bool doesKingAttackSquare(uint8_t king_square, uint8_t target_square);*/

        Chessboard makeMove(const std::unique_ptr<Move> &move);

        bool isMoveLegal(const std::unique_ptr<Move> &move);

        std::vector<std::unique_ptr<Move>> getPlayerLegalMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerPieceLegalMoves(Player player, Piece piece);


        std::vector<std::unique_ptr<Move>> getPlayerLegalPawnMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKnightMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalBishopMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalRookMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalQueenMoves(Player player);
        std::vector<std::unique_ptr<Move>> getPlayerLegalKingMoves(Player player);

        /*std::vector<std::unique_ptr<Move>> getPawnLegalMoves(Player player, Piece pawn, uint8_t pawn_square);
        std::vector<std::unique_ptr<Move>> getKnightLegalMoves(Player player, Piece knight, uint8_t knight_square);
        std::vector<std::unique_ptr<Move>> getBishopLegalMoves(Player player, Piece bishop, uint8_t bishop_square);
        std::vector<std::unique_ptr<Move>> getRookLegalMoves(Player player, Piece rook, uint8_t rook_square);
        std::vector<std::unique_ptr<Move>> getQueenLegalMoves(Player player, Piece queen, uint8_t queen_square);
        std::vector<std::unique_ptr<Move>> getKingLegalMoves(Player player, Piece queen, uint8_t king_square);
        */

    
        void printChessboard();
        void printChessboardFancy();
};



#endif