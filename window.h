#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "observer.h"
#include "board.h"

using namespace std;

class Observer;

class Xwindow : public Observer{
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];
  vector<Pixmap> pixs;
  size_t pixNum;
  const int grid_size = 80;

 public:
  Xwindow(int width, int height);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.

  enum {White=0, Black, Red, Green, Blue}; // Available colours.

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  void setUpFont(const std::string& fontname);

  // Draws a string
  void drawString(int x, int y, std::string msg, const std::string& fontName);

  void drawChessString(std::string& fontName);

  // Draws a pixmap
  void drawPixmap(unsigned int x, unsigned int y, string filename);

  void drawBoard();

  void drawChess(int x, int y, string description, PieceColour c);

  void notify(Chess& c, int x1, int y1, int x2, int y2) override;

};

#endif
