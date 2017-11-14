//globals.h
#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED
///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the valley
const int MAXCOLS = 20;             // max number of columns in the valley
const int MAXROBOTS = 15;           // max number of robots allowed
const int MAXSOURCES = MAXROWS*MAXCOLS; // max number of energy sources
const int FULL_ENERGY = 70;         // number of units when fully charged
const int SHARE_THRESHOLD = 35;     // will share energy if have at least this
const int SHARE_AMOUNT = 10;        // amount of energy to share
const int NORTH = 0;
const int SOUTH = 1;
const int EAST  = 2;
const int WEST  = 3;
///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declaration
///////////////////////////////////////////////////////////////////////////

void clearScreen();

#endif //GLOBALS_INCLUDED