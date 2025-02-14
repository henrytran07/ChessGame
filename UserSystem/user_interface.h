#pragma once 

#include "../ChessBoardFiles/abstract_chess_board.h"

#include <iostream>
#include <iomanip>
#include <map> 
#include <unordered_map>
#include <limits>
#include <set> 
#include <stdexcept>
#include <sstream> 
#include <utility>
using namespace std; 

namespace chess_game::user_interface {


class UserInterface {    
    private:  
        const int total_players = 2; 
    
        bool successful_registration(int no_players) {
            return no_players == total_players; 
        }

        void registration_stage(int team_order, string name) {
            Player* player = new Player(name, team_order);

            if (team_order == 1) 
                player_piece[player] = Base::piece_pos_set_1;
            else 
                player_piece[player] = Base::piece_pos_set_2; 

            player_pointer_map[team_order] = player;
            check_mate_map[team_order] = false;
        }

    public: 
        typedef chess_board::Abstract_Chess_Board Base; 
        typedef typename Base::Position Position; 
        typedef typename Base::Piece Piece; 
        typedef pair<Position*, Piece*> ChessPosition; 
        

        UserInterface() {
            // initialize_int_to_literal_position();
        }

        class Player {
                friend UserInterface; 

            public: 
                string name; 
                int team; 
            
            public: 
                Player(string nm, int no) : name{nm}, team{no} {}

                bool operator==(Player other_player) const {
                    return team == other_player.team;
                }

                friend ostream& operator<<(ostream& os, Player p) {
                    os << p.name; 
                    return os;
                }
        };

        struct Comparitor{
            bool operator()(Player* p1, Player* p2) const {
                if (p1 -> team != p2 -> team)
                    return p1 -> team < p2 -> team; 
                
                if (p1 -> name[0] != p2 -> name[0])
                    return p1 -> name[0] < p2 -> name[0];
                
                char lastCharP1 = p1 -> name.back();
                char lastCharP2 = p2 -> name.back();

                return (lastCharP1 - '0') < (lastCharP2 - '0');
            }
        };

        map<Player*, set<ChessPosition>, Comparitor> player_piece; 
        map<Player*, Piece*> stored_king_pointer; 
        map<int, Player*> player_pointer_map; 
        map<int, bool> check_mate_map; 

        static inline unordered_map<Position, string> int_to_literal_position; 

        static inline void initialize_int_to_literal_position() {
            for (int x = 0; x < 8; x++) {
                char c = 'a' + x; 
                for (int y = 0; y < 8; y++) {
                    int_to_literal_position[Position(x, y)] = c + to_string(y + 1); 
                }
            } 
        }

        // Having the instructions and obtain players' name
        void stage1() {
            /// Leave the instruction file for now and come back later  
            int team_order = 0; 

            while (!successful_registration(team_order)) {
                // obtain players' name 
                cout << endl;
                string name; 
                cout << "Player " << team_order + 1 << ": please enter your name/nickname:" << endl; 
                getline(cin, name);
                registration_stage(team_order + 1, name);
                team_order++;     
            }
        }

        void stage2(char& user_selection, string name, Position currentPosition, bool& valid_stage) {
            char temp_selection;
            cout << setw(6) << setfill(' ') << ""
                "For " << name << " at " << int_to_literal_position[currentPosition] << ". ";
            cout << "Click \'y\' if you want to choose this piece for this step or \'n\' to move to the next piece:" << endl; 
            cin >> temp_selection;

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (isalpha(temp_selection)) {
                if (temp_selection == 'y' || temp_selection == 'n') {
                    valid_stage = true;  
                    user_selection = temp_selection; 
                }
            } else if (isalnum(temp_selection)) 
                throw runtime_error("Input does not contain a number.");
             else 
                throw runtime_error("error in conversion");   
        }
          
        void printMap() {
            for (auto iter = int_to_literal_position.begin(); iter != int_to_literal_position.end(); iter++) {
                cout << iter -> first << " " << iter -> second << endl;
            }
        }

        void updatePlayerPieceMap(bool is_it_player_1, set<ChessPosition> piece_pos) {
            int player_order = (is_it_player_1 ? 1 : 2);
            for (auto iter = player_piece.begin(); iter != player_piece.end(); iter++) {
                if (iter -> first -> team == player_order)
                    player_piece[iter -> first] = piece_pos; 
            }
        }

        ~UserInterface() {
            player_piece.clear();
            stored_king_pointer.clear();
            player_pointer_map.clear();
            check_mate_map.clear();
        }
};
}