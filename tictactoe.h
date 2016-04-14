//
// Created by Jorge Castillo on 4/10/16.
//
#ifndef TICTACTOE_TICTACTOE_H
#define TICTACTOE_TICTACTOE_H
#include <vector>
#include <string>
#include <map>

class State {

public:

    State();
    void advanceTurn(); // move turn to next player
    int getMovesByO() const; // returns the amt of moves done by O
    bool isTerminal(); // checks if current state is draw/win
    bool isX() const; // checks if it's x's turn
    std::vector<int> getEmptyCells(); // returns an ARRAY of available indices
    std::vector<char>getBoard();
    void SetCell(int index, char who);
    std::string getResult()const;
    State& operator = (const State& stateCopy);

private:

    bool turnIsX;
    std::vector<char> board;
    int movesByO; //measures depth
    std::string result; // "N/A","X-WON","O-WON"
};

class Ai{

public:

    int calculateMiniMax(State _state,int alpha = -10000, int beta = 10000);
    std::map<int, int> mapValues(State state);

};

/////////////////

class Game {

public:

    Game();
    ~Game();

    void Start(int mode);
    bool isSinglePlayer(); // if false, O is an AI
    bool TurnIsX();
    void advanceToNewState();
    std::string getStatus() const;
    std::string getCurrentResult() const;
    std::vector<char> getCurrentBoard() const;

private:

    State* currentState;
    Ai* ai;
    std::string status; // "MENUSELECT" "RUNNING" "ENDED"
    bool mode; // single or two player mode

};


#endif //TICTACTOE_TICTACTOE_H
