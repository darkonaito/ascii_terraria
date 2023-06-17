// functions

void userSettings()
{
    system("title pre alpha ... esc - quit, space - break block, wasd - move/jump");

    char choice;

    Console::setOutptClr(colors::BlackFore | colors::WhiteBack);

    system("mode 70, 20");

    cout << "\n\tRandom? [y/n]: ";
    cin >> choice;

    if(choice == 'n')
    {
        Console::clear();

        cout << "\n\tEnter Seed : ";
        cin >> seed;

        cout << "\n\t Enter Noise Level [1]: ";
        cin >> noise;

        cout << "\n\t  Enter Milliseconds to wait for Each Frame [25]: ";
        cin >> waitTime;

        cout << "\n\t   Enter Min and Max Tree Height: ";
        cin >> minTreeHeight;
        cin >> maxTreeHeight;

        cout << "\n\t    Enter Min And Max Distance between Trees: ";
        cin >> minTrees;
        cin >> maxTrees;

        cout << "\n\t    Enter Min And Max Distance between Plants: ";
        cin >> minPlants;
        cin >> maxPlants;
    } else
    {
        seed  = time(nullptr);
        noise = 1;
        waitTime      = 25;
        minTreeHeight = 55;
        maxTreeHeight = 66;
        minTrees      = 5;
        maxTrees      = 10;
        minPlants     = 3;
        maxPlants     = 7;
    }
}

void setup()
{
    srand(seed);
    system("mode 300, 150");
    grphd::console::Console::setCursorInfo(1, false);
}

Coords getAddCoords(char dir)
{
    switch(dir)
    {
        case 'w':
        case 'W':
           return Coords{0, 1};

        case 'a':
        case 'A':
            return Coords{-1, 0};

        case 's':
        case 'S':
            return Coords{0, -1};

        case 'd':
        case 'D':
            return Coords{1, 0};

        default:
            return Coords{};
    }
}

Coords getFirstBlock(Coords c, char dir, Pixel block)
{
    Coords addCoords = getAddCoords(dir);

    while(grid.getPixel(c += addCoords) != block)
    {
        if(c.oneAxisGrtrEqu(grid.getMaxXY()) || c.oneAxisLesser({0, 0})) return {0, 0};
    }

    return c;
}

Pixel getFirstBlock(Coords c, char dir, Pixel skipBlock, Coords* blockCoords)
{
    Coords addCoords = getAddCoords(dir);
    Pixel  nextBlock;

    while((nextBlock = grid.getPixel(c += addCoords)) == skipBlock);

    if(blockCoords != nullptr) *blockCoords = c;

    return nextBlock;
}

Coords getFirstBlockAvailable(Coords c, char dir, Pixel skipBlock)
{
    Coords addCoords = getAddCoords(dir);

    while((grid.getPixel(c += addCoords)) == skipBlock);

    return c;
}

uint16_t getTreeIndex(coordsCounter x)
{
    for(uint8_t i = 0; i < trees.getLength(); ++i)
    {
        if(trees[i]->getCoords().x == x) return i;
    }

    return numeric_limits<uint16_t>::max();
}

void growTrees(coordsCounter min_distance, coordsCounter max_distance, coordsCounter start_x, coordsCounter end_x, uint8_t index)
{
    Coords treeCoords;

    for(coordsCounter i {start_x + 5}; i < end_x - 4; i += getRandBetweenEqu(min_distance, max_distance))
    {
        if((treeCoords = getFirstBlock({i, static_cast<coordsCounter>(grid.getMaxXY().y - 1)}, 's', grassTexture) + Coords{0, 1}) != Coords{0, 1})
        {
            trees.append(new Tree(treeCoords, woodTextures[index], leavesTextures[index], getRandBetweenEqu(minTreeHeight, maxTreeHeight)));
        }
    }
}

void growPlants(uint8_t m, uint8_t M)
{
    Coords plantCoords;
    Coords searchCoords;

    for(coordsCounter i = 5; i < grid.getMaxXY().x - 4; i += getRandBetweenEqu(m, M))
    {
        searchCoords = Coords{i, static_cast<coordsCounter>(grid.getMaxXY().y - 1)};
        
        if((plantCoords = getFirstBlock(searchCoords, 's', grassTexture)) != Coords{0, 0})
        {
            plants.append(Plant(plantCoords, plantTexture, getRandBetweenEqu(250, 300)));
        } else 
        {
            // ALGHE
            Coords first_water_block_coords      {getFirstBlock({i, seaLevel - 6}, 's', waterTexture)};

            Coords first_sand_under_water_coords {getFirstBlock(first_water_block_coords, 's', sandTexture)};

            if(first_sand_under_water_coords != Coords{0, 0})
            {
                plants.append(Plant(first_sand_under_water_coords + Coords{0, 1}, seaweedTexture, getRandBetweenEqu(100, 200), getRandBetweenEqu(2, 6)));
            }
        }
    }
}

void generateTerrain(int ns, uint8_t minTree, uint8_t maxTree, uint8_t minPlant, uint8_t maxPlant)
{
    const char biome {getRandBetweenEqu(0, 2)};

    float noisyY = 0.f;

    coordsCounter stoneLvl = stoneLevelEnd;
    coordsCounter dirtLvl  = dirtLevelEnd;
    coordsCounter grassLvl = grassLevelEnd;

    grid.distend({0, 0}, {static_cast<coordsCounter>(grid.getMaxXY().x - 1), seaLevel}, waterTexture);

    for(coordsCounter i = 0; i < grid.getMaxXY().x; ++i)
    {
        noisyY += static_cast<float>(getRandBetweenEqu(-ns, ns));;

        // CLIFFS
        /*
        if(rand() % 30 == 0)
        {
            noisyY -= getRandBetweenEqu(3, 5);
        }*/

        stoneLvl = stoneLevelEnd + noisyY;
        dirtLvl  = stoneLvl + 5;
        grassLvl = dirtLvl + 1;

        grid.drawLine({i, 0}, {i, stoneLvl}, stoneTexture);


        if(dirtLvl > seaLevel)
        {
            grid.drawLine({i, static_cast<coordsCounter>(stoneLvl + 1)}, {i, dirtLvl}, dirtTexture);

            grid.drawLine({i, static_cast<coordsCounter>(dirtLvl + 1)}, {i, grassLvl}, grassTexture);
        }
        else
        {
            grid.drawLine({i, static_cast<coordsCounter>(stoneLvl + 1)}, {i, grassLvl}, sandTexture);
        }
    }
    
    growTrees(minTree, maxTree, 0, grid.getMaxXY().x, biome);

    growPlants(minPlant, maxPlant); 

    if(biome)
    {
        for(coordsCounter i {}; i < grid.getMaxXY().x; ++i)
        {
            Coords first_block_coords {getFirstBlockAvailable({i, grid.getMaxXY().y - 1}, 's', airTexture)};

            if(first_block_coords.y >= 0)
            {
                if(grid.getPixel(first_block_coords) != waterTexture)
                {
                    ++first_block_coords.y;

                    grid.drawPxl(first_block_coords, snowTexture);
                } else 
                {
                    if(rand() % 10) grid.drawPxl(first_block_coords, iceTexture);
                }
            }
        }
    }
}
