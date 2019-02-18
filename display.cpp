#include "display.h"

#include <thread>

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

//DEBUG
void TerminalGameboard(){
  for (auto& row : tetris_engine::gameboard){
    for (auto cell : row){
      std::cout << cell;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

display::TerminalSettingsModifier::TerminalSettingsModifier(){
  tcgetattr(STDIN_FILENO, &(this->original_tio));
  this->game_tio = this->original_tio;
  this->game_tio.c_lflag &= (~ICANON & ~ECHO);// Disable Canonical mode
  tcsetattr(STDIN_FILENO, TCSANOW, &(this->game_tio));
}


display::TerminalSettingsModifier::~TerminalSettingsModifier(){
  tcsetattr(STDIN_FILENO, TCSANOW, &this->original_tio);
}


void GameLoop(rgb_matrix::RGBMatrix &matrix) {
  const std::chrono::steady_clock::duration interval = std::chrono::seconds(1);
  std::chrono::steady_clock::time_point lastTickTime = std::chrono::steady_clock::now();
  bool exitLoop = false;

  tetris_engine::ClearGameboard();
  tetris_engine::InitializeTetriminoQueue();
  tetris_engine::NextTetrimino();
  
  while (!exitLoop) {
    std::chrono::steady_clock::time_point currentTickTime = std::chrono::steady_clock::now();
    
    if (currentTickTime - lastTickTime >= interval) {
      if (!tetris_engine::MoveActiveDown()){
        tetris_engine::NextTetrimino();
      }
      lastTickTime = currentTickTime;
    }

    // HANDLE INPUTS
    char key=getchar();// this is a blocking function :(
    if (key == 'q') {
      exitLoop = true; 
    }
    std::cin.clear();
    
    DisplayGameboard(&matrix, tetris_engine::gameboard);
  } 
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

  TerminalSettingsModifier termialSettings;

  GameLoop(*matrix);

  std::cout << "Engine Thread exited" << std::endl;

  return 0;
}

