#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "player.h"
#include "human.h"
#include "computer.h"
#include "levelone.h"
#include "textdisplay.h"
#include "board.h"
#include "leveltwo.h"
#include "levelthree.h"
#include "levelfour.h"
#include "window.h"
using namespace std;




void SetUpStandard(Board& b) { 

    b.placeChess("K", 7, 4);
    b.placeChess("Q", 7, 3);
    b.placeChess("R", 7, 0);
    b.placeChess("R", 7, 7);
    b.placeChess("N", 7, 1);
    b.placeChess("N", 7, 6);
    b.placeChess("B", 7, 2);
    b.placeChess("B", 7, 5);

    b.placeChess("k", 0, 4);
    b.placeChess("q", 0, 3);
    b.placeChess("r", 0, 0);
    b.placeChess("r", 0, 7);
    b.placeChess("n", 0, 1);
    b.placeChess("n", 0, 6);
    b.placeChess("b", 0, 2);
    b.placeChess("b", 0, 5);

    for (int i = 0; i < 8; ++i) {
        b.placeChess("P", 6, i);
        b.placeChess("p", 1, i);
    }
}

// if transfer is sucessful, return true; false othewise
bool transferLocation(string loc, int& x1, int& y1) {

    char c1 = loc[0];
    char c2 = loc[1];
    if (c1 < 'a' || c1 > 'h') return false;
    int x = c2 - '0';
    if (x < 1 || x > 8) return false;
    x1 = 8 - x;
    y1 = c1 - 'a';
    return true;
}


// add Xwindow later
void printDisplay(TextDisplay* td) {
    cout << *td << endl;
}


void printScore(int white, int black) {
    cout << "Final Score:" << endl;
    cout << "White: " << white << endl;
    cout << "Black: " << black << endl;
}


int main() {

    string cmd, player1, player2, location1,location2, promotion_type, pieceColour;
    int x1, y1, x2, y2;
    // Player* white_player = nullptr;
    unique_ptr<Player> white_player = nullptr;
    // Player* black_player = nullptr;
    unique_ptr<Player> black_player = nullptr;
    int white_score = 0;
    int black_score = 0;
    Player* thisTurn = white_player.get();

    string font = "-*-helvetica-*-r-*-*-24-*-*-*-*-*-*-*";


    while (true) {
        bool isSetup = false;
        Board board;

        unique_ptr<TextDisplay> td = make_unique<TextDisplay>();
        board.attachTD(td.get());

        unique_ptr<Xwindow> gd = make_unique<Xwindow>(720, 720);
        board.attachGD(gd.get());
        gd->drawBoard();
        gd->drawChessString(font);

        PieceColour initial = PieceColour::White;
        bool white_is_computer, black_is_computer;

        for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        board.placeChess("", i, j);
                    }
                }

        while (true) {
            
            cout << "setup: enter set up mode" << endl;
            cout << "game: start a new game" << endl;
            cin >> cmd;
            if (cin.eof()) {
                printScore(white_score, black_score);
                return 0;
            }

            // enter game or setup to start the game
            if (cmd == "game") {
                
                cout << "Please enter player white: human or computer[1-4]" << endl;
                while (true) {
                    cin >> player1;
                    if (cin.eof()) {
                        printScore(white_score, black_score);
                        return 0;
                    }
                    if (player1 == "human" || player1 == "Human") {
                        // white_player = new Human{"human", PieceColour::White, &board};
                        white_player = make_unique<Human>("human", PieceColour::White, &board);
                        white_is_computer = false;
                        break;
                    } else if (player1 == "computer1" || player1 == "Computer1") {
                        // white_player = new LevelOne{"Computer1", PieceColour::White, &board};
                        white_player = make_unique<LevelOne>("Computer1", PieceColour::White, &board);
                        white_is_computer = true;
                        break;
                    } else if (player1 == "computer2" || player1 == "Computer2") {
                        // white_player = new LevelTwo{"Computer2", PieceColour::White, &board};
                        white_player = make_unique<LevelTwo>("Computer3", PieceColour::White, &board);
                        white_is_computer = true;
                        break;
                    } else if (player1 == "computer3" || player1 == "Computer3") {
                        // white_player = new LevelThree{"Computer3", PieceColour::White, &board};
                        white_player = make_unique<LevelThree>("Computer3", PieceColour::White, &board);
                        white_is_computer = true;
                        break;
                    } else if (player1 == "computer4" || player1 == "Computer4") {
                        // white_player = new LevelFour{"Computer4", PieceColour::White, &board};
                        white_player = make_unique<LevelFour>("Computer4", PieceColour::White, &board);
                        white_is_computer = true;
                        break;
                    } else {
                        // cout << "Invalid input, please enter \"human\" or \"computer[1-4]\"" << endl;
                    }
                }
                cout << "Please enter player2: human or computer[1-4]" << endl;
                while (true) {
                    cin >> player2;
                    if (cin.eof()) {
                        printScore(white_score, black_score);
                        return 0;
                    }
                    if (player2 == "human" || player2 == "Human") {
                        black_player = make_unique<Human>("human", PieceColour::Black, &board);
                        black_is_computer = false;
                        break;
                    } else if (player2 == "computer1" || player2 == "Computer1") {
                        // black_player = new LevelOne{"Computer1", PieceColour::Black, &board};
                        black_player = make_unique<LevelOne>("Computer1", PieceColour::Black, &board);
                        black_is_computer = true;
                        break;
                    } else if (player2 == "computer2" || player2 == "Computer2") {
                        // black_player = new LevelTwo{"Computer2", PieceColour::Black, &board};
                        black_player = make_unique<LevelTwo>("Computer2", PieceColour::Black, &board);
                        black_is_computer = true;
                        break;
                    } else if (player2 == "computer3" || player2 == "Computer3") {
                        // black_player = new LevelThree{"Computer3", PieceColour::Black, &board};
                        black_player = make_unique<LevelThree>("Computer3", PieceColour::Black, &board);
                        black_is_computer = true;
                        break;
                    } else if (player2 == "computer4" || player2 == "Computer4") {
                        // black_player = new LevelFour{"Computer4", PieceColour::Black, &board};
                        black_player = make_unique<LevelFour>("Computer4", PieceColour::Black, &board);
                        black_is_computer = true;
                        break;
                    } else {
                        cout << "Invalid input, please enter \"human\" or \"computer[1-4]\"" << endl;
                    }
                }
                // gd->drawBoard();
                if (!isSetup) SetUpStandard(board);
                if (initial == PieceColour::White) thisTurn = white_player.get();
                else thisTurn = black_player.get();
                printDisplay(td.get());
                break;
        }

        if (cmd == "setup" || cmd == "Setup") {
            printDisplay(td.get());
            string piece;

            while (true) {
                cout << "setup: + - = done" << endl;
                cin >> cmd;
                if (cin.eof()) {
                    printScore(white_score, black_score);
                    return 0;
                }
                // printDisplay(td.get());
                if (cmd == "+") {

                    cin >> piece >>  location1;
                    if (cin.eof()) {
                        printScore(white_score, black_score);
                        return 0;
                    }
                    if (!transferLocation(location1, x1, y1)) {
                        cout << "Invalid location" << endl;
                        continue;
                    }
                    if (piece != "k" && piece != "K" && piece != "q" && piece != "Q" && piece != "P" && piece != "p" 
                        && piece != "b" && piece != "B" && piece != "N" && piece != "n" && piece != "r" && piece != "R") {
                            cout << "Invalid piece" << endl;
                            continue;
                        }
                    if (board.isEmpty(x1, y1)) board.removeChess(x1, y1);
                    board.placeChess(piece, x1, y1);
                    printDisplay(td.get());
                }

                else if (cmd == "-") {
                    
                    cin >> location1;
                    if (cin.eof()) {
                        printScore(white_score, black_score);
                        return 0;
                    }
                    if (!transferLocation(location1, x1, y1)) {
                        cout << "Invalid location" << endl;
                        continue;
                    }
                    board.removeChess(x1, y1);
                    printDisplay(td.get());
                }

                else if (cmd == "=") {
                    cin >> pieceColour;
                    if (cin.eof()) {
                        printScore(white_score, black_score);
                        return 0;
                    }
                    if (pieceColour == "black") initial = PieceColour::Black;
                    else if (pieceColour == "white") initial = PieceColour::White;
                    else cout << "Invalid colour" << endl;
                }

                else if (cmd == "done") {
                    if (!board.isLegalSetup()) {
                        cout << "Invalid set up" << endl;
                        continue;
                    }
                    break;
                }

                else {
                    cout << "invalid input, please enter: + - = done" << endl;
                }
            }
            isSetup = true;
        }

        else {
            cout << "Invalid input" << endl;
        }
    }


    while (true) {
        // if (board.isCheckmate(PieceColour::Black)) {
        if (board.returnGamestate() == GameState::WhiteWins) {
            white_score++;
            cout << "White wins!" << endl;
            break;
        }
        // if (board.isCheckmate(PieceColour::White)) {
        if (board.returnGamestate() == GameState::BlackWins) {
            black_score++;
            cout << "Black wins!" << endl;
            break;
        }
        PieceColour cur;
        if (thisTurn == white_player.get())  cur = PieceColour::White;
        else cur = PieceColour::Black;
        if (board.isCheck(thisTurn->getColour()) != thisTurn->getColour() && board.isStalemate(thisTurn->getColour()) && board.returnGamestate() == GameState::Stalemate) {
        // if (board.isStalemate(PieceColour::Black) || board.isStalemate(PieceColour::White)) {
            cout << "draw" << endl;
            black_score += 0.5;
            white_score += 0.5;
            break;
        }
        if (board.isCheckmate(cur)) {
            // black_score++;
            cout << "someone wins!" << endl;
            break;
        }

        if (thisTurn == white_player.get()) {
            cout << "Now it's white player's turn" << endl;
        }
        else  {
            cout << "Now it's black player's turn" << endl;
        }
        cout << "Please enter: resign move" << endl;
        cin >> cmd;
        if (cin.eof()) {
            printScore(white_score, black_score);
            return 0;
        }
        
        if (cmd == "resign" || cmd == "Resign") {
            if (!(white_is_computer && black_is_computer) && ((thisTurn == white_player.get() && white_is_computer) || (thisTurn == black_player.get() && black_is_computer))) {
                cout << "You cannot resign for computer" << endl;
                continue;
            }
            if (thisTurn == white_player.get()) {
                black_score++;
                cout << "Black wins!" << endl;
            }
            else if (thisTurn == black_player.get()) {
                white_score++;
                cout << "White wins!" << endl;
            }
            break;
        }

        else if (cmd == "move" || cmd == "Move") {
            bool valid;
            try {
                valid = thisTurn->makeMove();
            } catch (eof) {
                printScore(white_score, black_score);
                return 0;
            }

            if (valid) {
                if (thisTurn == black_player.get()) {
                    thisTurn = white_player.get();
                } else {
                    thisTurn = black_player.get(); 
                }
                printDisplay(td.get());

                if (board.isCheck(PieceColour::Black) == PieceColour::Black) {
                    cout << "Black is in check" << endl;
                    board.isCheckmate(thisTurn->getColour());
                } else if (board.isCheck(PieceColour::White) == PieceColour::White) {
                    cout << "White is in check" << endl;
                    board.isCheckmate(thisTurn->getColour());
                } else {
                    board.isStalemate(thisTurn->getColour());
                }
            }

        }

    }
    board.clearBoard();
    // delete white_player;
    // delete black_player;
    cout << "Do you want to play again? Y/ N" << endl;
    cin >> cmd;
    if (cin.eof()) {
        printScore(white_score, black_score);
        return 0;
    }
    if (cmd == "Y" || cmd == "y") continue;
    if (cmd == "N" || cmd == "n") {
        cout << "game end" << endl;
        printScore(white_score, black_score);
        break;
    }
    cout << "Invalid input, automatically start next game" << endl;
  }
}
