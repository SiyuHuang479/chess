#ifndef __ROOK_H__
#define __ROOK_H__
#include "chess.h"

class Board;

class Rook: public Chess {
 public:
    Rook(Board* board, PieceColour colour);
    bool isEmpty() const override;
    // bool isFirstMove() const;
    bool isLegalMove(int x1, int y1, int x2, int y2) const override;
    bool isBlocked(int x1, int y1, int x2, int y2) const override;
};

#endif
