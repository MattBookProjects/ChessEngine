#include "chessboard.hpp"
#include <iostream>
#include <bitset>



bool test_init_chessboard(){
    uint8_t *chessboard = Chessboard::initChessboard();

    printChessboard(chessboard);
    delete chessboard;
}

bool test_castle(){
    uint8_t *chessboard = new uint8_t[8*12+2]{0};
    chessboard[8*WHITE_ROOK] = 129;
    chessboard[8*WHITE_KING] = 16;
    chessboard[8*BLACK_ROOK+7] = 129;
    chessboard[8*BLACK_KING+7] = 16;
    chessboard[8*12] = 3;
    Move *whiteShortCastle = new CastleMove((uint8_t)WHITE, (uint8_t)SHORT_CASTLE);
    uint8_t *new_chessboard = whiteShortCastle->makeMove(chessboard);
    std::cout<<"CHESSBOARD AFTER WHITE SHORT CASTLE:"<<std::endl;
    printChessboard(new_chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    
    if(chessboard[8*WHITE_ROOK] != 33){
        std::cout<<"Incorrect rook value for white short castle. Expected 33, got "<<(uint8_t)(chessboard[8*WHITE_ROOK])<<std::endl;
    }
    if(chessboard[8*WHITE_KING] != 64){
        std::cout<<"Incorrect king value for white short castle. Expected 64, got "<<(uint8_t)(chessboard[8*WHITE_KING])<<std::endl;
    }
    if(chessboard[8*12] != 130){
        std::cout<<"Incorrect flag value after white castle. Expected 130, got "<<(uint8_t)(chessboard[8*12])<<std::endl;
    }
    Move *blackLongCastle = new CastleMove(BLACK, LONG_CASTLE);
    new_chessboard = blackLongCastle->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
  
  
    if(chessboard[8*BLACK_ROOK+7] != 136){
        std::cout<<"Incorrect rook value for black long castle. Expected 136, got"<<chessboard[8*BLACK_ROOK + 7]<<std::endl;
        delete chessboard;
        return false;
    }
    if(chessboard[8*BLACK_KING+7] != 4){
        std::cout<<"Incorrect king value for black long castle. Expected 4, got"<<chessboard[8*BLACK_KING + 7]<<std::endl;
        delete chessboard;
        return false;
    }
    if(chessboard[8*12] != 0){
        std::cout<<"Incorrect flag value after black castle. Expected 0, got "<<chessboard[8*12]<<std::endl;
        delete chessboard;
        return false;
    }
    delete chessboard;
    return true;
}

bool test_enpassant(){
    uint8_t *chessboard = new uint8_t[8*12+3]{0};
    chessboard[8*WHITE_PAWN + 4] = 1;
    chessboard[8*BLACK_PAWN + 4] = 2;
    chessboard[8*12 + 2] = 2;
    Move *enpassant = new EnpassantMove((uint8_t)WHITE, (uint8_t)0, (uint8_t)1);
    uint8_t *new_chessboard = enpassant->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    std::cout<<"CHESSBOARD AFTER ENPASSANT:"<<std::endl;
    printChessboard(chessboard);
    if(chessboard[8*WHITE_PAWN + 4] != 0){
       std::cout<<"Incorrect old white pawn row value after enpassant. Expected 0, got "<<(uint8_t)(chessboard[8*WHITE_PAWN + 4])<<std::endl;
       delete chessboard;
       return false; 
    }
    if(chessboard[8*BLACK_PAWN + 4] != 0){
       std::cout<<"Incorrect old black pawn row value after enpassant. Expected 0, got "<<(uint8_t)(chessboard[8*BLACK_PAWN + 4])<<std::endl;
       delete chessboard;
       return false; 
    }
    if(chessboard[8*WHITE_PAWN + 5] != 2){
       std::cout<<"Incorrect new white pawn row value after enpassant. Expected 2, got "<<(uint8_t)(chessboard[8*WHITE_PAWN + 5])<<std::endl;
       delete chessboard;
       return false; 
    }
    delete chessboard;
    return true;
}

bool test_promotion(){
    uint8_t *chessboard = new uint8_t[8*12+3];
    chessboard[8*WHITE_PAWN+6] = 1;
    Move *move = new PromotionMove((uint8_t)WHITE_PAWN, (uint8_t)0, uint8_t(WHITE_QUEEN));
    uint8_t *new_chessboard = move->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    std::cout<<"CHESSBOARD AFTER PROMOTION:"<<std::endl;
    printChessboard(chessboard);
    if(chessboard[8*WHITE_PAWN+6] != 0){
         std::cout<<"Incorrect old white pawn row value after promotion. Expected 0, got "<<(uint8_t)(chessboard[8*WHITE_PAWN+6])<<std::endl;
         delete chessboard;
         return false;    
    }
    if(chessboard[8*WHITE_PAWN+7] != 0){
        std::cout<<"Incorrect new white pawn row value after promotion. Expected 0, got "<<(uint8_t)(chessboard[8*WHITE_PAWN+7])<<std::endl;
        delete chessboard;
        return false;
    }
    if(chessboard[8*WHITE_QUEEN+7] != 1){
        std::cout<<"Incorrect new white queen row value after promotion. Expected 1, got "<<(uint8_t)(chessboard[8*WHITE_QUEEN+7])<<std::endl;
        delete chessboard;
        return false;
    }
    delete chessboard;
    return true;
}


bool testIsSquareAttackedByPawn(){
    uint8_t *chessboard = new uint8_t[8*12+3];
    chessboard[8*WHITE_PAWN+3] = 145;
    if(!isSquareAttackedByPawn(chessboard, WHITE_PAWN, 33)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 33 to be attacked by white pawn but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByPawn(chessboard, WHITE_PAWN, 35)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 35 to be attacked by white pawn but isn't"<<std::endl;
        return false;
    } 
    if(!isSquareAttackedByPawn(chessboard, WHITE_PAWN, 37)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 37 to be attacked by white pawn but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByPawn(chessboard, WHITE_PAWN, 38)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 38 to be attacked by white pawn but isn't"<<std::endl;
        return false;
    }
    if(isSquareAttackedByPawn(chessboard, WHITE_PAWN, 31)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 31 to not be attacked by white pawn but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByPawn(chessboard, WHITE_PAWN, 40)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 40 to not be attacked by white pawn but it is"<<std::endl;
        return false;
    }
    delete chessboard;

    chessboard = new uint8_t[8*12+3];
    chessboard[8*BLACK_PAWN+4] = 145;
    if(!isSquareAttackedByPawn(chessboard, BLACK_PAWN, 25)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 25 to be attacked by black pawn but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByPawn(chessboard, BLACK_PAWN, 27)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 27 to be attacked by black pawn but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByPawn(chessboard, BLACK_PAWN, 29)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 29 to be attacked by black pawn but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByPawn(chessboard, BLACK_PAWN, 30)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 30 to be attacked by black pawn but isn't"<<std::endl;
        return false;
    }
    if(isSquareAttackedByPawn(chessboard, BLACK_PAWN, 23)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 23 to not be attacked by black pawn but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByPawn(chessboard, BLACK_PAWN, 32)){
        std::cout<<"testIsSquareAttackedByPawn failed. Expected square 32 to not be attacked by black pawn but it is"<<std::endl;
        return false;
    }
    return true;    
}  

bool testIsSquareAttackedByKnight(){
    uint8_t *chessboard = new uint8_t[99];
    chessboard[8*WHITE_KNIGHT+3] = 8;
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 42)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 42 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 44)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 44 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 33)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 33 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 37)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 37 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 21)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 21 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 17)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 17 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 12)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 12 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 10)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 10 to be attacked by knight but isn't"<<std::endl;
        return false;
    }
    delete chessboard;
    chessboard = new uint8_t[99];
    chessboard[8*WHITE_KNIGHT+3] = 2;
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 31)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 31 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 15)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 15 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 40)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 40 to be attacked by knight but it isnt"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 8)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 8 to be attacked by knight but it isnt"<<std::endl;
        return false;
    }

    delete chessboard;
    chessboard = new uint8_t[99];
    chessboard[8*WHITE_KNIGHT+3] = 64;
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 40)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 40 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 24)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 24 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 47)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 47 to be attacked by knight but it isnt"<<std::endl;
        return false;
    }
    if(!isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 15)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 15 to be attacked by knight but it isnt"<<std::endl;
        return false;
    }


    delete chessboard;
    chessboard = new uint8_t[99];
    chessboard[8*WHITE_KNIGHT+3] = 1;
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 39)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 39 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 7)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 24 not to be attacked by knight but it is"<<std::endl;
        return false;
    }

    delete chessboard;
    chessboard = new uint8_t[99];
    chessboard[8*WHITE_KNIGHT+3] = 128;    
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 48)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 48 not to be attacked by knight but it is"<<std::endl;
        return false;
    }
    if(isSquareAttackedByKnight(chessboard, WHITE_KNIGHT, 16)){
        std::cout<<"testIsKnightAttackedByPawn failed. Expected square 16 not to be attacked by knight but it is"<<std::endl;
        return false;
    }


    return true;
}   


int main(int argc, char **argv){
    //test_init_chessboard();
    /*uint8_t *chessboard = Chessboard::initChessboard();
    Move *move1 = new BasicMove((uint8_t)WHITE_PAWN, (uint8_t)11, (uint8_t)27);
    uint8_t *new_chessboard = move1->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    Move *move2 = new BasicMove((uint8_t)BLACK_PAWN, (uint8_t(52)), (uint8_t)36);
    new_chessboard = move2->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    Move *move3 = new TakeMove((uint8_t)WHITE_PAWN, (uint8_t)27, (uint8_t)BLACK_PAWN, (uint8_t)36);
    new_chessboard = move3->makeMove(chessboard);
    delete chessboard;
    chessboard = new_chessboard;
    printChessboard(chessboard);
    //delete new_chessboard;
    delete chessboard;*/
    //std::cout<<test_castle()<<std::endl;
   /* std::cout<<test_castle()<<std::endl;
    std::cout<<test_enpassant()<<std::endl;
    std::cout<<test_promotion()<<std::endl;*/

   // uint8_t *chessboard = Chessboard::initChessboard();
    //printChessboardFancy(chessboard);
    std::cout<<testIsSquareAttackedByPawn()<<std::endl;
    std::cout<<testIsSquareAttackedByKnight()<<std::endl;
}