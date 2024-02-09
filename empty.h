#ifndef _EMPTY_H_
#define _EMPTY_H_
#include "chess.h"

class Empty : public Chess {
    public:
        Empty(Board *board, PieceColour colour);
        ~Empty();

        bool isEmpty() const override; // Returns true if the current chess piece is the empty piece

        bool isLegalMove(int x1, int y1, int x2, int y2) const override;

        bool isBlocked(int x1, int y1, int x2, int y2) const override;
};

#endif
