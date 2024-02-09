#ifndef __CHESS_H__
#define __CHESS_H__
#include <iostream>
#include <vector>
#include <string>
#include "textdisplay.h"
#include "observer.h"

class Board;
enum class PieceColour { White, Black, Empty };

class Chess {
  int x;
  int y;
  std::vector<Observer*> observers;
  int numberOfMoves;

protected:
  bool empty;
  Board* board;
  PieceColour colour;
  std::string description;

 public:

  // constructor
  Chess(Board* board, PieceColour colour, bool empty);

  // getter and setter
  int getX() const; 
  int getY() const;
  void setColor(PieceColour p);
  void setCoords(int r, int c);  // Tells me my row and column number.
  void setDescription(std::string description);
  std::string returnDescription() const;
  PieceColour returnColour() const;
  void setEmpty(bool b);
  int getMoveNum() const; // returns the move number
  void addMoveNum(); // increments move num by 1

  virtual bool isEmpty() const; // Returns true if the current chess piece is the empty piece

  void attach(Observer *o);
  void detach(Observer *o);
  void notifyObservers(int x1, int y1, int x2, int y2);

  virtual bool isLegalMove(int x1, int y1, int x2, int y2) const = 0;

  virtual bool isBlocked(int x1, int y1, int x2, int y2) const = 0;

  virtual ~Chess();

};

#endif
