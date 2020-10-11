// C Text Display
// Copyright 2020 I.C.

//Renders a single character line of the TextDisplay screen.
//Target buffer should be allocated to HPixelRes*VCharSize*3.
//Resulting Full 24-bit RGB data should be accesses like this: tbuffer[((Y*HPixelRes)+X)*3+(/*0,1,2 - R,G,B values.*/]
void td_render_fullrgb_charline(TextDisplay* disp, uint16_t line, uint8_t* tbuffer) {
    //Check if the Display is fully allocated
    if(!disp->ScreenAllocated) return;
    if(!disp->CharSet1Loaded) return;
    if(disp->PaletteColorCount == 0) return;
    //Horizontal character count in the visible screen area
    uint16_t HCharCount = disp->HPixelRes / disp->HCharSize;
    if(HCharCount > disp->HCharRes) HCharCount = disp->HCharRes;
    //Beginning of this line in Screen
    uint16_t lineoff = ((line + disp->VCharOffset) % disp->VCharRes) * disp->HCharRes;
    //X and Y loop limits
    uint16_t lx = disp->HCharSize;
    uint16_t lyf = disp->VCharSize;
    uint16_t ly = disp->VCharSize - 1;
    //Horizontal res multiplied by 3
    uint16_t HPixelRes_X3 = disp->HPixelRes * 3;
    //Display Screen pointer
    TextDisplay_Character* screen = disp->Screen;
    //Charset information
    uint8_t fchar = disp->CharSet->CharFirst;
    uint8_t lchar = fchar + disp->CharSet->CharCount;
    //Character loop
    uint16_t cyoff;
    uint16_t coff = 0;
    uint16_t y, x;
    //Current character line pixel calculated into bit
    bool CharBit;
    //Offset to current line of current character in charset data
    uint32_t CharSet_CurrentLine;
    //Size of characters in bytes in charset data
    uint16_t CharSet_CharWidth = ceil((float)disp->HCharSize / 8);
    uint16_t CharSet_CharSize = CharSet_CharWidth * (disp->VCharSize - 1);
    uint32_t CharSet_CurrentCharOffset;
    //Bit mask index
    const uint8_t MaskIndex[8] = {
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001,
    };
    //Current character data
    TextDisplay_Character* CurrentCharPtr;
    TextDisplay_ColorPalette* CurrentFGColor;
    TextDisplay_ColorPalette* CurrentBGColor;
    TextDisplay_ColorPalette* CurrentColor;
    //Loop
    for(uint16_t cha=0; cha<HCharCount; cha++) {
        cyoff = disp->HCharSize * cha * 3;
        CurrentCharPtr = &screen[lineoff + ((cha + disp->HCharOffset) % disp->HCharRes)];
        CurrentFGColor = &disp->Palette[CurrentCharPtr->FGColor % disp->PaletteColorCount];
        CurrentBGColor = &disp->Palette[CurrentCharPtr->BGColor % disp->PaletteColorCount];
        //Write only BGColor for characters outside character range in loaded charset
        if(CurrentCharPtr->Char < fchar || CurrentCharPtr->Char > lchar) {
            CurrentColor = CurrentBGColor;
            for(y=0; y<lyf; y++) {
                coff = cyoff;
                for(x=0; x<lx; x++) {
                    tbuffer[coff++] = CurrentColor->R;
                    tbuffer[coff++] = CurrentColor->G;
                    tbuffer[coff++] = CurrentColor->B;
                }
                cyoff += HPixelRes_X3;
            }
            continue;
        }
        CharSet_CurrentCharOffset = (CurrentCharPtr->Char-fchar) * CharSet_CharSize;
        //Normal write
        for(y=0; y<ly; y++) {
            coff = cyoff;
            CharSet_CurrentLine = CharSet_CurrentCharOffset + (CharSet_CharWidth * y);
            for(x=0; x<lx; x++) {
                //Split character line to bits
                CharBit = disp->CharSet->Chars[CharSet_CurrentLine + (x >> 3)] & MaskIndex[x % 8];
                //Get color
                CurrentColor = (
                    CharBit ? CurrentFGColor : CurrentBGColor
                );
                tbuffer[coff++] = CurrentColor->R;
                tbuffer[coff++] = CurrentColor->G;
                tbuffer[coff++] = CurrentColor->B;
            }
            cyoff += HPixelRes_X3;
        }
        //Write last empty line
        CurrentColor = CurrentBGColor;
        coff = cyoff;
        for(x=0; x<lx; x++) {
            tbuffer[coff++] = CurrentColor->R;
            tbuffer[coff++] = CurrentColor->G;
            tbuffer[coff++] = CurrentColor->B;
        }
    }
}

