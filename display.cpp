#include "display.h"

#include <thread>
#include "readerwriterqueue/readerwriterqueue.h"

using namespace display;

moodycamel::ReaderWriterQueue<char> inputQueue(10);
bool stopInputHandler = false;

static void InterruptHandler(int signo){
  interrupted = true;
}

void SetScaledPixels(rgb_matrix::RGBMatrix &matrix, int scale, int column, int row, std::array<int, 3> color) {
  for (int i = 0; i < scale; ++i) {
    for (int j = 0; j < scale; ++j) {
      matrix.SetPixel(column*scale+i, row*scale+j, color[0], color[1], color[2]);
    }
  }
}

void display::DisplayGameboard(rgb_matrix::RGBMatrix* matrix, const GameBoard gameboard){
  for (int row=0; row < gameboard.size(); row++) {
    for (int column = 0; column < gameboard[row].size(); column++) {
      std::array<int, 3> color = tetris_engine::kTetriminoColors[tetris_engine::GetShapeAt({column, row})];
        SetScaledPixels(*matrix, 3, column, row, color);
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


void InputHandlerThread() {
  while (!stopInputHandler) {
    char key = getchar();
    inputQueue.try_enqueue(key);
  }
}

void GameLoop(rgb_matrix::RGBMatrix &matrix) {
  const std::chrono::steady_clock::duration interval = std::chrono::seconds(1);
  std::chrono::steady_clock::time_point lastTickTime = std::chrono::steady_clock::now();
  bool exitLoop = false;

  std::thread inputThread(InputHandlerThread);

  tetris_engine::ClearGameboard();
  tetris_engine::InitializeTetriminoQueue();
  tetris_engine::NextTetrimino();
  
  while (!exitLoop) {
    std::chrono::steady_clock::time_point currentTickTime = std::chrono::steady_clock::now();
    
    if (currentTickTime - lastTickTime >= interval) {
      if (!tetris_engine::MoveActiveDown()){
        tetris_engine::ClearFullRows(); 
        if (!tetris_engine::NextTetrimino()) {
          // Game Over;
          std::cout << "Game Over" << std::endl;
        }
      }
      lastTickTime = currentTickTime;
    }

    // Handle Inputs
    char key = '\0';
    while (inputQueue.try_dequeue(key)) {
      switch (key) { 
        case 'q':
          exitLoop = true; 
          break;

        case 's':
          tetris_engine::MoveActiveDown();
          break;

        case 'a':
          tetris_engine::MoveActiveLeft();
          break;

        case 'd':
          tetris_engine::MoveActiveRight();
          break;

        case 'w':
          tetris_engine::RotateClockwise();
          break;
      }
    }
    

    DisplayGameboard(&matrix, tetris_engine::gameboard);
  } 

  stopInputHandler = true; 
  std::cout << "Press any key to quit" << std::endl; //Bodge to allow InputHanlder thread to exit (getchar blocks)
  inputThread.join();
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

