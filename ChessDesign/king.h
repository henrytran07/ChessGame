#pragma once 

#include "../ChessBoardFiles/abstract_chess_board.h" 

#include <cstdlib> 
#include <cmath>
#include <vector> 
#include <utility>

using namespace std; 

using namespace std; 

namespace chess_game::king {

    class King : public chess_board::Abstract_Chess_Board::Piece {
        public: 
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position; 
            typedef pair<Position*, Piece*> ChessPosition; 
            
            static inline vector<pair<int, int>> initial_pos = {{4, 0}, {4, 7}};
            static inline vector<pair<string, int>> initial_alias_king = {{"K1", 1}, {"K2", 2}};

            King(string name, int team_no, bool is_king = true) : Base::Piece::Piece(name, team_no, is_king) {}

            void determineMove() {
                ChessPosition pairPos = *pos;
                Position currentPosition = *pairPos.first; 

                legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
        
                for (auto iter = found_pair.begin(); iter != found_pair.end();) {
                    if (is_in_checkmate_pos(*iter)){
                        iter = found_pair.erase(iter);
                    } else iter++; 
                }
            }

            bool is_in_checkmate_pos(const Position& pos) const {
                set<ChessPosition>& rival_set = (team == 1 ? Base::piece_pos_set_2 : Base::piece_pos_set_1);

                bool king_in_check = false; 
                bool checkmate_usage = true;
                for (auto iter = rival_set.begin(); iter != rival_set.end(); iter++) {
                    set<Position> attack_set; 

                    iter -> second -> legalMove(attack_set, iter -> first -> GetX(), iter -> first -> GetY(), *iter -> first, checkmate_usage);  

                    if (attack_set.count(pos) > 0) {
                        king_in_check = true;  
                        break; 
                    }
                }
                return king_in_check; 
            }

            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, 
                            const Position& original_pos, bool checkmate_usage = false) const {                

                Position p_difference = anticipated_pos - original_pos; 
                if (p_difference.GetX() * p_difference.GetX() + p_difference.GetY() * p_difference.GetY() > 2)
                    return false; 
                    
                if (!Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage))
                    return false; 
                
                return true; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_pos, bool checkmate_usage = false)  {
                Position anticipated_position(x, y);

                if (!findLegalMove(found, anticipated_position, original_pos, checkmate_usage))
                    return;
            
                found.insert(anticipated_position);

                legalMove(found, x + 1, y, original_pos, checkmate_usage);

                legalMove(found, x, y + 1, original_pos, checkmate_usage);

                legalMove(found, x - 1, y + 1, original_pos, checkmate_usage);

                legalMove(found, x - 1, y - 1, original_pos, checkmate_usage);

                legalMove(found, x + 1, y - 1, original_pos, checkmate_usage);

                legalMove(found, x + 1, y + 1, original_pos, checkmate_usage);

                if (found.count(original_pos) == 1) {
                    found.erase(original_pos);
                }
            }
    };
}