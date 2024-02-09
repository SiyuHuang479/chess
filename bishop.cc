#include <string>
#include <iostream>
#include "bishop.h"
#include "board.h"
#include <vector>
using namespace std;

Bishop::Bishop(Board* board, PieceColour colour) : Chess{board, colour, false} {
    setDescription("bishop");
}

bool Bishop::isEmpty() const { return empty; }

bool Bishop::isLegalMove(int x1, int y1, int x2, int y2) const {
    if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0) return false; // moved bounds checking
    vector<vector<int>> legalPos; // create an array of legal positions for the bishop
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (abs(x - x1) == abs(y - y1) && !(x == x1 && y == y1)) { // diagonal to current position
                legalPos.push_back({x, y});
            }
        }
    }

    for (auto pos : legalPos) {
        if (x2 == pos[0] && y2 == pos[1]) {
            return true;
        }
    }
    return false;
}

bool Bishop::isBlocked(int x1, int y1, int x2, int y2) const {
    // check along the way of the array of legal positions up until (x2,y2) if the board is empty at that location
    int directionX = x2 > x1 ? 1 : -1;
    int directionY = y2 > y1 ? 1 : -1;

    int x = x1 + directionX;
    int y = y1 + directionY;
    
    while (x != x2 && y != y2) {
        if (!board->isEmpty(x, y)) {
            return true;  // Bishop is blocked if the square is not empty
        }
        x += directionX;
        y += directionY;
    }

    // Check the final position (x2, y2)
    if (!board->isEmpty(x2, y2) && board->returnChess(x2, y2)->returnColour() == colour) {
        return true;  // Bishop is blocked if the destination square contains piece of own colour
    }
    
    return false;
}
