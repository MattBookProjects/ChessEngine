#include "piece.hpp"

PawnMoveBehaviour::PawnMoveBehaviour(uint8_t starting_rank, uint8_t penultimate_rank, int8_t forward_move_step){
    this->starting_rank = starting_rank;
    this->penultimate_rank = penultimate_rank;
    this->forward_move_step = forward_move_step;
}

std::vector<std::unique_ptr<Move>> PawnMoveBehaviour::getLegalMoves
(
    std::vector>std::unique_ptr<Move>> legal_moves{};
    Piece piece,
    uint8_t square,
    uint64_t all_pieces_bitboard,
    uint64_t opponent_pieces_bitboard 
    std::unique_ptr<Chessboard> cheessboard
)
{
    const uint8_t square_rank = getSquareRank(square);
    const Player opponent = getOpponentPlayer(player);
    const uint64_t all_pieces_bitboard = chessboard->getAllPiecesBitboard();
     
    const uint8_t forward_square = square + 8*this->forward_move_step;
   
    const std::unique_ptr<Move> forward_move = std::make_unique<BasicMove>(piece, square, forward_square);
    if(!chessboard->isMoveLegal(forward_move)){

    } else if(square rank == this->penultimate_rank){
        
    }  
}

inline bool PawnMoveBehaviour::doesPieceAttackSquare(uint8_t piece_square, uint8_t target_square){
    return target_square == piece_square + this->forward_move_step * 8 + 1 ||
           target_square == piece_square + this->forward_move_step * 8 - 1;
}


KnightMoveBehaviour::KingMoveBehaviour(){

}

