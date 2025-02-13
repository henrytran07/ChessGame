#pragma once 

#include "abstract_chess_board.h"
#include "user_interface.h"

#include <utility> 
#include <vector>
using namespace std; 

namespace chess_game::knight {

    class Knight : public chess_board::Abstract_Chess_Board::Piece {
        public: 
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position; 
            typedef pair<Position*, Piece*> ChessPosition; 

            static inline vector<pair<int, int>> initial_pos = {{1, 0}, {6, 0}, {1, 7}, {6, 7}};

            // // test scenario 
            // static inline vector<pair<int, int>> initial_pos = {{1, 0}, {6, 7}};

             
            static inline vector<pair<string, int>> initial_alias_knight = {{"Kn1", 1}, {"Kn2", 1}, 
                                                                            {"Kn3", 2}, {"Kn4", 2}};
            
            // // test scenario    
            // static inline vector<pair<string, int>> initial_alias_knight = {{"Knight 1", 1}
            //                                                                 , {"Knight 4", 2}};
                                                                            
            Knight(string name, int team_no) : Base::Piece::Piece(name, team_no) {}

            void determineMove() {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
            }

            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, const Position& original_pos, bool checkmate_usage = false) const {
                if (anticipated_pos == original_pos)
                    return true; 
                
                Position pos_difference = anticipated_pos - original_pos; 
                

                int sum_difference = pos_difference.GetX() * pos_difference.GetX() + pos_difference.GetY() * pos_difference.GetY();

                if (sum_difference != 5) {
                    return false; 
                }

                if (!Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage))
                    return false; 

                return true; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_pos, bool checkmate_usage = false) const {
                Position anticipated_position(x, y);
                Position pos_difference = anticipated_position - original_pos;
                // x + 2 && y + 1 || x - 2 && y + 1  || x + 1 && y + 2 || x - 1 && y + 2 || x - 1 && y - 2 || x + 1 && y - 2   
                if (!findLegalMove(found, anticipated_position, original_pos, checkmate_usage))
                    return; 

                // if (pos_difference.GetX() * pos_difference.GetX() + pos_difference.GetY() * pos_difference.GetY() == 5) {
                found.insert(anticipated_position);
                // }
                
                legalMove(found, x + 2, y + 1, original_pos, checkmate_usage);

                legalMove(found, x - 2, y + 1, original_pos, checkmate_usage);

                legalMove(found, x + 1, y + 2, original_pos, checkmate_usage);

                legalMove(found, x - 1, y +  2, original_pos, checkmate_usage);

                legalMove(found, x - 1, y - 2, original_pos, checkmate_usage);

                legalMove(found, x + 1, y - 2, original_pos, checkmate_usage);

                legalMove(found, x - 2, y - 1, original_pos, checkmate_usage);

                legalMove(found, x + 2, y - 1, original_pos, checkmate_usage);

                if (found.count(original_pos) == 1)
                    found.erase(original_pos);
            }
    };
}