#pragma once 

#include "abstract_chess_board.h"


#include <vector> 
using namespace std; 

namespace chess_game::queen {
    class Queen : public chess_board::Abstract_Chess_Board::Piece {
        public: 
            typedef chess_board::Abstract_Chess_Board Base; 
            typedef typename Base::Position Position; 
            typedef pair<Position*, Piece*> ChessPosition; 

            static inline vector<pair<int, int>> initial_pos = {{3, 0}, {3, 7}};
            static inline vector<pair<string, int>> initial_alias_queen = {{"Q1", 1}, {"Q2", 2}};

            Queen(string name, int team_no) : Base::Piece(name, team_no) {} 

            void determineMove() override {
                ChessPosition pairPos = *pos; 
                Position currentPosition = *pairPos.first; 
                legalMove(found_pair, currentPosition.GetX(), currentPosition.GetY(), currentPosition);
            }


            bool findLegalMove(set<Position>& found, const Position& anticipated_pos, 
                                const Position& original_pos, bool checkmate_usage = false) const override{
                int dx = anticipated_pos.GetX() - original_pos.GetX();
                int dy = anticipated_pos.GetY() - original_pos.GetY();

                if (dx == 0 || dy == 0)
                    return Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage);
                else if (abs(dx) == abs(dy))
                    return Piece::findLegalMove(found, anticipated_pos, original_pos, checkmate_usage);
                return false; 
            }

            void legalMove(set<Position>& found, int x, int y, const Position& original_pos, bool checkmate_usage = false) const override {
                Position anticipated_pos(x, y);
                if (!findLegalMove(found, anticipated_pos, original_pos, checkmate_usage)) 
                    return; 

                found.insert(anticipated_pos);

                legalMove(found, x + 1, y, original_pos, checkmate_usage);

                legalMove(found, x - 1, y, original_pos, checkmate_usage);

                legalMove(found, x, y + 1, original_pos, checkmate_usage);

                legalMove(found, x, y - 1, original_pos, checkmate_usage);

                legalMove(found, x + 1, y + 1, original_pos, checkmate_usage);

                legalMove(found, x + 1, y - 1, original_pos, checkmate_usage);

                legalMove(found, x - 1, y + 1, original_pos, checkmate_usage); 

                legalMove(found, x - 1, y - 1, original_pos, checkmate_usage);

                if (found.count(original_pos) == 1)
                    found.erase(original_pos);
            }
    };
}

