
namespace tetris_engine{
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
// Shapes will be centered in the position square and rounded 
// to the left/upper cell where needed.
// 
// For example, the L tetrimino would be:
// __| - tetriminostates[tetriminoshapes.L][0] = {2, 4, 5, 6}
// L  - tetriminostates[tetriminoshapes.L][1] = {1, 5, 9, 10}
// |¯ - tetriminostates[tetriminoshapes.L][2] = {4, 5, 6, 8}
// ¯| - tetriminostates[tetriminoshapes.L][3] = {0, 1, 5, 9}
//
// See TetriminoReference.txt for details on all tetriminos.

// ST() Protects the commas in array literals (Otherwise the CPP treats them as argument seperators)
#define ST(...) __VA_ARGS__ 
#define TETRIMINOS_DEF \
  TETRIMINO_DEF(I, ST({8, 9, 10, 11}),ST({1, 5, 9, 13}), ST({4, 5, 6, 7}),  ST({2, 6, 10, 14})), \
  TETRIMINO_DEF(O, ST({5, 6, 9, 10}), ST({5, 6, 9, 10}), ST({5, 6, 9, 10}), ST({5, 6, 9, 10})), \
  TETRIMINO_DEF(T, ST({1, 4, 5, 6}),  ST({1, 5, 6, 9}),  ST({4, 5, 6, 9}),  ST({1, 4, 5, 9})), \
  TETRIMINO_DEF(S, ST({1, 2, 4, 5}),  ST({1, 5, 6, 10}), ST({5, 6, 8, 9}),  ST({0, 4, 5, 9})), \
  TETRIMINO_DEF(Z, ST({0, 1, 5, 6}),  ST({2, 5, 6, 9}),  ST({4, 5, 9, 10}), ST({1, 4, 5, 8})), \
  TETRIMINO_DEF(J, ST({0, 4, 5, 6}),  ST({1, 2, 5, 9}),  ST({4, 5, 6, 10}), ST({1, 5, 8, 9})), \
  TETRIMINO_DEF(L, ST({2, 4, 5, 6}),  ST({1, 5, 9, 10}), ST({4, 5, 6, 8}),  ST({0, 1, 5, 9}))

#define TETRIMINO_DEF(identifier, state0, state1, state2, state3) identifier
enum TetriminoShapes {TETRIMINOS_DEF};
#undef TETRIMINO_DEF 

#define TETRIMINO_DEF(identifier, state0, state1, state2, state3) \
    {state0, state1, state2, state3}
static const int kTetriminoStates[7][4][4] = {TETRIMINOS_DEF};
#undef TETRIMINO_DEF
#undef TETRIMINOS_DEF
#undef S

//Pointer to the tetrimino which occupies the space at that index. [down][left]
enum TetriminoShapes gameboard[10][20]; 
}
