// game classe methods

Inventory::Inventory(Coords c):
    size(c)
{
}

bool Inventory::withdraw(Coords c1, Inventory& i, Coords c2)
{
    //if(i.getFreeSlots() == 0)
    {
        return false;
    } //else
    {
        //i.insert()
    }
}

// player class

Player::Player(Coords c, Pixel pxl, coordsCounter uPerS, uint8_t stdJmp):
    BasicEntity(c, pxl), unitPerStep(uPerS), isJumping(false), stdJmpPow(stdJmp), jumpPow(stdJmpPow), jump(0), facing(' ')
{

}

void Player::update()
{
    // movement

    if(GetAsyncKeyState('W'))
    {
        facing = 'w';

        if(!isSteppable(grid.getPixel({coords.x, static_cast<coordsCounter>(coords.y - 1)})) || isInBlock(waterTexture))
        {
            isJumping = true;
            jump = jumpPow;
        }
    }

    if(GetAsyncKeyState('A'))
        speed.x -= unitPerStep, facing = 'a';
    if(GetAsyncKeyState('S'))
        speed.y -= unitPerStep, facing = 's';
    if(GetAsyncKeyState('D'))
        speed.x += unitPerStep, facing = 'd';

    if(isJumping)
    {
        --jump;
        speed.y += unitPerStep;

        if(jump == 0)
        {
            isJumping = false;
        }  
    }

    if(!isJumping) speed.y -= unitPerStep;

    // actions


    if(GetAsyncKeyState(' '))
    {
        Coords blockCoords = coords + getAddCoords(facing);

        if(blockCoords.oneAxisGrtrEqu(grid.getMaxXY()) || blockCoords.oneAxisLesser({0, 0}))
        {
            return;
        }
        else
        {
            Pixel nextPxl = grid.getPixel(blockCoords);

            if(nextPxl == woodTextures[0] || nextPxl == woodTextures[1] || nextPxl == woodTextures[2])
            {
                uint16_t tree = getTreeIndex(blockCoords.x);

                if(tree != numeric_limits<uint16_t>::max())
                {
                    uint8_t y = (blockCoords - trees[tree]->getCoords()).y;

                    if(y == 0)
                    {
                        delete trees[tree];
                        trees.remove(tree);
                    } else
                    {
                        trees[tree]->chop(static_cast<uint8_t>((blockCoords - trees[tree]->getCoords()).y));
                    }
                }
            }
            else if(nextPxl != grid.getDfltClrTxt() && nextPxl != waterTexture)
            {
                grid.drawPxl(blockCoords, grid.getDfltClrTxt());
            }
        }
    }

    safeMove(grid);
    resetSpeed();
}

bool Player::isInBlock(Pixel pxl)
{
    return lastPixel == pxl;
}


// plant class

Plant::Plant(Coords c, Pixel pxl, uint16_t rT, uint8_t mSH):
    coords(c), texture(pxl), uprooted(false), regenTime(rT), regenTimer(0), maxStemHeight(mSH)
{
    grow();

    if(pxl == seaweedTexture) waving_speed = 100;
    else waving_speed = 30;
}

void Plant::draw()
{
    for(const auto& i : stem)
    {
        grid.drawPxl(i, texture);
    }
}

void Plant::clear()
{
    for(const auto& i : stem)
    {
        Pixel fill_txt;

        if(texture == plantTexture)
        {
            fill_txt = airTexture;
        } else if(texture == seaweedTexture) 
        {
            fill_txt = waterTexture;
        }

        grid.drawPxl(i, fill_txt);
    }
}

void Plant::chop(uint8_t h)
{
    for(; stemHeight >= h; --stemHeight)
    {
        grid.drawPxl(stem[stemHeight], grid.getDfltClrTxt());

        if(stemHeight == 0)
        {
            uprooted = true;

            break;
        }
    }
}

void Plant::grow()
{
    if(stem.size() == 0)
    {
        stem.push_back(coords);

        grid.drawPxl(coords, texture);
    } else if(stem.size() < maxStemHeight)
    {
        uint8_t i = 0;

        Coords new_stem_coords {0, static_cast<coordsCounter>(stem.back().y + 1)};

        do
        {
            if(++i == testForFreeSpace) return;

            new_stem_coords.x = static_cast<coordsCounter>(stem.back().x + getRandBetweenEqu(-1, 1));
        }while(grid.getPixel(new_stem_coords) != grid.getDfltClrTxt() && grid.getPixel(new_stem_coords) != waterTexture);

        stem.push_back(new_stem_coords);

        grid.drawPxl(stem.back(), texture);
    }

}

void Plant::update()
{
    if(!uprooted)
    {
        if(regenTimer + 1 == regenTime)
        {
            regenTimer = 0;

            grow();
        } else
        {
            ++regenTimer;
        }

        if(waving && stem.size() > 1)
        {
            /*
            if(!(regenTimer % waving_speed))
            {
                clear();

                for(uint8_t i {1}; i < stem.size(); ++i)
                {
                    if(stem[i].x > stem[i - 1].x)
                    {
                        if(grid.getPixel(stem[i] - Coords{1, 0}) == waterTexture || grid.getPixel(stem[i] - Coords{1, 0}) == airTexture)
                            --stem[i].x;
                    } else 
                    {
                        if(grid.getPixel(stem[i] + Coords{1, 0}) == waterTexture || grid.getPixel(stem[i] - Coords{1, 0}) == airTexture)
                            ++stem[i].x;
                    }
                }

                draw();
            }
            */

            if(!(regenTimer % waving_speed))
            {
                clear();

                for(uint8_t i {1}; i < stem.size(); ++i)
                {
                    Coords new_coords {stem[i - 1].x + getRandBetweenEqu(-1, 1), stem[i].y};

                    if(grid.getPixel(new_coords) == waterTexture || grid.getPixel(new_coords) == airTexture)
                    {
                        clear();

                        stem[i] = new_coords;
                    }
                }

                draw();
            }

            /*
            if(!(regenTimer % (waving_speed / 2)))
            {
                uint8_t i {regenTimer % stem.size()};

                if(i > 0)
                {
                    Coords new_coords {stem[i - 1].x + getRandBetweenEqu(-1, 1), stem[i].y};

                    if(grid.getPixel(new_coords) == waterTexture || grid.getPixel(new_coords) == airTexture)
                    {
                        clear();

                        stem[i] = new_coords;

                        draw();
                    }
                }
            }
            */
        }
    }
}

void Plant::switchWave(uint8_t t)
{
    if(!t)
    {
        waving = !waving;
    } else 
    {
        waving_speed = t;
    }
}

// sapling class

Sapling::Sapling(Coords c, Pixel txtr, Pixel wd, Pixel lvs, uint16_t rT, uint16_t gT, uint8_t h):
    Plant(c, txtr, rT), wood(wd), leaves(lvs), growTime(gT), growTimer(0), treeHeight(h), stillSapling(true)
{

}

bool Sapling::isStillSapling()
{
    return stillSapling;
}

void Sapling::update()
{
    if(stillSapling)
    {
        if(growTimer + 1 < growTime)
        {
            ++growTimer;

            Plant::update();
        } else
        {
            stillSapling = false;

            chop(0);

            tree = new Tree(coords, wood, leaves, treeHeight);
        }
    }
}


// bush class

Bush::Bush(Coords c, Pixel pxl, uint8_t h):
    coords(c), height(h)
{
    grid.drawPxl(c, pxl);
}


// crown class

Crown::Crown()
{

}

Crown::Crown(Coords c, Pixel pxl, uint8_t w):
    coords(c), texture(pxl), width(w)
{
    startEnd1 = {static_cast<coordsCounter>(getRandBetweenEqu(1, 2)),
                 static_cast<coordsCounter>(getRandBetweenEqu(2, 4))};

    startEnd2 = {static_cast<coordsCounter>(startEnd1.x + getRandBetweenEqu(1, 2)),
                 static_cast<coordsCounter>(startEnd1.y - getRandBetweenEqu(1, 2))};
}

Crown::Crown(const Crown& crown):
    coords(crown.coords), texture(crown.texture), width(crown.width)
{
    startEnd1 = crown.startEnd1;
    startEnd2 = crown.startEnd2;
}

void Crown::draw(Pixel pxl, bool darkerLeaves)
{
    if(pxl == Pixel{0, 0}) pxl = texture;

    grid.distend(coords - startEnd1, coords + startEnd1, pxl);

    grid.distend(coords - startEnd2, coords + startEnd2, pxl);

    if(darkerLeaves)
    {
        for(coordsCounter j = coords.y - startEnd2.y; j <= coords.y + startEnd2.y; ++j)
        {
            for(coordsCounter i = coords.x - startEnd2.x; i <= coords.x + startEnd2.x; ++i)
            {
                if(rand() % 5 == 0) grid.drawPxl({i, j}, {static_cast<counter>(pxl.clr | colors::BlackBack), specialChars::block1});
                if(rand() % 11 == 0) grid.drawPxl({i, j}, {static_cast<counter>(pxl.clr | colors::GreenBack), specialChars::block0});
            }
        }
    }
}

Crown::~Crown()
{
    draw(grid.getDfltClrTxt(), false);
}

// log class

Log::Log()
{
}

Log::Log(Coords c, Pixel pxl, uint8_t h):
    coords(c), texture(pxl), height(h)
{
}

Log::Log(const Log& log):
    coords(log.coords), texture(log.texture), height(log.height)
{
}

Log::~Log()
{
    chop(0);
}

void Log::draw()
{
    grid.drawLine(coords, {coords.x, static_cast<coordsCounter>(coords.y + height)}, texture);
}

void Log::chop(uint8_t h)
{
    grid.drawLine({coords.x,
                   static_cast<coordsCounter>(coords.y + h)},
                  {coords.x,
                   static_cast<coordsCounter>(coords.y + height)},
                  grid.getDfltClrTxt());
}


// tree class

Tree::Tree():
    log(nullptr), crown(nullptr)
{

}

Tree::Tree(Coords c, Pixel wd, Pixel lvs, uint8_t h):
    log(new Log(c, wd, h)), crown(new Crown({c.x, static_cast<coordsCounter>(c.y + h)}, lvs, getRandBetweenEqu(3, 4)))
{
    draw();
}

Tree::Tree(const Tree& tree):
    log(new Log(tree.log->coords, tree.log->texture, tree.log->height)),
    crown(new Crown(tree.crown->coords, tree.crown->texture, tree.crown->width))
{
    draw();
}

Tree::Tree(const Tree&& tree):
    log(new Log(tree.log->coords, tree.log->texture, tree.log->height)),
    crown(new Crown(tree.crown->coords, tree.crown->texture, tree.crown->width))
{
    draw();
}

Tree& Tree::operator=(const Tree& tree)
{
    if(log != nullptr) delete log;
    if(crown != nullptr) delete crown;

    log   = new Log(*tree.log);
    crown = new Crown(*tree.crown);

    return *this;
}

Tree& Tree::operator=(const Tree&& tree)
{
    if(log != nullptr) delete log;
    if(crown != nullptr) delete crown;

    log   = new Log(*tree.log);
    crown = new Crown(*tree.crown);

    return *this;
}

void Tree::draw()
{
    log->draw();
    crown->draw();
}

Tree::~Tree()
{
    chop(0);
}

void Tree::chop(uint8_t h)
{
    if(crown != nullptr)
    {
        delete crown;
        crown = nullptr;
    }

    if(h == 0)
    {
        if(log != nullptr)
        {
            delete log;
            log = nullptr;
        }
    } else
    {
        if(h <= log->height)
        {
            log->chop(h);
        }
    }
}
