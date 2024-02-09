#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "observer.h"
#include "chess.h"

class Chess;
class Observer;
const int boardSize = 8;


class TextDisplay: public Observer {
  std::vector<std::vector<char>> theDisplay;
  
 public:
  TextDisplay();

  void notify(Chess &c, int x1, int y1, int x2, int y2) override;

  ~TextDisplay();

  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
