#include "knight.h"
#include "board.h"

const int boardDim = 8;

Knight::Knight(Board* board, PieceColour colour) : Chess{board, colour, false} {
      setDescription("knight");
}

bool Knight::isEmpty() const { return empty; }

bool Knight::isLegalMove(int x1, int y1, int x2, int y2) const {
    int xDist = abs(x2 - x1);
    int yDist = abs(y2 - y1);

    // check if the destination locates inside the board (8 x 8)
    if (x2 < 0 || x2 >= boardDim || y2 < 0 || y2 >= boardDim) return false;

    // check if move to (x+-2, y+-1) or (x+-1, y+-2)
    if ((xDist == 2 && yDist == 1) || (xDist == 1 && yDist == 2)) return true;
    return false;
}

bool Knight::isBlocked(int x1, int y1, int x2, int y2) const { 
    // if the destination has own side chess, blocked
    if (!board->isEmpty(x2, y2) && board->returnChess(x2, y2)->returnColour() == colour) {
        return true;
    }
    return false; 
}
