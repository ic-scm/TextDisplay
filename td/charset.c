// C Text Display
// Copyright 2020 I.C.

//Loads a font into a display.
//Returns 0 or error code.
uint8_t td_charset_load(TextDisplay* disp, const TextDisplay_CharSet* font) {
    if(font->Width > disp->HPixelRes || font->Height > disp->VPixelRes) {
        //Character size is bigger than display size.
        return 254;
    }
    disp->CharSet = font;
    disp->CharSet1Loaded = 1;
    disp->HCharSize = font->Width;
    disp->VCharSize = font->Height;
    return 0;
}
