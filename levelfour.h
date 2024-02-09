#ifndef _LEVELFOUR_H_
#define _LEVELFOUR_H_
#include <iostream>
#include <string>
#include <map>
#include "computer.h"

class LevelFour: public Computer {
    map<string, int> weightedMove;
    bool inCapture(int x, int y, const vector<vector<int>> opChess);
    bool checkEscape(int kingX, int kingY, const vector<vector<int>> opChess, const vector<vector<int>> grid);
    void addScore(int x1, int y1, int x2, int y2, map<vector<int>, int>& moveScore, bool byStartPos);
    void deductScore(int x1, int y1, int x2, int y2, map<vector<int>, int>& moveScore);
 public:
    LevelFour(string name, PieceColour colour, Board* board);
    bool makeMove() override;
};

#endif
