#pragma once 

#include "abstract_chess_board.h"

#include <iostream> 
#include <list> 
#include <vector>
#include <set> 
#include <utility> 

namespace chess_game::rook {
    
    class Rook : public chess_board::Abstract_Chess_Board::Piece{
        public:  
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position;
            typedef pair<Position*, Piece*> ChessPosition;
              
            static inline vector<pair<int, int>> initial_pos = {{0, 0}, {7, 0}, {0, 7}, {7, 7}};   

            static inline vector<pair<string, int>> initial_alias_rk = {{"R1", 1}, {"R2", 1}, 
                                                                             {"R3", 2}, {"R4", 2}};

            Rook(string name, int team_no) : Base::Piece::Piece(name, team_no) {}

            void determineMove() {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
            }
            
            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, const Position& original_spot, bool checkmate_usage = false) const {
                if (!Piece::findLegalMove(found, anticipated_pos, original_spot, checkmate_usage)) 
                    return false; 

                if (!(anticipated_pos.GetX() - original_spot.GetX() == 0 || anticipated_pos.GetY() - original_spot.GetY() == 0))        
                    return false; 

                return true; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_spot, bool checkmate_usage = false) const {    
                Position anticipated_position(x, y);

                if (!findLegalMove(found, anticipated_position, original_spot, checkmate_usage)) 
                    return;
                
                // cout << name << ": name, " << original_spot << ": original_spot, " << anticipated_position << ": anticipated position" << endl << endl;  
                   
                found.insert(anticipated_position);
                legalMove(found, x + 1, y, original_spot, checkmate_usage);

                legalMove(found, x - 1, y, original_spot, checkmate_usage);

                legalMove(found, x, y + 1, original_spot, checkmate_usage);

                legalMove(found, x, y - 1, original_spot, checkmate_usage);

                if (found.count(original_spot) == 1) { 
                    found.erase(original_spot);
                }
            }
    };

}