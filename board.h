#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "observer.h"
#include "king.h"
#include "queen.h"
#include "pawn.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
#include "textdisplay.h"
#include "empty.h"
#include "window.h"
using namespace std;

class Chess;
class Xwindow;

enum class GameState { Ongoing, WhiteWins, BlackWins, Stalemate };

class Board : public Observer {
 protected:
    GameState gameState;
    vector<vector<unique_ptr<Chess>>> board;
    vector<vector<int>> lastMove;

  // fields to be implemented:
    TextDisplay* td;
    Xwindow* gd;

 public:
  Board();  // Default constructor

  void clearBoard();
  // void init();
  // return whether the location(x,y) is empty; return true if it is empty, false otherwise

  void setLastMove(vector<vector<int>> move);
  const vector<vector<int>> getLastMove() const;

  bool isEmpty(int x, int y) const; 

  bool isBlocked(int x1, int y1, int x2, int y2) const;

  bool canMove(int x1, int y1, int x2, int y2) const;

  void move (int x1, int y1, int x2, int y2);

  void notify(Chess &piece, int x1, int y1, int x2, int y2);

  PieceColour isCheck(PieceColour curColour) const;
  bool isStalemate(PieceColour curColour);
  bool isCheckmate(PieceColour curColour);

  void setGameState(GameState state);
  GameState returnGamestate() const;
  Chess* returnChess(int x, int y);

  // for board setup
  void placeChess(string piece, int x, int y);
  void removeChess(int x, int y);
  bool isLegalSetup() const;
  void attachTD(TextDisplay* td);
  void attachGD(Xwindow* gd);
};

#endif
