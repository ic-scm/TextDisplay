// C Text Display
// Copyright 2020 I.C.

//Allocates a new character buffer (TextDisplay_Character* Screen) of specified size.
//Size can be bigger than what could fit in the pixel resolution, the renderer will cut off characters that don't fit.
//Returns 0 or error code.
uint8_t td_screen_alloc(TextDisplay* disp, uint16_t HCharRes, uint16_t VCharRes) {
    //Free any previously allocated screen
    if(disp->ScreenAllocated) {
        free(disp->Screen);
        disp->ScreenAllocated = 0;
    }
    disp->HCharRes = 0;
    disp->VCharRes = 0;
    //Allocate new screen
    disp->Screen = (TextDisplay_Character*)malloc( sizeof(TextDisplay_Character) * (HCharRes * VCharRes) );
    //Check for malloc error
    if(disp->Screen == NULL) {
        return 253;
    }
    disp->ScreenAllocated = 1;
    disp->HCharRes = HCharRes;
    disp->VCharRes = VCharRes;
    return 0;
}

//Clears all characters on the Screen.
void td_screen_clear(TextDisplay* disp) {
    if(disp->ScreenAllocated) {
        uint32_t CharCount = disp->HCharRes * disp->VCharRes;
        TextDisplay_Character* screen = disp->Screen;
        for(uint32_t i=0; i<CharCount; i++) {
            screen[i].Char = 0;
            screen[i].BGColor = 0;
            screen[i].FGColor = 1;
        }
    }
}
