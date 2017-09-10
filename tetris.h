#ifndef TETRIS_H
#define TETRIS_H

#include <array>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include <unistd.h>

namespace tetris_engine {

// The index of the active cells for each orientation of each shape of tetrimino.
// sctive cells are defined in a 4x4 grid with each cell identified by a number 0-15 which
// increases going left to right and wraps around to the next line.
//  -----------
// | 0| 1| 2| 3|
// | 4| 5| 6| 7|
// | 8| 9|10|11|
// |12|13|14|15|
//  -----------
//
// Orientation 0 will be the state blocks enter the board in.
// Increasing orientation index will rotate clockwise.
// See TetriminoReference.txt for the derivation of positions for all tetrominos.

// PROT() Protects the commas in array literals (Otherwise the CPP treats them as argument seperators)
#define PROT(...) __VA_ARGS__ 
#define TETRIMINOS_DEF \
  TETRIMINO_DEF(I, PROT({8, 9, 10, 11}),PROT({1, 5, 9, 13}), PROT({4, 5, 6, 7}),  PROT({2, 6, 10, 14}), PROT({85, 22, 255})), \
  TETRIMINO_DEF(O, PROT({5, 6, 9, 10}), PROT({5, 6, 9, 10}), PROT({5, 6, 9, 10}), PROT({5, 6, 9, 10}),  PROT({255, 255, 0})), \
  TETRIMINO_DEF(T, PROT({1, 4, 5, 6}),  PROT({1, 5, 6, 9}),  PROT({4, 5, 6, 9}),  PROT({1, 4, 5, 9}),   PROT({140, 0, 255})), \
  TETRIMINO_DEF(S, PROT({1, 2, 4, 5}),  PROT({1, 5, 6, 10}), PROT({5, 6, 8, 9}),  PROT({0, 4, 5, 9}),   PROT({90, 214, 80})), \
  TETRIMINO_DEF(Z, PROT({0, 1, 5, 6}),  PROT({2, 5, 6, 9}),  PROT({4, 5, 9, 10}), PROT({1, 4, 5, 8}),   PROT({255, 0, 0})), \
  TETRIMINO_DEF(J, PROT({0, 4, 5, 6}),  PROT({1, 2, 5, 9}),  PROT({4, 5, 6, 10}), PROT({1, 5, 8, 9}),   PROT({0, 0, 255})), \
  TETRIMINO_DEF(L, PROT({2, 4, 5, 6}),  PROT({1, 5, 9, 10}), PROT({4, 5, 6, 8}),  PROT({0, 1, 5, 9}),   PROT({255, 128, 0}))

#define TETRIMINO_DEF(identifier, state0, state1, state2, state3, color) identifier
enum TetriminoShapes {TETRIMINOS_DEF, EMPTY, OUT_OF_BOUNDS};
#undef TETRIMINO_DEF 

#define TETRIMINO_DEF(identifier, state0, state1, state2, state3, color) \
  {{{state0}, {state1}, {state2}, {state3}}}
static const std::array<std::array<std::array<int, 4>, 4>, 7> 
  kTetriminoStates = {{TETRIMINOS_DEF}};
#undef TETRIMINO_DEF

#define TETRIMINO_DEF(identifier, s0, s1, s2, s3, color) {color}
static const std::array<std::array<int, 3>, 9> kTetriminoColors = {{TETRIMINOS_DEF, {{0, 255, 0}}, {{255,255,255}}}};
#undef TETRIMINO_DEF
#undef TETRIMINOS_DEF
#undef PROT


struct Coord {
  int x;
  int y;
};

// Contains the description of a tetromino.  This will be used for keeping track of the
// active tetromino as it moves and rotates.
struct Tetrimino {
  TetriminoShapes type;
  Coord position;
  int orientation;
};

const int kGameboardWidth = 10;
const int kGameboardHeight = 20;
const int kNumberOfTetrimioShapes = 7; // Number of actual types of shape.
const Coord kTetriminoStartPosition = {3, 0};
const int kTetriminoQueueSize = 5;

typedef std::array<std::array<TetriminoShapes, kGameboardWidth>, kGameboardHeight> GameBoard;

// Stores details of what type of block is at each position on the gameboard.
extern GameBoard gameboard;

// The tetromino which is being controlled by the player currently.
extern Tetrimino active_tetrimino;

// Queue of the next teriminos to be placed on the board.
extern std::queue<Tetrimino> tetrimino_queue;

//Fills the gameboard with EMPTY.
void ClearGameboard ();

// Tries to move the active tetrimino down by 1 cell on the gameboard.
// Updates the gameboard and the active tetromino's position.
// Returns true if the active tetrimino has been moved down.
// Returns false if the active tetrimino has not been moved down due to an obstruction.
bool MoveActiveDown ();

// Sets the cells occupied by the specified tetromino to the specified type.
void SetCells (Tetrimino tetrimino, TetriminoShapes new_shape);

// Checks whether the cells in the of the tetrimino specified already contain blocks. 
// TODO: the active tetrimino will intefere with itself!
bool IsObstructed (Tetrimino tetrimino);

// Returns an array of Coordinates whoch correspond to the position of the tetrimino specified.
// IE it transforms the local-frame positions of the tetrimino block to the gameboard-frame.
std::array<Coord, 4> FindTetriminoPosition (Tetrimino tetrimino);

// Returns the type of block found at the given position.
// Returns TetriminoShapes::OUT_OF_BOUNDS if the speified position is outside the gameboard.
TetriminoShapes GetShapeAt (Coord position);

// Sets the active tetrimino to the next in the Queue and generates 
// a new terimino to go at the end of the queue.
void NextTetrimino();

// Generates a new random tetrimino at the default start location in state 0
Tetrimino GenerateNewTetrimino();

// Generates the first 'kTettriminoQueueSize' tetriminos for the queue and inserts them.
void InitializeTetriminoQueue();
}

#endif //TETRIS_H

