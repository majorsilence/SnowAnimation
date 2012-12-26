#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>

const int NUM_FLAKES = 100;
const int MAX_SPEED = 3;


typedef struct flake_s
{
        int x, y; // position on the screen
        int dx, dy; // movment vector
} flake_t, *flake_p;

static flake_t flakes[NUM_FLAKES];
static SDL_Surface *screen;
static SDL_Surface *flake;

static void init_flakes()
{

    for(int i=0; i<NUM_FLAKES; i++)
    {

        flakes[i].x = rand() % screen->w;
        flakes[i].y = rand() % (screen->h / 2);
        flakes[i].dx = (rand() % (MAX_SPEED * 2)- MAX_SPEED)+1;
        flakes[i].dy = abs((rand() % (MAX_SPEED * 2)) - MAX_SPEED)+1;

    }

}


static void move_flakes()
{

    for (int i=0; i<NUM_FLAKES; i++)
    {
        flakes[i].x += flakes[i].dx;
        flakes[i].y += flakes[i].dy;

        if(flakes[i].y > screen->h -1)
        {
            flakes[i].x = rand() % screen->w;
            flakes[i].y = 0);
        }
    }

}


static void draw_flakes()
{
    SDL_Rect src, dest;

    for(int i=0; i<NUM_FLAKES; i++)
    {

        src.x = 0;
        src.y = 0;
        src.w = flake->w;
        src.h = flake->h;

        // The flake's position specifies its center
        dest.x = flakes[i].x - flake->w /2;
        dest.y = flakes[i].y - flake->h /2;
        dest.w = flake->w;
        dest.h = flake->h;

        SDL_BlitSurface(flake, &src, screen, &dest);

    }

}

int main ( int argc, char** argv )
{

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(640, 480, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface *temp;
    temp = SDL_LoadBMP("snow_flake.bmp");
    if (!temp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL , (Uint16) SDL_MapRGB(temp->format, 0, 0, 0));
    flake = SDL_DisplayFormat(temp);
    if (flake == NULL)
    {
        printf("Unable to convert bitmap.\n");
    }
    SDL_FreeSurface(temp);

    init_flakes();


    // program main loop
    bool done = false;

    const int FPS = 120;
    const int framems = 1000 / FPS;
    while (!done)
    {
        int startms = SDL_GetTicks();

        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE



        SDL_Rect src, dest;

        src.x=0;
        src.y=0;
        src.w=screen->w;
        src.h=screen->h;
        dest=src;
        SDL_FillRect(screen, &dest, (Uint16) SDL_MapRGB(flake->format, 0, 0, 0));

        draw_flakes();

        SDL_Flip(screen);

        move_flakes();


        // DRAWING ENDS HERE


        int endms = SDL_GetTicks();
        // how long to delay
        int delayms = framems - (endms - startms);
        if (delayms > 0)
        {
            SDL_Delay(delayms);
        }

    } // end main loop


    SDL_FreeSurface(flake);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;

}
