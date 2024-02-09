#ifndef _LEVELTHREE_H_
#define _LEVELTHREE_H_
#include <iostream>
#include <string>
#include "computer.h"

class LevelThree: public Computer {
    bool inCapture(int x, int y, const vector<vector<int>> opChess);
    bool checkEscape(int kingX, int kingY, const vector<vector<int>> opChess, const vector<vector<int>> grid);
 public:
    LevelThree(string name, PieceColour colour, Board* board);
    bool makeMove() override;
};

#endif
