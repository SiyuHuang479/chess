#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "levelthree.h"
using namespace std;

const int boardDim = 8;

LevelThree::LevelThree(string name, PieceColour colour, Board* board): 
    Computer{name, colour, board, Level::Level3} {}

bool LevelThree::inCapture(int x, int y, const vector<vector<int>> opChess) { 
    for (size_t i = 0; i < opChess.size(); i++) {
        int opX = opChess[i][0];
        int opY = opChess[i][1];
        // if curr opchess being captured
        if (opX == x && opY == y) {
            continue;
        }
        // check pawn capture
        if (board->returnChess(opX, opY)->returnDescription() == "pawn" &&
            isPawnCapture(opX, opY, x, y)) {
            return true;
        } 
        // check normal capture
        if (board->canMove(opX, opY, x, y)) {
            return true;
        } else continue;
    }
    // not in capture, safe position
    return false;
}

bool LevelThree::checkEscape(int kingX, int kingY, const vector<vector<int>> opChess, const vector<vector<int>> grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        int x2 = grid[i][0];
        int y2 = grid[i][1];
        if (board->canMove(kingX, kingY, x2, y2)) {
            // check potential capture at (x2, y2)
            PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
            bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
            board->returnChess(x2, y2)->setColor(board->returnChess(kingX, kingY)->returnColour());
            board->returnChess(x2, y2)->setEmpty(false);

            PieceColour prevStartCor = board->returnChess(kingX, kingY)->returnColour();
            bool prevStartEptState = board->returnChess(kingX, kingY)->isEmpty();
            board->returnChess(kingX, kingY)->setColor(PieceColour::Empty);
            board->returnChess(kingX, kingY)->setEmpty(true);

            if (!inCapture(x2, y2, opChess)) {
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(kingX, kingY)->setColor(prevStartCor);
                board->returnChess(kingX, kingY)->setEmpty(prevStartEptState);
                board->move(kingX, kingY, x2, y2);
                return true;
            } else {
                // restore and continue
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(kingX, kingY)->setColor(prevStartCor);
                board->returnChess(kingX, kingY)->setEmpty(prevStartEptState);
                continue;
            }
        }
    }
    // check king's legal move
    for (size_t i = 0; i < grid.size(); i++) {
        int x2 = grid[i][0];
        int y2 = grid[i][1];
        if (board->canMove(kingX, kingY, x2, y2)) {
            board->move(kingX, kingY, x2, y2);
            return true;
        }
    }
    return false;
}

bool LevelThree::makeMove() {
    // record positions of own side chess
    vector<vector<int>> myChessList;
    // record positions of opponent's chess
    vector<vector<int>> opChessList;
    // record coordinates of available grids (opponent occupied/empty)
    vector<vector<int>> availableGrid;
    // record coordinates of empty grids
    vector<vector<int>> emptyGrid;
    // record coordinates of king
    int opKingX, opKingY;
    int myKingX, myKingY;

    for (int i = 0; i < boardDim; i++) {
        for (int j = 0; j < boardDim; j++) {
            if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() == colour) {
                // if own side chess
                myChessList.emplace_back(vector<int>{i,j});
                // if my king
                if (board->returnChess(i, j)->returnDescription() == "king") {
                    myKingX = i;
                    myKingY = j;
                }
            } else if (!board->returnChess(i, j)->isEmpty() &&
                board->returnChess(i, j)->returnColour() != colour) {
                // if opponent's chess
                opChessList.emplace_back(vector<int> {i, j});
                if (board->returnChess(i, j)->returnDescription() == "king") {
                    opKingX = i;
                    opKingY = j;
                }
                // opponent occupied grid
                availableGrid.emplace_back(vector<int> {i, j});
            } else {
                // if empty grid
                emptyGrid.emplace_back(vector<int> {i, j});
                availableGrid.emplace_back(vector<int> {i, j});
            }
        }
    }

    // reference: https://cplusplus.com/reference/algorithm/shuffle/
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // get a random list of own side chess
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    // get a random list of available move
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));

    // check escape if currently in check
    if (inCapture(myKingX, myKingY, opChessList)) {
        if (checkEscape(myKingX, myKingY, opChessList, availableGrid)) {
            return true;
        }
        // if king cannot escape, check whether other piece can block the check
        else {
            for (size_t i = 0; i < myChessList.size(); i++) {
                int x1, y1, x2, y2;
                x1 = myChessList[i][0];
                y1 = myChessList[i][1];
                for (size_t j = 0; j < availableGrid.size(); j++) {
                    x2 = availableGrid[j][0];
                    y2 = availableGrid[j][1];
                    if (board->canMove(x1, y1, x2, y2)) {
                        // restore and change color & empty state of dest chess to stimulate move
                        PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                        bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                        board->returnChess(x2, y2)->setColor(board->returnChess(x1, y1)->returnColour());
                        board->returnChess(x2, y2)->setEmpty(false);

                        PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                        bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                        board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                        board->returnChess(x1, y1)->setEmpty(true);
                        // if king still in capture after chess move
                        if (inCapture(myKingX, myKingY, opChessList)) {
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            continue;
                        } else {
                            // restore the dest chess and move
                            board->returnChess(x2, y2)->setColor(prevDestCor);
                            board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                            board->returnChess(x1, y1)->setColor(prevStartCor);
                            board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                            board->move(x1, y1, x2, y2);
                        }
                    }
                }
            }
        }
    }
    // no legal move/saving movefor king in check, find other legal moves

    // find the first capturing/check move without potentially being captured
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < availableGrid.size(); j++) {
            x2 = availableGrid[j][0];
            y2 = availableGrid[j][1];
            if (board->canMove(x1, y1, x2, y2)) {
                // restore and change color & empty state of dest chess to stimulate move
                PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                board->returnChess(x2, y2)->setColor(board->returnChess(x1, y1)->returnColour());
                board->returnChess(x2, y2)->setEmpty(false);

                PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                board->returnChess(x1, y1)->setEmpty(true);
                // if in capture at destination
                if (inCapture(x2, y2, opChessList)) {
                    board->returnChess(x2, y2)->setColor(prevDestCor);
                    board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                    board->returnChess(x1, y1)->setColor(prevStartCor);
                    board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                    continue;
                }
                // restore the dest chess
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(x1, y1)->setColor(prevStartCor);
                board->returnChess(x1, y1)->setEmpty(prevStartEptState);

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

    // check capture-avoided move if currently in capture
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1 = myChessList[i][0];
        int y1 = myChessList[i][1];
        if (inCapture(x1, y1, opChessList)) {
            // check for a safe position
            for (size_t j = 0; j < availableGrid.size(); j++) {
                int x2 = availableGrid[j][0];
                int y2 = availableGrid[j][1];
                if (board->canMove(x1, y1, x2, y2)) {
                    // check whether in capture at destination
                    // restore and change color & empty state of dest chess to stimulate move
                    PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                    bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                    board->returnChess(x2, y2)->setColor(board->returnChess(x1, y1)->returnColour());
                    board->returnChess(x2, y2)->setEmpty(false);

                    PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                    bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                    board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                    board->returnChess(x1, y1)->setEmpty(true);
                    if (inCapture(x2, y2, opChessList)) {
                        // restore and move on to the next dest
                        board->returnChess(x2, y2)->setColor(prevDestCor);
                        board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                        board->returnChess(x1, y1)->setColor(prevStartCor);
                        board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                        continue;
                    } 
                    board->returnChess(x2, y2)->setColor(prevDestCor);
                    board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                    board->returnChess(x1, y1)->setColor(prevStartCor);
                    board->returnChess(x1, y1)->setEmpty(prevStartEptState);
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
        }
    }

    // no check or capturing move, not in capture. perform random legal & capture-avoided move
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < availableGrid.size(); j++) {
            x2 = availableGrid[j][0];
            y2 = availableGrid[j][1];
            if (board->canMove(x1, y1, x2, y2)) {
                // check whether in capture at destination
                // restore and change color & empty state of dest chess to stimulate move
                PieceColour prevDestCor = board->returnChess(x2, y2)->returnColour();
                bool prevDestEptState = board->returnChess(x2, y2)->isEmpty();
                board->returnChess(x2, y2)->setColor(board->returnChess(x1, y1)->returnColour());
                board->returnChess(x2, y2)->setEmpty(false);

                PieceColour prevStartCor = board->returnChess(x1, y1)->returnColour();
                bool prevStartEptState = board->returnChess(x1, y1)->isEmpty();
                board->returnChess(x1, y1)->setColor(PieceColour::Empty);
                board->returnChess(x1, y1)->setEmpty(true);

                if (inCapture(x2, y2, opChessList)) {
                    // restore and move on to the next dest
                    board->returnChess(x2, y2)->setColor(prevDestCor);
                    board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                    board->returnChess(x1, y1)->setColor(prevStartCor);
                    board->returnChess(x1, y1)->setEmpty(prevStartEptState);
                    continue;
                } 
                board->returnChess(x2, y2)->setColor(prevDestCor);
                board->returnChess(x2, y2)->setEmpty(prevDestEptState);
                board->returnChess(x1, y1)->setColor(prevStartCor);
                board->returnChess(x1, y1)->setEmpty(prevStartEptState);
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
        // no capturing-avoided moves for curr chess
    }

    // check random legal moves
    shuffle(myChessList.begin(), myChessList.end(), default_random_engine(seed));
    shuffle(availableGrid.begin(), availableGrid.end(), default_random_engine(seed));
    for (size_t i = 0; i < myChessList.size(); i++) {
        int x1, y1, x2, y2;
        x1 = myChessList[i][0];
        y1 = myChessList[i][1];
        for (size_t j = 0; j < availableGrid.size(); j++) {
            x2 = availableGrid[j][0];
            y2 = availableGrid[j][1];
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
    }
    // no legal moves
    return false;
}
