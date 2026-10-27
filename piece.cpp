#include "piece.hpp"
#include "chessboard.hpp"

constexpr std::array<Piece, 6> WHITE_PIECES = {
    Piece::WhitePawn, 
    Piece::WhiteKnight, 
    Piece::WhiteBishop, 
    Piece::WhiteRook,
    Piece::WhiteQueen,
    Piece::WhiteKing
};

constexpr std::array<Piece, 6> BLACK_PIECES{
    Piece::BlackPawn, 
    Piece::BlackKnight, 
    Piece::BlackBishop, 
    Piece::BlackRook,
    Piece::BlackQueen,
    Piece::BlackKing
};

/*
std::unique_ptr<PieceMoveBehaviour> white_pawn_move_behaviour = std::make_unique<PawnMoveBehaviour>(0, 6, 1);
std::unique_ptr<PieceMoveBehaviour> black_pawn_move_behaviour = std::make_unique<PawnMoveBehaviour>(7, 1, -1);
std::unique_ptr<PieceMoveBehaviour> knight_move_behaviour = std::make_unique<KnightMoveBehaviour>();
std::unique_ptr<PieceMoveBehaviour> bishop_move_behaviour = std::make_unique<BishopMoveBehaviour>();
std::unique_ptr<PieceMoveBehaviour> rook_move_behaviour = std::make_unique<RookMoveBehaviour>();
std::unique_ptr<PieceMoveBehaviour> queen_pawn_move_behaviour = std::make_unique<QueenMoveBehaviour>();
std::unique_ptr<PieceMoveBehaviour> king_move_behaviour = std::make_unique<KingMoveBehaviour>();
*/

const PawnMoveBehaviour white_pawn_move_behaviour = PawnMoveBehaviour(0, 6, 4, 1);
const PawnMoveBehaviour black_pawn_move_behaviour = PawnMoveBehaviour(7, 1, 3, -1);
const KnightMoveBehaviour knight_move_behaviour = KnightMoveBehaviour();
const BishopMoveBehaviour bishop_move_behaviour = BishopMoveBehaviour();
const RookMoveBehaviour rook_move_behaviour = RookMoveBehaviour();
const QueenMoveBehaviour queen_move_behaviour = QueenMoveBehaviour();
const KingMoveBehaviour king_move_behaviour = KingMoveBehaviour();


std::unique_ptr<PieceMoveBehaviour> getPieceMoveBehaviour(Piece piece){
    switch(piece){
        case Piece::WhitePawn:
            return std::make_unique<PieceMoveBehaviour>(white_pawn_move_behaviour);
        case Piece::BlackPawn:
            return std::make_unique<PieceMoveBehaviour>(black_pawn_move_behaviour);
        case Piece::WhiteKnight:
        case Piece::BlackKnight:
            return std::make_unique<PieceMoveBehaviour>(knight_move_behaviour);
        case Piece::WhiteBishop:
        case Piece::BlackBishop:
            return std::make_unique<PieceMoveBehaviour>(bishop_move_behaviour);
        case Piece::WhiteRook:
        case Piece::BlackRook:
            return std::make_unique<PieceMoveBehaviour>(rook_move_behaviour);
        case Piece::WhiteQueen:
        case Piece::BlackQueen:
            return std::make_unique<PieceMoveBehaviour>(queen_move_behaviour);
        case Piece::WhiteKing:
        case Piece::BlackKing:
            return std::make_unique<PieceMoveBehaviour>(king_move_behaviour);       
    }
}


inline std::array<Piece, 6> getPlayerPieces(Player player){
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
    std::unique_ptr<Chessboard> chessboard
)
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const uint8_t square_rank = getSquareRank(square);
    const uint8_t square_file = getSquareFile(square);
    const Player opponent = getOpponentPlayer(player);
    const uint64_t all_pieces_bitboard = chessboard->getAllPiecesBitboard();
    const uint64_t opponent_pieces_bitboard = chessboard->getPlayerPiecesBitboard(player);
    const std::vector<uint8_t> all_pieces_square_indexes = getSquareIndexesFromBitboard(all_pieces_bitboard);
    const std::vector<uint8_t> opponent_pieces_square_indexes = getSquareIndexesFromBitboard(opponent_pieces_bitboard);

    const uint8_t forward_square = square + 8*this->forward_move_step;
    if(!chessboard->isSquareOccupied(forward_square)){
    /*auto is_forward_square_occupied = std::find(all_pieces_square_indexes.begin(), all_pieces_square_indexes.end(), forward_square);
    if(is_forward_square_occupied != all_pieces_square_indexes.end()){*/
        const std::unique_ptr<Move> forward_move = std::make_unique<BasicMove>(piece, square, forward_square);

        if(!chessboard->isMoveLegal(forward_move)){

        } else if(square_rank == this->penultimate_rank){
            const auto promotion_moves = getPromotionMoves(player, piece, square, forward_square);
            legal_moves.insert(legal_moves.end(), promotion_moves.begin(), promotion_moves.end());
        }  else {
            legal_moves.push_back(forward_move);
        }
        if(square_rank == this->starting_rank){
            const uint8_t double_forward_square = square + 16*this->forward_move_step;
            if(!chessboard->isSquareOccupied(double_forward_square)){
                const std::unique_ptr<Move> double_forward_move = std::make_unique<BasicMove>(piece, square, double_forward_square);
                legal_moves.push_back(double_forward_move);
            } 
        }
    }
    if(square_file > 0){
        const uint8_t left_target_square = square -9 + (this->forward_move_step + 1)*8;
        const Piece target_piece = chessboard->getPlayerPieceFromSquare(opponent, left_target_square);
        if(target_piece != Piece::Empty){
            const std::unique_ptr<Move> left_take_move = std::make_unique<TakeMove>(piece, square, target_piece, left_target_square);
            if(!chessboard->isMoveLegal(left_take_move)){

            } else if(square_rank == this->penultimate_rank){
                const auto left_take_with_promotion_moves = getTakeWithPromotionMoves
                (
                    player, 
                    piece, 
                    square, 
                    target_piece, 
                    left_target_square
                );
                legal_moves.insert(legal_moves.end(), left_take_with_promotion_moves.begin(), left_take_with_promotion_moves.end());

                //const std::unique_ptr<Move> left_take_with_promotion_move = std::make_unique<TakeWithPromotionMove()
            } else{
                legal_moves.push_back(left_take_move);
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
        const Piece target_piece = chessboard->getPlayerPieceFromSquare(opponent, right_target_square);
        if(target_piece != Piece::Empty){
            const std::unique_ptr<Move> right_take_move = std::make_unique<TakeMove>(piece, square, target_piece, right_target_square);
            if(!chessboard->isMoveLegal(right_take_move)){

            } else if(square_rank == this->penultimate_rank){
                const auto right_take_with_promotion_moves = getTakeWithPromotionMoves
                (
                    player, 
                    piece, 
                    square, 
                    target_piece, 
                    right_target_square
                );
                legal_moves.insert(legal_moves.end(), right_take_with_promotion_moves.begin(), right_take_with_promotion_moves.end());

                //const std::unique_ptr<Move> left_take_with_promotion_move = std::make_unique<TakeWithPromotionMove()
            } else{
                legal_moves.push_back(right_take_move);
            }
        }
    }
    const auto enpassant = chessboard->getEnpassant();
    if(enpassant != -1 && square_rank == this->enpassant_rank){
        const Piece opponent_pawn = getPlayerPawn(opponent);
        if(square_file - 1 == enpassant){
            const std::unique_ptr<Move> enpassant_move = std::make_unique<EnpassantMove>
            (
                piece, 
                square, 
                square -9 + (this->forward_move_step + 1)*8, 
                opponent_pawn, 
                square - 1
            );
            if(chessboard->isMoveLegal(enpassant_move)){
                legal_moves.push_back(enpassant_move);
            }
            

        } else if(square_file + 1 == enpassant){
            const std::unique_ptr<Move> enpassant_move = std::make_unique<EnpassantMove>
            (
                piece, 
                square, 
                square -7 + (this->forward_move_step + 1)*8, 
                opponent_pawn, 
                square + 1
            );
            if(chessboard->isMoveLegal(enpassant_move)){
                legal_moves.push_back(enpassant_move);
            }   
            
        }
    }
    return legal_moves;
}

inline bool PawnMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
    return target_square == piece_square + this->forward_move_step * 8 + 1 ||
           target_square == piece_square + this->forward_move_step * 8 - 1;
}



std::vector<std::unique_ptr<Move>> KnightMoveBehaviour::getLegalMoves
(
    Player player, 
    Piece piece,
    uint8_t square,
    std::unique_ptr<Chessboard> chessboard
)
{
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const auto square_file = square % 8;
    const auto square_rank = square >> 3;
    const Player opponent = getOpponentPlayer(player)
    if(square_file < 7 && square_rank < 6){
        const auto target_square = square + 17;
        if(!chessboard->isSquareOccupied(target_square)){
            std::unique_ptr<Move> move = std::make_unique<BasicMove>(piece, square, target_square);
            if(chessboard->isMoveLegal(move)){
                legal_moves.push_back(move);
            }
        } else if((const Piece piece = chessboard->getPlayerPieceFromSquare(opponent, square)) != Piece::Empty){
            std::unique_ptr<Move> move = std::make_unique<BasicMove>(piece, square, target_square);
        }
    }
}

inline bool KnightMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
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


bool BishopMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 9 != 0 && diff % 7 != 0)
        return false;
    const auto step = diff % 9 == 0 ? 9 : 7;
    return chessboard->isDiagonalBetweenSquaresEmpty(start, end, step);
}

bool RookMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 8 != 0 && diff > 7){
        return false;
    }
    const auto step = diff % 8 == 0 ? 8 : 1;
    return chessboard->isStraightBetweenSquaresEmpty(start, end, step);
}


bool QueenMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
    const auto diff = target_square > piece_square ? target_square - piece_square : piece_square - target_square;
    const auto start = target_square > piece_square ? piece_square : target_square;
    const auto end = target_square > piece_square ? target_square : piece_square;
    if(diff % 9 == 0 || diff % 7 == 0 && diff != 56){
        const auto step = diff % 9 == 0 ? 9 : 7;
        return chessboard->isDiagonalBetweenSquaresEmpty(start, end, step);
    }
    else if(diff % 8 == 0 || diff < 7){
        const auto step = diff % 8 == 0 ? 8 : 1;
        return chessboard->isStraightBetweenSquaresEmpty(start, end, step);
    }
    return false;
}

inline bool KingMoveBehaviour::doesPieceAttackSquare(std::unique_ptr<Chessboard> chessboard, uint8_t piece_square, uint8_t target_square){
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
