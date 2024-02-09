#ifndef __QUEEN_H__
#define __QUEEN_H__
#include "chess.h"

class Board;

class Queen : public Chess {

  public:
    Queen(Board* board, PieceColour colour);
    bool isLegalMove(int x1, int y1, int x2, int y2) const override;
    bool isBlocked(int x1, int y1, int x2, int y2) const override;
    bool isEmpty() const override;
};



#endif 
