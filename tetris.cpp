#include "tetris.h"

using namespace tetris_engine;

//TODO: Consider using a MAtrix type
GameBoard tetris_engine::gameboard; 
Tetrimino tetris_engine::active_tetrimino;
std::queue<Tetrimino> tetris_engine::tetrimino_queue;
std::atomic_bool tetris_engine::stop_engine;

void tetris_engine::ClearGameboard(){
  for (auto& row : gameboard){
    for(auto& cell : row){
      cell = TetriminoShapes::EMPTY;
    }
  }
}

bool tetris_engine::MoveActiveDown(){
  Tetrimino next_tetrimino = active_tetrimino;
  next_tetrimino.position.y++;
 
  SetCells(active_tetrimino,  TetriminoShapes::EMPTY);
  if (IsObstructed(next_tetrimino)) {
    SetCells(active_tetrimino,  active_tetrimino.type);
    return false;
  } else {
    active_tetrimino = next_tetrimino;
    SetCells(active_tetrimino,  active_tetrimino.type);
    return true;
  }
}

void tetris_engine::SetCells(Tetrimino tetrimino, TetriminoShapes new_shape){
  std::array<Coord, 4> tetrimino_cell_positions = FindTetriminoPosition(tetrimino);

  for (auto pos : tetrimino_cell_positions){
    bool in_vertical_bounds = pos.y >= 0 && pos.y < kGameboardHeight; 
    bool in_horizontal_bounds = pos.x >= 0 && pos.x < kGameboardWidth;
    if (in_vertical_bounds && in_horizontal_bounds){
      gameboard[pos.y][pos.x] = new_shape;
    }
  }
}

bool tetris_engine::IsObstructed(Tetrimino tetrimino){
  std::array<Coord, 4> tetrimino_cell_positions = FindTetriminoPosition(tetrimino);

  for (auto position : tetrimino_cell_positions) {
    if (GetShapeAt(position) != TetriminoShapes::EMPTY){
      return true;
    }
  }
  return false;
}

std::array<Coord, 4> tetris_engine::FindTetriminoPosition(Tetrimino tetrimino){
  std::array<int, 4> internal_positions = 
    kTetriminoStates[(int)tetrimino.type][tetrimino.orientation];
  std::array<Coord, 4> gameboard_positions;

  for (int i = 0; i < 4; i++){
    gameboard_positions[i].x = tetrimino.position.x + internal_positions[i]%4;  
    gameboard_positions[i].y = tetrimino.position.y + internal_positions[i]/4; 
  } 
  return gameboard_positions;
}

TetriminoShapes tetris_engine::GetShapeAt(Coord position){
  if (position.x > kGameboardWidth || position.x < 0){
    return TetriminoShapes::OUT_OF_BOUNDS;
  } else if (position.y > kGameboardHeight){
    return TetriminoShapes::OUT_OF_BOUNDS;
  } else if (position.y < 0) {
    return TetriminoShapes::EMPTY;
  } else {
    return gameboard[position.y][position.x];
  }
}

void tetris_engine::NextTetrimino(){
  active_tetrimino = tetrimino_queue.front();
  tetrimino_queue.pop();
  tetrimino_queue.push(GenerateNewTetrimino());
}

Tetrimino tetris_engine::GenerateNewTetrimino(){
  return {
    static_cast<TetriminoShapes> (std::rand() % kNumberOfTetrimioShapes), //Type
    kTetriminoStartPosition,
    0 
  };
}

void tetris_engine::InitializeTetriminoQueue(){
  for (int i=0; i < kTetriminoQueueSize; i++){
    tetrimino_queue.push(GenerateNewTetrimino());
  } 
}

