#include "ChessBoardFiles/chess_board.h"

using namespace std; 
using namespace chess_game; 

int main() {
    typedef chess_board::ChessBoard ChessBoard; 
    ChessBoard chess_board; 
    chess_board.gamePlay();
}