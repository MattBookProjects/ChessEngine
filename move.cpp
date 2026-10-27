#include "move.hpp"

Chessboard Move::applyMove(Chessboard chessboard){
    auto move_bitboard = this->constructMoveBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < chessboard.bitboard.size(); ++i){
        new_bitboard[i] = chessboard.bitboard[i] ^ move_bitboard[i]; 
    }
    auto player_to_move = chessboard.player_to_move == WHITE ? BLACK : WHITE;
    return Chessboard(
        new_bitboard,
        player_to_move,
        chessboard.can_white_castle,
        chessboard.can_black_castle,
        -1
    );
}

BasicMove::BasicMove(Piece piece, uint8_t old_square, uint8_t new_square){
    this->piece = piece;
    this->old_square = old_square;
    this->new_square = new_square;
}

Chessboard BasicMove::applyMove(Chessboard chessboard){
    auto move_bitboard = this->constructMoveBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < chessboad.bitboard.size(); ++i){
        new_bitboard[i] = cheessboard.bitboard[i] ^ move_bitboard[i];
    }
    Player player_to_move = cheessboard.player_to_move == WHITE ? BLACK : WHITE;
    int8_t enpassant = -1;
    if(this->piece == WHITE_PAWN && this->new_square - this->old_square == 16 ||
        this->piece == BLACK_PAWN && this->old_square - this->new_square == 16){
            enpassant = this->old_square % 8;
        }
    return Chessboard(
        new_bitboard, 
        player_to_move, 
        cheessboard.can_white_castle, 
        cheessboard.can_black_castle, 
        enpassant
    );
}


std::array<uint64_t, 12> BasicMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[piece] = squreIndexToU64(old_square) | squareIndexToU64(new_square);
    return move_bitboard;
}


TakeMove::TakeMove(uint8_t attacking_piece, uint8_t attacker_square, uint8_t target_piece, uint8_t target_square){
    this->attacking_piece = attacking_piece;
    this->attacker_square = attacker_square;
    this->target_piece = target_piece;
    this->target_square = target_square; 
}


uint64_t TakeMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[this->attacking_piece] = squareIndexToU64(this->attacker_square) | squareIndexToU64(this->target_square);
    move_bitboard[this->target_piece] = squareIndexToU64(this->target_square);
    return move_bitboard;
}

CastleMove::CastleMove(Player player, CastleSide side){
    this->player = player;
    this->side = side;
}

Chessboard CastleMove::applyMove(Chessboard chessboard){
    auto move_bitboard = this->constructMoveBitboard();
    std::array<uint64_t, 12> new_bitboard{};
    for(auto i = 0; i < chessboard.bitboard.size(); ++i){
        new_bitboard[i] = chessboard.bitboard[i] ^ move_bitboard[i];
    }
    Player player_to_move = chessboard.player_to_move == WHITE ? BLACK : WHITE;
    bool can_white_castle = player == WHITE ? false : chessboard.can_white_castle;
    bool can_black_castle = player == BLACK ? false : chessboard.can_black_castle;
    return Chessboard(
        new_bitboard,
        player_to_move,
        can_white_castle,
        can_black_castle,
        -1
    )
}

uint64_t CastleMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    auto rook = player == WHITE ? WHITE_ROOK : BLACK_ROOK;
    auto king = player == WHITE ? WHITE_KING : BLACK_KING;
    
    if(side == SHORT_CASTLE){
        move_bitboard[rook] = squareIndexToU64(7) | squareIndexToU64(5);
        move_bitboard[king] = squareIndexToU64(4) | squareIndexToU64(6);
    } else{
        move_bitboard[rook] = squareIndexToU64(0) | squareIndexToU64(3);
        move_bitboard[king] = squareIndexToU64(4) | squareIndexToU64(2);
    }
    return move_bitboard;
}


EnpassantMove::EnpassantMove(Piece attacking_pawn, uint8_t attacker_old_square, uint8_t attacker_new_square, Piece target_pawn, uint8_t target_square){
    this->attacking_pawn = attacking_pawn;
    this->attacker_old_square = attacker_old_square;
    this->attacked_new_square = attacked_new_square;
    this->target_pawn = target_pawn;
    this->target_square = target_square;
}



uint64_t EnpassantMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[this->attacking_pawn] = squareIndexToU64(this->attacker_old_square) + squareIndexToU64(this->attacker_new_square);
    move_bitboard[this->target_pawn] = squareIndexToU64(this->target_square);
    return move_bitboard;
}

PromotionMove::PromotionMove(Piece pawn, uint8_t old_square, uint8_t new_square, Piece promoted_piece){
    this->pawn = pawn;
    this->old_square = old_square;
    this->new_square = new_square;
    this->promoted_piece = promoted_piece;
}


uint64_t PromotionMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[this->pawn] = squareIndexToU64(this->old_square);
    move_bitboard[this-?promoted_piece] = squareIndexToU64(this->new_square);
    return move_bitboard;
}


TakeWithPromotionMove::TakeWithPromotionMove
(
    Piece pawn, 
    uint8_t pawn_square, 
    Piece target_piece, 
    uint8_t target_square, 
    Piece promoted_piece
){
    this->pawn = pawn;
    this->pawn_square = pawn_square;
    this->target_piece = target_piece;
    this->target_square = target_square;
    this->promoted_piece = promoted_piece;
}


uint64_t TakeWithPromotionMove::constructMoveBitboard(){
    std::array<uint64_t, 12> move_bitboard{};
    move_bitboard[this->pawn] = squareIndexToU64(this->pawn_square);
    move_bitboard[this->target_piece] = squareIndexToU64(this->target_square);
    move_bitboard[this->promoted_piece] = squareIndexToU64(this->target_square);
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