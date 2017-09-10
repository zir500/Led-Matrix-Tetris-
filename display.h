#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>
#include <signal.h>
#include <iostream>

#include "graphics.h"
#include "led-matrix.h"
#include "transformer.h"

#include "tetris.h"

namespace display {

typedef std::array<std::array<tetris_engine::TetriminoShapes, tetris_engine::kGameboardWidth>, 
        tetris_engine::kGameboardHeight> GameBoard;

// Number of panels in each chain. NB. 32x64 panels are actually made up of 2 32x32 panels.
const int kPanelChainLength = 4;

// Hardware mapping type. eg. adafruit hat.  regular is just hard wired without any interface.
const char* kHardwareMapping = "regular";

// Number of led rows in each panel.
const int kNumberOfRows = 32;

const int kBrightness = 50;

// Indicates that an interrupt signal has been received (SIGINT or SIGTERM).
volatile bool interrupted = false;

void DisplayGameboard(rgb_matrix::RGBMatrix* matrix, const GameBoard gameboarda);

void InitializeTetriminoQueue();
}

#endif //DISPLAY_H

