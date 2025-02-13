#pragma once 

#include "abstract_chess_board.h"
#include "rook.h"
#include "user_interface.h"
#include "king.h"
#include "knight.h"
#include "bishops.h"
#include "queen.h"
#include "pawn.h"
#include "user_validation.h"
#include "initialize_step.h"
#include "handle_pawn_promotion.h"
#include "game_over.h"
#include "print_chess_game.h"

#include <unistd.h>
#include <poll.h>

#include <chrono>  
#include <ctime> 
#include <iostream> 
#include <memory>
#include <thread>
#include <string>
#include <functional>

using namespace std; 

namespace chess_game::chess_board {
    class ChessBoard : public Abstract_Chess_Board {
        private: 
            int sz{0};
            static bool inline fifty_move_rule = true; 
            static int inline pawn_move = 0; 
            map<ChessPosition, int> three_fold_map; 
        protected: 
            typedef Abstract_Chess_Board Base; 
            typedef rook::Rook rook; 
            typedef king::King king; 
            typedef knight::Knight knight; 
            typedef bishop::Bishop bishop; 
            typedef queen::Queen queen; 
            typedef pawn::Pawn pawn; 
            typedef user_interface::UserInterface user_interface; 
            typedef handle_pawn_promotion::PromotionType PromotionType; 
            using Base::piece_pos_set_1, Base::piece_pos_set_2;  
            using typename Base::Piece, typename Base::Position; 
            typedef typename user_interface::Player Player;
             
            friend Base;
        public: 
            user_interface users; 
        
        private:

            void initializeRook() {
                vector<Piece*> rook_pointer; 
                chess_game::general_initial::general_initial_step<rook>(rook::initial_pos, rook::initial_alias_rk, rook_pointer);
            }

            void initializeKing() {
                vector<Piece*> king_pointer;
                chess_game::general_initial::general_initial_step<king>(king::initial_pos, king::initial_alias_king, king_pointer);
                for (int j = 0; j < king_pointer.size(); j++)
                    users.stored_king_pointer.emplace(users.player_pointer_map[j + 1], king_pointer[j]);    
            }

            void initializeKnight() {
                vector<Piece*> knight_pointer; 
                chess_game::general_initial::general_initial_step<knight>(knight::initial_pos, knight::initial_alias_knight, knight_pointer);
            }

            void initializeBishop() {
                vector<Piece*> bishop_pointer; 
                chess_game::general_initial::general_initial_step<bishop>(bishop::initial_pos, bishop::initial_alias_bishop, bishop_pointer);
            }

            void initializeQueen() {
                vector<Piece*> queen_pointer; 
                chess_game::general_initial::general_initial_step<queen> (queen::initial_pos, queen::initial_alias_queen, queen_pointer);
            }

            void initializePawn() {
                vector<Piece*> pawn_pointer; 
                chess_game::general_initial::general_initial_step<pawn> (pawn::initial_pos, pawn::initial_alias_pawn, pawn_pointer);
                
                for (auto iter_set = pawn_pointer.begin(); iter_set != pawn_pointer.end(); iter_set++) {
                    (*iter_set) -> is_pawn = true; 
                }
            }

            void helpingFunctionForThreeFoldConditionMap(const set<ChessPosition>& set) {
                for (auto iter = set.begin(); iter != set.end(); iter++) {
                    three_fold_map[*iter] += 1;
                }
            }

            void three_fold_condition_map_init() {
                for (int i = 0; i < upper_horizontal_limit; i++) {
                    for (int j = 0; j < upper_vertical_limit; j++) {
                        auto key = make_pair(position_map[Position(i, j)], new Piece());
                        three_fold_map.emplace(key, 0); 
                    }
                }

                helpingFunctionForThreeFoldConditionMap(piece_pos_set_1);
                helpingFunctionForThreeFoldConditionMap(piece_pos_set_2);
            }

            void board_initialization() {
                initializeRook();
                initializeKing();
                initializeKnight();
                initializeBishop();
                initializeQueen();
                initializePawn();
                
                three_fold_condition_map_init();
                chess_game::print_board::board_init();
                // users.initialize_int_to_literal_position();
                user_interface::UserInterface::initialize_int_to_literal_position();

                users.updatePlayerPieceMap(true, piece_pos_set_1);
                users.updatePlayerPieceMap(false, piece_pos_set_2); 
            }

            void printSet(const set<Position>& found_pair) {
                cout << endl << "Here are some possible moves you can make on the board:" << endl;
                cout << setw(6) << setfill(' ') << "";

                for (auto iter = found_pair.begin(); iter != found_pair.end(); iter++) { 
                    bool is_rival = false; 

                    if (board[position_map[*iter]])
                        is_rival = true; 
                    
                    cout << users.int_to_literal_position[*iter];

                    if (is_rival) {
                        cout << " (opponent's)";
                    }

                    if (iter == (--found_pair.end())) 
                        cout << endl; 
                    else 
                        cout << ", "; 
                }
                cout << endl;
            }
        protected: 
            using Base::ChessPosition; 

            void make_board() {
                for (int i = 0; i < upper_horizontal_limit; i++) {
                    for (int j = 0; j < upper_vertical_limit; j++) {
                        Position* new_pos = new Position(i, j);
                        board[new_pos] = false; 
                        position_map.emplace(Position(i, j), new_pos);
                    }
                }
            }
        
        // Consider having the clean-up function in order to free up the memory 
        private: 
            void clean_up() {
                piece_pos_set_1.clear();
                piece_pos_set_2.clear();
                Base::board.clear();
                three_fold_map.clear();
            }

            void update_board_after_attack(bool is_it_player_1, Position inital_pos, Position attacked_pos,
                                           set<ChessPosition>::iterator its_erase_pos, Piece* piece) {
                set<ChessPosition>& its_pos_set = (is_it_player_1 ? piece_pos_set_1 : piece_pos_set_2);
                set<ChessPosition>& rival_pos_set = (is_it_player_1 ? piece_pos_set_2 : piece_pos_set_1);

                cout << "Destroying the piece from the board of player " << (is_it_player_1 ? 2 : 1) << " at " << user_interface::int_to_literal_position[attacked_pos] << endl; 
                set<ChessPosition>::iterator rival_erase_iter; 

                for (auto iter = rival_pos_set.begin(); iter != rival_pos_set.end(); iter++) {
                    auto& rival = *iter; 
                    auto pos_ptr = rival.second -> pos; 

                    if (pos_ptr -> second != nullptr && *rival.first == attacked_pos) {
                        rival_erase_iter = pos_ptr; 
                        break;
                    } 
                }
                
                // cout << *rival_erase_iter -> first << ": rival_erase_iter" << endl;
                // cout << *its_erase_pos -> first << ": its_erase iter" << endl;

                if (!rival_erase_iter -> second -> is_pawn && fifty_move_rule)
                    pawn_move += 1; 
                else if (rival_erase_iter -> second -> is_pawn && pawn_move < 50)
                    fifty_move_rule = false; 

                rival_pos_set.erase(rival_erase_iter);
                its_pos_set.erase(its_erase_pos);

                board[position_map[inital_pos]] = false; 
                auto temp_iter = its_pos_set.insert({position_map[attacked_pos], piece});
                piece -> pos = temp_iter.first;

                users.updatePlayerPieceMap(is_it_player_1, its_pos_set);
                users.updatePlayerPieceMap(!is_it_player_1, rival_pos_set);
            }

            void handlePawnPromotion(Piece* currentPiece, Position currentPosition) {
                int team = currentPiece -> team; 
                set<ChessPosition>& temp_set = (team == 1 ? piece_pos_set_1 : piece_pos_set_2);
                temp_set.erase(currentPiece -> pos);
                delete currentPiece;

                string user_input; 
                cout << "Congratulations! Your pawn has reached the final rank." 
                        "Choose a piece to promote it to and strengthen your army (Queen, Rook, Bishop, or Knight):" << endl;    
                cin >> user_input;  

                PromotionType promotion_type = chess_game::handle_pawn_promotion::getPromotionType(user_input);

                bool valid = false; 
                while (!valid) {
                    switch(promotion_type) {
                        case PromotionType::Queen: 
                            currentPiece = new queen("promoted queen", team);
                            valid = true;
                            break; 
                        case PromotionType::Rook:   
                            currentPiece = new rook("promoted rook", team);
                            valid = true; 
                            break;
                        case PromotionType::Knight:
                            currentPiece = new knight("promoted knight", team);
                            valid = true; 
                            break; 
                        case PromotionType::Bishop: 
                            currentPiece = new bishop("promoted bishop", team);
                            valid = true; 
                            break; 
                        case PromotionType::Invalid: 
                            cout << "Invalid promotion type! Please choose from Queen, Rook, Bishop, or Knight." << endl;
                            break;
                    }  

                    if (!valid) {
                        string input; 
                        cin >> input; 
                        promotion_type = chess_game::handle_pawn_promotion::getPromotionType(input);
                    }
                } 

                auto temp_iter = temp_set.insert({position_map[currentPosition], currentPiece});
                currentPiece -> pos = temp_iter.first; 
                users.updatePlayerPieceMap(team == 1, temp_set);
            }

            void update_board_after_move(const Position& initial_pos, const Position& final_pos, 
                                           bool is_it_player_1, set<ChessPosition>::iterator erase_pos, Piece* piece) {
                cout << endl << "Moving the piece from " << user_interface::int_to_literal_position[initial_pos] 
                                                        << " to " << user_interface::int_to_literal_position[final_pos] << endl; 

                board[position_map[initial_pos]] = false; 
                
                set<ChessPosition>& temp_pos = (is_it_player_1 ? piece_pos_set_1 : piece_pos_set_2);
                temp_pos.erase(erase_pos);
                auto temp_iter = temp_pos.insert({position_map[final_pos], piece});
                piece -> pos = temp_iter.first; 
                users.updatePlayerPieceMap(is_it_player_1, temp_pos);
                board[position_map[final_pos]] = true; 

                if (piece -> is_pawn && fifty_move_rule)
                    pawn_move += 1; 
            }   

            void escapeCheckMate(int team, Piece* piece) {
                if (piece == nullptr) {
                    cout << "nullptr piece in escapeCheckMate" << endl;
                    exit(1);
                }

                piece -> determineMove();
                if (piece -> found_pair.empty()) {
                    cout << "Game over...." << endl;
                    exit(0);
                } 

                int x, y; 
                bool ontime_input = true; 
                cout << setw(10) << setfill(' ') << ""
                    "Your king is in checkmate. You must move your king to escape checkmate."  << endl;
                chess_game::user_validation::userInputValidation(x, y, piece, ontime_input);
                if (!ontime_input) {
                    cout << "Game over..." << endl;
                    exit(0);
                }

                users.check_mate_map[team] = false; 
            }  



        public:      

            ChessBoard() {
                users.stage1();
                make_board();
                board_initialization();
                // users.printMap();
                chess_game::print_board::printBoard();
            }

            ~ChessBoard() {clean_up();}
            
            int num_elements() const {return sz;}

            void printBoard() {
                for (auto iter = board.begin(); iter != board.end(); iter++) {
                    cout << '(' << iter -> first -> GetX() 
                        << ", " << iter -> first -> GetY() << ')' << endl;
                    cout << "True/False: " << (iter -> second == 0 ? "False" : "True") << endl << endl;      
                }
            }

            void makeMove() {
                cout << endl;

                for (auto iter = users.player_piece.begin(); iter != users.player_piece.end();) {
                    bool back_to_the_original_menu = false;
                    bool ontime_input = true; 

                    Player* p = iter -> first; 
                    cout << *p << endl << endl;
                    set<ChessPosition>& temp_set = iter -> second; 

                    if (users.check_mate_map[p -> team]) {
                        Piece* king_pointer = users.stored_king_pointer[p];
                        escapeCheckMate(p -> team, king_pointer);
                        exit(0);
                    }   
 
                    for (auto iter_set = temp_set.begin(); iter_set != temp_set.end(); iter_set++) {
                        char user_selection;
                        bool valid_input = false; 

                        do {
                            try {
                                users.stage2(user_selection, iter_set -> second -> name, *iter_set -> first, valid_input);
                            } catch(const runtime_error& err) {
                                cout << "Error: " << err.what() << endl; 
                            } 

                            if (!valid_input) {
                                cout << "Invalid input: please choose either 'y' or 'n'." << endl;        
                            }

                        } while (!valid_input);
                        
                        
                        if (user_selection == 'y') {
                            int x, y; 
                            if (!iter_set -> second -> found_pair.empty()) {
                                cout << "found pair is not empty before calling determineMove" << endl;
                                exit(1);
                            }
                            
                            if (iter_set -> second == nullptr) {
                                cout << "nullptr" << endl;
                                exit(0);
                            }

                            iter_set -> second -> determineMove();   
                            printSet(iter_set-> second -> found_pair);

                            if (iter_set -> second -> found_pair.empty()) {
                                cout << "No valid moves available for the " << iter_set->second->name << ". Please select another piece to continue." << endl;
                                continue;
                            }     

                            chess_game::user_validation::userInputValidation(x, y, iter_set -> second, ontime_input);

                            if (!ontime_input) {
                                iter++; 
                                break; 
                            } 

                            Position input_position(x, y); 

                            Position initial_pos = *iter_set -> first; 
                            Position final_pos = input_position; 
                            bool is_it_player_1 = (iter -> first -> team == 1);
                            set<ChessPosition>::iterator erase_iter = iter_set -> second -> pos;  
                            Piece* piece = iter_set -> second; 
                            string piece_name = iter_set -> second -> name;

                            if (iter_set -> second -> attackEnemy(input_position)){                            
                                update_board_after_attack(is_it_player_1, initial_pos, final_pos, erase_iter, piece);
                            } else if (temp_set.count({position_map[input_position], iter_set -> second}) == 0) {            
                                update_board_after_move(initial_pos, final_pos, is_it_player_1, erase_iter, piece);
                            } 
                            cout << endl; 

                            
                            chess_game::print_board::modifiedBoard(initial_pos, final_pos, piece_name);

                            if (piece -> checkMate(final_pos)) {
                                cout << "Check mate" << endl << endl;
                                int rival = (is_it_player_1 ? 2 : 1);
                                users.check_mate_map[rival] = true; 
                            } 

                            if (piece -> is_pawn && piece -> pawnPromotion()) {
                                handlePawnPromotion(piece, final_pos);
                            }

                            piece -> found_pair.clear();
                            chess_game::print_board::printBoard();
                            three_fold_map[{position_map[final_pos], iter_set -> second}] += 1; 
                            iter++; 
                            break;
                        } else {
                            auto temp_iter_set = iter_set; 
                            temp_iter_set++; 

                            if (temp_iter_set == (temp_set.end())) 
                                back_to_the_original_menu = true; 
                            
                            if (back_to_the_original_menu)
                                cout << "Please select a piece from the chessboard to make your move." << endl;
                        }
                    }
                    cout << endl << endl;

                    if (game_over() 
                    || stalemate(p -> team == 1 ? 2 : 1) 
                    || three_fold_condition()) 
                        exit(0);
                }      
            }

            bool game_over() const {
                bool stale_mate = false; 
                if (game_over::insufficient_material() || fifty_move_rule && pawn_move == 50) {
                    cout << "tie game due to deadend rule/ 50 move rule" << endl;
                    return true; 
                }
                return false; 
            }

            bool stalemate(int its_team) {
                if (!users.check_mate_map.at(its_team)) {
                    const set<ChessPosition>& its_set = (its_team == 1 ? piece_pos_set_1 : piece_pos_set_2);
                    const set<ChessPosition>& rival_set = (its_team == 1 ? piece_pos_set_2 : piece_pos_set_1);

                    ChessPosition king_pair; 
                    set<Position> temp_possible_move; 

                    bool all_position_is_checkmate = true; 

                    bool king_found = false;
                    for (const auto& piece : its_set) {
                        if (piece.second -> is_king) {
                            king_pair = piece;
                            king_found = true; 
                        }
                    }

                    if (!king_found) {
                        cout << "Error: King not found in the piece set. Unable to determine stalemate condition." << endl;
                        exit(1);
                    }

                    Position* king_position = king_pair.first; 
                    king* king_temp = dynamic_cast<king*> (king_pair.second); 
                    king_temp -> legalMove(temp_possible_move, king_position -> GetX(), king_position -> GetY(), *king_position);

                    if (temp_possible_move.empty()) {
                        for (const auto& piece : its_set) {
                            if (piece.second -> is_king)
                                continue;
                            
                            set<Position> piece_possible_move; 
                            piece.second -> legalMove(piece_possible_move, piece.first -> GetX(), piece.first -> GetY(), *piece.first);
                            if (!piece_possible_move.empty())
                                return false; 
                        }
                        cout << "Stale mate. Tie game!" << endl;
                        return true; 
                    }

                }
                
                return false; 
            }

            bool three_fold_condition() {  
                for (auto iter = three_fold_map.begin(); iter != three_fold_map.end(); iter++) {
                    if (iter -> second == 3) {
                        cout << "three fold condition. Tie game!" << endl;
                        return true; 
                    }
                }
         
                return false; 
            }

            void gamePlay() {
                while (!game_over()) {
                    makeMove();
                }
            }
    };
}