/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "qdisplay.h"
#include "board.h"

void TextDisplay::update(Board* b) {
  //Extra line for spacing
  std::cout << std::endl;
  //Write game info
  std::cout << "Level: " << std::setw(8) << b->getLevel() << std::endl;
  std::cout << "Score: " << std::setw(8) << b->getScore() << std::endl;
  std::cout << "Hi Score: " << std::setw(5) << b->getHS() << std::endl;
  //Draw the game board
  std::cout << "----------" << std::endl;
  for (int r = 0; r < boardH; ++r) {
     for (int c = 0; c < boardW; ++c)
       std::cout << b->getBlockAt(r,c);
     std::cout << std::endl;
  }
  std::cout << "----------" << std::endl;
  //Draw the next piece
  std::cout << "Next:" << std::endl;
  for(int r = 0; r < 3; ++r) {
    for(int c = 0; c < 4; ++c)
      std::cout << b->getNext(r,c);
    std::cout << std::endl;
  }
  //Write a command prompt
  std::cout << "Command: " << std::endl;
}

void FancyTextDisplay::update(Board* b) {
  //Extra line for spacing
  std::cout << std::endl;
  //Draw the board, with info on the side
  std::cout << "+----------+" << std::endl;
  for (int r = 0; r < boardH; ++r) {
    //Draw the board first
    std::cout << "|";
    for (int c = 0; c < boardW; ++c) {
      bool isShadow = false;
      for(int i = 0; i < 4; ++i) {
        if (b->getShadowPos(i,0) == r && b->getShadowPos(i,1) == c) {
          std::cout << '^';
          isShadow = true;
        }
      }
      if (!isShadow) std::cout << b->getBlockAt(r,c);
    }
    std::cout << "|";
    //Draw any of these if necessary
    if (r == 1) std::cout << "   Level: " << std::setw(8) << b->getLevel() << std::endl; //Level info
    else if (r == 2) std::cout << "   Score: " << std::setw(8) << b->getScore() << std::endl; //Score info
    else if (r == 3) std::cout << "   Hi Score: " << std::setw(5) << b->getHS() << std::endl; //High score info
    else if (r == 6) std::cout << "   Next:" << std::endl; // Next block info
    else if (r > 6 && r < 10) {
      std::cout << "   ";
      for(int c = 0; c < 4; ++c)
        std::cout << b->getNext(r - 7, c);
      std::cout << std::endl;
    }
    else std::cout << std::endl; //End the line
  }
  std::cout << "+----------+" << std::endl;
  //Write a command prompt
  std::cout << "Command: " << std::endl;
}


// The fallowing is the code for the Normal Display class

// Constructor:
XDisplay::XDisplay() {
  a = new Xwindow(400,400);   
   
  //Filling the virtudal board
  for (int i=0;i<boardH;i++) {
     for (int j=0;j<boardW;j++) { 
        virtualboard[i][j] =0;
     }
  } 
            
}


// Distructor:
XDisplay::~XDisplay() {
   delete a;
}
 
 
// Method ZFunction called Update: 
void XDisplay::update(Board* b) {

  // updating the level and score and high score:
  a->fillRectangle(100,0,200,80,0);
  int level = b->getLevel();
  int score = b->getScore();
  int hscore = b->getHS();
  std::stringstream ss1;
  std::stringstream ss2;
  std::stringstream ss3;
  ss1 << level;
  ss2 << score;
  ss3 << hscore;
  std::string lvl = ss1.str();  // level in string
  std::string scr = ss2.str();  // score in string
  std::string hscr = ss3.str(); // high score in string
  a->drawString(10,20, "Level: ");
  a->drawString(200,20,lvl);
  a->drawString(10,40, "Score: ");
  a->drawString(200,40, scr);
  a->drawString(10,60, "HighScore: ");
  a->drawString(200,60, hscr);
  
  
  
  
  // updating the board:
  char t; // stores the type of blocks(current and next)
  int sze = 10; // size of each cell
  for (int i=0;i< boardH;i++) {
     for(int j=0; j<boardW; j++) {
     t = b->getBlockAt(i,j);
     if (t != ' ') {
         if (t == 'I') a->fillRectangle(10*j+100,10*i+100,sze,sze,1);
         if (t == 'J') a->fillRectangle(10*j+100,10*i+100,sze,sze,2);
         if (t == 'L') a->fillRectangle(10*j+100,10*i+100,sze,sze,3);
         if (t == 'O') a->fillRectangle(10*j+100,10*i+100,sze,sze,4);
         if (t == 'S') a->fillRectangle(10*j+100,10*i+100,sze,sze,5);
         if (t == 'Z') a->fillRectangle(10*j+100,10*i+100,sze,sze,6);
         if (t == 'T') a->fillRectangle(10*j+100,10*i+100,sze,sze,7);
         virtualboard[i][j] = 1;
     }
     else virtualboard[i][j] = 0; // this tells us which parts of the board are empty
   }
 } 
 
 //Filling the board colour in the empty part of the board using virtualboard field
 for (int i=0;i<boardH;i++) {
    for (int j=0;j<boardW;j++) {
        if (virtualboard[i][j] == 0) a->fillRectangle(10*j+100,10*i+100,sze,sze,8);
    }
 }       
       
   
   
   // updating next in the game 
   a->drawString(40, 330, "Next Block: ");
   for (int i=0;i< 4;i++) {
     for(int j=0; j<4; j++) {
     t = b->getNext(i,j);
     if (t == 'I') a->fillRectangle(50+10*j,350+10*i,10,10,1);
     if (t == 'J') a->fillRectangle(50+10*j,350+10*i,10,10,2);
     if (t == 'L') a->fillRectangle(50+10*j,350+10*i,10,10,3);
     if (t == 'O') a->fillRectangle(50+10*j,350+10*i,10,10,4);
     if (t == 'S') a->fillRectangle(50+10*j,350+10*i,10,10,5);
     if (t == 'Z') a->fillRectangle(50+10*j,350+10*i,10,10,6);
     if (t == 'T') a->fillRectangle(50+10*j,350+10*i,10,10,7);
     if (t == ' ') a->fillRectangle(50+10*j,350+10*i,10,10,0);
     }
 }   
     
}


// The fallowing is the code for Fancy Display call:

//Constructor:
FancyXDisplay::FancyXDisplay() {
  f = new Xwindow(800,700);
  
  f->drawBigString(2,2, " ",5);

  //colouring the whole display
  f->fillRectangle(0,0,800,700,5); 
  
  //stuff
  f->drawBigString(390,130, "Level: ",2);
  f->drawBigString(390,230, "Score: ", 2);
  f->drawBigString(390,330, "HighScore: ", 2);
  f->fillRectangle(30,30,30*boardW,30*boardH);
  
  
  // for loop for filling the virtual board
  for (int i=0;i<boardH;i++) {
    for (int j=0;j<boardW;j++) {
      virtualboard[i][j]=0;
    }
  }    
}

//destructor :
FancyXDisplay::~FancyXDisplay() {
  delete f;
}  


// method function called update for FancyDisplay
void FancyXDisplay::update(Board* b) {

  // updating the level and score and high score:
  f->fillRectangle(650,80,150,350,5);
  int level = b->getLevel();
  int score = b->getScore();
  int hscore = b->getHS();
  std::stringstream ss1;
  std::stringstream ss2;
  std::stringstream ss3;
  ss1 << level;
  ss2 << score;
  ss3 << hscore;
  std::string lvl = ss1.str();  // level in string
  std::string scr = ss2.str();  // score in string
  std::string hscr = ss3.str(); // high score in string
  f->drawBigString(660,130, lvl);
  f->drawBigString(660,230, scr);
  f->drawBigString(660,330, hscr);
  
  
  
  // updating the board with shadow (shadow is white):
  char t; // stores the type of blocks(current and next)
  int sze = 30; // size of each cell
  int clmn = 30;
  int row = 30;
  for (int i=0;i< boardH;i++) {
     for(int j=0; j<boardW; j++) {
     t = b->getBlockAt(i,j);
       if (t != ' ') {
        if (t == 'I') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,8);
        if (t == 'J') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,2);
        if (t == 'L') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,3);
        if (t == 'O') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,9);
        if (t == 'S') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,4);
        if (t == 'Z') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,6);
        if (t == 'T') f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze,7);
        virtualboard[i][j] = 1;
       }
       else virtualboard[i][j] = 0;
     }
 }
   // colouring the empty parts of the board with black color
   for (int i=0;i< boardH;i++) {
     for (int j=0;j <boardW;j++) {
        if(virtualboard[i][j] == 0) {
          f->fillRectangle(sze*j+clmn,sze*i+row,sze,sze);
        }
     }
   }       
 

 // putting the shadow of the current block (using white color)           
 for (int i=0;i<4;i++) {
     int r;
     int c;
     r = b->getShadowPos(i,0);
     c = b->getShadowPos(i,1); 
     f->fillRectangle(sze*c+clmn,sze*r+row,sze,sze,0);
 }    
   
   
   // updating next in the game 
   f->drawBigString(400, 500, "Next Block: ",2);
   for (int i=0;i< 4;i++) {
     for(int j=0; j<4; j++) {
     t = b->getNext(i,j);
     int c=550;
     int r=600;
     if (t == 'I') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,8);
     if (t == 'J') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,2);
     if (t == 'L') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,3);
     if (t == 'O') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,9);
     if (t == 'S') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,4);
     if (t == 'Z') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,6);
     if (t == 'T') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,7);
     if (t == ' ') f->fillRectangle(sze*j+r,sze*i+c,sze,sze,5);
     }
   } 
     
}
