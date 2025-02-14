#pragma once 

// #include "user_interface.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <functional>
#include <utility>
#include <set>
#include <map>
#include <stdexcept>


using namespace std; 


namespace chess_game::chess_board {
/*
    alter the type of piece set from unordered_set to set for now (will consider to change it later)
*/
const int upper_horizontal_limit = 8; 
const int upper_vertical_limit = 8; 

const int lower_horizontal_limit = -1; 
const int lower_vertical_limit = -1;

class Abstract_Chess_Board {
    public: 
        class Piece; 
        class Position; 
        typedef pair<Position*, Piece*> ChessPosition;

        class abstract_iter_rep {
            public: 
                typename set<ChessPosition>::iterator piece_iter; 
                abstract_iter_rep(typename set<ChessPosition>::iterator iter) : piece_iter{iter} {}
                virtual bool occupied_spot() const = 0; 
                virtual Position& get_position() const = 0; 
                virtual void advance() = 0; 
                virtual bool equals(const abstract_iter_rep* other) const = 0; 
                virtual abstract_iter_rep* clone(const abstract_iter_rep* other) const = 0; 
                virtual ~abstract_iter_rep() {}
        };

        struct PositionComparison; 

        class Position { 
                friend PositionComparison;
            private: 
                int x_pos, y_pos; 

            public: 
                Position(int x = numeric_limits<int>::min(), int y = numeric_limits<int>::min()) : x_pos{x}, y_pos{y} {}
                bool operator==(Position other) const {return x_pos == other.x_pos && y_pos == other.y_pos;}
                bool operator!=(Position other) const {
                    return x_pos != other.x_pos || y_pos != other.y_pos;
                }

                Position& operator=(const Position& other) {
                    if (this != &other) {
                        x_pos = other.x_pos;    
                        y_pos = other.y_pos;
                    }
                    return *this;
                }
                
                Position(const Position& other) : x_pos{other.x_pos}, y_pos{other.y_pos} {}

                int GetX() const {return x_pos;}
                int& GetX() {return x_pos;}

                int GetY() const {return y_pos;}
                int& GetY() {return y_pos;}

                bool operator<(const Position& other) const {
                    if (x_pos != other.x_pos) {
                        return x_pos < other.x_pos; 
                    }

                    return y_pos < other.y_pos;
                }

                friend Position operator-(const Position& p1, const Position& p2) {
                    Position p_result; 
                    p_result.x_pos = p1.x_pos - p2.x_pos;
                    p_result.y_pos = p1.y_pos - p2.y_pos; 
                    return p_result; 
                }

                friend ostream& operator<<(ostream& os, const Position& pos) {
                    os << '(' << pos.GetX() << ", " << pos.GetY() << ')'; 
                    return os; 
                }
        };
         
        friend struct std::hash<Position>; 

        static inline map<Position*, bool> board;
        static inline map<Position, Position*> position_map; 
        typedef set<ChessPosition> PieceSet;

        static inline PieceSet piece_pos_set_1;
        static inline PieceSet piece_pos_set_2;  

        class Piece {
            public: 
                string name;    
                int team;
                bool is_king; 
                bool is_pawn = false;   
                typename set<ChessPosition>::iterator pos;
                typename set<ChessPosition>::iterator king_pointer; 

                set<Position> found_pair; 
                Piece(string n = string(), int no = int(), bool king_bool = false) : name{n}, team{no}, is_king{king_bool}{}

                virtual void determineMove() {
                    cout << "I am here" << endl;
                } 

                virtual void legalMove(set<Position>& found, int x, int y, const Position& original_spot, bool checkmate_usage = false)  {} 

                bool operator<(const Piece& other) const {
                    return (size_t(this) < size_t(&other));
                }

                bool attackEnemy(const Position& input_pos) {
                    set<ChessPosition>& enemy_set = (team == 1 ? piece_pos_set_2 : piece_pos_set_1);
                    for (auto iter = enemy_set.begin(); iter != enemy_set.end(); iter++) {
                        if ((*iter -> first == input_pos))
                            return true; 
                    }
                    return false; 
                }

                bool checkMate(const Position& projectile_future_pos)  {
                    set<Position> found;  

                    legalMove(found, projectile_future_pos.GetX(), projectile_future_pos.GetY(), projectile_future_pos, true);

                    set<ChessPosition>& rival_set_pos = (team == 1 ? piece_pos_set_2 : piece_pos_set_1);
                    
                    for (auto iter = found.begin(); iter != found.end(); iter++) { 
                        bool rival_king_check = false; 
                        for (auto iter_set = rival_set_pos.begin(); iter_set != rival_set_pos.end(); iter_set++) {
                            if (iter_set -> second -> is_king && *iter_set -> first == *iter) 
                                rival_king_check = true; 
                        }

                        if (rival_king_check)
                            return true; 
                    }

                    return false; 
                } 

                virtual bool findLegalMove(set<Position>& found, const Position& anticipated_pos, 
                                            const Position& original_spot, bool checkmate_usage = false) const {
                    if (anticipated_pos.GetX() >= upper_horizontal_limit || anticipated_pos.GetY() >= upper_vertical_limit)
                        return false;
                    
                    if (anticipated_pos.GetX() <= lower_horizontal_limit || anticipated_pos.GetY() <= lower_vertical_limit)
                        return false;
                    
                    if (found.count(anticipated_pos) == 1)
                        return false; 

                    if (board[position_map[anticipated_pos]] && anticipated_pos != original_spot) {   
                        set<ChessPosition> &rival_set = (team == 1 ? piece_pos_set_2 : piece_pos_set_1);
                        bool same_team = true; 
                        for (auto iter = rival_set.begin(); iter != rival_set.end(); iter++) {
                            if (anticipated_pos == *iter -> first) 
                                same_team = false; 
                        }     

                        if ((!same_team) && checkmate_usage)
                            return true; 
                        if ((!same_team) && (!checkmate_usage)) {
                            found.insert(anticipated_pos); 
                        } 
                        return false; 
                    }
                    

                    return true; 
                } 

                virtual bool pawnPromotion() {
                    return false; 
                }

                virtual ~Piece() {}
        };
        
        friend Piece; 

        virtual int num_elements() const = 0; 

        ~Abstract_Chess_Board() {}

};
}

namespace std {
    template <>
    struct hash<chess_game::chess_board::Abstract_Chess_Board::Position> {
        size_t operator()(const chess_game::chess_board::Abstract_Chess_Board::Position& pos) const {
            // Combine the hash of x_pos and y_pos
            return hash<int>()(pos.GetX()) ^ (hash<int>()(pos.GetY()) << 1);
        }
    };
}
