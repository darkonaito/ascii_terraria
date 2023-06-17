 /*****************************\
 |****** G r a p h i c d ******|
 |*****       4 - 2       *****|
 \*****************************/

template <class T>
T getMax(T a, T b) {return a>b?a:b;}
template <class T>
T getMin(T a, T b) {return a<b?a:b;}

struct grphd::generic::Coords
{
    int16_t x, y;

    bool xIsGreater(Coords c) const {return x > c.x;}
    bool yIsGreater(Coords c) const {return y > c.y;}

    bool xIsGrtrEqu(Coords c) const {return x >= c.x;}
    bool yIsGrtrEqu(Coords c) const {return y >= c.y;}

    bool xIsLesser(Coords c) const {return x < c.x;}
    bool yIsLesser(Coords c) const {return y < c.y;}

    bool xIsLessEqu(Coords c) const {return x <= c.x;}
    bool yIsLessEqu(Coords c) const {return y <= c.y;}

    bool oneAxisGrtr(Coords c) const {return x > c.x || y > c.y;}
    bool oneAxisGrtrEqu(Coords c) const {return x >= c.x || y >= c.y;}

    bool oneAxisLesser(Coords c) const {return x < c.x || y < c.y;}
    bool oneAxisLessEqu(Coords c) const {return x <= c.x || y <= c.y;}

    Coords operator*(grphd::generic::coordsCounter) const;
    Coords operator*=(grphd::generic::coordsCounter);
};

grphd::generic::Coords grphd::generic::Coords::operator*(grphd::generic::coordsCounter n) const
{
    return {static_cast<grphd::generic::coordsCounter>(x * n), static_cast<grphd::generic::coordsCounter>(y * n)};
}

grphd::generic::Coords grphd::generic::Coords::operator*=(grphd::generic::coordsCounter n)
{
    return {x *= n, y *= n};
}


struct grphd::graphic::Pixel
{
    generic::counter clr;
    unsigned char   txt;

    void paint(const grphd::generic::Coords&) const;
};

int grphd::generic::getRandBetweenEqu(int minim, int maxim)
{
    return (rand() % (maxim + 1 - minim)) + minim;
}

namespace grphd::specialSymbs
{
    constexpr generic::counter stdClr = colors::WhiteFore | colors::BlackBack;
    constexpr generic::counter errClr = 0;
    constexpr generic::counter errTxt = 0;
    constexpr graphic::Pixel   errPxl  {errClr, errTxt};
}

// O P E R A T O R S

bool operator==(grphd::graphic::Pixel t_pxl1, grphd::graphic::Pixel t_pxl2)
{
    return (t_pxl1.txt == t_pxl2.txt) && (t_pxl1.clr == t_pxl2.clr);
}

bool operator!=(grphd::generic::Coords a, grphd::generic::Coords b)
{
    return a.x != b.x || a.y != b.y;
}

bool operator==(grphd::generic::Coords a, grphd::generic::Coords b)
{
    return a.x == b.x && a.y == b.y;
}

const grphd::generic::Coords& operator+=(grphd::generic::Coords& a, const grphd::generic::Coords& b)
{
    return a = {static_cast<grphd::generic::coordsCounter>(a.x + b.x), static_cast<grphd::generic::coordsCounter>(a.y + b.y)};
}
const grphd::generic::Coords& operator-=(grphd::generic::Coords& a, const grphd::generic::Coords& b)
{
    return a = {static_cast<grphd::generic::coordsCounter>(a.x - b.x), static_cast<grphd::generic::coordsCounter>(a.y - b.y)};
}

grphd::generic::Coords operator+(const grphd::generic::Coords& a, const grphd::generic::Coords& b)
{
    return {static_cast<grphd::generic::coordsCounter>(a.x + b.x), static_cast<grphd::generic::coordsCounter>(a.y + b.y)};
}
grphd::generic::Coords operator-(const grphd::generic::Coords& a, const grphd::generic::Coords& b)
{
    return {static_cast<grphd::generic::coordsCounter>(a.x - b.x), static_cast<grphd::generic::coordsCounter>(a.y - b.y)};
}

bool operator!=(grphd::graphic::Pixel p1, grphd::graphic::Pixel p2)
{
    return p1.txt != p2.txt || p1.clr != p2.clr;
}

// C O N S O L E

/*
    CHAR_INFO *m_bufScreen;


      m_bufScreen = new CHAR_INFO[maxCoords.x * maxCoords.y];
      memset(m_bufScreen, 0, sizeof(CHAR_INFO) * maxCoords.x * maxCoords.y);

    for(grphd::generic::counter j = 0; j < maxCoords.y; j++)
    {
        grphd::console::Console::gotoCoords(rel_coords + grphd::generic::Coords{0, static_cast<grphd::generic::coordsCounter>(j)});

        for(grphd::generic::counter i = 0; i < maxCoords.x; i++)
        {
			m_bufScreen[j * maxCoords.x + i].Char.UnicodeChar = pixels[i][j].txt;
			m_bufScreen[j * maxCoords.x + i].Attributes = pixels[i][j].clr;
        }
    }

    delete[] m_bufScreen;

    HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT m_rectWindow = {0, 0, static_cast<short>(maxCoords.x - 1), static_cast<short>(maxCoords.y - 1)};

    WriteConsoleOutput(m_hConsole, m_bufScreen, {maxCoords.x, maxCoords.y}, { 0,0 }, &m_rectWindow);
*/

class grphd::console::Console
{
    public:
        Console(generic::Coords, generic::Coords);
        ~Console();

        CHAR_INFO& cells(generic::Coords c)
        {
            return screenBuffer[c.x + maxCoords.x * c.y];
        }

        void  setChar(generic::Coords c, char txt)             {cells(c).Char.AsciiChar  = txt;}
        void setColor(generic::Coords c, generic::counter clr) {cells(c).Attributes = clr;}
        void setPixel(generic::Coords c, graphic::Pixel pxl)   {cells(c).Char.AsciiChar  = pxl.txt, cells(c).Attributes = pxl.clr;}

        bool  safeSetChar(generic::Coords, char);
        bool safeSetColor(generic::Coords, generic::counter);
        bool safeSetPixel(generic::Coords, graphic::Pixel);

        char             getChar(generic::Coords c)  {return cells(c).Char.AsciiChar;}
        generic::counter getColor(generic::Coords c) {return static_cast<generic::counter>(cells(c).Attributes);}
        graphic::Pixel   getPixel(generic::Coords c)
        {
            return
            {
                static_cast<generic::counter>(cells(c).Attributes),
                static_cast<unsigned char>  (cells(c).Char.AsciiChar)
            };
        }

        char             safeGetChar(generic::Coords);
        generic::counter safeGetColor(generic::Coords);
        graphic::Pixel   safeGetPixel(generic::Coords);

        static void gotoCoords(generic::Coords);

        bool safeGotoCoords(generic::Coords);

        void draw();

        static void setOutptClr(grphd::generic::counter);

        static void stop();

        static void setCursorInfo(grphd::generic::counter, bool);

        static void resetColor();

        static void clear() {
            system("cls");
        }

    private:
        generic::Coords maxCoords;

        HANDLE consoleHandle;
        SMALL_RECT screenRect;
        CHAR_INFO* screenBuffer;
};

void grphd::console::Console::gotoCoords(generic::Coords c)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {c.x, c.y});
}

bool grphd::console::Console::safeGotoCoords(generic::Coords c)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        gotoCoords(c);

        return true;
    } else
    {
        return false;
    }
}

char grphd::console::Console::safeGetChar(generic::Coords c)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        return cells(c).Char.AsciiChar;
    } else
    {
        return specialSymbs::errTxt;
    }
}

grphd::generic::counter grphd::console::Console::safeGetColor(generic::Coords c)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        return cells(c).Attributes;
    } else
    {
        return specialSymbs::errClr;
    }
}


grphd::graphic::Pixel grphd::console::Console::safeGetPixel(generic::Coords c)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        return
        {
            static_cast<generic::counter>(cells(c).Attributes),
            static_cast<unsigned char>  (cells(c).Char.AsciiChar)
        };
    } else
    {
        return specialSymbs::errPxl;
    }
}


bool grphd::console::Console::safeSetChar(generic::Coords c, char txt)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        cells(c).Char.AsciiChar = txt;

        return true;
    } else
    {
        return false;
    }
}

bool grphd::console::Console::safeSetColor(generic::Coords c, generic::counter clr)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        cells(c).Attributes = clr;

        return true;
    } else
    {
        return false;
    }
}

bool grphd::console::Console::safeSetPixel(generic::Coords c, graphic::Pixel pxl)
{
    if(!c.oneAxisLesser({0, 0}) && !c.oneAxisGrtrEqu(maxCoords))
    {
        cells(c).Char.AsciiChar = pxl.txt;
        cells(c).Attributes     = pxl.clr;

        return true;
    } else
    {
        return false;
    }
}

void grphd::console::Console::draw()
{
    WriteConsoleOutput(consoleHandle, screenBuffer, {maxCoords.x, maxCoords.y}, {0, 0}, &screenRect);


}

grphd::console::Console::Console(generic::Coords c, generic::Coords offset):
    maxCoords(c), consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)),
    screenRect(offset.x, offset.y, static_cast<short>(c.x - 1), static_cast<short>(c.y - 1)),
    screenBuffer(new CHAR_INFO[c.x * c.y])
{
}

grphd::console::Console::~Console()
{
    delete[] screenBuffer;
}

void grphd::console::Console::setOutptClr(generic::counter clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<generic::counter>(clr));
}

void grphd::console::Console::stop()
{
    system("pause>nul");
}

void grphd::console::Console::resetColor()
{
    grphd::console::Console::setOutptClr(grphd::specialSymbs::stdClr);
}

void grphd::console::Console::setCursorInfo(generic::counter v, bool f)
{
   CONSOLE_CURSOR_INFO info {v, f};
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// G R I D

class grphd::graphic::Grid
{
    public :
        Grid(generic::Coords mXY = {1, 1}, graphic::Pixel = {0, ' '}, generic::Coords t_relativePoint = {});
        Grid(const Grid&);

        Grid& operator=(const Grid&);

        ~Grid();

        void paint();
        void testPaint();

        void reset(generic::Coords mXY = {1, 1}, graphic::Pixel = {0, ' '});

        void defaultRefill();
        void refill(graphic::Pixel = {0, ' '});

        void distend(generic::Coords, generic::Coords, graphic::Pixel);
        void drawPxl(generic::Coords, graphic::Pixel);
        void drawLine(generic::Coords, generic::Coords, graphic::Pixel);
        void drawSquare(generic::Coords, generic::Coords, graphic::Pixel);

        generic::Coords actualCoords(generic::Coords) const;

        const generic::Coords& getMaxXY() const;
        const generic::Coords& getRelCoords() const {return rel_coords;}

        const graphic::Pixel& getDfltClrTxt() const;

        generic::counter getEntitiesNumber() const {return entities.getLength();}

        graphic::Pixel getPixel(generic::Coords t_coords) const {return this->priv_getPixel(t_coords);}

        graphic::BasicEntity*& addEntity(graphic::BasicEntity &e);
        bool removeEntity(generic::counter n);
        bool kickoutLastEntity();

        void update();

    private:
        generic::Coords maxCoords, rel_coords;
        DynArr<BasicEntity*> entities;
        Pixel dfltClrTxt;
        Pixel **pixels;

        void freePixels();
        void allocatePixels();
        graphic::Pixel& priv_getPixel(generic::Coords) const;
        void paintOwn(const graphic::Entity&);
        void paintDef(const generic::Coords&);
};


// B A S I C   E N T I T Y

class grphd::graphic::BasicEntity
{
    friend class grphd::graphic::Grid;

    public:
        BasicEntity();
        BasicEntity(generic::Coords, graphic::Pixel);
        virtual ~BasicEntity() = default;

        virtual graphic::Pixel setPixel(graphic::Pixel);
        virtual generic::Coords setCoords(generic::Coords);
        virtual generic::Coords addCoords(generic::Coords c) {return lastCoords = coords, coords += c;}

        virtual void safeMove(const graphic::Grid&);
        virtual bool nextPxlSafe(const Grid&, char);
        virtual bool nextPxlSafe(const Grid&);
        virtual bool nextPxlIs(const Grid&, Pixel);
        virtual bool isSteppable(Pixel);

        virtual bool coordsOutOfBonds(const Grid&);

        virtual generic::Coords setSpeed(generic::Coords s) {return speed = s;}
        virtual generic::Coords addSpeed(generic::Coords s) {return speed += s;}
        virtual generic::Coords resetSpeed() {return speed = generic::Coords{0, 0};}
        virtual generic::Coords getSpeed() {return speed;}

        virtual graphic::Pixel getPixel() {return pixel;}
        virtual generic::Coords getCoords() {return coords;}

        virtual bool isVisible() {return _isVisible;}
        virtual bool switchVisibility() {return _isVisible = !_isVisible;}

        virtual graphic::Pixel addSteppablePixel(graphic::Pixel);
        virtual bool removeSteppablePixel(uint8_t);
        virtual uint16_t getSteppablePxlsNumber() {return steppablePixels.getLength();}


    protected:
        graphic::Pixel pixel;
        graphic::Pixel lastPixel;

        generic::Coords coords;
        generic::Coords lastCoords;

        generic::Coords speed;

        bool _isVisible;

        DynArr<graphic::Pixel> steppablePixels;
};

#if 1

bool grphd::graphic::BasicEntity::isSteppable(Pixel p)
{
    for(generic::counter i = 0; i < steppablePixels.getLength(); ++i)
        if(p == steppablePixels[i]) return true;

    return false;
}

bool grphd::graphic::BasicEntity::coordsOutOfBonds(const Grid& grid)
{
    generic::Coords tmpCoords = coords + speed;

    if(tmpCoords.oneAxisGrtrEqu(grid.getMaxXY()) || tmpCoords.oneAxisLesser({0, 0})) return true;
    return false;
}

bool grphd::graphic::BasicEntity::nextPxlIs(const Grid& grid, Pixel pxl)
{
    generic::Coords tmpCoords = coords + speed;

    if(tmpCoords.oneAxisGrtrEqu(grid.getMaxXY()) || tmpCoords.oneAxisLesser({0, 0})) return false;

    if(grid.getPixel(tmpCoords) == pxl) return true;

    return false;
}

bool grphd::graphic::BasicEntity::nextPxlSafe(const Grid& grid)
{
    generic::Coords tmpCoords = coords + speed;

    if(tmpCoords.oneAxisGrtrEqu(grid.getMaxXY()) || tmpCoords.oneAxisLesser({0, 0})) return false;

    Pixel pxl = grid.getPixel(tmpCoords);

    for(generic::counter i = 0; i < steppablePixels.getLength(); ++i)
    {
        if(pxl == steppablePixels[i]) return true;
    }

    return false;
}

bool grphd::graphic::BasicEntity::nextPxlSafe(const Grid& grid, char dir)
{
    generic::Coords tmpCoords = coords;

    switch(dir)
    {
        case 'w':
        case 'W':
            ++tmpCoords.y;
            break;

        case 'a':
        case 'A':
            --tmpCoords.x;
            break;

        case 's':
        case 'S':
            --tmpCoords.y;
            break;

        case 'd':
        case 'D':
            ++tmpCoords.x;
            break;

        default:
            return false;
    }

    if(tmpCoords.oneAxisGrtrEqu(grid.getMaxXY()) || tmpCoords.oneAxisLesser({0, 0})) return false;

    Pixel pxl = grid.getPixel(tmpCoords);

    for(generic::counter i = 0; i < steppablePixels.getLength(); ++i)
    {
        if(pxl == steppablePixels[i]) return true;
    }

    return false;
}

void grphd::graphic::BasicEntity::safeMove(const Grid& g)
{
    if(speed == generic::Coords{0, 0}) return;

    lastCoords = coords;
    generic::Coords tmpSpeed = speed;

    for(; tmpSpeed.x > 0 && nextPxlSafe(g, 'd');) ++coords.x, --tmpSpeed.x;

    for(; tmpSpeed.x < 0 && nextPxlSafe(g, 'a');) --coords.x, ++tmpSpeed.x;

    for(; tmpSpeed.y > 0 && nextPxlSafe(g, 'w');) ++coords.y, --tmpSpeed.y;

    for(; tmpSpeed.y < 0 && nextPxlSafe(g, 's');) --coords.y, ++tmpSpeed.y;
}

#endif // 1


grphd::graphic::Pixel grphd::graphic::BasicEntity::addSteppablePixel(graphic::Pixel p)
{
    return steppablePixels.append(p);
}

bool grphd::graphic::BasicEntity::removeSteppablePixel(uint8_t n)
{
    return steppablePixels.remove(n);
}

grphd::graphic::BasicEntity::BasicEntity(generic::Coords c, graphic::Pixel p):
    pixel(p), lastPixel(p), coords(c), lastCoords(c), speed({0, 0}), _isVisible(1), steppablePixels()
{

}

grphd::graphic::BasicEntity::BasicEntity():
    pixel({}), lastPixel(pixel), coords({}), lastCoords(coords), speed({}), _isVisible(1), steppablePixels()
{

}

grphd::graphic::Pixel grphd::graphic::BasicEntity::setPixel(grphd::graphic::Pixel p)
{
    lastPixel = pixel;
    return pixel = p;
}
grphd::generic::Coords grphd::graphic::BasicEntity::setCoords(grphd::generic::Coords c)
{
    lastCoords = coords;
    return coords = c;
}

// E N T I T Y

class grphd::graphic::Entity : public BasicEntity
{
    public:
        Entity(generic::Coords coords = {}, graphic::Pixel pixel = {0, ' '});

        generic::counter incrsX(generic::counter v = 1) {lastCoords = coords; coords.x += v; return coords.x;} // return *this;
        generic::counter incrsY(generic::counter v = 1) {lastCoords = coords; coords.y += v; return coords.y;}
        generic::counter decrsX(generic::counter v = 1) {lastCoords = coords; coords.x -= v; return coords.x;}
        generic::counter decrsY(generic::counter v = 1) {lastCoords = coords; coords.y -= v; return coords.y;}
        generic::Coords& relativeXY(generic::Coords);
        generic::Coords& absoluteXY(generic::Coords);
        generic::Coords& safeRelaXY(generic::Coords, const graphic::Grid&, graphic::Pixel);

        bool collidesWith(const graphic::Grid&, graphic::Pixel, unsigned char);
        bool relPixelIs(const Grid, Pixel, generic::Coords);

    // friend class grphd::graphic::Grid;
};
#if 0
grphd::generic::Coords& grphd::graphic::Entity::safeRelaXY(generic::Coords c, const graphic::Grid& grid, graphic::Pixel p)
{

    generic::Coords safeCoords{};

    if(c.x > 0)
    {
        for(; (safeCoords.x < c.x) && collidesWith(grid, p, 'd'); ++safeCoords.x)
        {

        }
    } else if(c.x < 0)
    {
        for(; (safeCoords.x > c.x) && collidesWith(grid, p, 'a'); --safeCoords.x)
        {

        }
    }

    if(c.y > 0)
    {
        for(; (safeCoords.y < c.y) && relPixelIs(grid, p, {safeCoords.x, static_cast<grphd::generic::coordsCounter>(safeCoords.y + 1)}); ++safeCoords.y)
        {

        }
    } else if(c.y < 0)
    {
        for(; (safeCoords.y > c.y) && relPixelIs(grid, p, {safeCoords.x, static_cast<grphd::generic::coordsCounter>(safeCoords.y - 1)}); --safeCoords.y)
        {

        }
    }

    return relativeXY(safeCoords);


}
#endif // 0

void grphd::graphic::Grid::distend(grphd::generic::Coords c1, grphd::generic::Coords c2, grphd::graphic::Pixel pxl)
{
    for(generic::counter i = c1.x; i <= c2.x; ++i)
    {
        for(generic::counter  j = c1.y; j <= c2.y; ++j)
        {
            drawPxl({static_cast<grphd::generic::coordsCounter>(i), static_cast<grphd::generic::coordsCounter>(j)}, pxl);
        }
    }
}

void grphd::graphic::Grid::drawPxl(grphd::generic::Coords c, grphd::graphic::Pixel pxl)
{
    priv_getPixel(c) = pxl;

    console::Console::gotoCoords(actualCoords(c));
    console::Console::setOutptClr(pxl.clr);
    std::cout << pxl.txt;
}

grphd::graphic::Grid::Grid(const grphd::graphic::Grid& g):
    maxCoords(g.maxCoords), rel_coords(g.rel_coords), entities(g.entities), dfltClrTxt(g.dfltClrTxt), pixels(new grphd::graphic::Pixel* [maxCoords.y])
{
    for(uint8_t j = 0; j < maxCoords.y; ++j)
    {
        for(uint8_t i = 0; i < maxCoords.x; ++i)
        {
            pixels[i][j] = g.pixels[i][j];
        }
    }

}

grphd::graphic::Grid& grphd::graphic::Grid::operator=(const grphd::graphic::Grid& g)
{
    maxCoords = g.maxCoords;
    rel_coords = g.rel_coords;
    entities = g.entities;
    dfltClrTxt = g.dfltClrTxt;

    for(uint8_t j = 0; j < maxCoords.y; ++j)
    {
        for(uint8_t i = 0; i < maxCoords.x; ++i)
        {
            pixels[i][j] = g.pixels[i][j];
        }
    }

    return *this;
}

void grphd::graphic::Grid::paintDef(const grphd::generic::Coords& c)
{
    priv_getPixel(c + rel_coords) = dfltClrTxt;
    console::Console::gotoCoords(c + rel_coords);
    console::Console::setOutptClr(dfltClrTxt.clr);
    std::cout << dfltClrTxt.txt;
}

grphd::generic::Coords grphd::graphic::Grid::actualCoords(grphd::generic::Coords c) const
{
    return {static_cast<grphd::generic::coordsCounter>(c.x + rel_coords.x), static_cast<grphd::generic::coordsCounter>((maxCoords.y - 1) - c.y + rel_coords.y)};
}

void grphd::graphic::Grid::paintOwn(const grphd::graphic::Entity& e)
{
    priv_getPixel(e.coords + rel_coords) = e.pixel;
    console::Console::gotoCoords(e.coords + rel_coords);
    console::Console::setOutptClr(e.pixel.clr);
    std::cout << e.pixel.txt;
}

grphd::graphic::Pixel& grphd::graphic::Grid::priv_getPixel(grphd::generic::Coords c) const
{
    c = actualCoords(c) - rel_coords;
    return pixels[c.x][c.y];
}

grphd::graphic::BasicEntity*& grphd::graphic::Grid::addEntity(graphic::BasicEntity &e)
{
    e.lastPixel = {priv_getPixel(e.coords)};

    return entities.append(&e);
}

bool grphd::graphic::Grid::removeEntity(generic::counter n)
{
    return entities.remove(n);
}

bool grphd::graphic::Grid::kickoutLastEntity()
{
    return entities.kickoutLast();
}

void grphd::graphic::Grid::allocatePixels()
{
    #if 1
    try
    {
        for(generic::counter i = 0; i < maxCoords.x; i++)
        {
            pixels[i] = new Pixel[maxCoords.y];

            for(generic::counter j = 0; j < maxCoords.y; j++)
            {
                pixels[i][j] = dfltClrTxt;

            }
        }
    } catch(std::exception &e)
    {
        std::cout << "Exception on allocatePixels: ";
        std::cout << e.what() << '\n';
    }
    #endif // 0
}

void grphd::graphic::Grid::freePixels()
{
    try
    {
        for(generic::counter i = 0; i < maxCoords.x; i++)
        {
            delete [] pixels[i];
        }

        delete [] pixels;
        pixels = nullptr;
    } catch(std::exception &e)
    {
        std::cout << "Exception on freePixels: ";
        std::cout << e.what() << '\n';
    }
}

inline const grphd::generic::Coords& grphd::graphic::Grid::getMaxXY() const
{
    return maxCoords;
}

inline const grphd::graphic::Pixel& grphd::graphic::Grid::getDfltClrTxt() const
{
    return dfltClrTxt;
}

grphd::graphic::Grid::Grid(generic::Coords xY, graphic::Pixel clrTxt, generic::Coords t_relativePoint):
    maxCoords(xY), rel_coords(t_relativePoint), entities(), dfltClrTxt(clrTxt), pixels(new grphd::graphic::Pixel* [maxCoords.x])
{
    allocatePixels();
}

grphd::graphic::Grid::~Grid()
{
    freePixels();
}

void grphd::graphic::Grid::reset(generic::Coords t_maxXY, graphic::Pixel t_dfltClrTxt)
{
    if(t_maxXY != maxCoords)
    {
        freePixels();
        maxCoords = t_maxXY;
        allocatePixels();
    }

    dfltClrTxt = t_dfltClrTxt;
}

void grphd::graphic::Grid::defaultRefill()
{
    for(generic::counter j = 0; j < maxCoords.y; j++)
    {
        for(generic::counter i = 0; i < maxCoords.x; i++)
        {
            pixels[i][j] = dfltClrTxt;
        }
    }
}

void grphd::graphic::Grid::refill(grphd::graphic::Pixel clrTxt)
{
    dfltClrTxt = clrTxt;

    for(generic::counter i = 0; i < entities.getLength(); ++i)
    {
        entities[i]->lastPixel = dfltClrTxt;
    }

    for(generic::counter i = 0; i < maxCoords.x; ++i)
    {
        for(generic::counter j = 0; j < maxCoords.y; ++j)
        {
            if(true) pixels[i][j] = clrTxt;
        }
    }
}

inline grphd::generic::Coords& grphd::graphic::Entity::relativeXY(generic::Coords addCoords)
{
    lastCoords = coords;
    coords += addCoords; return coords;
}

inline grphd::generic::Coords& grphd::graphic::Entity::absoluteXY(generic::Coords newCoords)
{
    lastCoords = coords,
    coords = newCoords; return coords;
}

grphd::graphic::Entity::Entity(grphd::generic::Coords c, grphd::graphic::Pixel pxl):
    BasicEntity(c, pxl)
{
}

enum SpecialCharacters : uint8_t
{
    e_WHITE_SMILE, e_BLACK_SMILE, e_HEART, e_DIAMONDS,
    e_CLUBS, e_HEARTS, e_SPADES, e_POINT,
    e_BACKGROUND0 = 176, e_BACKGROUND1, e_BACKGROUND2,
    e_BACKGROUND3 = 219,
    UP_ARROW = VK_UP, DOWN_ARROW = VK_DOWN, RIGHT_ARROW = VK_RIGHT, LEFT_ARROW = VK_LEFT
};

void grphd::graphic::Grid::update()
{
    #define entity entities[i]

    for(uint16_t i = 0; i < entities.getLength(); i++)
    {
        if(getPixel(entity->coords) != entity->pixel) // && entity->lastCoords != entity->coords
        {
            drawPxl(entity->lastCoords, entity->lastPixel);

            entity->lastPixel = getPixel(entity->coords);

            drawPxl(entity->coords, entity->pixel);
        } else
        {

        }
    }

    #undef entity
}

void grphd::graphic::Grid::paint()
{
    for(grphd::generic::counter j = 0; j < maxCoords.y; j++)
    {
        grphd::console::Console::gotoCoords(rel_coords + grphd::generic::Coords{0, static_cast<grphd::generic::coordsCounter>(j)});

        for(grphd::generic::counter i = 0; i < maxCoords.x; i++)
        {
            grphd::console::Console::setOutptClr(pixels[i][j].clr);
            std::cout << pixels[i][j].txt;
        }

        grphd::console::Console::resetColor();
    }
}

void grphd::graphic::Grid::testPaint()
{
    CHAR_INFO *m_bufScreen;


      m_bufScreen = new CHAR_INFO[maxCoords.x * maxCoords.y];
      memset(m_bufScreen, 0, sizeof(CHAR_INFO) * maxCoords.x * maxCoords.y);

    for(grphd::generic::counter j = 0; j < maxCoords.y; j++)
    {
        grphd::console::Console::gotoCoords(rel_coords + grphd::generic::Coords{0, static_cast<grphd::generic::coordsCounter>(j)});

        for(grphd::generic::counter i = 0; i < maxCoords.x; i++)
        {
			m_bufScreen[j * maxCoords.x + i].Char.UnicodeChar = pixels[i][j].txt;
			m_bufScreen[j * maxCoords.x + i].Attributes = pixels[i][j].clr;
        }
    }

    delete[] m_bufScreen;

    HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT m_rectWindow = {0, 0, static_cast<short>(maxCoords.x - 1), static_cast<short>(maxCoords.y - 1)};

    WriteConsoleOutput(m_hConsole, m_bufScreen, {maxCoords.x, maxCoords.y}, { 0,0 }, &m_rectWindow);
}

void grphd::graphic::Grid::drawSquare(grphd::generic::Coords t_c1, grphd::generic::Coords t_c2, grphd::graphic::Pixel t_pxl)
{
    grphd::generic::Coords c3{t_c2.x, t_c1.y};
    grphd::generic::Coords c4{t_c1.x, t_c2.y};

    drawLine(t_c1, c3, t_pxl);
    drawLine(t_c1, c4, t_pxl);
    drawLine(c3, t_c2, t_pxl);
    drawLine(c4, t_c2, t_pxl);

}

void grphd::graphic::Grid::drawLine(grphd::generic::Coords t_c1, grphd::generic::Coords t_c2, grphd::graphic::Pixel t_pxl)
{
    if(t_c1.x == t_c2.x)
    {
        for(grphd::generic::counter i = t_c1.y; i <= t_c2.y; i++)
        {
            drawPxl({t_c1.x, static_cast<grphd::generic::coordsCounter>(i)}, t_pxl);
        }

        return;
    }

    if(t_c1.y == t_c2.y)
    {
        for(grphd::generic::counter i = t_c1.x; i <= t_c2.x; i++)
        {
            drawPxl({static_cast<grphd::generic::coordsCounter>(i), t_c1.y}, t_pxl);
        }

        return;
    }

    float x = getMax(t_c1.x, t_c2.x) - getMin(t_c1.x, t_c2.x);
    float y = getMax(t_c1.y, t_c2.y) - getMin(t_c1.y, t_c2.y);

    float c = 1.0f;
    float k = y / x;

    for(float i = t_c1.x; grphd::generic::coordsCounter(i) <= x; i+= c)
    {
        drawPxl({static_cast<grphd::generic::coordsCounter>(i), static_cast<grphd::generic::coordsCounter>(i * k)}, t_pxl);
    }

}

bool grphd::graphic::Entity::collidesWith(const grphd::graphic::Grid& t_grid, Pixel t_pxl, unsigned char t_dir)
{
    auto virtual_coords = coords;

    switch(t_dir)
    {
        case 'w':
        case 'W':
            ++virtual_coords.y;
            break;

        case 'a':
        case 'A':
            --virtual_coords.x;
            break;

        case 's':
        case 'S':
            --virtual_coords.y;
            break;

        case 'd':
        case 'D':
            ++virtual_coords.x;
            break;

        default:
            return false;
    }

    if(virtual_coords.x < 0 || virtual_coords.y < 0
       || virtual_coords.x == t_grid.getMaxXY().x || virtual_coords.y == t_grid.getMaxXY().y) return false;

    if(t_grid.getPixel(virtual_coords) == t_pxl) return true;
    return false;
}

