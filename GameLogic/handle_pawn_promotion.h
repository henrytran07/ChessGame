#pragma once 

#include "../ChessBoardFiles/abstract_chess_board.h"

#include "../ChessDesign/rook.h"
#include "../ChessDesign/knight.h"
#include "../ChessDesign/bishops.h"
#include "../ChessDesign/queen.h"

#include <algorithm>

using namespace std; 

namespace chess_game :: handle_pawn_promotion {

    typedef chess_game::chess_board::Abstract_Chess_Board Base; 
    typedef Base::Piece Piece; 
    typedef Base::Position Position; 
    typedef pair<Position*, Piece*> ChessPosition; 

    enum class PromotionType {
        Queen,
        Rook,  
        Knight, 
        Bishop, 
        Invalid, 
    };

    PromotionType getPromotionType(const string& userInput) {
        string lowerInput = userInput; 
        transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

        if (lowerInput == "queen")
            return PromotionType::Queen; 
        else if (lowerInput == "rook")
            return PromotionType::Rook; 
        else if (lowerInput == "knight")
            return PromotionType::Knight; 
        else if (lowerInput == "bishop")
            return PromotionType::Bishop; 
        return PromotionType::Invalid; 
    }

}