// C Text Display
// Copyright 2020 I.C.

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

//Disable this if you're not going to use the td_errorstr function to save program storage space.
#define TD_ENABLE_ERRORSTR_FUNC

typedef struct TextDisplay_Character TextDisplay_Character;
typedef struct TextDisplay_ColorPalette TextDisplay_ColorPalette;
typedef struct TextDisplay_ColorPalette_Store TextDisplay_ColorPalette_Store;
typedef struct TextDisplay_CharSet TextDisplay_CharSet;
typedef struct TextDisplay TextDisplay;

//Initializes a new TextDisplay struct.
//td_display_free should be used first if using this function on a previously initialized struct.
//Returns 0 or error code.
uint8_t td_display_init(TextDisplay* disp, uint16_t HPixelRes, uint16_t VPixelRes);

//Loads a font into a display.
//Returns 0 or error code.
uint8_t td_charset_load(TextDisplay* disp, const TextDisplay_CharSet* font);

//Allocates a new character buffer (TextDisplay_Character* Screen) of specified size.
//Returns 0 or error code.
uint8_t td_screen_alloc(TextDisplay* disp, uint16_t HCharRes, uint16_t VCharRes);

//Clears all characters on the Screen.
void td_screen_clear(TextDisplay* disp);

//Allocates a Palette with the specified number of colors.
//Returns 0 or error code.
uint8_t td_palette_alloc(TextDisplay* disp, uint8_t colorcount);

//Copies a palette from a TextDisplay_ColorPalette_Store into the TextDisplay_ColorPalette in a TextDisplay.
//The Palette will be allocated/reallocated if the currently allocated TextDisplay_ColorPalette in the TextDisplay is not the same size as the palette in the TextDisplay_ColorPalette_Store.
//Returns 0 or error code.
uint8_t td_palette_load(TextDisplay* disp, const TextDisplay_ColorPalette_Store* pal);

//Renders a single character line of the TextDisplay screen.
//Target buffer should be allocated to HPixelRes*VCharSize*3.
//Resulting Full 24-bit RGB data should be accesses like this: tbuffer[((Y*HPixelRes)+X)*3+(/*0,1,2 - R,G,B values.*/]
void td_render_fullrgb_charline(TextDisplay* disp, uint16_t line, uint8_t* tbuffer);

//Frees all allocated memory in a TextDisplay struct.
void td_display_free(TextDisplay* disp);

#ifdef TD_ENABLE_ERRORSTR_FUNC
//Returns a readable error string from an error code returned by another function of this library.
const char* td_errorstr(uint8_t code);
#endif

#include "td.c"
