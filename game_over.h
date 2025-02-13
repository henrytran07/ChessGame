#pragma once 

#include "abstract_chess_board.h"

#include <sstream>
#include <utility>

using namespace std; 

namespace chess_game::game_over {

typedef chess_game::chess_board::Abstract_Chess_Board Base; 
typedef typename chess_game::chess_board::Abstract_Chess_Board::Position Position;
typedef typename chess_game::chess_board::Abstract_Chess_Board::Piece Piece; 

typedef pair<Position*, Piece*> ChessPosition; 

void determine_having_bishop(const set<ChessPosition>& temp_set, bool& has_bishop) {
    for (const auto& piece : temp_set) {
        if (piece.second -> is_king)
            continue;
        string piece_name; 
        istringstream iss(piece.second -> name);
        iss >> piece_name;
        if (piece_name == "bishop" || piece_name == "promoted bishop")
            has_bishop = true; 
    }
}

bool insufficient_material() {
    bool insufficient_material = false;    
    if (Base::piece_pos_set_1.size() == Base::piece_pos_set_2.size()) {
        if (Base::piece_pos_set_1.size() == 1) {
            if (Base::piece_pos_set_1.begin() -> second -> is_king 
                && Base::piece_pos_set_2.begin() -> second -> is_king)
                insufficient_material = true; 
            else if (Base::piece_pos_set_1.size() == 2) {
                const set<ChessPosition>& set1 = Base::piece_pos_set_1; 
                const set<ChessPosition>& set2 = Base::piece_pos_set_2;
                bool has_bishop1 = false; 
                bool has_bishop2 = false; 

                determine_having_bishop(set1, has_bishop1);
                determine_having_bishop(set2, has_bishop2);

                if (has_bishop1 && has_bishop2)
                    insufficient_material = true; 
            }
        }
    }  else {
        bool is_size_1 = (Base::piece_pos_set_1.size() == 1 ||
                         Base::piece_pos_set_2.size() == 1 ? true : false);

        if (is_size_1) {
            const set<ChessPosition>& single_piece_set = (Base::piece_pos_set_1.size() == 1 ? Base::piece_pos_set_1 : Base::piece_pos_set_2);
            const set<ChessPosition>& other_set = (Base::piece_pos_set_1.size() == 1 ? Base::piece_pos_set_2 : Base::piece_pos_set_1);

            if (single_piece_set.begin() -> second -> is_king) {
                if (other_set.size() == 2) {
                    bool has_knight_or_bishop = false; 
                    for (const auto& piece : other_set) {
                        if (!piece.second -> is_king) {
                            string piece_name; 
                            istringstream iss(piece.second -> name);
                            iss >> piece_name;
                            if (piece_name == "bishop" || piece_name == "promoted bishop" || 
                                piece_name == "knight" || piece_name == "promoted knight") {
                                    has_knight_or_bishop = true; 
                                }
                        }
                    }

                    if (has_knight_or_bishop) {
                        insufficient_material = true; 
                    }
                }
            }

        }
    }

    if (insufficient_material) 
        return true; 
    
    return false; 
}

}