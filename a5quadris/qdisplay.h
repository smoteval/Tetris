/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/

// QuadrisDisplay is an Abstract class with 4 subclasses for the purpose of displaying the game

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "window.h"
#include "board.h"

class Board;


class QuadrisDisplay {
public:
  //Called to update the display
  virtual void update(Board*) = 0;
};

class TextDisplay: public QuadrisDisplay {
public:
  //Called to update the display
  void update(Board* b);
};

class FancyTextDisplay: public QuadrisDisplay {
public:
  //Called to update the display
  void update(Board* b);
};

class XDisplay: public QuadrisDisplay {
  Xwindow* a; 
  int virtualboard[boardH][boardW]; // this field tells us which parts of the board are empty and which parts are not
public:
  ~XDisplay();
  XDisplay();
  void update(Board* b);
};

class FancyXDisplay: public QuadrisDisplay {
  Xwindow* f;
  int virtualboard[boardH][boardW]; // this field tells us which parts of the board are empty and which parts are not
public:
  ~FancyXDisplay();
  FancyXDisplay();
  void update(Board* b);
};    
  

#endif
