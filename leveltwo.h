#ifndef _LEVELTWO_H_
#define _LEVELTWO_H_
#include <iostream>
#include <string>
#include "computer.h"

class LevelTwo: public Computer {

 public:
    LevelTwo(string name, PieceColour colour, Board* board);
    bool makeMove() override;
};

#endif
