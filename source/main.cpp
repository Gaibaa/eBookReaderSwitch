#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <switch.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#ifdef DEBUG
    #include <twili.h>
#endif

extern "C" {
    #include "common.h"
    #include "textures.h"
    #include "MenuChooser.h"
}

SDL_Renderer* RENDERER;
SDL_Window* WINDOW;
//SDL_Surface* WINDOW_SURFACE;
SDL_Event EVENT;
TTF_Font *ARIAL, *ARIAL_35, *ARIAL_30, *ARIAL_27, *ARIAL_25, *ARIAL_20, *ARIAL_15;

bool run = true;

void Term_Services() {
    std::cout << "Terminate Serices" << std::endl;
    run = false;

    timeExit();
    TTF_CloseFont(ARIAL_35);
    TTF_CloseFont(ARIAL_30);
    TTF_CloseFont(ARIAL_27);
    TTF_CloseFont(ARIAL_25);
    TTF_CloseFont(ARIAL_20);
    TTF_CloseFont(ARIAL_15);
    TTF_CloseFont(ARIAL);
    TTF_Quit();

    Textures_Free();
    romfsExit();

    IMG_Quit();

    SDL_DestroyRenderer(RENDERER);
    //SDL_FreeSurface(WINDOW_SURFACE);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();

    #ifdef DEBUG
        twiliExit();
    #endif
}

void Init_Services() {
    #ifdef DEBUG
        twiliInitialize();
    #endif

    std::cout << "Initalize Serices" << std::endl;

    romfsInit();
    std::cout << "Initalized RomFs" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        Term_Services();
    }
    std::cout << "Initalized SDL" << std::endl;

    timeInitialize();
    std::cout << "Initalized Time" << std::endl;

    if (SDL_CreateWindowAndRenderer(1280, 720, 0, &WINDOW, &RENDERER) == -1)  {
        SDL_Log("SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
        Term_Services();
    }
    std::cout << "Initalized Window and Renderer" << std::endl;

    /*WINDOW_SURFACE = SDL_GetWindowSurface(WINDOW);
    if (!WINDOW_SURFACE) {
        SDL_Log("SDL_GetWindowSurface: %s\n", SDL_GetError());
        Term_Services();
    }
    std::cout << "Retrevied Window Surface" << std::endl;*/

    SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
        SDL_Log("IMG_Init: %s\n", IMG_GetError());
        Term_Services();
    }
    std::cout << "Initalized Image" << std::endl;

    if(TTF_Init() == -1) {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
        Term_Services();
    }
    std::cout << "Initalized TTF" << std::endl;

    #ifdef EXPERIMENTAL
        std::cout << "Loading Textures" << std::endl;
        Textures_Load();
    #endif

    ARIAL_35 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 35);
    ARIAL_30 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 30);
    ARIAL_27 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 27);
    ARIAL_25 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 25);
    ARIAL_20 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 20);
    ARIAL_15 = TTF_OpenFont("romfs:/resources/font/arial.ttf", 15);
    ARIAL = TTF_OpenFont("romfs:/resources/font/arial.ttf", 20);
    if (!ARIAL_35 || !ARIAL_25 || !ARIAL_15 || !ARIAL) {
        std::cout << "Failure to retrieve fonts" << std::endl;
        Term_Services();
    }
    std::cout << "Retrevied Fonts" << std::endl;
    
    for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            SDL_Log("SDL_JoystickOpen: %s\n", SDL_GetError());
            Term_Services();
        }
    }
    std::cout << "Initalized Input" << std::endl;
}

int main(int argc, char *argv[]) {
    Init_Services();

    Menu_StartChoosing();

    bool isBookReading = false;
    while (run) {
        while (SDL_PollEvent(&EVENT)) {
            switch (EVENT.type) {
                case SDL_JOYAXISMOTION:
                    break;
                case SDL_JOYBUTTONDOWN:
                    // https://github.com/devkitPro/SDL/blob/switch-sdl2/src/joystick/switch/SDL_sysjoystick.c#L52
                    // seek for joystick #0
                    if (EVENT.jbutton.which == 0) {
                        if (EVENT.jbutton.button == 0) {
                            // (A) button down
                        } else if (EVENT.jbutton.button == 10) {
                            // (+) button down
                            run = false;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
        /*SDL_ClearScreen(RENDERER, WHITE);
		SDL_RenderClear(RENDERER);

        SDL_RenderPresent(RENDERER);*/
        /*if (!isBookReading) {
            SDL_ClearScreen(RENDERER, WHITE);
            std::cout << "Cleared Screen" << std::endl;
            SDL_RenderClear(RENDERER);
            std::cout << "Render Clear" << std::endl;*/

            /*int space_index = 0;
            while ((entry = readdir(dir)) != NULL) {
                SDL_DrawText(RENDERER, ARIAL, 20, 20 + (30 * space_index), BLACK, entry->d_name);
                space_index++;
                //cout << entry->d_name << endl;
            }*/

            /*SDL_DrawText(RENDERER, ARIAL, 50, 50, BLACK, "TEST");
            std::cout << "Draw text" << std::endl;

            SDL_RenderPresent(RENDERER);
            std::cout << "Render Present" << std::endl;
        }*/
    }

    Term_Services();
    return 0;
}
