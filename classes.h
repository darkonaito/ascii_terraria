#ifndef HEADER_95402DD1C3B89DEB
#define HEADER_95402DD1C3B89DEB

class Player;
class Plant;
class Bush;
class Log;
class Crown;
class Tree;
class Sapling;

class Item
{
    public:
        Item(Pixel, coordsCounter);

        void setName(const std::string& n)
        {
            this->name = n;
        }

    private:
        Pixel       texture;
        counter     id;
        std::string name;
};

class Inventory
{
    public:
        Inventory(Coords);

        void draw(Coords);

        bool withdraw(Coords, Inventory&, Coords);
        void remove(Coords);
        void insert(Coords, Item);

        void arrange();

        coordsCounter getFreeSlots();

    private:
        Coords size;

        const counter getItemID();   

};

class Player : public BasicEntity
{
    public:
        Player(Coords, Pixel, coordsCounter uPerS = 1, uint8_t stdJmp = 4);

        void update();

        bool isInBlock(Pixel);


    private:
        coordsCounter unitPerStep;

        bool isJumping;

        uint8_t stdJmpPow;
        uint8_t jumpPow;
        uint8_t jump;

        char facing;
};

class Plant
{
    public:
        Plant() = default;
        Plant(Coords, Pixel, uint16_t, uint8_t = grassMaxStemHeight);

        void chop(uint8_t);
        void update();
        void grow();

        void switchWave(uint8_t t = 0);

    protected:
        void draw();
        void clear();

        Coords coords;
        Pixel  texture;

        bool     uprooted;
        bool     waving {true};
        uint16_t waving_speed;
        uint16_t regenTime;
        uint16_t regenTimer;
        uint8_t  maxStemHeight;
        uint8_t  stemHeight;

        std::vector<Coords> stem;
};

class Sapling : public Plant
{
    public:
        Sapling(Coords, Pixel, Pixel, Pixel, uint16_t, uint16_t, uint8_t = grassMaxStemHeight);

        void update();
        bool isStillSapling();

    private:
        Tree* tree;

        Pixel wood;
        Pixel leaves;

        uint16_t growTime;
        uint16_t growTimer;
        uint8_t  treeHeight;

        bool stillSapling;
};

class Bush
{
    public:
        Bush(Coords, Pixel, uint8_t);

    private:
        Coords coords;
        uint8_t height;
};

class Crown
{
    public:
        Crown();
        Crown(Coords, Pixel, uint8_t);
        Crown(const Crown&);
        ~Crown();

        void draw(Pixel pxl = Pixel{}, bool darkerLeaves = true);

    private:
        Coords coords;
        Pixel  texture;

        Coords startEnd1;
        Coords startEnd2;

        uint8_t width;

    friend Tree;
};

class Log
{
    public:
        Log();
        Log(Coords, Pixel, uint8_t);
        Log(const Log&);
        ~Log();

        void draw();
        void chop(uint8_t);

        Coords  getCoords() {return coords;}
        uint8_t getHeight() {return height;}

    private:
        Coords coords;
        Pixel  texture;

        uint8_t height;

    friend Tree;
};


class Tree
{
    public:
        Tree();
        Tree(Coords, Pixel, Pixel, uint8_t);
        Tree(const Tree&);
        Tree(const Tree&&);
        ~Tree();

        Tree& operator=(const Tree&);
        Tree& operator=(const Tree&&);

        void draw();
        void chop(uint8_t);

        Coords getCoords() {return log->coords;}

    private:
        Log*     log;
        Crown* crown;
};


//#include "classes.cpp"

#endif // header guard

