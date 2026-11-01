#include "move.hpp"
#include "chessboard.hpp"
#include "piece.hpp"


Chessboard Move::applyMove(Chessboard &chessboard){
    const auto current_bitboard = chessboard.getBitboard();
    auto move_bitboard = this->constructMoveBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < current_bitboard.size(); ++i){
        new_bitboard[i] = current_bitboard[i] ^ move_bitboard[i]; 
    }
    auto player_to_move = chessboard.getPlayerToMove() == Player::White ? Player::Black : Player::White;
    return Chessboard(
        new_bitboard,
        player_to_move,
        chessboard.canWhiteCastle(),
        chessboard.canBlackCastle(),
        -1
    );
}

BasicMove::BasicMove(Piece piece, uint8_t old_square, uint8_t new_square){
    this->piece = piece;
    this->old_square = old_square;
    this->new_square = new_square;
}

Chessboard BasicMove::applyMove(Chessboard &chessboard){
    const auto current_bitboard = chessboard.getBitboard();
    const auto move_bitboard = this->constructMoveBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < current_bitboard.size(); ++i){
        new_bitboard[i] = current_bitboard[i] ^ move_bitboard[i];
    }
    Player player_to_move = chessboard.getPlayerToMove() == Player::White ? Player::Black : Player::White;
    int8_t enpassant = -1;
    if(this->piece == Piece::WhitePawn && this->new_square - this->old_square == 16 ||
        this->piece == Piece::BlackPawn && this->old_square - this->new_square == 16){
            enpassant = this->old_square % 8;
        }
    return Chessboard(
        new_bitboard, 
        player_to_move, 
        chessboard.canWhiteCastle(), 
        chessboard.canBlackCastle(), 
        enpassant
    );
}


std::array<uint64_t, 12> BasicMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[static_cast<uint8_t>(piece)] = squareIndexToBitboard(old_square) | squareIndexToBitboard(new_square);
    return move_bitboard;
}


TakeMove::TakeMove(Piece attacking_piece, uint8_t attacker_square, Piece target_piece, uint8_t target_square){
    this->attacking_piece = attacking_piece;
    this->attacker_square = attacker_square;
    this->target_piece = target_piece;
    this->target_square = target_square; 
}


std::array<uint64_t, 12> TakeMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[static_cast<uint8_t>(this->attacking_piece)] = squareIndexToBitboard(this->attacker_square) | squareIndexToBitboard(this->target_square);
    move_bitboard[static_cast<uint8_t>(this->target_piece)] = squareIndexToBitboard(this->target_square);
    return move_bitboard;
}

CastleMove::CastleMove(Player player, CastleSide side){
    this->player = player;
    this->side = side;
}

Chessboard CastleMove::applyMove(Chessboard &chessboard){
    const auto move_bitboard = this->constructMoveBitboard();
    const auto current_bitboard = chessboard.getBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < current_bitboard.size(); ++i){
        new_bitboard[i] = current_bitboard[i] ^ move_bitboard[i];
    }
    Player player_to_move = chessboard.getPlayerToMove() == Player::White ? Player::Black : Player::White;
    bool can_white_castle = player_to_move == Player::White ? false : chessboard.canWhiteCastle();
    bool can_black_castle = player_to_move == Player::Black ? false : chessboard.canBlackCastle();
    return Chessboard(
        new_bitboard,
        player_to_move,
        can_white_castle,
        can_black_castle,
        -1
    );
}

std::array<uint64_t,12> CastleMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    Piece rook = player == Player::White ? Piece::WhiteRook : Piece::BlackRook;
    Piece king = player == Player::White ? Piece::WhiteKing : Piece::BlackKing;
    
    if(side == CastleSide::SHORT_CASTLE){
        move_bitboard[static_cast<uint8_t>(rook)] = squareIndexToBitboard(7) | squareIndexToBitboard(5);
        move_bitboard[static_cast<uint8_t>(king)] = squareIndexToBitboard(4) | squareIndexToBitboard(6);
    } else{
        move_bitboard[static_cast<uint8_t>(rook)] = squareIndexToBitboard(0) | squareIndexToBitboard(3);
        move_bitboard[static_cast<uint8_t>(king)] = squareIndexToBitboard(4) | squareIndexToBitboard(2);
    }
    return move_bitboard;
}


EnpassantMove::EnpassantMove(Piece attacking_pawn, uint8_t attacker_old_square, uint8_t attacker_new_square, Piece target_pawn, uint8_t target_square){
    this->attacking_pawn = attacking_pawn;
    this->attacker_old_square = attacker_old_square;
    this->attacker_new_square = attacker_new_square;
    this->target_pawn = target_pawn;
    this->target_square = target_square;
}



std::array<uint64_t,12> EnpassantMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[static_cast<uint8_t>(this->attacking_pawn)] = squareIndexToBitboard(this->attacker_old_square) + squareIndexToBitboard(this->attacker_new_square);
    move_bitboard[static_cast<uint8_t>(this->target_pawn)] = squareIndexToBitboard(this->target_square);
    return move_bitboard;
}

PromotionMove::PromotionMove(Piece pawn, uint8_t old_square, uint8_t new_square, Piece promoted_piece){
    this->pawn = pawn;
    this->old_square = old_square;
    this->new_square = new_square;
    this->promoted_piece = promoted_piece;
}


std::array<uint64_t,12> PromotionMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[static_cast<int8_t>(this->pawn)] = squareIndexToBitboard(this->old_square);
    move_bitboard[static_cast<int8_t>(this->promoted_piece)] = squareIndexToBitboard(this->new_square);
    return move_bitboard;
}


TakeWithPromotionMove::TakeWithPromotionMove
(
    Piece attacking_pawn, 
    uint8_t attacking_pawn_square, 
    Piece target_piece, 
    uint8_t target_square, 
    Piece promoted_piece
){
    this->attacking_pawn = attacking_pawn;
    this->attacking_pawn_square = attacking_pawn_square;
    this->target_piece = target_piece;
    this->target_square = target_square;
    this->promoted_piece = promoted_piece;
}


std::array<uint64_t,12> TakeWithPromotionMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[static_cast<uint8_t>(this->attacking_pawn)] = squareIndexToBitboard(this->attacking_pawn_square);
    move_bitboard[static_cast<uint8_t>(this->target_piece)] = squareIndexToBitboard(this->target_square);
    move_bitboard[static_cast<uint8_t>(this->promoted_piece)] = squareIndexToBitboard(this->target_square);
    return move_bitboard;
}

std::array<std::unique_ptr<Move>, 4> getPromotionMoves
(
    Player player, 
    Piece pawn, 
    uint8_t old_square, 
    uint8_t new_square
)
{
    std::array<std::unique_ptr<Move>, 4> moves{};
    moves[0] = std::make_unique<PromotionMove>(pawn, old_square, new_square, getPlayerKnight(player));
    moves[1] = std::make_unique<PromotionMove>(pawn, old_square, new_square, getPlayerBishop(player));
    moves[2] = std::make_unique<PromotionMove>(pawn, old_square, new_square, getPlayerRook(player));
    moves[3] = std::make_unique<PromotionMove>(pawn, old_square, new_square, getPlayerQueen(player));
    return moves;
}

std::array<std::unique_ptr<Move>, 4> getTakeWithPromotionMoves
(
    Player player, 
    Piece pawn, 
    uint8_t pawn_square, 
    Piece target_piece, 
    uint8_t target_square
)
{
    std::array<std::unique_ptr<Move>,4> moves{};
    moves[0] = std::make_unique<TakeWithPromotionMove>
    (
        pawn, 
        pawn_square, 
        target_piece, 
        target_square, 
        getPlayerKnight(player)
    );
    moves[1] = std::make_unique<TakeWithPromotionMove>
    (
        pawn, 
        pawn_square, 
        target_piece, 
        target_square, 
        getPlayerBishop(player)
    );
    moves[2] = std::make_unique<TakeWithPromotionMove>
    (
        pawn, 
        pawn_square, 
        target_piece, 
        target_square, 
        getPlayerRook(player)
    );
    moves[3] = std::make_unique<TakeWithPromotionMove>
    (
        pawn, 
        pawn_square, 
        target_piece, 
        target_square, 
        getPlayerQueen(player)
    );
    return moves;
}