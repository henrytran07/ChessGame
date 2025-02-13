#pragma once 
#include "abstract_chess_board.h"
#include "user_interface.h"

#include <iostream> 
#include <utility> 
#include <set> 
#include <memory> 

using namespace std; 

namespace chess_game::general_initial {

    typedef chess_game::chess_board::Abstract_Chess_Board Base;
    typedef Base::ChessPosition ChessPosition; 

    template <typename E> 
    void general_initial_step(vector<pair<int, int>>& initial_pos, vector<pair<string, int>>& initial_alias, vector<Base::Piece*>& pointer) {
        for (int j = 0; j < initial_pos.size(); j++) {
            pair<int, int> initial_pair = initial_pos[j];
            Base::Piece* new_piece = new E(initial_alias[j].first, initial_alias[j].second);

            set<ChessPosition>& temp_set_pos = (initial_alias[j].second == 1 ? Base::piece_pos_set_1 : Base::piece_pos_set_2);
            auto temp_iter = temp_set_pos.insert({Base::position_map[Base::Position(initial_pair.first, initial_pair.second)], new_piece});
            Base::board[Base::position_map[Base::Position(initial_pair.first, initial_pair.second)]] = true; 
            if (temp_iter.second)
                new_piece -> pos = temp_iter.first; 
            else    
                cout << "Unsuccessful insertion process" << endl;
            pointer.push_back(temp_iter.first -> second);
        }

        initial_pos.clear();
        initial_pos.shrink_to_fit();
        initial_alias.clear();
        initial_alias.shrink_to_fit();
    }

}