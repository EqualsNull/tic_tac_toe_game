/*
 * Created by Jorge Castillo on 4/10/16.
 *
 * Contains the "back-end" logic
 * Sources used for the AI/Minimax algorithm:
 *
 *      MIT LECTURE                  : https://www.youtube.com/watch?v=STjW3eH0Cik
 *      MIT RECITATION               : https://www.youtube.com/watch?v=hM2EAvMkhtk
 *      MINIMAX ALGORITHM YT VIDEO   : https://www.youtube.com/watch?v=6ELUvkSkCts
 *      MINIMAX ANIMATION YT VIDEO   : https://www.youtube.com/watch?v=zDskcx8FStA
 *      WIKIPEDIA MINIMAX            : https://en.wikipedia.org/wiki/Minimax#Example
 *      WIKIPEDIA ALPHA/BETA PRUNING : https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
 *
 * */

#include <iostream>
#include <algorithm>
#include "tictactoe.h"

/* BEGIN STATE FUNCTION MEMBERS */
State::State() { // initial game state

    result = "DNF";
    turnIsX = true;
    movesByO = 0;
    board= {'E','E','E',
            'E','E','E',
            'E','E','E'
    };

    return;
}

void State::advanceTurn() {

    if (turnIsX) {    // move turn to opponent

        turnIsX = false;

    } else {          // move turn to first player

        movesByO++; // increment depth;
        turnIsX = true;

    }
    return;
}

bool State::isX() const {

    return turnIsX;

}

bool State::isTerminal() {

    /* TO DO: Update game with the winner */

    // horizontal check
    for (int i = 0; i < 9; i+=3) {

        // sanity check, we don't need any E's
        bool isFirstCellE = board.at(i) == 'E';
        bool isHorizontalCellsEqual;

        isHorizontalCellsEqual =
                board.at(i) == board.at(i + 1) &&
                board.at(i + 1)== board.at(i + 2);

        if (!isFirstCellE && isHorizontalCellsEqual) {

            // This is a cheap way of grabbing who won:
            // When the UI is updated again it will catch
            // the first element and print it as winner
            board.at(0) = board.at(i);

            if(board.at(i) == 'X') this->result = "X-WON";
            else if(board.at(i) == 'O') this->result = "O-WON";

            return true;
        }

    }

    // vertical check
    for (int i = 0; i < 3; i++) {

        bool isFirstCellE = board.at(i) == 'E';

        bool isVerticalCellsEqual =
                board.at(i) == board.at(i + 3)&&
                board.at(i + 3) == board.at(i + 6);

        if(!isFirstCellE && isVerticalCellsEqual) {

            board.at(0) = board.at(i);

            if(board.at(i) == 'X') this->result = "X-WON";
            else if(board.at(i) == 'O') this->result = "O-WON";

            return true;
        }
    }

    {   // diagonal check


        bool isMiddleE = board.at(4) == 'E';

        bool isUpperLeftToLowerRightEqual =
                board.at(0) == board.at(4) &&
                board.at(4) == board.at(8);

        bool isUpperRightToLowerLeftEqual =
                board.at(2) == board.at(4) &&
                board.at(4) == board.at(6);


        if(!isMiddleE &&
           (isUpperLeftToLowerRightEqual ||
            isUpperRightToLowerLeftEqual)) {

            if(board.at(4) == 'X') this->result = "X-WON";
            else if(board.at(4) == 'O') this->result = "O-WON";

            return true;
        }


    }

    // Draw check
    if (getEmptyCells().empty()) {

        this->result = "DRAW";
        return true;
    }

    return false;
}

int State::getMovesByO() const {

    return movesByO;

}

std::vector<int> State::getEmptyCells() {

    std::vector<int> indices(0);

    for(int i = 0; i < 9; i++){

        if(board.at(i) == 'E')
            indices.push_back(i);

    }

    return indices;
}

std::vector<char> State::getBoard() {

    return this->board;

}

void State::SetCell(int index, char who) {

    this->board.at(index) = who;

}

State& State::operator=(const State &stateCopy) {

    if (this != &stateCopy) {

        this->turnIsX = stateCopy.turnIsX;
        this->board = stateCopy.board;
        this->movesByO = stateCopy.movesByO;

    }
    return *this;
}

std::string State::getResult() const {

    return this->result;

}
/* END STATE FUNCTION MEMBERS */


/* BEGIN AI FUNCTION MEMBERS */
std::map<int,int> Ai::mapValues(State state){

    std::vector<int> available = state.getEmptyCells();
    std::vector<int>::iterator it = available.begin();
    std::map<int, int> minimaxMap;

    while(it != available.end()){
        State nextState = state;

        /* Simulate a move */
        nextState.SetCell(*it, 'O');
        nextState.advanceTurn();

        minimaxMap[*it] = this->calculateMiniMax(nextState);
        it++;
    }
    return minimaxMap;
}

int Ai::calculateMiniMax(State _state, int alpha, int beta) {

    if (_state.isTerminal()) {
        int value = 0;

        if (_state.getResult() == "X-WON")
            value = -10;

        else if (_state.getResult() == "O-WON")
            value = 10;

        return value - _state.getMovesByO(); //heuristic value;

    }else{

        // iterate over all possibilities
        std::vector<int> availableMoves =_state.getEmptyCells();
        std::vector<int>::const_iterator it;
        it = availableMoves.begin();

        if (!_state.isX()) { // MAX
            while (it != availableMoves.end()) {

                State nextState = _state;

                /* Simulate a move */
                nextState.SetCell(*it, 'O');
                nextState.advanceTurn();

                alpha = std::max(alpha, this->calculateMiniMax(nextState,alpha,beta));

                if(beta <= alpha)
                    break;


                it++;
            }

            return alpha; // MAXIMUM BETWEEN BETA AND TERMINAL

        } else {

            while (it != availableMoves.end()) {

                State nextState = _state;

                /* Simulate a move */
                nextState.SetCell(*it,'X');
                nextState.advanceTurn();

                beta = std::min(beta, this->calculateMiniMax(nextState, alpha, beta));

                if(beta <= alpha)
                    break;

                it++;
            }
            return beta; // MAXIMUM BETWEEN BETA AND TERMINAL
        }

    }

}

/* END AI FUNCTION MEMBERS */


/* BEGIN GAME FUNCTION MEMBERS */
Game::Game(){

    currentState = new State(); // start a new game
    this->status = "MENUSELECT";

}

Game::~Game() {

    std::cout << "Game obj is out of scope";

    delete ai;
    delete currentState;

}

void Game::Start(int mode) {

    this->status = "RUNNING";

    if(mode == 1) { // user chose to play against AI
        this->mode = true;
        this->ai = new Ai;
    }

    else if(mode == 2) // user chose to play against human
        this->mode = false;

    return;
}

bool Game::isSinglePlayer() {

    return mode;

}

bool Game::TurnIsX() {

    return this->currentState->isX();

}

void Game::advanceToNewState() {

    int boardInput;
    char whoIsIt;

    // ask for input
    if (TurnIsX()) {

        do{

            std::cin >> boardInput;

        }while(boardInput < 0 && boardInput > 9);

        whoIsIt = 'X';

    }

    if (isSinglePlayer() && !TurnIsX()) { // AI GAME

        std::map<int,int> mappedValues = ai->mapValues(*(this->currentState));
        std::map<int,int>::iterator it = mappedValues.begin();

        int max = -1000;
        whoIsIt = 'O';

        // assess best choice
        for(it; it != mappedValues.end(); it++){

            if(max < it->second) {

                max = it->second;
                boardInput = it->first;

            }
        }

    } else if (!isSinglePlayer() && !TurnIsX()) { // SINGLE PLAYER

        do{

            std::cin >> boardInput;

        }while(boardInput < 9 && boardInput > 0);

        whoIsIt = 'O';

    }

    this->currentState->SetCell(boardInput, whoIsIt);
    this->currentState->advanceTurn();

    if (this->currentState->isTerminal())
        this->status = "ENDED";

}

std::string Game::getStatus() const {

    return this->status;

}

std::vector<char> Game::getCurrentBoard() const {

    return this->currentState->getBoard();

}

std::string Game::getCurrentResult() const {

    return this->currentState->getResult();

}