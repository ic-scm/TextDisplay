// C Text Display
// Copyright 2020 I.C.

//Allocates a Palette with the specified number of colors.
//Returns 0 or error code.
uint8_t td_palette_alloc(TextDisplay* disp, uint8_t colorcount) {
    //Free any existing palette
    if(disp->PaletteColorCount > 0) {
        free(disp->Palette);
        disp->PaletteColorCount = 0;
    }
    //Allocate new palette
    disp->Palette = (TextDisplay_ColorPalette*)malloc(sizeof(TextDisplay_ColorPalette) * colorcount);
    //Check for malloc error
    if(disp->Palette == NULL) {
        return 253;
    }
    disp->PaletteColorCount = colorcount;
    return 0;
}

//Copies a palette from a TextDisplay_ColorPalette_Store into the TextDisplay_ColorPalette in a TextDisplay.
//The Palette will be allocated/reallocated if the currently allocated TextDisplay_ColorPalette in the TextDisplay is not the same size as the palette in the TextDisplay_ColorPalette_Store.
//Returns 0 or error code.
uint8_t td_palette_load(TextDisplay* disp, const TextDisplay_ColorPalette_Store* pal) {
    if(disp->PaletteColorCount != pal->ColorCount) {
        uint8_t tmp_res = td_palette_alloc(disp, pal->ColorCount);
        //Return on error
        if(tmp_res > 0) return tmp_res;
    }
    for(uint8_t c=0; c<pal->ColorCount; c++) {
        disp->Palette[c] = pal->Colors[c];
    }
    return 0;
}
