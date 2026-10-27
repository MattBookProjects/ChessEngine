#include "chessboard.hpp"





Chessboard::Chessboard
(
    std::array<uint64_t, 12> bitboard,
    Player player_to_move, 
    bool can_white_castle, 
    bool can_black_castle, 
    int8_t enpassant
)
{
    this->bitboard = bitboard;
    this->player_to_move = player_to_move;
    this->can_white_castle = can_white_castle;
    this->can_black_castle= can_black_castle;
    this->enpassant = enpassant;
    this->game_result = GameResult::Ongoing;
    
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
    for(uint8_t i = 0; i < 8; i++){
        this->bitboard[static_cast<std::size_t>(Piece::WhitePawn)] |= squareIndexToU64(8+i);
        this->bitboard[static_cast<std::size_t>(Piece::BlackPawn)] |= squareIndexToU64(56-i);
    }
}

void Chessboard::initKnights(){
    this->bitboard[static_cast<std::size_t>(Piece::WhiteKnight)] = (squareIndexToU64(1) | squareIndexToU64(6));
    this->bitboard[static_cast<std::size_t>(Piece::BlackKnight)] = (squareIndexToU64(57) | squareIndexToU64(62));
}

void Chessboard::initBishops(){
    this->bitboard[static_cast<std::size_t>(Piece::WhiteBishop)] = (squareIndexToU64(2) | squareIndexToU64(5));
    this->bitboard[static_cast<std::size_t>(Piece::BlackBishop)] = (squareIndexToU64(58) | squareIndexToU64(61));
}

void Chessboard::initRooks(){
    this->bitboard[static_cast<std::size_t>(Piece::WhiteRook)] = (squareIndexToU64(0) | squareIndexToU64(7));
    this->bitboard[static_cast<std::size_t>(Piece::BlackRook)] = (squareIndexToU64(56) | squareIndexToU64(63));
}

void Chessboard::initQueens(){
    this->bitboard[static_cast<std::size_t>(Piece::WhiteQueen)] = squareIndexToU64(3);
    this->bitboard[static_cast<std::size_t>(Piece::BlackQueen)] = squareIndexToU64(59);
}

void Chessboard::initKings(){
    this->bitboard[static_cast<std::size_t>(Piece::WhiteKing)] = squareIndexToU64(4);
    this->bitboard[static_cast<std::size_t>(Piece::BlackKing)] = squareIndexToU64(60);
}

uint64_t Chessboard::getPieceBitboard(Piece piece){
    return this->bitboard[static_cast<std::size_t>(piece)];
}

uint64_t Chessboard::getPlayerPiecesBitboard(Player player){
    const auto &pieces = getPlayerPieces(player);
    uint64_t result = 0;
    for(auto it=pieces.begin(); it != pieces.end(); ++it){
        result |= this->bitboard[static_cast<std::size_t>(*it)];
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

Piece Chessboard::getPlayerPieceFromSquare(Player player, uint8_t square){
    const auto player_pieces = getPlayerPieces(player);
    for(const auto &piece: player_pieces){
        const auto piece_bitboard = this->bitboard[static_cast<std::size_t>(piece)];
        const auto piece_squares_indexes = getSquareIndexesFromBitboard(piece_bitboard);
        auto it = std::find(piece_squares_indexes.begin(), piece_squares_indexes.end(), square);
        if(it != piece_squares_indexes.end()){
            return piece;
        }
    }
    return Piece::Empty;
}

GameResult Chessboard::getGameResult(){
    return this->game_result;
}

int8_t Chessboard::getEnpassant(){
    return this->enpassant;
}

bool Chessboard::isPlayerInCheck(Player player){
    const Player opponent = getOpponentPlayer(player);
    const uint8_t player_king_square = this->getPlayerKingSquare(player);
    return this->isSquareAttackedByPlayerPieces(opponent, player_king_square);
}


  bool Chessboard::isDiagonalBetweenSquaresEmpty(uint8_t start_square, uint8_t end_square, uint8_t step){
    const uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
    auto current_file  = start_square % 8 + 1;
    auto current_rank = start_square >> 3 + 1;
    const auto rank_step = 1;
    const auto file_step = step - 8;
    const auto last_file = file_step == 1 ? 7 : 0;
    if(current_file == last_file || current_rank == 7){
        return false;
    }
    current_file += file_step;
    current_rank++;
    auto current_square = start_square + step;
    while(current_square < end_square && current_rank < 7 && current_file != last_file){
        if(all_pieces_bitboard & squareIndexToBitboard(current_square) != 0){
            return false;
        }
        current_rank++;
        current_file += file_step;
        current_square += step;
    }
    return current_square == end_square;
  }


  bool Chessboard::isStraightBetweenSquaresEmpty(uint8_t start_square, uint8_t end_square, uint8_t step){
        const uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
        auto current_square = start_square;
        auto current_file_or_rank = step == 1 ? start_square % 8 : start_square >> 3;
        if(current_file_or_rank == 7){
            return false;
        }
        current_file_or_rank++;
        current_square++;
        while(current_square < end_square && current_file_or_rank < 7){
            if(all_pieces_bitboard & squareIndexToBitboard(current_square) != 0){
                return false;
            }
            current_square+=step;
            current_file_or_rank++;
        }
        return current_square == end_square;
  }


  /*
  bool Chessboard::isRankBetweenSquaresEmpty(uint8_t start_square, uint8_t end_square){
        const uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
        auto current_square = start_square + 1;
        auto current_file = start_square % 8;
        if(current_file == 7){
            return false;
        }
        current_file++;
        while(current_square < end_square && current_file < 7){
            if(all_pieces_bitboard & squareIndexToBitboard(current_square) != 0){
                return false;
            }
        }

  }*/

/*
bool Chessboard::isGameFinished(){
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
}*/

uint8_t Chessboard::getPlayerKingSquare(Player player){
    const Piece king = player == Player::White ? Piece::WhiteKing : Piece::BlackKing;
    return getSquareIndexesFromBitboard(this->bitboard[king]).front();
}

bool Chessboard::isSquareAttackedByPlayerPieces(Player player, uint8_t square){
    const auto pieces = getPlayerPieces(player);
    for(auto it = pieces.begin(); it != pieces.end(); ++it){
        if(this->isSquareAttackedByPiece(*it, square)){
            return true;
        }
    }
    return false;

/*
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
            this->isSquareAttackedByPlayersKing(king, square);*/
}


std::optional<Piece> Chessboard::getPlayerPieceOnSquare(Player player, uint8_t square){
    const auto pieces = player == WHITE ? WHITE_PIECES : BLACK_PIECES;
    for(const auto &piece : pieces){
        const auto piece_bitboard = this->bitboard[(std::static_cast<std::size>(*piece))];
        const auto piece_squares_indexes = getSquareIndexesFromBitboard(piece_bitboard);
        const auto found = std::find(piece_squares_indexes.begin(), piece_squares_indexes.end(), square);
        if(found != piece_squares.end()){
            return *found;
        }
    }
    return std::nullopt;
}

bool Chessboard::isSquareAttackedByPiece(Piece piece, uint8_t square){
    const auto piece_move_behaviour = getPieceMoveBehaviour(piece);
    const auto piece_squares = getSquareIndexesFromBitboard(this->bitboard[static_cast<std::size_t>(piece)]);
    for(auto it = piece_squares.begin(); it != piece_squares.end(); ++it){
        if(piece_move_behaviour->doesPieceAttackSquare(*it, square)){
            return true;
        }
    }
    return false;
}


bool Chessboard::isSquareOccupied(uint8_t square){
    const uint64_t all_pieces_bitboard = this->getAllPiecesBitboard();
    const uint64_t square_bitboard = squareIndexToBitboard(square);
    if(all_pieces_bitboard & square_bitboard > 0){
        return true;
    }
    return false;
}

/*

bool Chessboard::doesPieceAttackSquare(Piece piece, uint8_t piece_square, uint8_t target_square){
    switch(piece){
        case WHITE_PAWN:
        case BLACK_PAWN:
            return doesPawnAttackSquare(piece, piece_square, target_square);
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            return doesKnightAttackSquare(piece_square, target_square);
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            return doesBishopAttackSquare(piece_square, target_square);
        case WHITE_ROOK:
        case BLACK_ROOK:
            return doesRookAttackSquare(piece_square, target_square);
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            return doesQueenAttackSquare(piece_square, target_square);
        case WHITE_KING:
        case BLACK_KING:
            return doesKingAttackSquare(piece_square, target_square);
        default:
            return false;

    }
}

bool Cheessboard::doesPawnAttackSquare(Pawn pawn, uint8_t pawn_square, uint8_t target_square){
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
}*/

Chessboard Chessboard::makeMove(const std::unique_ptr<Move> &move){
    return move->applyMove(*this);
}

bool Chessboard::isMoveLegal(const std::unique_ptr<Move> &move){
    const auto chessboard_after_move = this->makeMove(move);

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

std::vector<std::unique_ptr<Move>> Chessboard::getPlayerLegalMoves(Player player){
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const auto player_pieces = getPlayerPieces(player);
    for(auto it=player_pieces.begin(); it != player_pieces.end(); ++it){
        const auto piece_legal_moves = this->getPlayerPieceLegalMoves(player, *it);
        legal_moves.insert(legal_moves.end(), piece_legal_moves.begin(), piece_legal_moves.end());
    }
}

std::vector<std::unique_ptr<Move>> Chessboard::getPlayerPieceLegalMoves(Player player, Piece piece){
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const std::unique_ptr<PieceMoveBehaviour> piece_move_behaviour = getPieceMoveBehaviour(piece);
    const std::vector<uint8_t> piece_squares = getSquareIndexesFromBitboard(this->bitboard[static_cast<std::size_t>(piece)]);
    for(const auto &square: piece_squares){
        const auto piece_on_square_legal_moves = piece_move_behaviour->getLegalMoves(player, piece, square, std::make_unique<Chessboard>(this));
        legal_moves.insert(legal_moves.end(), piece_on_square_legal_moves.begin(), piece_on_square_legal_moves.end());
    }
    return legal_moves;
}

/*
std::vector<std::unique_ptr<Move>> getPlayerLegalPawnMoves(Player player){
    const auto pawn = player == WHITE ? WHITE_PAWN : BLACK_PAWN;
    const auto pawn_squares = getSquareIndexesFromBitboard(this->bitboard[pawn]);
    std::vector<std::unique_ptr<Move>> legal_moves{};
    for(auto it = pawn_squares.begin(); it != pawn_squares.end(); ++it){
        auto pawn_legal_moves = this->getPawnLegalMove(player, pawn, *it);
        legal_moves.insert(legal_moves.end(), pawn_legal_moves.begin(), pawn_legal_moves.end());  
    }
    return legal_moves;     
}


std::vector<std::unique_ptr<Move>> getPlayerLegalPieceMoves(Player player, Piece piece){
    const auto piece_squares = getSquareIndexesFromBitboard(this->bitboard[piece]);
    std::vector<std::unique_ptr<Move>> legal_moves{};
    for(auto it = piece_squares.begin(); it != piece_squares.end(); ++it){
        auto pawn_legal_moves = this->getPieceLegalMoves(player, piece, *it);
        legal_moves.insert(legal_moves.end(), pawn_legal_moves.begin(), pawn_legal_moves.end());  
    }
    return legal_moves;  
}

std::vector<std::unique_ptr<Move>> getPieceLegalMoves(Player player, Piece piece, uint8_t piece_square){
    switch(piece){
        case WHITE_PAWN:
        case BLACK_PAWN:
            return getPawnLegalMoves(player, piece, piece_square);
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            return getKnightLegalMoves(player, piece, piece_square);
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            return getBishopLegalMoves(player, piece, piece_square);
        case WHITE_ROOK:
        case BLACK_ROOK:
            return getRookLegalMoves(player, piece, piece_square);
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            return getQueenLegalMoves(player, piece, piece_square);
        case WHITE_KING:
        case BLACK_KING:
            return getKingLegalMoves(player, piece, piece_square);
        //default:
           // co tu zwrocic XD

    }
}

std::vector<std::unique_ptr<Move>> getPawnLegalMoves(Player player, Piece pawn. uint8_t pawn_square){
    const auto opponent = player == WHITE ? BLACK : WHITE;
    std::vector<std::unique_ptr<Move>> legal_moves{};
    const uint8_t pawn_rank = pawn_square >> 3;
    const uint8_t pawn_file = pawn_square % 8;
    const uint8_t penultimate_rank = player == WHITE ? 6 : 1; 
    const int8_t diff_forward = PAWN == WHITE_PAWN ? 8 : -8;
    const int8_t diff_left_attack = PAWN == WHITE_PAWN ? 7 : -9;
    const int8_t diff_right_attack = PAWN == WHITE_PAWN ? 9 : -7;
    const auto all_pieces_indexes = getSquareIndexesFromBitboard(this->getAllPiecesBitboard();)
    const auto opponent_pieces_indexes = getSquareIndexesFromBitboard(this->getPlayerPiecesBitboard(opponent));
    const auto piece_in_front = std::find(all_pieces_indexes.begin(); all_pieces_indexes.end(); pawn_square+diff_forward);
    const auto left_attacked_piece = std::find(opponent_pieces_indexes.begin(), opponent_pieces_indexes.end(), pawn_square+left_attacked_square_diff);
    const auto right_attacked_piece = std::find(opponent_pieces_indexes.begin(), opponent_pieces_indexes.end(), pawn_square+right_attacked_square_diff);
    if(piece_in_front == all_pieces_index.end()){
        auto move = std::unique_ptr<Move>(new BasicMove(pawn, pawn_square, pawn_square+diff_forward));
        if(!this->isMoveLegal(move)){}
        else if(pawn_rank == penultimate_rank){
            auto legal_promotion_moves = getAllPromotionMoves();
            legal_moves.insert(legal_moves.end(), legal_promotion_moves.start(), legal_promotion_moves.end());
        } else {
            legal_moves.push_back(move);
        }    
        
    }
    if(left_attacked_piece !== all_pieces_indexe.end()){
        const auto attacked_piece = getPlayerPieceFromSquare(*left_attacked_piece);
        if(attacked_piece.has_value()){
            const auto move = std::unique_ptr<Move>(new TakeMove(pawn, pawn_square, *attacked_piece, pawn_square+left_attacked_square_diff));
            if(!this->isMoveLegal(move)){}
            else if(pawn_rank == penultimate_rank){
                auto legal_promotion_moves = getAllPromotionMoves();
        }
            legal_moves.push_back(move);
        }
        }
       
    }
    if(right_attacked_piece !== all_pieces_indexe.end()){
        const auto attacked_piece = getPlayerPieceFromSquare(*right_attacked_piece);
        if(!attacked_piece.has_value()){
            break;
        }
        if(!this->isMoveLegal(move)){}
        if(pawn_rank == penultimate_rank){
            const std::unique_ptr<Move> move = std::make_unique<TakeWithPromotionMove>(player, pawn_file, attacked_piece, );
        }
        const auto move = std::unique_ptr<Move>(new TakeMove(pawn, pawn_square, *attacked_piece, pawn_square+left_attacked_square_diff));
       
            legal_moves.push_back(move);
        }
    }
    return legal_moves;

    const auto left_target = std::find(opponent_pieces_indexes.begin(), opponent_pieces_indexes.end(), pawn_square + diff_left_attack);
    if(left_target != opponent_pieces_indexes.end()){
        auto move = std::unique_ptr<Move>(new TakeMove(pawn, pawn_square, *left_target, pawn_square+diff_left_attack));
    }

    //std::unique_ptr<Move> move;
        if(player == WHITE){
        
           
        }
        if(player == WHITE && pawn_rank == 6){
            move = std::unique_ptr<Move>(new PromotionMove(player, pawn_square % 8, WHITE_KNIGHT));
        }
        
    }
    const auto can_take_left = std::find
    (
        opponent_pieces_indexes.begin(), 
        opponent_pieces_indexes.end(),
        pawn_square + diff_left_attack
    ); 
    if(can_go_forward == opponent_pieces_index.end()){
        std::unique_ptr<Move> move;
        if(player == WHITE && pawn_rank == 6){
            auto legal_promotion_moves = getAllPromotionMoves()
            legal_moves.insert(legal_moves.end())
            move = std::unique_ptr<Move>(new PromotionMove(player, pawn_square % 8, WHITE_KNIGHT));
        }
        auto move = std::unique_ptr<Move>(new BasicMove(pawn, pawn_square, pawn_square+diff_forward));
        if(this->isMoveLegal(move)){
            legal_moves.push_back(move);
        }
    }
    const auto can_take_right = std::find
    (
        opponent_pieces_bitboard.begin(), 
        opponent_pieces_bitboard.end(),
        pawn_square + diff_right_attack
    );

  

    
}

std::vector<std::unique_ptr<Move>> getPlayerLegalKnightMoves(Player player){

}

std::vector<std::unique_ptr<Move>> getPlayerLegalBishopMoves(Player player);
std::vector<std::unique_ptr<Move>> getPlayerLegalRookMoves(Player player);
std::vector<std::unique_ptr<Move>> getPlayerLegalQueenMoves(Player player);
std::vector<std::unique_ptr<Move>> getPlayerLegalKingMoves(Player player);

std::vector<std::unique_ptr<Move>> getAllPromotionMoves(Player player, pawn_file){
    std::vector<std::unique_ptr>> legal_moves{};
    const std::array<Piece, 4> pieces_for_promotion = player == WHITE ? 
    {WHITE_KNIGHT, WHITE_BISHOP,  WHITE_ROOK, WHITE_QUEEN} :
    {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};

    for(auto it = pieces_for_promotion.begin(); it != pieces_for_promotion.end(); ++it){
        auto move = std::unique_ptr<Move>(new PromotionMove(player, pawn_file, *it));
        if(isMoveLegal(move)){
            legal_moves.push_back(move);
        }
        return legal_moves;
    }
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


########################################################################################################33














bool isMoveLegal(uint8_t *chessboard, Move *move){
    uint8_t player = chessboard[96] & 128 ? BLACK: WHITE;
    uint8_t *board_after_move = move->makeMove(chessboard);
    return !this->isPlayerInCheck(board_after_move, player);
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
