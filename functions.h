#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void userSettings();
void setup();

void generateTerrain(int, uint8_t, uint8_t, uint8_t, uint8_t);
void growTrees(coordsCounter, coordsCounter, coordsCounter, coordsCounter, uint8_t);
void growPlants(uint8_t, uint8_t);

Coords getFirstBlock(Coords, char, Pixel);
Pixel  getFirstBlock(Coords, char, Pixel, Coords*);
Coords getFirstBlockAvailable(Coords, char, Pixel);

uint16_t getTreeIndex(coordsCounter);

Coords getAddCoords(char);



//#include "functions.cpp"
#endif // FUNCTIONS_H_INCLUDED
