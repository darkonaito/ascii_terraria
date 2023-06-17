#include <iostream>
#include <vector>
#include <array>
#include <limits>
#include <time.h>
#include "dyn_arr.hpp"
#include "graphicd4-2.hpp"

using namespace std;
using namespace grphd;
using namespace grphd::console;
using namespace grphd::generic;
using namespace grphd::graphic;

unsigned short seed;
unsigned short noise;
unsigned short waitTime;
unsigned short minTreeHeight;
unsigned short maxTreeHeight;
unsigned short minTrees;
unsigned short maxTrees;
unsigned short minPlants;
unsigned short maxPlants;

constexpr uint8_t grassMaxStemHeight   {3};
constexpr uint8_t seaweedMaxStemHeight {10};

constexpr uint8_t testForFreeSpace {50};
constexpr uint8_t seaLevel         {30};
constexpr uint8_t stoneLevelEnd    {25};
constexpr uint8_t dirtLevelEnd     {stoneLevelEnd + 2};
constexpr uint8_t grassLevelEnd    {dirtLevelEnd + 1};

#include "textures.h"

Grid grid({300, 150}, airTexture);

#include "classes.h"
#include "functions.h"

Player plr({30, 100}, plrTexture);

DynArr<Tree*> trees;
DynArr<Plant> plants;


int main()
{

    // add entities to grid

    grid.addEntity(plr);

    // append entities steppable blocks

    plr.addSteppablePixel(grid.getDfltClrTxt());
    plr.addSteppablePixel(waterTexture);
    plr.addSteppablePixel(snowTexture);

    // setup

    userSettings();
    setup();

    generateTerrain(noise, minTrees, maxTrees, minPlants, maxPlants);

    vector<Sapling> saplings;

    grid.paint();

    while(Sleep(waitTime), !GetAsyncKeyState(27))
    {
        // SNOW
        for(uint16_t i {}; i < grid.getMaxXY().x ; ++i)
        {
            for(uint16_t j {}; j < grid.getMaxXY().y; ++j)
            {
                const Pixel sub_pixel {grid.getPixel({i, j - 1})};

                if(grid.getPixel({i, j}) == snowTexture)
                {
                    if(sub_pixel == grid.getDfltClrTxt())
                    {
                        grid.drawPxl({i, j}, grid.getDfltClrTxt());
                        grid.drawPxl({i, j - 1}, snowTexture);
                    } else if(sub_pixel == waterTexture)
                    {
                        grid.drawPxl({i, j}, grid.getDfltClrTxt());
                    }
                }
            }
        }

        // WATER
        for(uint16_t i {0}; i < grid.getMaxXY().x; ++i)
        {
            for(uint16_t j {0}; j < grid.getMaxXY().y; ++j)
            {
                Coords block {i, j};

                if(grid.getPixel(block) == waterTexture) // dopo non è più acqua
                {

                    if
                    (
                        Coords next_block_coords {i, j - 1}; 
                        next_block_coords.y >= 0 && 
                        grid.getPixel(next_block_coords) == airTexture 
                    ) 
                    {
                        grid.drawPxl(next_block_coords, waterTexture);
                        grid.drawPxl(block, airTexture);
                    }

                    else if
                    (
                        Coords next_block_coords {i - 1, j}; 
                        next_block_coords.x >= 0 && 
                        grid.getPixel(next_block_coords) == airTexture &&
                        (grid.getPixel(block + Coords{1, 0}) == waterTexture || grid.getPixel(block + Coords{0, 1}) == waterTexture)
                    ) 
                    {
                        grid.drawPxl(next_block_coords, waterTexture);
                        grid.drawPxl(block, airTexture);
                    }

                    else if
                    (
                        Coords next_block_coords {i + 1, j}; 
                        next_block_coords.x < grid.getMaxXY().x && 
                        grid.getPixel(next_block_coords) == airTexture &&
                        (grid.getPixel(block - Coords{1, 0}) == waterTexture || grid.getPixel(block + Coords{0, 1}) == waterTexture)
                    ) 
                    {
                        grid.drawPxl(next_block_coords, waterTexture);
                        grid.drawPxl(block, airTexture);
                    }
                }
            }
        }


        if(GetAsyncKeyState('O'))
        {
            Coords block_coords {getFirstBlock({plr.getCoords().x, 60}, 's', grassTexture)};

            if(block_coords != Coords{0, 0})
            {
                saplings.push_back(
                    {
                        block_coords,
                        {getRandBetweenEqu(0, 256), ' '},
                        {getRandBetweenEqu(0, 256), ' '},
                        {getRandBetweenEqu(0, 256), ' '}, getRandBetweenEqu(2, 7), getRandBetweenEqu(20, 300), getRandBetweenEqu(8, 80)
                    }
                );
            }
        }

        for(auto& i : saplings)
        {
            i.update();
        }
        
        // update plants

        for(uint8_t i = 0; i < plants.getLength(); ++i)
            plants[i].update();

        // update player

        plr.update();


        // update grid
        grid.update();
    }

    return 0;
}

#include "functions.cpp"
#include "classes.cpp"
