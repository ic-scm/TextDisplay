// C Text Display
// Copyright 2020 I.C.

struct TextDisplay_Character {
    uint8_t Char;
    uint8_t FGColor;
    uint8_t BGColor;
};

struct TextDisplay_ColorPalette {
    uint8_t R,G,B;
};

//For storing a palette to be loaded later.
struct TextDisplay_ColorPalette_Store {
    uint8_t ColorCount;
    const TextDisplay_ColorPalette* Colors;
};

struct TextDisplay_CharSet {
    const uint8_t Width;
    const uint8_t Height;
    //First character in chars (ASCII table)
    const uint8_t CharFirst;
    //Amount of characters in chars after the first
    const uint8_t CharCount;
    //Name of the font that can be shown to the user.
    const char* FontName;
    //1 bit per pixel, aligned to bytes for every line.
    //-1 for height, the last line and column will always be empty for space between characters
    //(so the actual font size would be Width-1 and Height-1).
    //Expected size of each char: Ceil(Width/8)*(Height-1)
    const uint8_t* Chars;
};

struct TextDisplay {
    //Text display - Array of characters
    TextDisplay_Character* Screen;
    bool ScreenAllocated;
    //Color palette - Array of colors (max uint8)
    TextDisplay_ColorPalette* Palette;
    //If this variable is 0, Palette should be free.
    uint8_t PaletteColorCount;
    //Loaded character set - Single pointer to an external TextDisplay_CharSet struct - Do not allocate internally!
    const TextDisplay_CharSet* CharSet;
    bool CharSet1Loaded;
    //Size of characters, same as Width and Height in TextDisplay_CharSet.
    uint8_t HCharSize;
    uint8_t VCharSize;
    //Size of the visible screen area
    uint16_t HPixelRes;
    uint16_t VPixelRes;
    //Offset first character to be rendered.
    //The renderer will wrap around to the beginning if the visible screen area reaches the end of the character data.
    uint16_t HCharOffset;
    uint16_t VCharOffset;
    //Amount of characters in the screen. Can be bigger than what would fit in the visible screen area.
    uint16_t HCharRes;
    uint16_t VCharRes;
};

#include "screen.c"
#include "palette.c"
#include "charset.c"
#include "renderer.c"

//Initializes a new TextDisplay struct and allocates a new Screen buffer.
//td_display_free should be used first if using this function on a previously initialized struct.
/*
- HPixelRes: Horizontal pixel resolution of the screen
- VPixelRes: Vertical pixel resolution of the screen
*/
//Returns 0 or error code.
uint8_t td_display_init(TextDisplay* disp, uint16_t HPixelRes, uint16_t VPixelRes) {
    //Validate input data.
    if(HPixelRes < 1 || VPixelRes < 1) {
        //Display size too small.
        return 255;
    }
    //Initalize all struct variables to their default values.
    disp->Screen = NULL;
    disp->ScreenAllocated = 0;
    disp->Palette = NULL;
    disp->PaletteColorCount = 0;
    disp->CharSet = NULL;
    disp->CharSet1Loaded = 0;
    disp->HCharSize = 0;
    disp->VCharSize = 0;
    disp->HPixelRes = 0;
    disp->VPixelRes = 0;
    disp->HCharOffset = 0;
    disp->VCharOffset = 0;
    disp->HCharRes = 0;
    disp->VCharRes = 0;
    
    disp->HPixelRes = HPixelRes;
    disp->VPixelRes = VPixelRes;
    
    return 0;
}

//Frees all allocated memory in a TextDisplay struct.
void td_display_free(TextDisplay* disp) {
    if(disp->ScreenAllocated) {
        free(disp->Screen);
        disp->ScreenAllocated = 0;
    }
    if(disp->PaletteColorCount > 0) {
        free(disp->Palette);
        disp->PaletteColorCount = 0;
    }
    //CharSet is not freed because it should point to an external TextDisplay_CharSet struct.
}

#ifdef TD_ENABLE_ERRORSTR_FUNC
//Returns a readable error string from an error code returned by another function of this library.
const char* td_errorstr(uint8_t code) {
    switch(code) {
        case 255: return "Display resolution too small";
        case 254: return "Character size cannot be bigger than display resolution";
        case 253: return "Memory allocation failed";
    }
    return "Unknown error";
}
#endif
