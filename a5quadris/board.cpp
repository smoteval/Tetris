/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/

#include "board.h"
#include "level.h"

//Constructor
Board::Board(std::string file, int levelNum): highScore(100) {
  //The following arrays indicate the probabilities
  //for generating blocks in each level. Each array
  //has the probabilities for block I, J, L, O, S,
  //T, and Z, in that order
  int probs1[7] = {2, 2, 2, 2, 1, 2, 1};
  int probs2[7] = {1, 1, 1, 1, 1, 1, 1};
  int probs3[7] = {1, 1, 1, 1, 2, 1, 2};
  lvls[0] = new Level(probs1, file);
  lvls[1] = new Level(probs1, "");
  lvls[2] = new Level(probs2, "");
  lvls[3] = new Level(probs3, "");
  //Set all cells to empty
  for(int r = 0; r < boardH; ++r) {
    for(int c = 0; c < boardW; ++c)
      theBoard[r][c] = NULL;
  }
  //Start a new game
  newGame(levelNum);
}

//Setting a new game
void Board::newGame(int levelNum) {
  //The argument indicates the level to start at
  currentLevel = levelNum;
  //Clear the board of all previous blocks
  for(int i = 0; i < boardH; ++i) {
    eraseRow(i, false);
  }
  //Generate the current and next blocks
  current = (this->lvls[currentLevel])->createBlock();
  current->level = currentLevel;
  next = (this->lvls[currentLevel])->createBlock();
  //Copy the orientation of the current block 
  //into the Board's buffer
  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      currentShape[i][j] = current->shape[i][j];
      theBoard[i + 3][j] = (current->shape[i][j]) ? current : NULL;
    }
  }
  //Copy current block position into curr array
  //This is used in the rotate and move methods
  int n=0;
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if (currentShape[i][j] == 1) {
        curr[n][0] =i + 3;
        curr[n][1] =j;
        n = n+1;
      }
    }
  }
  //Initialize the shadow
  int shadowH = 20;
  for(int i = 0; i < 4; ++i) {
    int temp = boardH - curr[i][0] - 1;
    shadowH = (temp < shadowH) ? temp : shadowH;
  }
  for(int i = 0; i < 4; ++i) {
    shadow[i][0] = curr[i][0] + shadowH;
    shadow[i][1] = curr[i][1];
  }
  //Set score to 0
  score = 0;
}

//Destructor
Board::~Board() {
  //Cleat the board (avoids memory leaks)
  for(int i = 0; i < boardH; ++i) {
    eraseRow(i, false);
  }
  //Delete the next block
  delete next;
  //Delete the level objects
  for(int i = 0; i < maxLevel; ++i)
    delete lvls[i];
}

//Changes current level based on input
//Positive number is 1 up, and vice versa
void Board::changeLevel(int n) {
  if (n > 0 && currentLevel < maxLevel - 1)
    ++currentLevel;
  else if (n < 0 && currentLevel > 0)
    --currentLevel;
}

//Obtain type of block at a cell
char Board::getBlockAt (int r, int c) {
  if (theBoard[r][c])
    return (theBoard[r][c])->type;
  return ' ';
}

//Get the coordinates of the i-th shadow block
//Coordinate 0 is row, and 1 is column
int Board::getShadowPos(int i, int c) {return shadow[i][c];}

//Update the position of the shadow blocks
void Board::updateShadow() {
  //Find the shortest distance from a current block to
  //the bottom of the board or an occupied cell
  int shadowH = 20;
  for(int i = 0; i < 4; ++i) {
    int temp = 0;
    while (curr[i][0] + temp < boardH &&
           (theBoard[curr[i][0] + temp][curr[i][1]] == NULL
            || theBoard[curr[i][0] + temp][curr[i][1]] == current))
      ++temp;
    shadowH = (temp < shadowH) ? temp : shadowH;
  }
  --shadowH;
  //Update shadow block positions by shifting down the
  //current block by the amount obtained above
  for(int i = 0; i < 4; ++i) {
    shadow[i][0] = curr[i][0] + shadowH;
    shadow[i][1] = curr[i][1];
  }
}

//Rotates the current block in the direction indicated
// by n: 1 is clockwise, -1 is counter-clockwise
void Board::rotate(int n) {
  //Find the lower left corner
  int u = boardH, v = boardW, U = 0, V = 0;
  for(int i = 0; i < 4; ++i) {
    if (curr[i][0] < u) u = curr[i][0];
    if (curr[i][0] > U) U = curr[i][0];
    if (curr[i][1] < v) v = curr[i][1];
    if (curr[i][1] > V) V = curr[i][1];
  }
  //Calculate coordinates of rotated block
  //and store them in temporary variable
  //(hooray for linear algebra to simplify
  //the calculations below!)
  int temp[4][2];
  for(int i = 0; i < 4; ++i) {
    if(n == 1) {
      temp[i][0] = curr[i][1] + U - V;
      temp[i][1] = U + v - curr[i][0];
    }
    else if (n == -1) {
      temp[i][0] = U + v - curr[i][1];
      temp[i][1] = curr[i][0] + v - u;
    }
  }
  //Check that the rotated coordinates
  //do not conflict with other blocks or the
  //edge of the board, and exit if there
  //are any conflicts
  for(int i = 0; i < 4; ++i) {
    if (temp[i][0] < 0 ||
        temp[i][0] >= boardH ||
        temp[i][1] < 0 ||
        temp[i][1] >= boardW ||
        (theBoard[temp[i][0]][temp[i][1]] != 0 &&
         theBoard[temp[i][0]][temp[i][1]] != current))
      return;
  }
  //If there are no conflicts, erase current block
  //from board, and copy in new position, updating
  //the board's buffer
  for(int i = 0; i < 4; ++i) {
    theBoard[curr[i][0]][curr[i][1]] = 0;
  }
  for(int i = 0; i < 4; ++i) {
    curr[i][0] = temp[i][0];
    curr[i][1] = temp[i][1];
    theBoard[curr[i][0]][curr[i][1]] = current;
  }
  //Update the shadow
  updateShadow();
}

// Move method for moving left, right, down and drop
void Board::move(int dir) {
  bool res = true; // this boolean flag will help me know if it is possible to make the move or not
  //moving left:
  if (dir == 0) {
    for (int i=0;i<4;i++) {
      if (curr[i][1]-1 <0 || curr[i][1]-1 > boardW-1) res = false; // res = false implies the move is impossible
    }
    for (int i=0;i<4 && res;i++) {
      if (theBoard[curr[i][0]][curr[i][1]-1] != current && theBoard[curr[i][0]][curr[i][1]-1] != 0) res = false;
    }
    // now if move is possible the fallowing for loops will make the move
    if (res) {
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]][curr[i][1]] = NULL;
      }
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]][curr[i][1]-1] = current;
      }
      for (int i=0;i<4;i++) {
        curr[i][1] = curr[i][1]-1;
      }
    }
  }

  // moving right: (same method of implementation as left)
  if (dir == 1) {
    for (int i=0;i<4;i++) {
      if (curr[i][1]+1 <0 || curr[i][1]+1 > boardW-1) res = false;
    }
    for (int i=0;i<4 && res;i++) {
      if (theBoard[curr[i][0]][curr[i][1]+1] != current && theBoard[curr[i][0]][curr[i][1]+1] != 0) res = false;
    }
    if (res) {
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]][curr[i][1]] = NULL;
      }
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]][curr[i][1]+1] = current;
      }
      for (int i=0;i<4;i++) {
        curr[i][1] = curr[i][1]+1;
      }
    }
  }

  // moving down: (same method of implementation as left)
  if (dir == 2) {
    for (int i=0;i<4;i++) {
      if (curr[i][0]+1 <0 || curr[i][0] > boardH-2) res = false;
    }
    for (int i=0;i<4 && res;i++) {
      if (theBoard[curr[i][0]+1][curr[i][1]] != current && theBoard[curr[i][0]+1][curr[i][1]] != 0) res = false;
    }
    if (res) {
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]][curr[i][1]] = NULL;
      }
      for (int i=0;i<4;i++) {
        theBoard[curr[i][0]+1][curr[i][1]] = current;
      }
      for (int i=0;i<4;i++) {
        curr[i][0] = curr[i][0]+1;
      }
    }
  }

  // dropping
  if (dir == 3) {
    int n = 0;  //this counts how many cells can we go down without hitting a new block
    // the fallowing while loop will determine where the block can land
    while (res) {
      n = n+1;
      for (int i=0;i<4;i++) {
        if (theBoard[curr[i][0]+n][curr[i][1]] != current && theBoard[curr[i][0]+n][curr[i][1]] != 0) res = false;
        if (curr[i][0]+n > boardH-1) res = false;
      }
    }

    // the fallowing for loops will land the block according to the previous while loop
    for (int i=0;i<4;i++) {
      theBoard[curr[i][0]][curr[i][1]] = NULL;
    } 
    for (int i=0;i<4;i++) {
      theBoard[curr[i][0]+n-1][curr[i][1]] = current;
    }
    for (int i=0;i<4;i++) {
      curr[i][0] = curr[i][0]+n-1;
    }
  }
  updateShadow();
}

//Erases a cell, ensuring that the corresponding Block
//is deleted if it is no longer on the board
void Board::eraseCell(int r,int c, bool scoring) {
  Block* temp = theBoard[r][c];
  //Reduce the count of the block
  --(temp->count);
  //Erase cell from board
  theBoard[r][c] = 0;
  //If the block is completely off the board, score and delete
  if (temp->count <= 0) {
    if (scoring) score += (1 + temp->level) * (1 + temp->level);
    delete temp;
  }
}

//Erases a row, ensuring that all fully removed Blocks
//are deleted
void Board::eraseRow(int r, bool scoring) {
  //Iterate through row, deleting as necessary
  for (int c = 0; c < boardW; ++c) {
    if (theBoard[r][c]) {
      eraseCell(r, c, scoring);
    }
  }
}

//Shifts down a row, deleting blocks as necessary
void Board::shiftDown(int i) {
  //Erase the given row
  eraseRow(i, true);
  //Shift values above the row down
  for (int r = i; r >= 0; --r) {
    for (int c = 0; c < boardW; ++c)
      theBoard[r][c] = (r > 0) ? theBoard[r - 1][c] : 0;
  }
}

//Checks whether a block would fit in the upper right corner
bool Board::blockFits(Block* b) {
  for (int i = 0; i < 4; ++i) {
    if(theBoard[curr[i][0]][curr[i][1]] != 0)
      return false;
  }
  return true;
}

//Returns current level
int Board::getLevel() {return currentLevel;}

//Returns current score
int Board::getScore() {return score;}

//Returns current high score
int Board::getHS() {return highScore;}

//Checks for full rows, deletes blocks if necessary,
//adds points to the score, attempts to generate a
//new block, and returns whether the game has been
//lost or not (true = not lost yet)
bool Board::check() {
  //Erase rows as necessary, while keeping track
  //of the number of rows erased
  int rowsErased = 0;
  for (int r = 0; r < boardH; ++r) {
    bool fullRow = true;
    for (int c = 0; c < boardW && fullRow; ++c)
      fullRow = fullRow && (theBoard[r][c] != 0);
    if (fullRow) {
      shiftDown(r);
      ++rowsErased;
    }
  }
  //If any rows were erased, score them
  if(rowsErased > 0) 
    score += (currentLevel + rowsErased) * (currentLevel + rowsErased);
  //Update high score if necessary
  if (score > highScore)
    highScore = score;
  //Generate new blocks
  current = next;
  current->level = currentLevel;
  next = (lvls[currentLevel])->createBlock();
  //Copy new blocks position
  int n=0;
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if (current->shape[i][j] == 1) {
        curr[n][0] = i + 3;
        curr[n][1] = j;
        n = n+1;
      }
    }
  }
  //Check that the block fits, and signal exit if not
  if (!blockFits(current)) {
    delete current;
    return false;
  }
  //Copy new block onto board
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      if (current->shape[r][c] == 1) {
        theBoard[r + 3][c] = current;
        currentShape[r][c] = current->shape[r][c];
      }
    }
  }
  //Update shadow
  updateShadow();

  return true;
}

//Gives information of the next block
char Board::getNext(int r, int c) {
  return (next->shape[r][c]) ? next->type : ' ';
}

//Outputs board, for debugging
//std::ostream &operator<<(std::ostream &out, Board &b) {
//  out << "+----------+" << std::endl;
//  for (int r = 0; r < boardH; ++r) {
//     out << "|";
//     for (int c = 0; c < boardW; ++c)
//       out << b.getBlockAt(r,c);
//     out << "|" << std::endl;
//  }
//  out << "+----------+" << std::endl;
//
//  return out;
//}
