#include "chessboard.hpp"


const Player WHITE = 0;
const Player BLACK = 1;

const Piece WHITE_PAWN = 0;
const Piece WHITE_KNIGHT = 1;
const Piece WHITE_BISHOP = 2;
const Piece WHITE_ROOK = 3;
const Piece WHITE_QUEEN = 4;
const Piece WHITE_KING = 5;
const Piece BLACK_PAWN = 6;
const Piece BLACK_KNIGHT = 7;
const Piece BLACK_BISHOP = 8;
const Piece BLACK_ROOK = 9;
const Piece BLACK_QUEEN = 10;
const Piece BLACK_KING = 11;

const Result WHITE_WIN = 1;
const Result DRAW = 0;
const Result BLACK_WIN = -1;

const std::array<Piece, 6> WHITE_PIECES = {WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING};
const std::array<Piece, 6> BLACK_PIECES = {BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING};

Chessboard::Chessboard
(
    std::array<uint64_t, 12> bitboard,
    enum Player player_to_move, 
    bool can_white_castle, 
    bool can_black_castle, 
    int8_t white_enpassant, 
    int8_t black_enpassant
)
{
    this->bitboard = bitboard;
    this->player_to_move = player_to_move;
    this->can_white_castle = can_white_castle;
    this->can_black_castle = can_black_castle;
    this->white_enpassant = white_enpassant;
    this->black_enpassant = black_enpassant;
    
}


Chessboard Chessboard::initChessboard(){
    Chessboard chessboard = Chessboard();
    chessboard.initPawns();
    chessboard.initKnights();
    chessboard.initBishops();
    chessboard.initRooks();
    chessboard.initQueens();
    chessboard.initKings();
    return chessboard;
}

void Chessboard::initPawns(){
    for(int i = 0; i < 8; i++){
        this->bitboard[WHITE_PAWN] |= squareIndexToU64(8+i);
        this->bitboard[BLACK_PAWN] != squareIndexToU64(56-i);
    }
}

void Chessboard::initKnights(){
    this->bitboard[WHITE_KNIGHT] |= (squareIndexToU64(1) | squareIndexToU64(6));
    this->bitboard[BLACK_KNIGHT] |= (squareIndexToU64(57) | squareIndexToU64(62));
}

void Chessboard::initBishops(){
    this->bitboard[WHITE_BISHOP] |= (squareIndexToU64(2) | squareIndexToU64(5));
    this->bitboard[BLACK_BISHOP] |= (squareIndexToU64(58) | squareIndexToU64(61));
}

void Chessboard::initRooks(){
    this->bitboard[WHITE_ROOK] |= (squareIndexToU64(0) | squareIndexToU64(7));
    this->bitboard[BLACK_ROOK] |= (squareIndexToU64(56) | squareIndexToU64(63));
}

void Chessboard::initQueens(){
    this->bitboard[WHITE_QUEEN] |= squareIndexToU64(3);
    this->bitboard[BLACK_QUEEN] |= squareIndexToU64(59);
}

void Chessboard::initKings(){
    this->bitboard[WHITE_KING] |= squareIndexToU64(4);
    this->bitboard[BLACK_KING] |= squareIndexToU64(60);
}

uint64_t Chessboard::getPieceBitboard(Piece piece){
    return this->bitboard[piece];
}

uint64_t Chessboard::getPlayerBitboard(Player player){
    const auto &pieces = player == WHITE ? WHITE_PIECES : BLACK_PIECES;
    uint64_t result = 0;
    for(auto it=pieces.beign(); it != pieces.end(); ++it){
        result |= this->bitboard[it];
    }
    return result;
}

uint64_t Chessboard::getAllPiecesBitboard(){
    uint64_t result = 0;
    for(auto i=0; i < 12; i++){
        result |= this->bitboard[i];
    }
    return result;
}

bool Chessboard::isPlayerInCheck(Player player){
    
    Player opponent = player == WHITE ? BLACK : WHITE;
    
}



bool Cheessboard::isGameFinished(){
    auto legal_moves = this->getPlayerLegalMoves(this->player_to_move);
    if(legal_moves.size() > 0){
        return false;
    }
    if(isPlayerInCheck(this->player_to_move())){
        this->result = this->player_to_move == WHITE ? BLACK_WIN: WHITE_WIN;
    }
    else {
        this->result = DRAW;
    }
    return true;
}

uint8_t Cheessboard::getPLayersKingSquare(Player player){
    Piece king = player == WHITE ? WHITE_KING : BLACK_KING;
    return getSquareIndexesFromBitboard(this->bitboard[king]).front();
}

bool Chessboard::isSquareAttackedByPlayer(Player player, uint8_t square){
    Piece pawn = player == WHITE ? WHITE_PAWN : BLACK_PAWN;
    Piece knight = player == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT;
    Piece bishop = player == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
    Piece rook = player == WHITE ? WHITE_ROOK: BLACK_ROOK;
    Piece queen = player == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
    Piece king = player == WHITE ? WHITE_KING : BLACK_KING;
    return this->isSquareAttackedBySomePlyersPawn(pawn, square) ||
            this->isSquareAttackedBySomePlayersKnight(knight, square) ||
            this->isSquareAttackedBySomePlayersBishop(bishop, square) ||
            this->isSquareAttackedBySomePlayersRook(rook, square) ||
            this->isSquareAttackedBySomePlayerQueen(queen, square) ||
            this->isSquareAttackedByPlayersKing(king, square);
}


bool Chessboard::isSquareAttackedBySomePlayersPawns(Player player, uint8_t square){
    const auto pawn = player == WHITE ? WHITE_PAWN : BLACK_PAWN;
    const auto pawn_squares = getSquareIndexesFromBitboard(this->bitboard[pawn]);
    for(auto it = pawn_squares.begin(); it != pawn_squares.end(); ++it){
        if(doesPawnAttackSquare(player, *it, square)){
            return true;
        }
    }
    return false;
}





bool Cheessboard::isSquareAttackedBySomePlayersKnights(Player player, uint8_t square){
    const auto knight = player == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT;
    const auto knight_squares = getSquareIndexesFromBitboard(this->bitboard[knight]);
    for(auto it = knight_squares.begin(); it != knight_squares.end(); ++it){
        if(doesKnightAttackSquare(*it, square)){
            return true;
        }
    }
    return false;
}


bool Cheessboard::isSquareAttackedBySomePlayersBishops(Player player, uint8_t square){
    const auto bishop = player == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
    const auto bishop_squares = getSquareIndexesFromBitboard(this->bitboard[bishop]);
    for(auto it = bishop_squares.begin(); it != bishop_squares.end(); ++it){
        if(doesBishopAttackSquare(*it, square)){
            return true;
        }
    }
    return false;
}

bool Cheessboard::isSquareAttackedBySomePlayersRooks(Player player, uint8_t square){
    const auto rook = player == WHITE ? WHITE_ROOK : BLACK_ROOK;
    const auto rook_squares = getSquareIndexesFromBitboard(this->bitboard[rook]);
    for(auto it = rook_squares.begin(); it != rook_squares.end(); ++it){
        if(doesRookAttackSquare(*it, square)){
            return true;
        }
    }
    return false;
}

bool Cheessboard::isSquareAttackedBySomePlayerQueens(Player player, uint8_t square){
    const auto queen = player == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
    const auto queen_squares = getSquareIndexesFromBitboard(this->bitboard[queen]);
    for(auto it = queen_squares.begin(); it != queen_squares.end(); ++it){
        if(doesQueenAttackSquare(*it, square)){
            return true;
        }
    }
    return false;
}


bool Chessboard::isSquareAttackedByPlayersKing(Player player, uint8_t square){
    auto king_square = this->getPlayersKingSquare(player);
    return doesKingAttackSquare(king_square, square);
}

bool Cheessboard::doesPawnAttackSquare(Player player, uint8_t pawn_square, uint8_t target_square){
    int8_t left_attacked_square_diff = player == WHITE ? 7 : -9;
    int8_t right_attacked_square_diff = player == WHITE ? 9 : -7; 
    return target_square == pawn_square + left_attacked_square_diff && pawn_square % 8 > 0 || 
        target_square == pawn_square + right_attacked_square_diff && pawn_square % 8 < 7;   
}


bool Cheessboard::doesKnightAttackSquare(uint8_t knight_square, uint8_t target_square){
    int8_t diff = target_square - knight_square;
    uint8_t knight_rank = knight_square >> 3;
    uint8_t knight_file = knight_square % 8;
    return diff == 15 && knight_rank < 6 && knight_file > 0 ||
        diff == 17 && knight_rank < 6 && knight_file < 7 ||
        diff == 6 && knight_rank < 7 && knight_file > 1 ||
        diff == 10 && knight_rank < 7 && knight_file < 6 ||
        diff == -10 && knight_rank > 0 && knight_file > 1 ||
        diff == -6 && knight_rank > 0 && knight_file < 6 ||
        diff == -17 && knight_rank > 1 && knight_file > 0 ||
        diff == -15 && knight_rank > 1 && knight_file < 7;
}

bool Cheessboard::doesBishopAttackSquare(uint8_t bishop_square, uint8_t target_square){
    if(target_square - bishop_square % 7 != 0 && target_square - bishop_square != 9){
        return false;
    }
    uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
    auto occupied_squares = getSquareIndexesFromBitboard(all_pieces_bitboard);
    uint8_t start = target_square > bishop_square ? bishop_square : target_square;
    uint8_t end = square > bishop_square ? square : bishop_square;   
    uint8_t current_file = start % 8;
    uint8_t current_square = start;
    if(end - start % 7 == 0){
        current_square += 7;
        if(current_file == 0){
            return false;
        }
        current_file--;
        while (current_file > 0 && current_square != end){
            auto it = std::find(occupied_squares.begin(), occupied_squares.end(), current_square);
            if(*it != occupied_squares.end()){
                return false;
            }
            current_file--;
            current_square += 7;
        }
        if(current_square == end){
            return true;
        }
    } else{
        current_square += 9;
        if(current_file == 7){
            return false;
        }
        current_file++;
        while (current_file < 7 && current_square != end){
            auto it = std::find(occupied_squares.begin(), occupied_squares.end(), current_square);
            if(*it != occupied_squares.end()){
                return false;
            }
            current_file++;
            current_square += 9;
        }
        if(current_square == end){
            return true;
        }
    }
    return false;
}


bool Chessboard::doesRookAttackSquare(uint8_t rook_square, uint8_t target_square){
    if(rook_square - target_square % 8 != 0 && rook_square >> 3 != target_square >> 3){
        return false;
    }
    uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
    auto occupied_squares = getSquareIndexesFromBitboard(all_pieces_bitboard);
    uint8_t start = taarget_square > bishop_square ? bishop_square : target_square;
    uint8_t end = target_square > bishop_square ? target_square : bishop_square;
    if(end - start % 8 == 0){
        for(auto current=start+8; current<end; current+=8){
            auto it = std::find(occupied_squares.begin(), occupied_squares.end(), current);
            if(*it != occupied_squares.end()){
                return false;
            }
        }
    } else {
        for(auto current=start+1; current<end; ++current){
            auto it = std::find(occupied_squares.begin(), occupied_squares.end(), current);
            if(*it != occupied_squares.end()){
                return false;
            }
        }
    }
    return true;
}


bool Chessboard::doesQueenAttackSquare(uint8_t queen_square, uint8_t target_square){
    return this->doesBishopAttackSquare(queen_square, target_square) ||
            this->doesRookAttackSquare(queen_square, target_square);
}


bool Cheessboard::doesKingAttackSquare(uint8_t king_square, uint8_t target_square){
    uint8_t king_file = king_square % 8;
    int8_t diff = target_square - king_square;
    return diff == 8 || diff == -8 ||
            king_file > 0 && (diff == -9 || diff == 7) ||
            king_file < 7 && (diff == -7 || diff == 9);
}

std::array<uint8_t, 8> Chessboard::getPlayerPiecesBitboard(Player player){
    std::array<uint8_t, 8> bitboard = {};
    const auto &pieces = player == WHITE ? WHITE_PIECES: BLACK_PIECES;
    for(const auto p: pieces){
        for(auto i = 0; i < 8; i++){
            bitboard[i] |= this->bitboard[8*p + i];
        }
    }
    return bitboard;
}

std::array<uint8_t, 8> Chessboard::getAllPiecesBitboard(){

}

void printChessboard(uint8_t *chessboard){
    for(int p = 0; p < 12; p++){
        std::cout<<"piece: "<<p<<std::endl;
        for(int i = 0; i < 8; i++){
            std::cout<<(std::bitset<8>(chessboard[p*8+ i]))<<std::endl;
        }
    }
    std::cout<<"FINISHED PRINTING"<<std::endl;
    
}

void printChessboardFancy(uint8_t *chessboard){
    const char *fancy_board[64];
    for(int i = 0; i < 64; i++){
        fancy_board[i] = "  ";
    } 
    const char *piece_symbols[] = {"wP", "wN", "wB", "wR", "wQ", "wK", "bP", "bN", "bB", "bR", "bQ", "bK"};
   for(int i=0; i < 12; i++){
        for(int j=0; j<8; j++){
            if(chessboard[8*i+j] == 0){
                continue;
            }
            uint8_t rank_value = chessboard[8*i+j];
            for(int k = 7; k>=0; k--){
                uint8_t pow = pow_uint8((uint8_t)2, (uint8_t)k);
                if(rank_value < pow){
                    continue;
                }
                fancy_board[(7-j)*8 + k] = piece_symbols[i];
                rank_value -= pow;
            }
        }
    }
    for(int i = 0; i < 8; i++){
       // std::cout<<"PRINTING ROW "<<i<<std::endl;
        std::cout<<"-------------------------"<<std::endl;
        for(int j = 0; j < 8; j++){
            std::cout<<"|"<<fancy_board[8*i+j];
        }
        std::cout<<"|"<<std::endl;
    }
    std::cout<<"-------------------------"<<std::endl;
}


/*
std::vector<Move> Chessboard::getLegalMoves(){
    std::vector<Move> legal_moves = {};
    for (int i = 0; i < 64; i++){
        if this->squares[i] != 0 && doesPieceBelongToPlayer(this->player_to_move, this->squares[i]){
            switch(this->squares[i] % 8){
                case PAWN:

                case KNIGHT:
                case BISHOP:
                case ROOK:
                case QUEEN:
                case KING:
            }
        }
    }
    return legal_moves;
}


bool Chessboard::isPlayerInCheck(){
    uint8_t king_postion = this->getKingPosition(this->player_to_move);
    return this->isSquareAttackedByPlayer(this->board, (this->player_to_move+1)%2, king_postion);
    
}

uint8_t Chessboard::getKingPosition(enum Player player){
    for(int i = 0; i < 64; i++){
        if(this->squares[i] == player * 8 + KING){
            return i;
        }
    }
}





bool Chessboard::doesPieceBelongToPlayer(enum Player player, uint8_t piece){
    return player == WHITE && piece < 8 || player == BLACK && piece > 8;
}


bool Chessboard::isSquareAttackedByPlayer(uint8_t *board, enum Player player, uint8_t square){
    for(int i = 0; i < 64; i++){
        if(board[i] != 0 && doesPieceBelongToPlayer(player, board[i])){
            switch(board[i] % 8){
                case PAWN:
                    if(isPawnAttackingSquare(player, i, square)){
                        return true;
                    }
                    break;  
                case KNIGHT:
                    if(isKnightAttackingSquare(i, square)){
                        return true;
                    }
                    break;
                case BISHOP:
                    if (isBishopAttackingSquare(board, i, square)){
                        return true;
                    } 
                    break;
                case ROOK:
                    if (isRookAttackingSquare(board, i, square)){
                        return true;
                    }
                    break;
                case QUEEN:
                    if (isQueenAttackingSquare(board, i, square)){
                        return true;
                    }
                    break;
                case KING:
                    if (isKingAttackingSquare(i, square)){
                        return true;
                    }
                    break;
            }
        }
    }
    return false;
}


bool Chessboard::isPawnAttackingSquare(enum Player player, uint8_t pawn_square, uint8_t attacked_square){
    return player == WHITE && (attacked_square == pawn_square+7 || attacked_square == pawn_square+9) || player == BLACK && (attacked_square == pawn_square-7 || attacked_square == pawn_square-9)
}

bool Chessboard::isKnightAttackingSquare(uint8_t knight_square, uint8_t attacked_square){
    return attacked_square == knight_square+6 || attacked_square == knight_square+10 || attacked_square == i+15 || knight_square == i+17 || attacked_square == knight_square -6 || attacked_square == knight_square - 10 || attacked_square == knight_square - 15 || attacked_square == knight_square - 17;
}

bool Chessboard::isBishopAttackingSquare(uint8_t *board, uint8_t bishop_square, uint8_t attacked_square){
    if (bishop_square < attacked_squaresquare) {
        if(attacked_square - bishop_square % 9 == 0){
            int i = bishop_square + 9;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i+=9;
            }
            return true;
        } 
        if(attacked_squares - bishop_square % 7 == 0){
            int i = bishop_square + 7;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i+=7;
            }
            return true;
        }
        return false;
    } else {
        if(bishop_square - attacked_square % 9 == 0){
            int i = bishop_square - 9;
            while(i != attacked_square){
                if(board[i] != 0){
                    break;
                }
                i-=9;
            }
            return true;
        }
        if(bishop_square - attacked_square % 7 == 0){
            int i = bishop_square - 7;
            while(i != attacked_square){
                if(board[i] != 0){
                    break;
                }
                i-=7;
            }
            return true;
        }
    }
}

bool Chessboard::isRookAttackingSquare(uint8_t *board, uint8_t rook_square, uint8_t attacked_square){
    if(rook_square / 8 == attacked_square / 8){
        if(rook_square < attacked_square){
            int i = rook_square + 1;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i++;
            }
            return true;
        } else {
            int i = rook_square - 1;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i--;
            }
            return true;
        }
    }
    if(rook_square % 8 == attacked_square % 8){
        if(rook_square < attacked_square){
            int i = rook_square + 8;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i+=8;
            }
            return true;
        } else {
            int i = rook_square - 8;
            while(i != attacked_square){
                if(board[i] != 0){
                    return false;
                }
                i-=8;
            }
            return true;
        }
    }
    return false;
}

bool Chessboard::isQueenAttackingSquare(uint8_t *board, uint8_t queen_square, uint8_t attacked_square){
    return isBishopAttackingSquare(board, queen_square, attacked_square) || isRookAttackingSquare(board, queen_square, attacked_square);
}

bool Chessboard::isKingAttackingSquare(uint8_t king_square, uint8_t attacked_square){
    return attacked_square == king_square -9 || attacked_square == king_square - 8 || attacked_square == king_square - 7
    || attacked_square == king_square - 1 || attacked_square == king_square + 1 
    || attacked_square == king_square + 7 || attacked_square == king_square + 8 || attacked_square == king_square + 9;  
}

std::vector<Move> Chessboard::getPawnLegalMoves(enum Player player, uint8_t pawn_square){
    std::vector<Move> moves = {};
    uint8_t king_square = this->getKingPosition(player);
    if(player == WHITE){
        if(this->board[pawn_square+8] == 0){
            if(pawn_square / 8 == 6){
                
            }
        }
    }
}


*/

uint8_t *Chessboard::initChessboard(){
    uint8_t *chessboard = new uint8_t[8*12+3]{0};
    Chessboard::initPawns(chessboard);
    Chessboard::initKnights(chessboard);
    Chessboard::initBishops(chessboard);
    Chessboard::initRooks(chessboard);
    Chessboard::initQueens(chessboard);
    Chessboard::initKings(chessboard);
    Chessboard::initFlags(chessboard);
    return chessboard;
}

/*
void Chessboard::initPawns(uint8_t *chessboad){
    for(int i = 8; i < 16; i++){
        chessboad[i*12 + WHITE_PAWN] = 1;
        chessboad[(55-i)*12 + BLACK_PAWN] = 1;

    }
    return chessboad;
}*/

void Chessboard::initPawns(uint8_t *chessboard){
    chessboard[WHITE_PAWN * 8 + 1] = 255;
    chessboard[BLACK_PAWN * 8 + 6] = 255;
}

void Chessboard::initKnights(uint8_t *chessboard){
    chessboard[WHITE_KNIGHT * 8] = 64 | 2;
    chessboard[BLACK_KNIGHT * 8 + 7] = 64 | 2;
}

void Chessboard::initBishops(uint8_t *chessboard){
    chessboard[WHITE_BISHOP * 8] = 32 | 4;
    chessboard[BLACK_BISHOP * 8 + 7] = 32 | 4;
}

void Chessboard::initRooks(uint8_t *chessboard){
    chessboard[WHITE_ROOK * 8] = 128 | 1;
    chessboard[BLACK_ROOK * 8 + 7] = 128 | 1;
}

void Chessboard::initQueens(uint8_t *chessboard){
    chessboard[WHITE_QUEEN * 8] = 8;
    chessboard[BLACK_QUEEN * 8 + 7] = 8;
}

void Chessboard::initKings(uint8_t *chessboard){
    chessboard[WHITE_KING * 8] = 16;
    chessboard[BLACK_KING * 8 + 7] = 16;   
}

void Chessboard::initFlags(uint8_t *chessboard){
    chessboard[8*12] = 15;
}

uint8_t *Move::makeMove(uint8_t *chessboard){
    uint8_t *result = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+3);
    uint8_t *move = this->constructMoveBitboard(chessboard);
    array_xor(result, move, 8*12+3);
    return result;
}

BasicMove::BasicMove(uint8_t piece, uint8_t old_square, uint8_t new_square){
    this->old_square = old_square;
    this->new_square = new_square;
    this->piece = piece;
    std::cout<<"old_square: "<<(int)this->old_square<<"\nnew_square: "<<(int)this->new_square<<"\n piece: "<<(int)this->piece<<std::endl;
}

uint8_t *BasicMove::constructMoveBitboard(uint8_t *chessboard){
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    uint8_t old_square_rank = this->old_square >> 3;
    uint8_t old_square_file = pow_uint8((uint8_t)2, (uint8_t)this->old_square & 7);
    uint8_t new_square_rank = this->new_square >> 3;
    uint8_t new_square_file = pow_uint8((uint8_t)2, (uint8_t)this->new_square & 7);
    move[8*this->piece + old_square_rank] += old_square_file;
    move[8*this->piece + new_square_rank] += new_square_file; 
    move[8*12] = 128;
    if(this->piece == WHITE_PAWN && this->new_square - this->old_square == 16){
        move[8*12+1] = new_square_file;
    }
    else if(this->piece == BLACK_PAWN && this->old_square - this->new_square == 16){
        move[8*12+2] = new_square_file;
    }
    return move;
}


/*
uint8_t *BasicMove::makeMove(uint8_t *chessboard){
    uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = this->constructMoveBitboard(chessboard);
    array_xor(result, move, 8*12+3);
    return result;
    /*uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+3);
    uint8_t old_square_rank = this->old_square >> 3;
    std::cout<<"old_square_rank: "<<(int)old_square_rank<<std::endl;
    uint8_t old_square_file = pow_uint8((uint8_t)2, (uint8_t)this->old_square & 7);
    std::cout<<"old_square_file: "<<(int)old_square_file<<std::endl;
    uint8_t new_square_rank = this->new_square >> 3;
    std::cout<<"new_square_rank: "<<(int)new_square_rank<<std::endl;
    uint8_t new_square_file = pow_uint8((uint8_t)2, (uint8_t)this->new_square & 7);
    std::cout<<"new_square_file: "<<(int)new_square_file<<std::endl;
    move[8*this->piece + old_square_rank] += old_square_file;
    move[8*this->piece + new_square_rank] += new_square_file; 
    move[8*12] = 128;
    if(this->piece == WHITE_PAWN && this->new_square - this->old_square == 16){
        move[8*12+1] = new_square_file;
        move[8*12+2] = chessboard[8*12+2];
    }
    else if(this->piece == BLACK_PAWN && this->old_square - this->new_square == 16){
        move[8*12+1] = chessboard[8*12+1];
        move[8*12+2] = new_square_file;
    }
    else {
       move[8*12+1] = chessboard[8*12+1];
       move[8*12+2] = chessboard[8*12+2];
    }
    result[8*12] ^= move[8*12];
    result[this->piece * 8 + old_square_rank] ^= move[this->piece * 8 + old_square_rank];
    if(old_square_rank != new_square_rank){
        result[this->piece * 8 + new_square_rank] ^= move[this->piece * 8 + new_square_rank];
    }
    delete move;
    return result
}*/


TakeMove::TakeMove(uint8_t attacking_piece, uint8_t attacker_square, uint8_t target_piece, uint8_t target_square){
    this->attacking_piece = attacking_piece;
    this->attacker_square = attacker_square;
    this->target_piece = target_piece;
    this->target_square = target_square;
}

uint8_t *TakeMove::constructMoveBitboard(uint8_t *chessboard){
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    uint8_t attacker_square_rank = this->attacker_square >> 3;
    uint8_t attacker_square_file = pow_uint8((uint8_t)2, (uint8_t)this->attacker_square & 7);
    uint8_t target_square_rank = this->target_square >> 3;
    uint8_t target_square_file = pow_uint8((uint8_t)2, (uint8_t)this->target_square & 7);
    move[8*this->attacking_piece + attacker_square_rank] += attacker_square_file;
    move[8*this->attacking_piece + target_square_rank] += target_square_file;
    move[8*this->target_piece + target_square_rank] += target_square_file;
    move[8*12] = 128;
    return move;
}

/*
uint8_t *TakeMove::makeMove(uint8_t *chessboard){
    uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+2);
    uint8_t attacker_square_rank = this->attacker_square >> 3;
    uint8_t attacker_square_file = pow_uint8((uint8_t)2, (uint8_t)this->attacker_square & 7);
    uint8_t target_square_rank = this->target_square >> 3;
    uint8_t target_square_file = pow_uint8((uint8_t)2, (uint8_t)this->target_square & 7);
    move[8*this->attacking_piece + attacker_square_rank] += attacker_square_file;
    move[8*this->attacking_piece + target_square_rank] += target_square_file;
    move[8*this->target_piece + target_square_rank] += target_square_file;
    move[8*12] = 128;
    move[8*12+1] = chessboard[8*12+1];
    move[8*12+2] = chessboard[8*12+2];
    result[8*12] ^= move[8*12];
    result[8*12+1] ^= move[8*12+1];
    result[8*this->target_piece + target_square_rank] ^= move[8*this->target_piece + target_square_rank];
    result[8*this->attacking_piece + attacker_square_rank] ^= move[8*this->attacking_piece + attacker_square_rank];
    if(attacker_square_rank != target_square_rank){
        result[8*this->attacking_piece + target_square_rank] ^= move[8*this->attacking_piece + target_square_rank];
    }
    delete move;
    return result;
}
*/

CastleMove::CastleMove(uint8_t player, uint8_t side){
    this->player = player;
    this->side = side;
}

uint8_t *CastleMove::constructMoveBitboard(uint8_t *chessboard){
    //std::cout<<"CastleMove::constructMoveBitboard start"<<std::endl;
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    uint8_t rook_piece;
    uint8_t king_piece; 
    uint8_t rank;
    uint8_t rook_rank_value;
    uint8_t king_rank_value;
    uint8_t player_castle_flag = this->player + 1;
    if(this->player == WHITE){
        rook_piece = WHITE_ROOK;
        king_piece = WHITE_KING;
        rank = 0;
    } else {
        rook_piece = BLACK_ROOK;
        king_piece = BLACK_KING;
        rank = 7;
    }
    if(this->side == SHORT_CASTLE){
        rook_rank_value = 128 | 32;
        king_rank_value = 16 | 64;
    } else {
        rook_rank_value = 1 | 8;
        king_rank_value = 16 | 4;
    }
    move[8*rook_piece + rank] = rook_rank_value;
    move[8*king_piece + rank] = king_rank_value;
    move[8*12] = 128 + player_castle_flag;
    //std::cout<<"CastleMove::constructMoveBitboard end"<<std::endl;
    //printChessboard(move);
    return move;
}

/*
uint8_t *CastleMove::makeMove(uint8_t *chessboard){
    uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+2);
    uint8_t rook_piece;
    uint8_t king_piece; 
    uint8_t row_num;
    uint8_t rook_row_value;
    uint8_t king_row_value;
    uint8_t player_castle_flag = this->player + 1;
    if(this->player == WHITE){
        rook_piece = WHITE_ROOK;
        king_piece = WHITE_KING;
        row_num = 0;
    } else {
        rook_piece = BLACK_ROOK;
        king_piece = BLACK_KING;
        row_num = 7;
    }
    if(this->side == SHORT_CASTLE){
        rook_row_value = 128 | 32;
        king_row_value = 16 | 64;
    } else {
        rook_row_value = 1 | 8;
        king_row_value = 16 | 4;
    }
    move[8*rook_piece + row_num] = rook_row_value;
    move[8*king_piece + row_num] = king_row_value;
    move[8*12] = 128 + player_castle_flag;
    move[8*12+1] = chessboard[8*12+1];
    move[8*12+2] = chessboard[8*12+2];
    result[8*rook_piece + row_num] ^= move[8*rook_piece + row_num];
    result[8*king_piece + row_num] ^= move[8*king_piece + row_num];
    result[8*12] ^= move[8*12];
    delete move;
    return result;
}*/


EnpassantMove::EnpassantMove(uint8_t player, uint8_t attacker_file, uint8_t target_file){
    this->player = player;
    this->attacker_file = attacker_file;
    this->target_file = target_file;
}

/*
uint8_t *EnpassantMove::makeMove(uint8_t *chessboard){
    uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+3);
    move[8*12] = 128;
    move[8*12+1] = chessboard[8*12+1];
    move[8*12+2] = chessboard[8*12+2];
    result[8*12] ^= move[8*12];
    result[8*12+1] ^= move[8*12+1];
    result[8*12+2] ^= move[8*12+2];
    if(this->player == WHITE){
        move[8*WHITE_PAWN + 4] = pow_uint8((uint8_t)2, this->attacker_file);
        move[8*WHITE_PAWN + 5] = pow_uint8((uint8_t)2, this->target_file);
        move[8*BLACK_PAWN + 4] = pow_uint8((uint8_t)2, this->target_file);
        result[8*WHITE_PAWN + 4] ^= move[8*WHITE_PAWN + 4];
        result[8*WHITE_PAWN + 5] ^= move[8*WHITE_PAWN + 5];
        result[8*BLACK_PAWN + 4] ^= move[8*BLACK_PAWN + 4];
    } else{
        move[8*BLACK_PAWN + 3] = pow_uint8((uint8_t)2, this->attacker_file);
        move[8*BLACK_PAWN + 2] = pow_uint8((uint8_t)2, this->target_file);
        move[8*WHITE_PAWN + 3] = pow_uint8((uint8_t)2, this->target_file);
        result[8*BLACK_PAWN + 3] ^= move[8*BLACK_PAWN + 3];
        result[8*BLACK_PAWN + 2] ^= move[8*BLACK_PAWN + 2];
        result[8*WHITE_PAWN + 3] ^= move[8*WHITE_PAWN + 3];
    }
    delete move;
    return result;
}*/

uint8_t *EnpassantMove::constructMoveBitboard(uint8_t *chessboard){
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    move[8*12] = 128;
    uint8_t attacker_file_value = pow_uint8((uint8_t)2, this->attacker_file);
    uint8_t target_file_value = pow_uint8((uint8_t)2, this->target_file);
    if(this->player == WHITE){
        move[8*WHITE_PAWN + 4] = attacker_file_value;
        move[8*WHITE_PAWN + 5] = target_file_value;
        move[8*BLACK_PAWN + 4] = target_file_value;
    } else{
        move[8*BLACK_PAWN + 3] = attacker_file_value;
        move[8*BLACK_PAWN + 2] = target_file_value;
        move[8*WHITE_PAWN + 3] = target_file_value;
    }
    std::cout<<"ENPASSANT MOVE BITBOARD:"<<std::endl;
    printChessboard(move);
    return move;
}

TakeWithPromotionMove::TakeWithPromotionMove(uint8_t attacking_pawn, uint8_t attacker_file, uint8_t target_piece, uint8_t target_file, uint8_t promoted_piece){
    this->attacking_pawn = attacking_pawn;
    this->attacker_file = attacker_file;
    this->target_piece = target_piece;
    this->target_file = target_file;
    this->promoted_piece = promoted_piece;
}

/*
uint8_t *TakeWithPromotionMove::makeMove(uint8_t *chessboad){
    uint8_t *result = new uint8_t[8*12+3]{0};
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(result, chessboard, 8*12+3);
    move[8*12] = 128;
    move[8*12+1] = chessboard[8*12+1];
    move[8*12+2] = chessboard[8*12+2];
    result[8*12] ^= move[8*12];
    result[8*12+1] ^= move[8*12+1];
    result[8*12+2] ^= move[8*12+2];
    uint8_t attacking_pawn;
    uint8_t attacker_rank;
    uint8_t target_rank;
    if(attacking_player == WHITE){
        attacker_rank = 6;
        target_rank = 7;
        attacking_pawn = WHITE_PAWN;
    } else {
        attacker_rank = 1;
        target_rank = 0;
        attacking_pawn = BLACK_PAWN;
    }

    move[8*this->attacking_pawn+attacker_rank] = pow_uint8((uint8_t)2, attacker_file);
    move[8*this->target_piece+target_rank] = pow_uint8((uint8_t)2, target_file);
    move[8*this->promoted_piece+target_rank] = pow_uint8((uint8_t)2. target_file);
    result[8*this->attacking_pawn+attacker_rank] ^= move[8*this->attacking_pawn+attacker_rank];
    result[8*this->target_piece+target_rank] ^= move[8*this->target_piece+target_rank];
    result[8*this->promoted_piece+target_rank] ^= move[8*this->promoted_piece+target_rank];
    delete move;
    return result;
}*/

uint8_t *TakeWithPromotionMove::constructMoveBitboard(uint8_t *chessboard){
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    move[8*12] = 128;
    uint8_t attacker_rank;
    uint8_t target_rank;
    if(this->attacking_pawn == WHITE_PAWN){
        attacker_rank = 6;
        target_rank = 7;        
    } else {
        attacker_rank = 1;
        target_rank = 0;
    }
    uint8_t attacker_file_value = pow_uint8((uint8_t)2, attacker_file);
    uint8_t target_file_value = pow_uint8((uint8_t)2, target_file);
    move[8*this->attacking_pawn+attacker_rank] = attacker_file_value;
    move[8*this->target_piece+target_rank] = target_file_value;
    move[8*this->promoted_piece+target_rank] = target_file_value;
    return move;
}

PromotionMove::PromotionMove(uint8_t pawn, uint8_t file, uint8_t promoted_piece){
    this->pawn = pawn;
    this->file = file;
    this->promoted_piece = promoted_piece;
}

uint8_t *PromotionMove::constructMoveBitboard(uint8_t *chessboard){
    uint8_t *move = new uint8_t[8*12+3]{0};
    std::memcpy(move+8*12+1, chessboard+8*12+1, 2);
    move[8*12] + 128;
    uint8_t file_value = pow_uint8((uint8_t)2, this->file);
    uint8_t old_rank;
    uint8_t new_rank;
    if(this->pawn == WHITE_PAWN){
        old_rank = 6;
        new_rank = 7;
    } else{
        old_rank = 1;
        new_rank = 0;
    }
    move[8*this->pawn+old_rank] = file_value;
    move[8*this->promoted_piece+new_rank] = file_value;
    return move; 
}



bool isPlayerInCheck(uint8_t *chessboard, uint8_t player){
    uint8_t king_square = getKingSquare(chessboard, player);
    uint8_t attacking_player = player + 1 % 2;
    return isSquareAttackedByPlayer(chessboard, attacking_player, king_square);
}

uint8_t getKingSquare(uint8_t *chessboard, uint8_t player){
    uint8_t king;
    if(player == WHITE){
        king = WHITE_KING;
    }else {
        king = BLACK_KING;
    }
    for(int i = 0; i < 8; i++){
        uint8_t rank_value = chessboard[8*king+i];
        if(rank_value == 0){
            continue;
        }
        for(int j = 7; j >= 0; j++){
            uint8_t pow = pow_uint8((uint8_t)2, (uint8_t)j);
            if(rank_value != pow){
                continue;
            }
            return 8*i + j;
        }
    }
}


bool isSquareAttackedByPlayer(uint8_t *chessboard, uint8_t player, uint8_t square){
    uint8_t pawn;
    uint8_t knight;
    uint8_t bishop;
    uint8_t rook;
    uint8_t queen;
    uint8_t king;
    if(player == WHITE){
        pawn = WHITE_PAWN;
        knight = WHITE_KNIGHT;
        bishop = WHITE_BISHOP;
        rook = WHITE_ROOK;
        queen = WHITE_QUEEN;
        king = WHITE_KING;
    } else {
        pawn = BLACK_PAWN;
        knight = BLACK_KNIGHT;
        bishop = BLACK_BISHOP;
        rook = BLACK_ROOK;
        queen = BLACK_QUEEN;
        king = BLACK_KING;
    }

    return isSquareAttackedByPawn(chessboard, pawn, square)
        || isSquareAttackedByKnight(chessboard, knight, square)
        || isSquareAttackedByBishop(chessboard, bishop, square)
        || isSquareAttackedByRook(chessboard, rook, square)
        || isSquareAttackedByQueen(chessboard, queen, square)
        || isSquareAttackedByKing(chessboard, king, square);
}


bool isSquareAttackedByPawn(uint8_t *chessboard, uint8_t pawn, uint8_t square){
    if(pawn == WHITE_PAWN){
        for(int i=0; i<7; i++){
            if(chessboard[8*pawn+i] == 0){
                continue;
            }
            uint8_t rank_value = chessboard[8*pawn+i];
            for(int j=7; j>=0; j--){
                uint8_t pow = pow_uint8((uint8_t)2,(uint8_t)j);
                if(rank_value < pow){
                    continue;
                }
               // std::cout<<"current pow value: "<<(int)pow<<std::endl;
                uint8_t pawn_square = 8*i + j;
                uint8_t pawn_file = pawn_square % 8;
                if(pawn_file > 0 && square == pawn_square + 7 
                    || pawn_file < 7 && square == pawn_square + 9){
                    return true;
                }
                rank_value -= pow;
            }
        }
    }
    else{
        for(int i=7; i>0; i--){
            if(chessboard[8*pawn+i] == 0){
                continue;
            }
            uint8_t rank_value = chessboard[8*pawn+i];
            for(int j=7; j>=0; j--){
                uint8_t pow = pow_uint8((uint8_t)2,(uint8_t)j);
                if(rank_value < pow){
                    continue;
                }
                uint8_t pawn_square = 8*i + j;
                uint8_t pawn_file = pawn_square % 8;
              //  std::cout<<"pawn square: "<<(int)pawn_square<<"\n"<<"pawn file: "<<(int)pawn_file<<"\n"<<"pow: "<<(int)pow<<std::endl;
                if(pawn_file < 7 && square == pawn_square - 7  
                    || pawn_file > 0 && square == pawn_square - 9){
                    return true;
                }
                rank_value -= pow;
            }
        
        }
    }
    return false;    
}

bool isSquareAttackedByKnight(uint8_t *chessboard, uint8_t knight, uint8_t square){
    for(int i=0; i<8; i++){
        if(chessboard[8*knight+i] == 0){
            continue;
        }
        uint8_t rank_value = chessboard[8*knight+i];
        for(int j=7; j>=0; j--){
            uint8_t pow = pow_uint8((uint8_t)2,(uint8_t)j);
            if(rank_value < pow){
                continue;
            }
            uint8_t knight_square = 8*i + j;
            int8_t diff = square - knight_square;
            uint8_t knight_square_file = knight_square % 8;
            if( 
                (diff == 17 || diff == -15) && knight_square_file < 7
                || (diff == 15 || diff ==-17) && knight_square_file > 0
                || (diff == 10 || diff == -6) && knight_square_file < 6
                || (diff == 6 || diff == -10) && knight_square_file > 1
                ){
                    return true;
                }
                rank_value -= pow;
            
             
        }
    }
    return false;
}
    



bool isSquareAttackedByBishop(uint8_t *chessboard, uint8_t bishop, uint8_t square){
    uint8_t *all_pieces_bitboard = getAllPiecesBitboard(chessboard);
    for(int i=0; i<8; i++){
        uint8_t rank_value = chessboard[8*bishop+i];
        if(rank_value == 0){
            continue;
        }
        for(int j=7; j>=0; j--){
            uint8_t pow = pow_uint8((uint8_t)2,(uint8_t)j);
            if(rank_value < pow){
                continue;
            }
            uint8_t bishop_square = 8*i + j;
            if(square - bishop_square % 7 == 0){
                uint8_t start;
                uint8_t end;
                if(square > bishop_square){
                    start = bishop_square;
                    end = square;
                }else{
                    start = square;
                    end = bishop_square;
                }
                uint8_t current_file = start % 8;
                uint8_t current_square = start + 7;
                uint8_t current_rank = start >> 3 + 1;
                if(current_file == 0){
                    rank_value -= pow;
                    continue;
                }
                current_file--;
                bool is_blocked = false;
                while(current_file > 0 &&  current_square < end){
                    if(all_pieces_bitboard[current_rank] & pow_uint8((uint8_t)2, current_file)){
                        is_blocked = true;
                        break;
                    }
                    current_file--;
                    current_square+=7;
                }
                if(is_blocked){
                    rank_value -= pow;
                    continue;
                }
                if(current_square == square){
                    return true;
                } else{
                    rank_value -= pow;
                    continue;
                }
            }                    
            else if(square - bishop_square % 9 == 0){
                uint8_t start;
                uint8_t end;
                if(square > bishop_square){
                    start = bishop_square;
                    end = square;
                }else{
                    start = square;
                    end = bishop_square;
                }
                uint8_t current_file = start % 8;
                uint8_t current_square = start + 9;
                uint8_t current_rank = start >> 3 + 1;
                if(current_file == 7){
                    rank_value -= pow;
                    continue;
                }
                current_file++;
                bool is_blocked = false;
                while(current_file < 7 &&  current_square < end){
                    if(all_pieces_bitboard[current_rank] & pow_uint8((uint8_t)2, current_file)){
                        is_blocked = true;
                        break;
                    }
                    current_file++;
                    current_rank++;
                    current_square+=9;
                }
                if(is_blocked){
                    rank_value -= pow;
                    continue;
                }
                if(current_square == square){
                    return true;
                } else{
                    rank_value -= pow;
                    continue;
                }

            }
            rank_value -= pow;
        }
    }
    return false;
}


bool isSquareAttackedByRook(uint8_t *chessboard, uint8_t rook, uint8_t square){
    uint8_t *all_pieces_bitboard = getAllPiecesBitboard(chessboard);
    for(int i=0; i<8; i++){
        if(chessboard[8*rook+i] == 0){
            continue;
        }
        uint8_t rank_value = chessboard[8*rook+i];
        for(int j=7; j>=0; j--){
            uint8_t pow = pow_uint8((uint8_t)2,(uint8_t)j);
            if(rank_value < pow){
                continue;
            }
            uint8_t rook_square = 8*i + j;
            if(square - rook_square % 8 == 0){
                uint8_t start;
                uint8_t end;
                if(square > rook_square){
                    start = rook_square;
                    end = square;
                } else{
                    start = square;
                    end = rook_square;
                }
                uint8_t file = square % 8;
                uint8_t current_rank = start >> 3 + 1;
                uint8_t current_square = start + 8;
                bool is_blocked = false;
                while(current_square < end ){
                    if(all_pieces_bitboard[current_rank] & pow_uint8((uint8_t)2, file)){
                        is_blocked = true;
                        break;
                    }
                    current_square += 8;
                    current_rank++;
                }
                if(is_blocked){
                    rank_value -= pow;
                    continue;
                }
                return true;
            } else if(square >> 3 == rook_square >> 3){
                uint8_t start;
                uint8_t end;
                if(square > rook_square){
                    start = rook_square;
                    end = square;
                } else{
                    start = square;
                    end = rook_square;
                } 
                uint8_t current_file = start % 8 + 1;
                uint8_t rank = square >> 3;
                uint8_t current_square = start + 1;
                bool is_blocked = false;
                while(current_square < end){
                    if(all_pieces_bitboard[rank] & pow_uint8((uint8_t)2, current_file)){
                        is_blocked = true;
                        break;
                    }
                    current_file++;
                    current_square++;
                }
                if(is_blocked){
                    rank_value -= pow;
                    continue;
                }
                return true;
            }
            rank_value -= pow;
        }
    }
    return false;
}


bool isSquareAttackedByQueen(uint8_t *chessboard, uint8_t queen, uint8_t square){
    return isSquareAttackedByBishop(chessboard, queen, square) || isSquareAttackedByRook(chessboard, queen, square);
}

bool isSquareAttackedByKing(uint8_t *chessboard, uint8_t king, uint8_t square){
    for(int rank=0; rank<8; rank++){
        uint8_t rank_value = chessboard[8*king + rank];
        if(rank_value == 0){
            continue;
        }
        for(int file=7; file>=0; file--){
            if(rank_value != pow_uint8((uint8_t)2, (uint8_t)file)){
                continue;
            }
            uint8_t king_square = 8*rank + file;
            if(
                king_square - square == 8 || king_square - square == -8 
                || file > 0 && (king_square - square == 1 || king_square - square == 9 || king_square - square == -7)
                || file < 7 && (king_square - square == -1 || king_square - square == 7 || king_square - square == -9)
            ){
                return true;
            } else{
                return false;
            }
        }
    }
    return false;
}
/*
uint8_t *getPiecePseudoCoveredSquares(uint8_t *chessboard, uint8_t piece){

}

uint8_t *getPawnPseudoCoveredSquares(uint8_t *chessboard, uint8_t pawn){

}

uint8_t *getKnigthPseudoCoveredSquares(uint8_t *chessboard, uint8_t)
*/


uint8_t *getAllPiecesBitboard(uint8_t *chessboard){
    uint8_t *bitboard = new uint8_t[8]{0};
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 12; j++){
            bitboard[i] |= chessboard[j*8+i];
        }   
    }
    return bitboard;
}


uint8_t *getPlayerPiecesBitboard(uint8_t *chessboard, uint8_t player){
    uint8_t *bitboard = new uint8_t[8]{0};
    uint8_t start = player * 6;
    for(int i = 0; i <8; i++){
        for(int j = start; j < start+6; j++){
            bitboard[i] |= chessboard[j*8+i];
        }
    }
    return bitboard;
}

bool isMoveLegal(uint8_t *chessboard, Move *move){
    uint8_t player = chessboard[96] & 128 ? BLACK: WHITE;
    uint8_t *board_after_move = move->makeMove(chessboard);
    return !isPlayerInCheck(board_after_move, player);
}


std::vector<Move> getLegalMoves(uint8_t *chessboard){
    
}



std::vector<Move> getPlayersLegalPawnMoves(uint8_t *chessboard, uint8_t player){
    uint8_t opponent = player + 1 % 2;
    uint8_t pawn, knight, bishop, rook, queen, first_rank, penultimate_rank, last_rank, enpassant_rank;
    int8_t step, left_attacked_square_diff, right_attacked_square_diff;
    std::vector<*Move> potential_moves = new std::vector<>();
    std::vector<*Move> legal_moves = new std::vector<>();
    uint8_t *all_pieces_bitboard = getAllPiecesBitboard(chessboard);
    uint8_t *opponent_pieces_bitboard = getPlayerPiecesBitboard(opponent);

    if(pawn == WHITE_PAWN){
        pawn = WHITE_PAWN;
        knight = WHITE_KNIGHT;
        bishop = WHITE_BISHOP;
        rook = WHITE_ROOK;
        queen = WHITE_ROOK;
        opponent_pawn = BLACK_PAWN;
        opponent_king = BLACK_KING;
        first_rank = 0;
        penultimate_rank = 6;
        last_rank = 7;
        step = 1;
        left_attacked_square_diff = 7;
        right_attacked_square_diff = 9;
        enpassant_rank = 4;
    }
    else{
        pawn = BLACK_PAWN;
        knight = BLACK_KNIGHT;
        bishop = BLACK_BISHOP;
        rook = BLACK_ROOK;
        queen = BLACK_QUEEN;
        opponent_pawn = WHITE_PAWN;
        opponent_king = WHITE_KING;
        first_rank = 7;
        penultimate_rank = 1;
        last_rank = 0;
        step = -1;
        left_attacked_square_diff = -9;
        right_attacked_square_diff = -7;
        enpassant_rank = 3;
    }
    for(int i = first_rank; i != penultimate_rank; i+=step){
        uint8_t rank_value = chessboard[8*pawn+i];
        if(rank_value == 0){
                continue;
        }
        for(int j = 7; j <= 0; j++){
            uint8_t pow = pow_uint8((uint8_t)2, (uint8_t)j);
            if(rank_value<pow){
                continue;
            }
            uint8_t old_square = 8*i + j;
            if(!(all_pieces_bitboard[i+step] & pow)){
                uint8_t new_square = old_square + 8;
                potential_moves.push_back(new BasicMove((uint8_t)pawn, old_square, new_square));   
            }
            if(j > 0 && (opponent_pieces_bitboard[i+step] & (pow >> 1))){
                for(uint8_t k=BLACK_PAWN; k<BLACK_KING; k++){
                    if(chessboard[8*k+i+step] & (pow >> 1)){
                        uint8_t target_square = old_square + left_attacked_square_diff;
                        potential_moves.push_back(new TakeMove((uint8_t)pawn, old_square, k, target_square));
                    }
                }
            }
            if(j < 7 && (opponent_pieces_bitboard[i+step] & (pow << 1))){
                for(uint8_t k=opponent_pawn; k<opponent_king; k++){
                    if(chessboard[8*k+i+step] & (pow << 1)){
                        uint8_t target_square = old_square + right_attacked_square_diff;
                        potential_moves.push_back(new TakeMove((uint8_t)pawn, old_square, k, target_square));
                    }
                }
            }
            if(i == enpassant_rank && chessboard[97+opponent]){
                uint8_t enpassant_value = chessboard[97+opponent];
                if(enpassant_value == pow >> 1){
                    potential_moves.push_back(new EnpassantMove(player, j, j-1));
                }
                if(enpassant_value == pow << 1){
                    potential_moves.push_back(new EnpassantMove(player, j, j+1));
                }
                
            }
            rank_value -= pow;
        }
    }
    uint8_t penultimate_rank_value = chessboard[8*pawn+penultimate_rank];
    if(penultimate_rank_value != 0){
        for(uint8_t j = 7; j <= 0; j++){
            uint8_t pow = pow_uint8((uint8_t)2, j);
            if(rank_value<pow){
                continue;
            }
            if(!(all_pieces_bitboard[last_rank] & pow)){
                potential_moves.push_back(new PromotionMove((uint8_t)pawn, j, (uint8_t)knight));   
                potential_moves.push_back(new PromotionMove((uint8_t)pawn, j, (uint8_t)bishop));   
                potential_moves.push_back(new PromotionMove((uint8_t)pawn, j, (uint8_t)rook));    
                potential_moves.push_back(new PromotionMove((uint8_t)pawn, j, (uint8_t)queen));   
            }
            if(opponent_pieces_bitboard[last_rank] & (pow<<1)){
                for(uint8_t k=opponent_pawn; k<opponent_king; k++){
                    if(j > 0 && chessboard[8*k+last_rank] & (pow >> 1)){
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j-1, (uint8_t)knight));   
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j-1, (uint8_t)bishop));   
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j-1, (uint8_t)rook));    
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j-1, (uint8_t)queen));
                    }
                    if(j < 7 && chessboard[8*k+last_rank] & (pow << 1)){
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j+1, (uint8_t)knight));   
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j+1, (uint8_t)bishop));   
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j+1, (uint8_t)rook));    
                        potential_moves.push_back(new TakeWithPromotionMove((uint8_t)pawn, j, k, j+1, (uint8_t)queen));
                    }
                }
            }
        }
    }
    std::vector<*Move>::iterator it;
    for(it=potential_moves.begin(); it != potential_moves.end(); it++){
        if(isMoveLegal(chessboard, *it)){
            legal_moves.push_back(*it);
        } else{
            delete *it;
        }
    }
    return legal_moves;
}
   

std::vector<Move> getLegalKnightMoves(uint8_t *chessboard, uint8_t player){
    std::vector<*Move> potential_moves = new std::vector<>();
    std::vector<*Move> legal_moves = new std::vector<>();
    uint8_t knight, opponent, opponent_pawn, opponent_king;
    uint8_t *opponent_pieces_bitboard, *all_pieces_bitboard;
    if(player == WHITE){
        knight = WHITE_KNIGHT;
        opponent = BLACK;
        opponent_pawn = BLACK_PAWN;
        opponent_king = BLACK_KING;
    } else{
        knight = BLACK_KNIGHT;
        opponent = WHITE;
        opponent_pawn = WHITE_PAWN;
        opponent_king = WHITE_KING;
    }
    opponent_pieces_bitboard = getPlayerPiecesBitboard(chessboard, opponent);
    all_pieces_bitboard = getAllPiecesBitboard(chessboard);
    for(uint8_t i = 0; i < 8; i++){
        uint8_t rank_value = chessboard[8*knight + i] 
        if(rank_value == 0){
            continue;
        }
        for(uint8_t j = 7; j>=0; j++){
            uint8_t pow = pow_uint8((uint8_t)2, j);
            uint8_t old_square = 8*i+j;
            uint8_t new_square;
            if(rank_value < pow){
                continue;
            }
            if(i < 6 && j > 0){
                new_square = old_square + 15;
                if(!(all_pieces_bitboard[i+2] & pow >> 1)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i+2] & pow >> 1){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i+2] & pow >> 1){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i < 6 && j < 7){
                new_square = old_square + 17;
                if(!(all_pieces_bitboard[i+2] & pow << 1)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i+2] & pow << 1){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i+2] & pow << 1){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
             if(i < 7 && j > 1){
                new_square = old_square + 6;
                if(!(all_pieces_bitboard[i+1] & pow >> 2)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i+1] & pow >> 2){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i+1] & pow >> 2){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i < 7 && j < 6){
                new_square = old_square + 10;
                if(!(all_pieces_bitboard[i+1] & pow << 2)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i+1] & pow << 2){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i+1] & pow << 2){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i > 0 && j > 1){
                new_square = old_square -10;
                if(!(all_pieces_bitboard[i-1] & pow >> 2)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i-1] & pow >> 2){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i-1] & pow >> 2){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i > 0 && j < 6){
                new_square = old_square -6;
                if(!(all_pieces_bitboard[i-1] & pow << 2)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i-1] & pow << 2){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i-1] & pow << 2){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i > 1 && j > 0){
                new_square = old_square - 17;
                if(!(all_pieces_bitboard[i-2] & pow >> 1)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i-2] & pow >> 1){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i-2] & pow >> 1){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
            if(i > 1 && j < 7){
                new_square = old_square - 15;
                if(!(all_pieces_bitboard[i-2] & pow << 1)){
                    potential_moves.push_back(new Move(knight, old_square, new_square));
                }
                else if(opponent_pieces_bitboard[i-2] & pow << 1){
                    for(uint8_t k=opponent_pawn; k < opponent_king; k++){
                        if(chessboard[8*k+i-2] & pow << 1){
                            potential_moves.push_back(new TakeMove(knight, old_square, k, new_square));
                            break;
                        }
                    }
                }
            }
        }
    }
    std::vector<*Move>::iterator it;
    for(it=potential_moves.begin(); it!=potential_moves.end(); it++){
        if(isMoveLegal(chessboard, *it)){
            legal_moves.push_back(*it);
        }else{
            delete *it;
        }
    }
    return legal_moves;
}

std::vector<*Move> getBishopLegalMoves(uint8_t *chessboard, uint8_t player){
    std::vector<*Move> potential_moves = new std::vector<>();
    std::vector<*Move> legal_moves = new std::vector<>();
    uint8_t bishop, opponent, opponent_pawn, opponent_king;
    uint8_t *opponent_pieces_bitboard, *all_pieces_bitboard;
    if(player == WHITE){
        bishop = WHITE_BISHOP;
        opponent = BLACK;
        opponent_pawn = BLACK_PAWN;
        opponent_king = BLACK_KING;
    } else{
        bishop = BLACK_BISHOP;
        opponent = WHITE;
        opponent_pawn = WHITE_PAWN;
        opponent_king = WHITE_KING;
    }
    opponent_pieces_bitboard = getPlayerPiecesBitboard(chessboard, opponent);
    all_pieces_bitboard = getAllPiecesBitboard(chessboard);
    for(uint8_t i = 0; i < 8; i++){
        uint8_t rank_value = chessboard[8*bishop + i]; 
        if(rank_value == 0){
            continue;
        }
        for(uint8_t j = 7; j>=0; j++){
            uint8_t pow = pow_uint8((uint8_t)2, j);
            uint8_t old_square = 8*i+j;
            uint8_t new_square;
            if(rank_value < pow){
                continue;
            }
            uint8_t bishop_rank = i;
            uint8_t bishop_file = j;
            int8_t current_rank = bishop_rank + 1;
            int8_t current_file = bishop_file - 1;
            while(current_rank < 8 && current_file >= 0){
                
            }

    }
}


uint8_t u64ToSqureIndex(uint64_t bitboard){
    uint8_t result = 0;
    uint64_t current = bitboard;

}


std::vector<uint8_t> getSquareIndexesFromBitboard(uint64_t bitboard){
    std::vector<uint8_t> result{};
    uint8_t current_index;
    for(current_index=0; current_index<64; ++current_index){
        if(bitboard & (uint64_t)1 << current_index){
            result.push_back(current_index);
        }
    }
    return result;
}

uint64_t squareIndexToU64(uint8_t square_index){
    return (uint64_t)1 << square_index;
}
