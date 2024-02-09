#include "rook.h"
#include "board.h"
using namespace std;

const int boardDim = 8;

Rook::Rook(Board* board, PieceColour colour) : Chess{board, colour, false} {
      setDescription("rook");
}

bool Rook::isEmpty() const { return empty; }

// bool Rook::isFirstMove() const { return firstMove; }

bool Rook::isLegalMove(int x1, int y1, int x2, int y2) const {
    int xDist = x2 - x1;
    int yDist = y2 - y1;

    // check if the destination locates inside the board (8 x 8)
    if (x2 < 0 || x2 > boardDim - 1 || y2 < 0 || y2 > boardDim - 1) return false;

    // check if move horizontally/vertically
    if (((xDist == 0) && (yDist != 0)) || ((xDist != 0) && (yDist == 0))) return true;

    return false;
}

bool Rook::isBlocked(int x1, int y1, int x2, int y2) const {
    int xUnitMove, yUnitMove;

    if (x1 == x2) xUnitMove = 0;
    else if (x2 - x1 < 0) xUnitMove = -1;
    else xUnitMove = 1;

    if (y1 == y2) yUnitMove = 0;
    else if (y2 - y1 < 0) yUnitMove = -1;
    else yUnitMove = 1;

    x1 += xUnitMove;
    y1 += yUnitMove;
    while ((x1 != x2) || (y1 != y2)) {
        // check if blocked along the path other than the final desitination
        if (!board->isEmpty(x1, y1)) return true;
        x1 += xUnitMove;
        y1 += yUnitMove;
    }
    // if the destination has own side chess, cannot capture, blocked
    if ((!board->isEmpty(x2, y2)) && (board->returnChess(x2, y2)->returnColour() == colour)) {
        return true;
    }
    return false;
}
