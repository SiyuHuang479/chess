#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>	
#include <string>
#include <unistd.h>
#include <vector>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[6][10]={"white", "black", "red", "green", "blue", "grey"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 5; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // setup the count of # of pixmaps
  pixNum = 0;

  XSetWindowAttributes window_attributes;
  window_attributes.background_pixel = 0x888888; // Grey color (adjust as needed)
  XChangeWindowAttributes(d, w, CWBackPixel, &window_attributes);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSetForeground(d, gc, window_attributes.background_pixel);
  XFillRectangle(d, w, gc, 0, 0, 900, 900);
  XSetForeground(d, gc, window_attributes.background_pixel);

  XSynchronize(d,True);

  //usleep(1000);
}

Xwindow::~Xwindow() {
  for (auto it: pixs) {
    XFreePixmap(d, it);
  }
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawBoard() {
  for (int i = 1; i < 9; i++) {
    for (int j = 1; j < 9; j++) {
      if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) fillRectangle(80 * i, 80 *  j, 80, 80);
      else fillRectangle(80 * i, 80 *  j, 80, 80, 0);
    }
  }
}

// reference: https://github.com/QMonkey/Xlib-demo/blob/master/src/draw-pixmap.c
void Xwindow::drawPixmap(unsigned int x, unsigned int y, string filename) {
  /* this variable will contain the ID of the newly created pixmap.    */
  Pixmap bitmap;
  /* these variables will contain the dimensions of the loaded bitmap. */
  unsigned int bitmap_width, bitmap_height;
  /* these variables will contain the location of the hotspot of the   */
  /* loaded bitmap.                                                    */
  int hotspot_x, hotspot_y;
  XReadBitmapFile(d, w, filename.c_str(), &bitmap_width, &bitmap_height, &bitmap, 
                  &hotspot_x, &hotspot_y);
  // store the new pixmap into pixmap vector
  pixs.emplace_back(bitmap);
  pixNum++;
  XCopyPlane(d, bitmap, w, DefaultGC(d, s), hotspot_x, hotspot_y, 
              bitmap_width, bitmap_height, x, y, 1);
  XSync(d, False);
  XFlush(d);
}

void Xwindow::setUpFont(const std::string& fontname) {
    XFontStruct * font = XLoadQueryFont(d, fontname.c_str());
    if (! font ) {
        cerr << "unable to load font " << fontname << ": using fixed" << endl;
        font = XLoadQueryFont(d, "lucidasans-12");
    }
    XSetFont(d, gc, font->fid);
} // Xwindow::setUpFont

void Xwindow::drawString(int x, int y, string msg, const std::string& fontName) {
  unsigned long prevCol = 0x888888; // grey color
  XSetForeground(d, gc, Black);
  setUpFont(fontName);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
  XSetForeground(d, gc, prevCol);
}

void Xwindow::drawChessString(std::string& fontName) {
    const int cellSize = 81;

    for (int x = 0; x < 8; ++x) {
        char letter = 'a' + x;
        drawString((x + 1) * cellSize + 30, 50, string(1, letter), fontName);
    }

    for (int y = 0; y < 8; ++y) {
      char digit = '8' - y;
        drawString(30, (y + 1) * cellSize + 45, string(1, digit), fontName);
    }
}

void Xwindow::drawChess(int x, int y, string description, PieceColour c) {
    int gap = 88;

    if (c == PieceColour::Empty || description == "") return;
    if (c == PieceColour::Black) {
      if (description == "rook") {
        drawPixmap(size_t(gap +80 * y), size_t(gap +80 * x), "bitmap/black_rook.xbm");
        return;
      }
      if (description == "king") {
        drawPixmap(size_t(gap +80 * y), size_t(gap +80 * x), "bitmap/black_king.xbm");
        return;
      }
      if (description == "queen") {
        drawPixmap(size_t(gap +80 * y), size_t(gap +80 * x), "bitmap/black_queen.xbm");
        return;
      }
      if (description == "bishop") {
        drawPixmap(size_t(gap +80 * y), size_t(gap +80 * x), "bitmap/black_bishop.xbm");
        return;
      }
      if (description == "knight") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/black_knight.xbm");
        return;
      }
      if (description == "pawn") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/black_pawn.xbm");
        return;
      }
    }
    if (c == PieceColour::White) {
      if (description == "rook") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_rook.xbm");
        return;
      }
      if (description == "king") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_king.xbm");
        return;
      }
      if (description == "queen") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_queen.xbm");
        return;
      }
      if (description == "bishop") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_bishop.xbm");
        return;
      }
      if (description == "knight") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_knight.xbm");
        return;
      }
      if (description == "pawn") {
        drawPixmap(size_t(gap + 80 * y), size_t(gap + 80 * x), "bitmap/white_pawn.xbm");
        return;
      }
    }
}

void Xwindow::notify(Chess& c, int x1, int y1, int x2, int y2) {

  int gap = 80;
  // remove the old location
  if ((x1 % 2 == 0 && y1 % 2 == 0) || (x1 % 2 == 1 && y1 % 2 == 1)) {
    fillRectangle(gap + y1 * grid_size, gap + x1 * grid_size, grid_size, grid_size);
  } else {
    fillRectangle(gap + y1 * grid_size, gap + x1 * grid_size, grid_size, grid_size, 0);
  }
  // add to new location
  if ((x2 % 2 == 0 && y2 % 2 == 0) || (x2 % 2 == 1 && y2 % 2 == 1)) {
    fillRectangle(gap + y2 * grid_size, gap + x2 * grid_size, grid_size, grid_size);
  } else {
    fillRectangle(gap + y2 * grid_size, gap + x2 * grid_size, grid_size, grid_size, 0);
  }

  // add new chess
  drawChess(x2, y2, c.returnDescription(), c.returnColour());

}
