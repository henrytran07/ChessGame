#pragma once 

#include "abstract_chess_board.h"

#include <iomanip>
#include <vector> 
#include <algorithm>
#include <unordered_map>

using namespace std; 

namespace chess_game::print_board {

    typedef chess_board::Abstract_Chess_Board Base; 
    typedef typename Base::Position Position;

    vector<string> board_names_team1 = {"R1", "R2", "Kn1", "Kn2", "B1", "B2", "Q1", "K1",
                                        "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8"};
    
    vector<Position> position_team1 = {Position(0, 0), Position(7, 0), Position(1, 0), Position(6, 0), 
                                        Position(2, 0), Position(5, 0), Position(3, 0), Position(4, 0), 
                                        Position(0, 1), Position(1, 1), Position(2, 1), Position(3, 1),
                                        Position(4, 1), Position(5, 1), Position(6, 1), Position(7, 1)};

    vector<string> board_names_team2 = {"R3", "R4", "Kn4", "Kn3", "B3", "B4", "Q2", "K2",
                                        "P9", "P10", "P11", "P12", "P13", "P14", "P15", "P16"};

    vector<Position> position_team2 = {Position(0, 7), Position(7, 7), Position(1, 7), Position(6, 7), 
                                        Position(2, 7), Position(5, 7), Position(3, 7), Position(4, 7), 
                                        Position(0, 6), Position(1, 6), Position(2, 6), Position(3, 6),
                                        Position(4, 6), Position(5, 6), Position(6, 6), Position(7, 6)};

    class WrapperPosition {
        public: 
            string piece_name; 
            Position piece_position;  
            WrapperPosition(Position pos, string name = string()) : piece_name{name}, piece_position{pos} {}

            bool operator<(const WrapperPosition& other) const {
                return piece_position < other.piece_position;
            }

            friend ostream& operator<<(ostream& os, const WrapperPosition& wrapper) {
                os << wrapper.piece_name;
                return os; 
            }
    };

    vector<WrapperPosition> chess_print_board; 
    const int total_space = 12; 
    unordered_map<Position, string> position_to_name;

    void setupBoardPieces(const vector<string>& name_vector, const vector<Position>& position_vector) {
        for (size_t i = 0; i < position_vector.size(); i++) {
            position_to_name[position_vector[i]] = name_vector[i];
        }

        for (auto& pairPiece : chess_print_board) {
            if (position_to_name.find(pairPiece.piece_position) != position_to_name.end()) {
                pairPiece.piece_name = position_to_name[pairPiece.piece_position];
            }
        }
    }

    void board_init() {
        for (int y = 0; y < chess_board::upper_horizontal_limit; y++) {
            for (int x = 0; x < chess_board::upper_vertical_limit; x++) {
                chess_print_board.push_back(WrapperPosition(Position(x, y)));
            }
        }
        
        setupBoardPieces(board_names_team1, position_team1);
        setupBoardPieces(board_names_team2, position_team2);
    }

    void modifiedBoard(const Position& initial_pos, const Position& final_pos, const string& piece_name) {
        position_to_name[final_pos] = piece_name; 
        position_to_name.erase(initial_pos);
    
        for (auto& pairPiece : chess_print_board) {
            if (pairPiece.piece_position == initial_pos)
                pairPiece.piece_name = "";
            if (pairPiece.piece_position == final_pos)
                pairPiece.piece_name = position_to_name[final_pos];
        }
    }

    int padding(const string& name) {
        int name_size = name.size();
        int remaining_space = total_space - name_size;
        return remaining_space / 2;     
    }

    void printBoard() {
        cout << "  ";
        for (char k = 'a'; k <= 'h'; k++) {
            cout << setw(6) << setfill(' ') << "";
            cout << k; 
            cout << setw(6) << setfill(' ') << "";
        }
        cout << endl;

        int labeled_number = 1; 
        for (int i = 0; i < chess_print_board.size(); i++) {
            if (i % 8 == 0) {
                cout << " ";
                for (int j = 0; j < 8; j++) {
                    cout << '+' << setw(12) << setfill('-') << "";
                }
                cout << '+' << endl;
            }
  
            if (i % 8 == 0) {
                cout << labeled_number  << "|"; 
                labeled_number++; 
            } else
                cout << '|'; 
            
            int padding_space = padding(chess_print_board[i].piece_name);
            cout << setw(padding_space) << setfill(' ') << "";
            cout << chess_print_board[i]; 
            cout << setw(total_space - padding_space - chess_print_board[i].piece_name.size()) << setfill(' ') << "";
            
            if ((i + 1) % 8 == 0) {
                cout << '|' << endl;
            }
        }

        cout << " ";
        for (int j = 0; j < 8; j++) {
            cout << '+' << setw(12) << setfill('-') << "";
        }

        cout << '+' << endl;
    }
}