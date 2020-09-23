#ifndef UTIL
#define UTIL

void removeSong(string targetArtist, string targetSong);
void recordSong();
Grid<double> makeConstillation(Grid<double> & spectrum);
bool testCandidate(Grid<double> & spectrum, int center_r, int center_c);
void drawGrid(Grid<double> & grid);
int getRGB(double value);
int randomColor();
void drawNotes(Grid<double> & grid);

#endif // UTIL

