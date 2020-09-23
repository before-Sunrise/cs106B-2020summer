#include "vector.h"
#include "grid.h"

Vector<double> getMicrophoneSample(int seconds);

void playSample(Vector<double> input);

Grid<double> getSpectrogram(Vector<double> input);
