#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "leveltwo.h"
using namespace std;

const int boardDim = 8;

LevelTwo::LevelTwo(string name, PieceColour colour, Board* board): 
    Computer{name, colour, board, Level::Level2} {}

bool LevelTwo::makeMove() {

    // record positions of own side chess
    vector<vector<int>> myChessList;
    // record coordinates of available grids (opponent occupied/empty)
    vector<vector<int>> availableGrid;
    // record coordinates of empty grids
    vector<vector<int>> emptyGrid;

    // record coordinates of oppnent's king
    int opKingX, opKingY;

    for (int i = 0; i < boardDim; i++) {
        for (int j = 0; j < boardDim; j++) {
            if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() == colour) {
                // if own side chess
                myChessList.emplace_back(vector<int>{i,j});
            } else if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnDescription() == "king" &&
                board->returnChess(i, j)->returnColour() != colour) {
                // if opponent's king
                opKingX = i;
                opKingY = j;
                availableGrid.emplace_back(vector<int> {i, j});
            } else {
                if (board->returnChess(i, j)->isEmpty()) {
                    emptyGrid.emplace_back(vector<int> {i, j});
                }
                // if oppnent occupied / empty grid
                availableGrid.emplace_back(vector<int> {i, j});
            }
        }
    }

    // reference: https://cplusplus.com/reference/algorithm/shuffle/
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // get a random list of own side chess
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));

    // find the first capturing/check move
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < availableGrid.size(); j++) {
            x2 = availableGrid[j][0];
            y2 = availableGrid[j][1];
            if (board->canMove(x1, y1, x2, y2)) {
                // check checkmate
                if (board->returnChess(x2, y2)->returnDescription() == "king") {
                    board->move(x1, y1, x2, y2);
                    return true;
                }
                // check check move
                // if can move from (x2, y2) to the opponent king's location
                if (board->returnChess(x1, y1)->isLegalMove(x2, y2, opKingX, opKingY) &&
                    !board->returnChess(x1, y1)->isBlocked(x2, y2, opKingX, opKingY)) {
                    board->move(x1, y1, x2, y2);
                    return true;
                }
                // check pawn capturing move (& en passant)
                if (board->returnChess(x1, y1)->returnDescription() == "pawn" &&
                    isPawnCapture(x1, y1, x2, y2)) {
                    board->move(x1, y1, x2, y2);
                    return true;
                }
                // check other chess capturing move
                // if destination not empty
                if (!board->returnChess(x2, y2)->isEmpty()) {
                    // found capturing move
                    board->move(x1, y1, x2, y2);
                    return true;
                }
            }
        }
        // no capturing/check move for curr chess
    }

    // no check or capturing move, check legal move
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    shuffle(emptyGrid.begin(), emptyGrid.end(), default_random_engine(seed));
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < emptyGrid.size(); j++) {
            x2 = emptyGrid[j][0];
            y2 = emptyGrid[j][1];
            if (board->canMove(x1, y1, x2, y2)) {
                // perform pawn promotion if valid (queen the best choice)
                if (isPromotion(x1, y1, x2, y2)) {
                    if (colour == PieceColour::White) {
                        board->placeChess("Q", x2, y2);
                    } else {
                        board->placeChess("q", x2, y2);
                    }
                    board->removeChess(x1, y1);
                    return true;
                } 
                board->move(x1, y1, x2, y2);
                return true;
            }
        }
        // no legal move for curr chess
    }
    // no legal moves
    return false;
}
