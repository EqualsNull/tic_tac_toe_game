/*
 * Created by Jorge Castillo on 4/10/16.
 *
 * Contains the "front-end" logic
 * and interfaces with the game class
 *
 */

#include <iostream>
#include <iomanip>
#include "tictactoe.h"
using namespace std;

class UI{

public:

    UI(); // constructor
    void updateUI();
    void assignGame(Game* game); // allows access to the game obj

private:

    //helpers
    void ClearScreen();
    void updateHeader(string status);

    int width;
    int height;
    vector<string> header;
    string status;
    Game* game;

};

UI::UI() : width(80),
           height(9),
           game(NULL),
           status("NOT-RUNNING"),
           header(0) { return; }

// clears the screen and update with new content
void UI::updateUI() {
    ClearScreen();

    if (status != this->game->getStatus() || status == "RUNNING"){ // update header on status change

        status = this->game->getStatus();
        this->updateHeader(status);

    }

    vector<char> board;
    int headerHeight;

    board = this->game->getCurrentBoard();
    headerHeight = (int) this->header.size();

    /* TOP BORDER */
    cout << setw(width) << fixed << setfill('=') << "\n" << setfill(' ');

    /* HEADER */
    for (int i = 0; i < headerHeight; i++) {
        if (i < headerHeight) {
            cout << setw(width) << left << this->header.at(i);
        }
        cout << endl;
    }
    /* END HEADER */

    /* GENERATE TIC TAC TOE GRID */
    if(status == "RUNNING") {

        string padding = "               ";
        for (int i = 0; i < 9; i += 3) {
            cout << padding;
            for (int j = 0; j < 3; j++) {

                cout << setfill(' ');

                char cellValue = board.at(i + j);

                if (cellValue == 'E') // E -> index value for user input
                    cellValue = (i + j) + '0';

                if (j % 2 == 1)
                    cout << "|"; // vertical divider

                cout << setw(1) << " " << cellValue << " "; // Cell value

                if (j % 2 == 1)
                    cout << "|"; //vertical divider
            }
            if (i == 0 || i == 3) {
                cout << endl << padding << setfill('-') << setw(11) << "" << endl; // horizontal divider
            }
        }/* END GRID */
    }
    /* BOTTOM BORDER*/
    cout << setw(width) << fixed << setfill('=') << "\n\n";
    cout <<endl<<"INPUT: "<<endl;
}

void UI::updateHeader(string status){

    /* MAIN MENU */
    if(header.empty() && status== "MENUSELECT"){ // append menu, indexes 7 -> 9
        header.push_back("████████╗██╗ ██████╗    ████████╗ █████╗  ██████╗    ████████╗ ██████╗ ███████╗");
        header.push_back("╚══██╔══╝██║██╔════╝    ╚══██╔══╝██╔══██╗██╔════╝    ╚══██╔══╝██╔═══██╗██╔════╝");
        header.push_back("   ██║   ██║██║            ██║   ███████║██║            ██║   ██║   ██║█████╗  ");
        header.push_back("   ██║   ██║██║            ██║   ██╔══██║██║            ██║   ██║   ██║██╔══╝  ");
        header.push_back("   ██║   ██║╚██████╗       ██║   ██║  ██║╚██████╗       ██║   ╚██████╔╝███████╗");
        header.push_back("   ╚═╝   ╚═╝ ╚═════╝       ╚═╝   ╚═╝  ╚═╝ ╚═════╝       ╚═╝    ╚═════╝ ╚══════╝");
        header.push_back("Choose an opponent"); // index: 7
        header.push_back("1 -> SINGLE PLAYER MODE"); // index: 8
        header.push_back("2 -> TWO PLAYER MODE"); // index: 9
    } /* END MAIN MENU */

    else if (status == "RUNNING") {

        // wipe the title
        if(header.size() > 7){

            header.clear();
            header.resize(5);

        };


        /* AI GAME */
        if (!header.empty() && game->isSinglePlayer())
            header.at(2) = "SINGLE PLAYER GAME";


            /* TWO PLAYER GAME */
        else if (!header.empty() && !game->isSinglePlayer())
            header.at(2) = "TWO PLAYER GAME";


        if (this->game->TurnIsX()) {

            header.at(3) = "X's turn";

        } else {

            header.at(3) = "O's turn";

        }

    } else if (status == "ENDED"){

        string winner = this->game->getCurrentResult();
        string announce  = "\t\t\t\tGAME OVER : " + winner;

        header.at(4) = announce;
    }
}

void UI::ClearScreen() {
    cout << string( 100, '\n' );
}

void UI::assignGame(Game *game) {
    this->game = game;
}

int main() {

    UI ui;
    Game* game = new Game;
    ui.assignGame(game);
    int userInput;

    /* UI HEADER INDEX 0 -> 5 */
    ui.updateUI();

    cin >> userInput;
    if(userInput > 2 || userInput < 1) {

        cout << "\n\nINVALID TRY AGAIN\n\n";
        return 1;

    }

    game->Start(userInput);

    ui.updateUI();
    do {

        game->advanceToNewState();

        ui.updateUI();

    }while("ENDED" != game->getStatus());

    delete game;
    return 0;
};


