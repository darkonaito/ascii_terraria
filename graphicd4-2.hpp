#ifndef GRAPHICD4_HPP_INCLUDED
#define GRAPHICD4_HPP_INCLUDED

#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED
#include <windows.h>
#endif // WINDOWS_H_INCLUDED


namespace grphd
{
    namespace generic
    {
        using counter       = uint16_t;
        using coordsCounter = int16_t;

        struct Coords;

        int getRandBetweenEqu(int, int);
    }

    namespace colors
    {
        enum : generic::counter
        {
            BlackFore   = 0,
            MaroonFore  = FOREGROUND_RED,
            GreenFore   = FOREGROUND_GREEN,
            NavyFore    = FOREGROUND_BLUE,
            TealFore    = FOREGROUND_GREEN | FOREGROUND_BLUE,
            OliveFore   = FOREGROUND_RED | FOREGROUND_GREEN,
            PurpleFore  = FOREGROUND_RED | FOREGROUND_BLUE,
            GrayFore    = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            SilverFore  = FOREGROUND_INTENSITY,
            RedFore     = FOREGROUND_INTENSITY | FOREGROUND_RED,
            LimeFore    = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
            BlueFore    = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
            AquaFore    = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
            YellowFore  = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
            FuchsiaFore = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
            WhiteFore   = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

            BlackBack   = 0,
            MaroonBack  = BACKGROUND_RED,
            GreenBack   = BACKGROUND_GREEN,
            NavyBack    = BACKGROUND_BLUE,
            TealBack    = BACKGROUND_GREEN | BACKGROUND_BLUE,
            OliveBack   = BACKGROUND_RED | BACKGROUND_GREEN,
            PurpleBack  = BACKGROUND_RED | BACKGROUND_BLUE,
            GrayBack    = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
            SilverBack  = BACKGROUND_INTENSITY,
            RedBack     = BACKGROUND_INTENSITY | BACKGROUND_RED,
            LimeBack    = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
            BlueBack    = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
            AquaBack    = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
            YellowBack  = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
            FuchsiaBack = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
            WhiteBack   = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        };
    }

    namespace specialChars
    {
        enum : unsigned char
        {
            block0 = 176, block1, block2, full_block = 219, half_block_down, half_block_left, half_block_right, half_block_up,
            half_block_center = 254
        };
    }

    namespace console
    {
        class Console;
    }

    namespace graphic
    {
        struct Pixel;
        class  Grid;
        class  BasicEntity;
        class  Entity;
    }

    namespace specialSymbs{}
}

bool operator==(grphd::graphic::Pixel, grphd::graphic::Pixel);
bool operator!=(grphd::graphic::Pixel, grphd::graphic::Pixel);

bool operator==(grphd::generic::Coords, grphd::generic::Coords);
bool operator!=(grphd::generic::Coords, grphd::generic::Coords);

const grphd::generic::Coords& operator+=(grphd::generic::Coords&, const grphd::generic::Coords&);
const grphd::generic::Coords& operator-=(grphd::generic::Coords&, const grphd::generic::Coords&);

grphd::generic::Coords operator+(const grphd::generic::Coords&, const grphd::generic::Coords&);
grphd::generic::Coords operator-(const grphd::generic::Coords&, const grphd::generic::Coords&);

bool operator<(const grphd::generic::Coords&, const grphd::generic::Coords&);
bool operator>(const grphd::generic::Coords&, const grphd::generic::Coords&);

#include "graphicd4-2.cpp"

#endif // GRAPHICD4_HPP_INCLUDED
