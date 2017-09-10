#include "display.h"

using namespace display;

static void InterruptHandler(int signo){
  interrupted = true;
}


void display::DisplayGameboard(rgb_matrix::RGBMatrix* matrix, const GameBoard gameboard){
  for (int row=0; row < gameboard.size(); row++) {
    for (int column = 0; column < gameboard[row].size(); column++) {
      std::array<int, 3> color = tetris_engine::kTetriminoColors[tetris_engine::GetShapeAt({column, row})];
      matrix->SetPixel(column, row, color[0], color[1], color[2]); 
    }
  }
}

void TerminalGameboard(){
  for (auto& row : tetris_engine::gameboard){
    for (auto cell : row){
      std::cout << cell;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int main(){
  // Set some defaults
  rgb_matrix::RGBMatrix::Options display_settings;
  display_settings.hardware_mapping = kHardwareMapping; 
  display_settings.chain_length = kPanelChainLength;
  display_settings.rows = kNumberOfRows;

  rgb_matrix::RuntimeOptions runtime_settings;
  runtime_settings.drop_privileges = 1; //Drops privilege after initialising GPIO usage.
  std::unique_ptr<rgb_matrix::RGBMatrix> matrix(
      rgb_matrix::CreateMatrixFromOptions( display_settings, runtime_settings));
  if (!matrix) {
    return 1;
  }

  matrix->SetBrightness(kBrightness);
  matrix->ApplyStaticTransformer(rgb_matrix::UArrangementTransformer());

  rgb_matrix::Color color(100, 100, 100);

  tetris_engine::ClearGameboard();
	
  tetris_engine::InitializeTetriminoQueue();
  tetris_engine::NextTetrimino();

  while (!interrupted){
    TerminalGameboard();
    if (!tetris_engine::MoveActiveDown()){
      //Generate a new Active tetrimino.
      tetris_engine::NextTetrimino();
    }
    DisplayGameboard(matrix.get(), tetris_engine::gameboard);
    sleep(1);   
  }

  matrix->Clear();

  return 0;
}
