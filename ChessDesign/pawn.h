#pragma once 

#include "../ChessBoardFiles/abstract_chess_board.h"

#include <vector>

using namespace std; 

namespace chess_game::pawn {
    class Pawn : public chess_board::Abstract_Chess_Board::Piece {
        public: 
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position; 
            typedef pair<Position*, Piece*> ChessPosition; 

            Pawn(string name, int team_no) : Base::Piece(name, team_no) {}

            static inline vector<pair<int, int>> initial_pos = {{0, 1}, {1, 1}, {2, 1}, {3, 1},
                                                                {4, 1}, {5, 1}, {6, 1}, {7, 1}, 
                                                                {0, 6}, {1, 6}, {2, 6}, {3, 6}, 
                                                                {4, 6}, {5, 6}, {6, 6}, {7, 6},};
            static inline vector<pair<string, int>> initial_alias_pawn = {{"P1", 1}, {"P2", 1}, {"P3", 1}, {"P4", 1},
                                                                        {"P5", 1}, {"P6", 1}, {"P7", 1}, {"P8", 1}
                                                                        , {"P9", 2}, {"P10", 2}, {"P11", 2}, {"P12", 2}
                                                                        , {"P13", 2}, {"P14", 2}, {"P15", 2}, {"P16", 2}};
            bool first_step = true;
             
            void determineMove() {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                this -> legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
            }

            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, 
                                            const Position& original_pos, bool checkmate_usage = false) const {
                Position d_position = anticipated_pos - original_pos; 

                d_position.GetX() = abs(d_position.GetX());
                d_position.GetY() = abs(d_position.GetY());
                if (anticipated_pos == original_pos)
                    return true; 

                if (!((d_position.GetX() == 0 && d_position.GetY() == 1) || 
                    (d_position.GetX() == 1 && d_position.GetY() == 1) || (first_step && d_position.GetX() == 0 && d_position.GetY() == 2))) 
                    return false;  

                if (first_step) {
                    checkmate_usage = true; 
                }

                if (!Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage))
                    return false;

                if (Base::board[Base::position_map[anticipated_pos]]) {
                    if ((d_position.GetY() == 1 && d_position.GetX() == 0) || 
                        (first_step && d_position.GetX() == 0 && d_position.GetY() == 2))  {
                        return false; 
                    }

                    else if (d_position.GetY() == 1 && d_position.GetX() == 1) {
                        bool same_team  = false; 
                        set<ChessPosition>& temp_set = (team == 1 ? Base::piece_pos_set_1 : Base::piece_pos_set_2);
                        for (auto iter = temp_set.begin(); iter != temp_set.end(); iter++) {
                            if (anticipated_pos == *iter -> first)
                                same_team = true; 
                        }

                        if (same_team)
                            return false; 
                    }
                } else {
                    if (d_position.GetX() == 1 && d_position.GetY() == 1)
                        return false; 
                }
 
                return true; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_pos, bool checkmate_usage = false)  { 
                Position anticipated_pos(x, y);

                if (!findLegalMove(found, anticipated_pos, original_pos, checkmate_usage))
                    return;

                found.insert(anticipated_pos);
                
                if (team == 1) {
                    if (first_step) {
                        legalMove(found, x, y + 2, original_pos, checkmate_usage);
                        first_step = false; 
                    }

                    legalMove(found, x, y + 1, original_pos, checkmate_usage);
                    legalMove(found, x + 1, y + 1, original_pos, checkmate_usage);
                } else {
                    if (first_step) {
                        legalMove(found, x, y - 2, original_pos, checkmate_usage);
                        first_step = false; 
                    }

                    legalMove(found, x, y - 1, original_pos, checkmate_usage);
                    legalMove(found, x - 1, y - 1, original_pos, checkmate_usage);
                }

                if (found.count(original_pos) == 1)
                    found.erase(original_pos);
            }

            bool pawnPromotion() {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                Position initialY; 

                if (team == 1)  
                    initialY.GetY() = 1; 
                else 
                    initialY.GetY() = 6; 
                    
                if (currentPosition.GetY() - initialY.GetY() == 6) 
                    return true; 
                return false; 
            }
    };
}