// C Text Display test program
// Copyright 2020 I.C.

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //For random seed
#include <SDL2/SDL.h>
#include <math.h>

#include "td/td.h"
#include "td/default/font_8x8.h"
#include "td/default/DefaultPalette.h"

#define TEST_HRES 640
#define TEST_VRES 480
#define TEST_FPS_LIMIT 60

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
bool sdlinit(const char* title, uint16_t hres, uint16_t vres) {
    //Initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not be initialized! SDL_Error: %s", SDL_GetError());
        return 1;
    } else {
        //Create window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hres, vres, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf("Window could not be created! SDL_Error: %s", SDL_GetError());
            return 1;
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
            //Update the surface
            SDL_UpdateWindowSurface(window);
        }
    }
    return 0;
}

void sdlquit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** args) {
    srand(time(NULL));
    //Allocate TextDisplay
    TextDisplay* td;
    td = (TextDisplay*)malloc(sizeof(TextDisplay));
    if(td == NULL) {
        printf("Cannot allocate TextDisplay.\n");
        return 1;
    }
    //Initialize TextDisplay
    uint8_t res;
    res = td_display_init(td, TEST_HRES, TEST_VRES);
    if(res) { printf("td_display_init: %s\n",td_errorstr(res)); return 1; }
    res = td_charset_load(td, &td_font_8x8);
    if(res) { printf("td_charset_load: %s\n",td_errorstr(res)); return 1; }
    //Actual size of screen double of pixel resolution for scrolling demo
    res = td_screen_alloc(td, TEST_HRES/4, TEST_VRES/4);
    if(res) { printf("td_screen_alloc: %s\n",td_errorstr(res)); return 1; }
    res = td_palette_load(td, &td_default_palette);
    if(res) { printf("td_palette_load: %s\n",td_errorstr(res)); return 1; }
    td_screen_clear(td);
    
    //TextDisplay rendering buffer
    uint32_t test_bufsize = td->HPixelRes*td->VCharSize*3;
    uint8_t* display_buffer = malloc(test_bufsize);
    
    //Print test byte
    if(td->CharSet1Loaded) {
        unsigned int readatChar = 3;
        unsigned int readatCharByte = 2;
        unsigned int readPos = (readatChar * (td->CharSet->Height-1)) + readatCharByte;
        unsigned int readbyte = td->CharSet->Chars[readPos];
        printf("[TEST] Char byte at %u, %u: 0x%x\n", readatChar, readatCharByte, readbyte);
    }
    
    //Fill screen with random characters and colors
    {
        uint32_t CharCount = td->HCharRes * td->VCharRes;
        for(uint32_t i=0; i<CharCount; i++) {
            td->Screen[i].Char = rand() % 96 + 32;
            td->Screen[i].BGColor = rand() % 8;
            td->Screen[i].FGColor = rand() % 8;
        }
    }
    
    if(sdlinit("TextDisplay example test program", TEST_HRES, TEST_VRES)) return 1;
    
    bool run = 1;
    SDL_Event sdlEvent;
    uint32_t totalframecounter = 0;
    //FPS counter
    uint32_t fpsc_lastms = 0;
    uint32_t fpsc_counter = 0;
    uint32_t fpsc_fps = 0;
    //Frame time counter
    uint32_t ftc_begms = 0;
    uint32_t ftc_endms = 0;
    uint32_t ftc_restime = 0;
    float    ftc_avg = 0;
    uint32_t ftc_avgcounter = 0;
    uint32_t ftc_frameDelayTime = round((double)1000 / TEST_FPS_LIMIT);
    
    while(run) {
        ftc_begms = SDL_GetTicks();
        while(SDL_PollEvent( &sdlEvent ) != 0) { 
            //Quit request
            if(sdlEvent.type == SDL_QUIT) run = 0;
        }
        
        //Render the test screen
        uint16_t lines = td->VPixelRes / td->VCharSize;
        for(uint16_t l=0; l<lines; l++) {
            td_render_fullrgb_charline(td, l, display_buffer);
            
            //Fill buffer
            {
                uint32_t i,j,ri,c=0;
                uint32_t lb = l * (test_bufsize / 3);
                uint32_t li = (l+1) * (test_bufsize / 3);
                for(; c<3; c++) {
                    j = lb;
                    ri = 0;
                    for(i=lb; i<li; i++) {
                        ((uint8_t*)screenSurface->pixels)[(j++ << 2) + c] = display_buffer[(ri++ * 3) + c];
                    }
                }
            }
            //Clean buffer
            memset(display_buffer, 0, test_bufsize);
        }
        
        SDL_UpdateWindowSurface(window);
        
        //Frame counter
        totalframecounter++;
        if(totalframecounter % 1 == 0) {
            td->HCharOffset++;
            td->VCharOffset++;
        }
        
        //FPS counter
        fpsc_counter++;
        if(SDL_GetTicks() > fpsc_lastms+1000) {
            fpsc_lastms += 1000;
            fpsc_fps = fpsc_counter;
            fpsc_counter=0;
            //Frame time counter avg reset
            ftc_avg = (float)ftc_avgcounter / fpsc_fps;
            ftc_avgcounter = 0;
        }
        
        ftc_endms = SDL_GetTicks();
        ftc_restime = ftc_endms - ftc_begms;
        ftc_avgcounter += ftc_restime;
        
        printf("Frame: %u FPS: %u Frame time avg: %fms    \r", totalframecounter, fpsc_fps, ftc_avg); fflush(stdout);
        
        //Delay frame
        if(ftc_restime < ftc_frameDelayTime) {
            SDL_Delay(ftc_frameDelayTime - ftc_restime);
        }
    }
        
    
    //Free textdisplay stuff and then free the textdisplay struct itself
    td_display_free(td);
    free(td);
    free(display_buffer);
    sdlquit();
    return 0;
}
