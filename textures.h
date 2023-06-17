#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

constexpr Pixel airTexture {colors::BlackBack | colors::NavyFore, '.'};

constexpr Pixel plantTexture   {colors::LimeBack, ' '};
constexpr Pixel seaweedTexture {colors::TealBack, ' '};


constexpr Pixel woodTextures[] {
    {colors::OliveFore | colors::GreenBack, specialChars::block2},
    {colors::YellowFore | colors::GreenBack, specialChars::block1},
    {colors::GreenFore | colors::YellowBack, specialChars::block1}
};

constexpr Pixel leavesTextures[] {
    {colors::LimeFore | colors::BlackBack, specialChars::block2},
    {colors::GreenFore | colors::GrayBack, specialChars::block2},
    {colors::GreenBack | colors::MaroonBack, specialChars::block1}
};

constexpr Pixel grassTexture     {colors::LimeFore | colors::GreenBack, specialChars::block2};
constexpr Pixel dirtTexture      {colors::OliveFore | colors::BlackBack, specialChars::block1};
constexpr Pixel stoneTexture     {colors::GrayFore | colors::BlackBack, specialChars::block1};
constexpr Pixel sandTexture      {colors::YellowFore | colors::WhiteBack, specialChars::block2};
constexpr Pixel graySandTexture  {colors::YellowFore | colors::TealBack, specialChars::block2};

constexpr Pixel plankTexture {colors::WhiteFore | colors::BlackBack, specialChars::block2};

constexpr Pixel waterTexture {colors::BlueFore | colors::NavyBack, ' '};
constexpr Pixel iceTexture   {colors::AquaFore | colors::WhiteBack, specialChars::block0};
constexpr Pixel snowTexture  {colors::WhiteFore, specialChars::half_block_down};

constexpr Pixel plrTexture {colors::RedBack, ' '};


#endif // TEXTURES_H_INCLUDED
