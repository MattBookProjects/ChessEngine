#include "piece.hpp"
#include "chessboard.hpp"
#include "move.hpp"



const DiagonalMoveBehaviour diagonal_move_behaviour{};
const StraightMoveBehaviour straight_move_behaviour{};
const PawnMoveBehaviour white_pawn_move_behaviour{0, 6, 4, 1};
const PawnMoveBehaviour black_pawn_move_behaviour{7, 1, 3, -1};
const KnightMoveBehaviour knight_move_behaviour{};
const BishopMoveBehaviour bishop_move_behaviour{diagonal_move_behaviour};
const RookMoveBehaviour rook_move_behaviour{straight_move_behaviour};
const QueenMoveBehaviour queen_move_behaviour{straight_move_behaviour, diagonal_move_behaviour};
const KingMoveBehaviour king_move_behaviour{};

const PieceMoveBehaviour &getPieceMoveBehaviour(Piece piece){
    switch(piece){
        case Piece::WhitePawn:
            return white_pawn_move_behaviour;
        case Piece::BlackPawn:
            return black_pawn_move_behaviour;
        case Piece::WhiteKnight:
        case Piece::BlackKnight:
            return knight_move_behaviour;
        case Piece::WhiteBishop:
        case Piece::BlackBishop:
            return bishop_move_behaviour;
        case Piece::WhiteRook:
        case Piece::BlackRook:
            return rook_move_behaviour;
        case Piece::WhiteQueen:
        case Piece::BlackQueen:
            return queen_move_behaviour;
        case Piece::WhiteKing:
        case Piece::BlackKing:
            return king_move_behaviour;       
    }
}

/*
inline std::array<Piece, 6> getPlayerPieces(Player player){
    return player == Player::White ? WHITE_PIECES : BLACK_PIECES;
}*/



std::unique_ptr<Move> PieceMoveBehaviour::getLegalPieceToSquareMove
(
    Player player,
    Piece piece, 
    uint8_t square, 
    Chessboard &chessboard,
    uint8_t target_square
) const
{
    const Player opponent = getOpponentPlayer(player);
    if(!chessboard.isSquareOccupied(target_square)){
        std::unique_ptr<Move> move = std::make_unique<BasicMove>(piece, square, target_square);
        if(chessboard.isMoveLegal(move)){
            return move;
        }
    } else if(const Piece target_piece = chessboard.getPlayerPieceFromSquare(opponent, square); 
              target_piece != Piece::Empty){
        std::unique_ptr<Move> move = std::make_unique<TakeMove>(piece, square, target_piece, target_square);
        if(chessboard.isMoveLegal(move)){
            return move;
        }
    }
    std::printf("PieceMoveBehaviour::getLegalPieceToSquareMove RETURNING NULL");
    return nullptr;
} 

PawnMoveBehaviour::PawnMoveBehaviour(uint8_t starting_rank, uint8_t penultimate_rank, uint8_t enpassant_rank, int8_t forward_move_step){
    this->starting_rank = starting_rank;
    this->penultimate_rank = penultimate_rank;
    this->enpassant_rank = enpassant_rank;
    this->forward_move_step = forward_move_step;
}

std::vector<std::unique_ptr<Move>> PawnMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
   // std::printf("PawnMoveBehaviour::getLegalMoves\n");
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const uint8_t square_rank = getSquareRank(square);
    const uint8_t square_file = getSquareFile(square);
    const Player opponent = getOpponentPlayer(player);
    const uint64_t all_pieces_bitboard = chessboard.getAllPiecesBitboard();
    const uint64_t opponent_pieces_bitboard = chessboard.getPlayerPiecesBitboard(player);
    const std::vector<uint8_t> all_pieces_square_indexes = getSquareIndexesFromBitboard(all_pieces_bitboard);
    const std::vector<uint8_t> opponent_pieces_square_indexes = getSquareIndexesFromBitboard(opponent_pieces_bitboard);
    //std::cout<<"all pieces bitboard: "<<all_pieces_bitboard<<std::endl;
    const uint8_t forward_square = square + 8*this->forward_move_step;
    //std::printf("square: %u, target_square: %u\n", square, forward_square);
    //std::cout<<"all pieces squares indexes: " <<all_pieces_square_indexes<<std::endl;
    if(!(chessboard.isSquareOccupied(forward_square))){
       // std::printf("PawnMoveBehaviour::getLegalMoves SQUARE_NOT_OCCUPIED\n");
    /*auto is_forward_square_occupied = std::find(all_pieces_square_indexes.begin(), all_pieces_square_indexes.end(), forward_square);
    if(is_forward_square_occupied != all_pieces_square_indexes.end()){*/
        std::unique_ptr<Move> forward_move = std::make_unique<BasicMove>(piece, square, forward_square);

        if(!chessboard.isMoveLegal(forward_move)){

        } else if(square_rank == this->penultimate_rank){
            auto promotion_moves = getPromotionMoves(player, piece, square, forward_square);
            legal_moves.insert(legal_moves.end(), std::make_move_iterator(promotion_moves.begin()), std::make_move_iterator(promotion_moves.end()));
        }  else {
            legal_moves.push_back(std::move(forward_move));
        }
        if(square_rank == this->starting_rank){
            const uint8_t double_forward_square = square + 16*this->forward_move_step;
            if(!chessboard.isSquareOccupied(double_forward_square)){
                std::unique_ptr<Move> double_forward_move = std::make_unique<BasicMove>(piece, square, double_forward_square);
                legal_moves.push_back(std::move(double_forward_move));
            } 
        }
    }
    if(square_file > 0){
        const uint8_t left_target_square = square -9 + (this->forward_move_step + 1)*8;
        const Piece target_piece = chessboard.getPlayerPieceFromSquare(opponent, left_target_square);
        if(target_piece != Piece::Empty){
            std::unique_ptr<Move> left_take_move = std::make_unique<TakeMove>(piece, square, target_piece, left_target_square);
            if(!chessboard.isMoveLegal(left_take_move)){

            } else if(square_rank == this->penultimate_rank){
                auto left_take_with_promotion_moves = getTakeWithPromotionMoves
                (
                    player, 
                    piece, 
                    square, 
                    target_piece, 
                    left_target_square
                );
                legal_moves.insert
                (
                    legal_moves.end(), 
                    std::make_move_iterator(left_take_with_promotion_moves.begin()), 
                    std::make_move_iterator(left_take_with_promotion_moves.end())
                );

                //const std::unique_ptr<Move> left_take_with_promotion_move = std::make_unique<TakeWithPromotionMove()
            } else{
                legal_moves.push_back(std::move(left_take_move));
            }
        }
    } 
    if(square_file < 7){
        const uint8_t right_target_square = square -7 + (this->forward_move_step + 1)*8;
        auto is_opponent_piece_on_square = std::find
        (
            opponent_pieces_square_indexes.begin(), 
            opponent_pieces_square_indexes.end(), 
            right_target_square
        );
        const Piece target_piece = chessboard.getPlayerPieceFromSquare(opponent, right_target_square);
        if(target_piece != Piece::Empty){
            std::unique_ptr<Move> right_take_move = std::make_unique<TakeMove>(piece, square, target_piece, right_target_square);
            if(!chessboard.isMoveLegal(right_take_move)){

            } else if(square_rank == this->penultimate_rank){
                auto right_take_with_promotion_moves = getTakeWithPromotionMoves
                (
                    player, 
                    piece, 
                    square, 
                    target_piece, 
                    right_target_square
                );
                legal_moves.insert(
                    legal_moves.end(), 
                    std::make_move_iterator(right_take_with_promotion_moves.begin()), 
                    std::make_move_iterator(right_take_with_promotion_moves.end())
                );

                //const std::unique_ptr<Move> left_take_with_promotion_move = std::make_unique<TakeWithPromotionMove()
            } else{
                legal_moves.push_back(std::move(right_take_move));
            }
        }
    }
    const auto enpassant = chessboard.getEnpassant();
    if(enpassant != -1 && square_rank == this->enpassant_rank){
        const Piece opponent_pawn = getPlayerPawn(opponent);
        if(square_file - 1 == enpassant){
            std::unique_ptr<Move> enpassant_move = std::make_unique<EnpassantMove>
            (
                piece, 
                square, 
                square -9 + (this->forward_move_step + 1)*8, 
                opponent_pawn, 
                square - 1
            );
            if(chessboard.isMoveLegal(enpassant_move)){
                legal_moves.push_back(std::move(enpassant_move));
            }
            

        } else if(square_file + 1 == enpassant){
            std::unique_ptr<Move> enpassant_move = std::make_unique<EnpassantMove>
            (
                piece, 
                square, 
                square -7 + (this->forward_move_step + 1)*8, 
                opponent_pawn, 
                square + 1
            );
            if(chessboard.isMoveLegal(enpassant_move)){
                legal_moves.push_back(std::move(enpassant_move));
            }   
            
        }
    }
    return legal_moves;
}

inline bool PawnMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const{
    return target_square == piece_square + this->forward_move_step * 8 + 1 ||
           target_square == piece_square + this->forward_move_step * 8 - 1;
}

KnightMoveBehaviour::KnightMoveBehaviour(){}

std::vector<std::unique_ptr<Move>> KnightMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const auto square_file = square % 8;
    const auto square_rank = square >> 3;
    const Player opponent = getOpponentPlayer(player);
    if(square_file < 7 && square_rank < 6){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square+17);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file > 0 && square_rank < 6){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square+15);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file < 6 && square_rank < 7){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square+10);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file > 1 && square_rank < 7){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square+6);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file < 6 && square_rank > 0){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square-6);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file > 1 && square_rank > 0){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square-10);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file < 7 && square_rank > 1){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square-15);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    if(square_file > 0 && square_rank > 1){
        auto move = this->getLegalPieceToSquareMove(player, piece, square, chessboard, square-17);
        if(move != nullptr){
            legal_moves.push_back(std::move(move));
        }   
    }
    return legal_moves;
}


inline bool KnightMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const {
    const uint8_t square_rank = piece_square >> 3;
    const uint8_t square_file = piece_square % 8;
    return
       square_rank < 6 && square_file < 7 && target_square - piece_square == 17 ||
       square_rank < 6 && square_file > 0 && target_square - piece_square == 15 ||
       square_rank < 7 && square_file < 6 && target_square - piece_square == 10 ||
       square_rank < 7 && square_file > 1 && target_square - piece_square == 6 ||
       square_rank > 0 && square_file < 6 && target_square - piece_square == -6 ||
       square_rank > 0 && square_file > 1 && target_square - piece_square == -10 ||
       square_rank > 1 && square_file < 7 && target_square - piece_square == - 15 ||
       square_rank > 1 && square_file > 0 && target_square - piece_square == -17;
    
    
}


BishopMoveBehaviour::BishopMoveBehaviour(const DiagonalMoveBehaviour &diagonal_move_behaviour)
: diagonal_move_behaviour(diagonal_move_behaviour)
{
}

std::vector<std::unique_ptr<Move>> BishopMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    std::vector<std::unique_ptr<Move>> upper_right_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> upper_left_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> lower_right_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        -1,
        *this
    );
    std::vector<std::unique_ptr<Move>> lower_left_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        -1,
        *this
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(upper_right_diagonal_moves.begin()), 
        std::make_move_iterator(upper_right_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(upper_left_diagonal_moves.begin()), 
        std::make_move_iterator(upper_left_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(lower_right_diagonal_moves.begin()), 
        std::make_move_iterator(lower_right_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(lower_left_diagonal_moves.begin()), 
        std::make_move_iterator(lower_left_diagonal_moves.end())
    );
    return legal_moves;
}




bool BishopMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const{
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 9 != 0 && diff % 7 != 0)
        return false;
    const auto step = diff % 9 == 0 ? 9 : 7;
    return chessboard.isDiagonalBetweenSquaresEmpty(start, end, step);
}



RookMoveBehaviour::RookMoveBehaviour(const StraightMoveBehaviour &straight_move_behaviour)
: straight_move_behaviour(straight_move_behaviour)
{
}



std::vector<std::unique_ptr<Move>> RookMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
    std::vector<std::unique_ptr<Move>> legal_moves{};

    std::vector<std::unique_ptr<Move>> straight_up_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        0,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_down_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        0,
        -1,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_right_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        0,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_left_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        0,
        *this
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_up_moves.begin()), 
        std::make_move_iterator(straight_up_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_down_moves.begin()), 
        std::make_move_iterator(straight_down_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_right_moves.begin()), 
        std::make_move_iterator(straight_right_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_right_moves.begin()), 
        std::make_move_iterator(straight_left_moves.end())
    );
    return legal_moves;
}


bool RookMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const {
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 8 != 0 && diff > 7){
        return false;
    }
    const auto step = diff % 8 == 0 ? 8 : 1;
    return chessboard.isStraightBetweenSquaresEmpty(start, end, step);
}

QueenMoveBehaviour::QueenMoveBehaviour(const StraightMoveBehaviour &straight_move_behaviour, const DiagonalMoveBehaviour &diagonal_move_behaviour)
: straight_move_behaviour(straight_move_behaviour),
  diagonal_move_behaviour(diagonal_move_behaviour)
{

}

std::vector<std::unique_ptr<Move>> QueenMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    std::vector<std::unique_ptr<Move>> upper_right_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> upper_left_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> lower_right_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        -1,
        *this
    );
    std::vector<std::unique_ptr<Move>> lower_left_diagonal_moves = this->diagonal_move_behaviour.getLegalMovesOnDiagonal
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        -1,
        *this
    );


    std::vector<std::unique_ptr<Move>> straight_up_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        0,
        1,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_down_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        0,
        -1,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_right_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        1,
        0,
        *this
    );
    std::vector<std::unique_ptr<Move>> straight_left_moves = this->straight_move_behaviour.getLegalMovesOnStraight
    (
        player, 
        piece,
        square,
        chessboard,
        -1,
        0,
        *this
    );
    legal_moves.insert(
        legal_moves.end(), 
        std::make_move_iterator(straight_up_moves.begin()), 
        std::make_move_iterator(straight_up_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_down_moves.begin()), 
        std::make_move_iterator(straight_down_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_right_moves.begin()), 
        std::make_move_iterator(straight_right_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(straight_right_moves.begin()), 
        std::make_move_iterator(straight_left_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(upper_right_diagonal_moves.begin()), 
        std::make_move_iterator(upper_right_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(upper_left_diagonal_moves.begin()), 
        std::make_move_iterator(upper_left_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(lower_right_diagonal_moves.begin()), 
        std::make_move_iterator(lower_right_diagonal_moves.end())
    );
    legal_moves.insert
    (
        legal_moves.end(), 
        std::make_move_iterator(lower_left_diagonal_moves.begin()), 
        std::make_move_iterator(lower_left_diagonal_moves.end())
    );
    return legal_moves;
}










bool QueenMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const {
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 9 == 0 || diff % 7 == 0 && diff != 56){
        const auto step = diff % 9 == 0 ? 9 : 7;
        return chessboard.isDiagonalBetweenSquaresEmpty(start, end, step);
    }
    else if(diff % 8 == 0 || diff < 7){
        const auto step = diff % 8 == 0 ? 8 : 1;
        return chessboard.isStraightBetweenSquaresEmpty(start, end, step);
    }
    return false;
}

KingMoveBehaviour::KingMoveBehaviour(){}



std::vector<std::unique_ptr<Move>> KingMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    Chessboard &chessboard
) const
{
    std::printf("KingMoveBehaviour::getLegalMoves START\n");
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const auto king_rank = getSquareRank(square);
    const auto king_file = getSquareFile(square);
    if(king_rank < 7 && king_file < 7){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square + 9
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
     std::printf("KingMoveBehaviour::getLegalMoves AFTER FIRST CHECK\n");
    if(king_rank < 7){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square + 8
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_rank < 7 && king_file > 0){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square + 7
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_file < 7){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square + 1
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_file > 0){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square - 1
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_rank > 0 && king_file < 7){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square - 7
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_rank > 0){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square - 8
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    if(king_rank > 0 && king_file > 0){
        std::unique_ptr<Move> move = this->getLegalPieceToSquareMove
        (
            player,
            piece, 
            square, 
            chessboard,
            square - 9
        );
        if(move != nullptr  && chessboard.isMoveLegal(move)){
            legal_moves.push_back(std::move(move));
        }
    }
    return legal_moves;
}



inline bool KingMoveBehaviour::doesPieceAttackSquare(Chessboard &chessboard, uint8_t piece_square, uint8_t target_square) const {
    const auto king_rank = piece_square >> 3;
    const auto king_file = piece_square % 8;
    return king_rank < 7 && king_file < 7 && target_square - piece_square == 9 ||
           king_rank < 7 && target_square - piece_square == 8 ||
           king_rank < 7 && king_file > 0 && target_square - piece_square == 7 ||
           king_file < 7 && target_square - piece_square == 1 ||
           king_file > 0 && piece_square - target_square == 1 ||
           king_rank > 0 && king_file < 7 && piece_square - target_square == 7 ||
           king_rank > 0 && piece_square - target_square == 8 ||
           king_rank > 0 && king_file > 0 && piece_square - target_square == 9;
}

std::vector<std::unique_ptr<Move>> DiagonalMoveBehaviour::getLegalMovesOnDiagonal
(
    Player player,
    Piece piece, 
    uint8_t square, 
    Chessboard &chessboard,
    int8_t file_step,
    int8_t rank_step,
    const PieceMoveBehaviour &piece_move_behaviour
)const 
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const uint8_t rank_limit = rank_step == -1 ? 0 : 7;
    const uint8_t file_limit = file_step == -1 ? 0 : 7;
    uint8_t current_file = getSquareFile(square);
    uint8_t current_rank = getSquareRank(square);
    uint8_t current_square = square;
    const int8_t square_step = 8*rank_step + file_step;
    while(current_file != file_limit && current_rank != rank_step){
        current_file+=file_step;
        current_rank+=rank_step;
        current_square+=square_step;
        std::unique_ptr<Move> move = piece_move_behaviour.getLegalPieceToSquareMove
        (
            player,
            piece,
            square, 
            chessboard,
            current_square
        );
        if(move == nullptr){
            return legal_moves;
        }
        else{
            legal_moves.push_back(std::move(move));
            if(dynamic_cast<TakeMove*>(move.get())){
                return legal_moves;
            }
        }
        
    }
}

std::vector<std::unique_ptr<Move>> StraightMoveBehaviour::getLegalMovesOnStraight
(
    Player player,
    Piece piece, 
    uint8_t square, 
    Chessboard &chessboard,
    int8_t file_step,
    int8_t rank_step,
    const PieceMoveBehaviour &piece_move_behaviour
) const
{
    
    std::vector<std::unique_ptr<Move>> legal_moves{};
    uint8_t current_file = getSquareFile(square);
    uint8_t current_rank = getSquareRank(square);
    uint8_t current_square = square;
    uint8_t rank_or_file_limit;
    uint8_t current_rank_or_file = file_step == 0 ? current_rank : current_file;
    int8_t rank_or_file_step = file_step == 0 ? rank_step : file_step;
    int8_t square_step = file_step == 0 ? 8*rank_step : file_step;
    if(file_step==0 && rank_step==1 || rank_step==0 && file_step==1){
        rank_or_file_limit = 7;
    } else{
        rank_or_file_limit = 0;
    }
    while(current_rank_or_file != rank_or_file_limit){
        current_rank_or_file+=rank_or_file_step;
        current_square+=square_step;
        std::unique_ptr<Move> move = piece_move_behaviour.getLegalPieceToSquareMove
        (
            player,
            piece,
            square, 
            chessboard,
            current_square
        );
        if(move == nullptr){
            return legal_moves;
        }
        else{
            legal_moves.push_back(std::move(move));
            if(dynamic_cast<TakeMove*>(move.get())){
                return legal_moves;
            }
        }
        
    }
    return legal_moves;
}