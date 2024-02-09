#include <string>
#include "human.h"
using namespace std;

Human::Human(string name, PieceColour colour, Board* board): 
    Player{name, colour, board} {}


// if transfer is sucessful, return true; false othewise
bool transferLoc(string loc, int& x1, int& y1) {

    char c1 = loc[0];
    char c2 = loc[1];
    if (c1 < 'a' || c1 > 'h') return false;
    int x = c2 - '0';
    if (x < 1 || x > 8) return false;
    x1 = 8 - x;
    y1 = c1 - 'a';
    return true;
}

bool Human::makeMove() {
    string location1, location2;
    cin >> location1 >> location2;
    int x1, y1, x2, y2;
    if (!transferLoc(location1, x1, y1) || !transferLoc(location2, x2, y2)) {
        cout << "Invalid location" << endl;
        return false;
    }
    
    if (x1 == x2 && y1 == y2) {
        cout << "Same location" << endl;
        return false;
    }
    if (!board->canMove(x1, y1, x2, y2)) {
        cout << "Invalid move" << endl;
        return false;
    }

    // check if move own side chess
    if (board->returnChess(x1, y1)->returnColour() == colour) {
        // check if pawn promotion (reached the end of board)
        if (board->returnChess(x1, y1)->returnDescription() == "pawn" &&
            ((colour == PieceColour::Black && x2 == 7) || (colour == PieceColour::White && x2 == 0))) {
            bool invalidInput = true;
            string pieceType;
            while (invalidInput) {
                cin >> pieceType;
                if (cin.eof()) {
                    throw eof{} ;
                }
                if ( colour == PieceColour::White) {
                    if (pieceType != "R" && pieceType != "N"  && pieceType != "B" && pieceType != "Q") {
                        cout << "Invalid Piece Type" << endl;
                        invalidInput = true;
                    } else {
                        invalidInput = false;
                    }
                }
                else {
                    if (pieceType != "r" && pieceType != "n"  && pieceType != "b" && pieceType != "q") {
                        cout << "Invalid Piece Type" << endl;
                        invalidInput = true;
                    } else {
                        invalidInput = false;
                    }
                }
            }
            board->placeChess(pieceType, x2, y2);
            board->removeChess(x1, y1);
            return true;
        }
        else {
            board->move(x1, y1, x2, y2);
            return true;
        }
    } else {
        cout << "Invalid chess" << endl;
        return false;
    }
}
