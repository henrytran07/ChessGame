#pragma once 

#include "abstract_chess_board.h"
#include "user_interface.h"

#include <unistd.h>
#include <poll.h>

using namespace std; 

namespace chess_game::user_validation {

typedef chess_game::chess_board::Abstract_Chess_Board Base; 
typedef chess_game::user_interface::UserInterface user_interface; 
typedef Base::Piece Piece; 
typedef Base::Position Position; 

bool inputAvailable(int timeoutSeconds) {
    struct pollfd fds[1];
    fds[0].fd = STDERR_FILENO;  
    fds[0].events = POLLIN; 

    int ret = poll(fds, 1, timeoutSeconds * 1000);

    return (ret > 0 && (fds[0].revents && POLLIN));
}

void userInputValidation(int& x_coordi, int& y_coordi, Piece* piece, bool& ontime_input) {
    do {
        bool valid_input = false; 

        string user_input; 

        cout << "Please enter your move coordinates within 10 seconds:" << endl;

        if (inputAvailable(10)) {
            if (cin >> user_input) {
                cout << "You entered: " << user_input << endl;
            } 
        } else {
            cout << "Time's up ! You took too long to respond" << endl;
            ontime_input = false; 
            piece -> found_pair.clear();
            break; 
        }

        Position string_to_position_userInput; 
        bool found_position = false; 
        for (auto iter = user_interface::int_to_literal_position.begin(); iter != user_interface::int_to_literal_position.end(); iter++) {
            if (user_input == iter -> second) {
                string_to_position_userInput = iter -> first; 
                found_position = true; 
            }
        }

        if (!found_position) {
            valid_input = false; 
        }

        if (piece -> found_pair.count(string_to_position_userInput) == 1) 
            valid_input = true; 
        
        if (valid_input) {
            cout << "Valid input...Proceeding to the next phase" << endl;
            x_coordi = string_to_position_userInput.GetX(); 
            y_coordi = string_to_position_userInput.GetY(); 
            break; 
        } else 
            cout << "Invalid input" << endl << endl;
    } while (true);
}
}