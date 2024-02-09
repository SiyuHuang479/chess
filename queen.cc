#include "queen.h"
#include "board.h"
using namespace std;

Queen::Queen(Board* board, PieceColour colour) : Chess{board, colour, false} {
      setDescription("queen");
}


bool Queen::isLegalMove(int x1, int y1, int x2, int y2) const {
    int Xdistance = x2 - x1;
    int Ydistance = y2 - y1;

    // move vertically
    if (Xdistance == 0) {
        if (0 <= y2 && y2 < 8) return true;
        return false;
    }

    // move horizontally
    if (Ydistance == 0) {
        if (0 <= x2 && x2 < 8) return true;
        return false;
    }

    // move diagonally
    if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7) return false;
    if (Xdistance == Ydistance || Xdistance == -1 * Ydistance) return true;
    return false;
}


bool Queen::isBlocked(int x1, int y1, int x2, int y2) const {

    int eachX, eachY;

    if (x1 == x2) {
        eachX = 0;
    } else if (x1 > x2) {
        eachX = -1;
    } else {
        eachX = 1;
    }

    if (y1 == y2) {
        eachY = 0;
    } else if (y1 > y2) {
        eachY = -1;
    } else {
        eachY = 1;
    }

    x1 += eachX;
    y1 += eachY;
    while (x1 != x2 || y1 != y2) {
        if (!board->isEmpty(x1, y1)) return true;   // how do we know the board situation??
        x1 += eachX;
        y1 += eachY;
    }
    if (!board->isEmpty(x2, y2) && board->returnChess(x2,y2)->returnColour() == colour) return true;
    return false;
    
}

bool Queen::isEmpty() const { return empty; }
