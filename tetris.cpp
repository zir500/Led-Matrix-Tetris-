#include "tetris.h"

using namespace tetris_engine;
//TODO: typedef the tetriminostates table 
//specify a pos struct and replace throughout.
//specify a tetrimino struct containing type, position and orientation

void tetris_engine::Tick(){
  std::array<int, 2> position_below = 
  {active_tetrimino_position[0], active_tetrimino_position[1]+1};
  
  if (IsObstructed(active_tetrimino_type, position_below, active_tetrimino_orientation)) {
    //Lock Piece
  } else {
    SetCells(active_tetrimino_type, active_tetrimino_position, 
        active_tetrimino_orientation, TetriminoShapes::EMPTY);
    active_tetrimino_position = position_below; 
    SetCells(active_tetrimino_type, active_tetrimino_position, 
        active_tetrimino_orientation, active_tetrimino_type);
  }
}

//Sets the cells occipied by a specified tetrimino to a specified shape.
void tetris_engine::SetCells(TetriminoShapes shape, std::array<int, 2> position, 
    int orientation, TetriminoShapes new_shape){
  std::array<std::array<int, 2>, 4> tetrimino_cell_positions = 
    FindTetriminoPosition(shape, position, orientation);

  for (auto& pos : tetrimino_cell_positions){
    gameboard[pos[0]][pos[1]] = new_shape;
  }
}

//Shecks whether the cell locations of the given tetrimino are EMPTY or not.
bool tetris_engine::IsObstructed(TetriminoShapes type, std::array<int, 2>position, int orientation){
  std::array<std::array<int, 2>, 4> tetrimino_cell_positions = 
    FindTetriminoPosition(type, position, orientation);

  for (auto& position : tetrimino_cell_positions) {
    if (GetShapeAt(position) != TetriminoShapes::EMPTY){
      return true;
    }
  }
  return false;
}

// Takes a tetrimino type, position, and orientation and returns an array of
// 4 coordinates - the positions of each block in this tetrimino on the 
// gameboard.
// Range of allowed Orientations is 0 - 3 inclusive. 
std::array<std::array<int, 2>, 4> tetris_engine::FindTetriminoPosition(TetriminoShapes shape, std::array<int, 2> pos, int orientation){
  std::array<int, 4> internal_positions = kTetriminoStates[(int)shape][orientation];
  std::array<std::array<int, 2>, 4> gameboard_positions;

  for (int i = 0; i < 4; i++){
    gameboard_positions[i][0] = pos[0] + internal_positions[i]%4; // x
    gameboard_positions[i][1] = pos[1] + internal_positions[i]/4; // y
  } 
  return gameboard_positions;
}


TetriminoShapes tetris_engine::GetShapeAt(std::array<int, 2> position){
  if (position[0] >=kGameboardWidth || position[0] < 0){
    //Out of bounds.
    return TetriminoShapes::I;
  } else if (position[1] >= kGameboardHeight){
    //Out of bounds
    return TetriminoShapes::I;
  } else {
    return gameboard[position[0]][position[1]];
  }
}
