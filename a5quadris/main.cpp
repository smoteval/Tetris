/*
  Group Project
  Sina Motevalli Bashi and Emilio Verdugo Paredes
  Quadris
  Due April 4th 2014
  Written March 28th 2014
*/

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "buffertoggle.h"
#include "board.h"
#include "qdisplay.h"
#include "trie.h"
using namespace std;

//Outputs a usage message to terminal
void showUsage(string name) {
  cerr << "Usage: " << name << " [OPTION]..." << endl;
  cerr << "  Plays a game of Quadris\n" << endl;
  cerr << "Options:" << endl;
  cerr << "  -charmode\t\tUses single character input" << endl;
  cerr << "  -fancygraphics\tUses fancy graphics display" << endl;
  cerr << "  -fancytext\t\tUses fancy text display" << endl;
  cerr << "  -realtime\t\tPlay game in real time (Tetris)" << endl;
  cerr << "  -scriptfile file\tUse file as script for level 0" << endl;
  cerr << "  -seed n\t\tSeed the RNG with n" << endl;
  cerr << "  -startlevel n\t\tStart at level n" << endl;
  cerr << "  -text\t\t\tText mode only" << endl;
  cerr << endl;
}

//Used to pass data to real-time graphics thread
struct RealTimeData {
  Board* b;
  QuadrisDisplay* qd;
  bool* const progress;
  RealTimeData(Board* b, QuadrisDisplay* qd, bool* progress): 
    b(b), qd(qd), progress(progress) {}
};

//Delay between screen updates in real-time mode
const unsigned int delay = 1000000;

//Function that is called in the real-time thread
void* realTimeUpdate(void* d) {
  //Parameter must be void*, so we must cast to use it
  RealTimeData* data = reinterpret_cast<RealTimeData*>(d);
  //Exit when main loop is done
  while (*(data->progress)) {
    //Delay
    usleep(delay);
    //Move current piece down
    data->b->move(2);
    //Update the screen
    data->qd->update(data->b);
  }
  //Close thread
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  //Create initial settings with default values
  string file = "sequence.txt";
  int levelNum = 0;
  srand(time(NULL));
  bool textOnly = false;
  bool fancy = false;
  bool realTime = false;
  bool charMode = false;
  bool fancyGraphics = false;
  //Process command line arguments
  for(int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
      showUsage(argv[0]);
      return 10;
      //Help command
    }
    else if (arg == "-realtime")
      realTime = true;
      //Set real time mode (overrides text mode)
    else if (arg == "-text")
      textOnly = true;
      //Set text only mode
    else if (arg == "-seed") {
      if (i + 1 < argc) {
        stringstream strint(argv[++i]);
        int seed;
        strint >> seed;
        srand(seed);
      }
      else {
        showUsage(argv[0]);
        return 1;
      }
      //Seeds RNG
    }
    else if (arg == "-scriptfile") {
      if (i + 1 < argc) {
        file = argv[++i];
      }
      else {
        showUsage(argv[0]);
        return 1;
      }
      //Set input file for level 0
    }
    else if (arg == "-startlevel") {
      if (i + 1 < argc) {
        stringstream strint(argv[++i]);
        strint >> levelNum;
      }
      else {
        showUsage(argv[0]);
        return 1;
      }
      //Set starting level
    }
    else if (arg == "-fancytext")
      fancy = true;
      //Choose fancy text display
    else if (arg == "-charmode")
      charMode = true;
      //Choose character mode
    else if (arg == "-fancygraphics")
      fancyGraphics = true;
      //Choose fancy graphics display
  }
  //Create command dictionary and insert commands
  TrieNode* dict = new TrieNode();
  dict->insert("left");
  dict->insert("right");
  dict->insert("down");
  dict->insert("clockwise");
  dict->insert("counterclockwise");
  dict->insert("drop");
  dict->insert("levelup");
  dict->insert("leveldown");
  dict->insert("restart");
  //Initialize game objects and displays
  Board game(file, levelNum);
  bool inProgress = true;
  QuadrisDisplay* text = NULL;
  QuadrisDisplay* xWindow = NULL;
  pthread_t RTupdate;
  RealTimeData* data;
  if (!realTime) { //If not real time
    //Create (fancy) text display
    if (fancy) text = new FancyTextDisplay();
    else text = new TextDisplay();
    //If not text mode...
    if (!textOnly) {
      //...create graphics display
      if (!fancyGraphics)
        xWindow = new XDisplay();
      else
        xWindow = new FancyXDisplay();
      //Window is updated twice since we had
      //some issues with the image not showing
      xWindow->update(&game);
      xWindow->update(&game);
    }
    //Initialize the game screen
    text->update(&game);
  }
  else { //If real time
    //Notify X server of threading use
    XInitThreads();
    //Create (fancy) graphics display
    if (!fancyGraphics)
      xWindow = new XDisplay();
    else
      xWindow = new FancyXDisplay();
    //Initialize the screen
    xWindow->update(&game);
    //Create data pointer for thread
    data = new RealTimeData(&game, xWindow, &inProgress);
    //Initialize display thread
    int temp = pthread_create(&RTupdate, NULL, realTimeUpdate, reinterpret_cast<void*>(data));
    if (temp) { //Check for possible failure to start thread
      cerr << "Error: Unable to create thread" << endl;
      return -1;
    }
  }
  //Initialize commands
  string command = "LOL";
  char charCommand = '!';
  BufferToggle bt;
  if (charMode) {
    //If character mode is enabled,
    //turn off input buffer
    bt.off();
    cin >> charCommand;
  }
  else
    cin >> command;
  //Main game loop
  while (inProgress && !cin.eof()) {
    //Check for repeating command
    int repeat = 0;
    bool repeated = false;
    //Get the number of repetitions if the command
    //starts with a number
    while (command[0] > 47 && command[0] < 58) {
      repeat = repeat * 10 + command[0] - '0';
      command = command.substr(1);
      repeated = true;
    }
    repeat = repeated? repeat : 1;
    //If not in character mode, autocomplete
    if (!charMode) command = dict->autocomplete(command);
    if (command != "") {
      //Carry out commands as necessary
      for(int i = 0; i < repeat; ++i) {
        if ((charMode && charCommand == 'a') || command == "left")
          game.move(0);
          //Move left
        else if ((charMode && charCommand == 'd') || command == "right")
          game.move(1);
          //Move right
        else if ((charMode && charCommand == 's') || command == "down")
          game.move(2);
          //Move down
        else if ((charMode && charCommand == 'w') || command == "drop") {
          game.move(3);
          inProgress = game.check();
          //Drop piece and check
        }
        else if ((charMode && charCommand == 'e') || command == "clockwise")
          game.rotate(1);
          //Rotate clockwise
        else if ((charMode && charCommand == 'q') || command == "counterclockwise")
          game.rotate(-1);
          //Rotate counterclockwise
        else if ((charMode && charCommand == 'c') || command == "levelup")
          game.changeLevel(1);
          //Level up
        else if ((charMode && charCommand == 'z') || command == "leveldown")
          game.changeLevel(-1);
          //Level down
        else if ((charMode && charCommand == 'r') || command == "restart")
          game.newGame(0);
          //Restart game
        else if (charMode && charCommand == 'x')
          inProgress = false;
          //Exit game
      }
    }
    //Updating the screen
    if (!realTime) { //If not real time
      //Update existant displays
      text->update(&game);
      if (!textOnly) xWindow->update(&game);
    }
    else xWindow->update(&game);
    //Obtain new input
    if (charMode)
      cin >> charCommand;
    else
      cin >> command;
  }
  //Exit and cleanup
  //Set inProgress to false to thread will stop
  inProgress = false;
  //If real time...
  if (realTime) {
    void* status; //Required for status storage
    //Join (wait for) display thread to end
    int temp = pthread_join(RTupdate, &status);
    if (temp) { //If an error occurs
      cerr << "Error: Could not join thread" << endl;
      return -1;
    }
  }
  //Delete all heap-stored elements
  delete dict;
  delete text;
  delete xWindow;
  //Delete thread data
  if (realTime) delete data;
  //Turn on input buffer if it was turned off
  if (charMode) bt.on();

  return 0;
}
