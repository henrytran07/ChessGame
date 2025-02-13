#pragma once 

#include "abstract_chess_board.h"

#include <cstdlib>
#include <vector> 
#include <utility>

using namespace std; 

namespace chess_game::bishop {

    class Bishop : public chess_board::Abstract_Chess_Board::Piece {
        public: 
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position; 
            typedef pair<Position*, Piece*> ChessPosition; 

            // official set 
            static inline vector<pair<int, int>> initial_pos = {{2, 0}, {5, 0}, {2, 7}, {5, 7}};
            // test case 1: 
            // static inline vector<pair<int, int>> initial_pos = {{2, 0}, {5, 7}};

            // test case 2: 
            // static inline vector<pair<int, int>> initial_pos = {{5, 7}};

            // official set 
            static inline vector<pair<string, int>> initial_alias_bishop = {{"B1", 1}, {"B2", 1}, 
                                                                            {"B3", 2}, {"B4", 2}};
            // test case 
            // static inline vector<pair<string, int>> initial_alias_bishop = {{"Bishop 1", 1} 
            //                                                                 , {"Bishop 4", 2}}; 

            // test case 2: 
            // static inline vector<pair<string, int>> initial_alias_bishop = {{"Bishop 4", 2}};                                                                            
                                                             

            Bishop(string name, int team_no) : Base::Piece::Piece(name, team_no) {}

            void determineMove() {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
            }

            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, const Position& original_pos, bool checkmate_usage = false) const {
                Position different_in_pos = anticipated_pos - original_pos; 

                if (abs(different_in_pos.GetX()) != abs(different_in_pos.GetY())) 
                    return false; 
                if (!Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage)) 
                    return false; 

                return true; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_pos, bool checkmate_usage = false) const {
                Position anticipated_pos(x, y);

                if (!findLegalMove(found, anticipated_pos, original_pos, checkmate_usage)) 
                    return; 
                
                found.insert(anticipated_pos);
                legalMove(found, x + 1, y + 1, original_pos, checkmate_usage);
                legalMove(found, x + 1, y - 1, original_pos, checkmate_usage);
                legalMove(found, x - 1, y + 1, original_pos, checkmate_usage); 
                legalMove(found, x - 1, y - 1, original_pos, checkmate_usage);

                if (found.count(original_pos) == 1)
                    found.erase(original_pos);
            }
    };
}