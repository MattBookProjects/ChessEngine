#ifndef PIECE_H
#define PIECE_H

class PieceMoveBehaviour{
    public:
        virtual std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player,
            Piece piece,
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) = 0;
        virtual bool doesPieceAttackSquare(uint8_t square) = 0;
  
};

class PawnMoveBehaviour{
    public:
        PawnMoveBehaviour(uint8_t starting_rank, uint8_t penultimate_rank, int8_t forward_move_step);
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
    private:
        uint8_t penultimate_rank;
        uint8_t starting_rank;
        int8_t forward_move_step;

};

class KnightMoveBehaviour{
    public:
        KnightMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
};

class BishopMoveBehaviour{
    public:
        BishopMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
};


class RookMoveBehaviour{
    public:
        RookMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
};


class QueenMoveBehaviour{
    public:
        QueenkMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
};

class KingMoveBehaviour{
    public:
        KingMoveBehaviour();
        std::vector<std::unique_ptr<Move>> getLegalMoves
        (
            Player player, 
            uint8_t square, 
            std::unique_ptr<Chessboard> cheessboard
        ) override;
        bool doesPieceAttackSquare(uint8_t square) override;
};



#endif