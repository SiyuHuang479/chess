#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <iostream>
#include <string>
#include "board.h"

using namespace std;

class Player {
    string name;
    int score; 

 protected:
    Board* board;
    PieceColour colour;

 public:
    // ctor
    Player(string name, PieceColour colour, Board* board);
    
    // accessor and mutator
    string getName() const;
    int getScore() const;
    PieceColour getColour() const;
    void setScore(int new_score);
    
    virtual bool makeMove() = 0;
    virtual ~Player();
};

#endif
