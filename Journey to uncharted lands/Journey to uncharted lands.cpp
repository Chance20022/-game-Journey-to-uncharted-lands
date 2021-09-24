#include "Essence.h"

SDL_Texture* loadTexture(std::string path);
bool loadMedia();
void close();
bool init();

//Walking animation
const int WALKING_ANIMATION_FRAMES = 20;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;

SDL_Texture* Background;

int main(int argc, char* args[])
{
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Current animation frame
    int frame = 0;
    int frameSlower = 0;

    //Angle of rotation
    double degrees = 0;

    //Flip type
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (!init()) {
        printf("Error init() \n");
    }

    if (!loadMedia()) printf("Error loadMedia() \n");

    //While application is running
    while (!quit)
    {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_a:
                    degrees -= 15;
                    break;

                case SDLK_d:
                    degrees += 15;
                    break;

                case SDLK_q:
                    flipType = SDL_FLIP_HORIZONTAL;
                    break;

                case SDLK_w:
                    flipType = SDL_FLIP_NONE;
                    break;

                case SDLK_e:
                    flipType = SDL_FLIP_VERTICAL;
                    break;
                }
            }
        }

        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, Background, NULL, NULL);

        //Render current frame
        SDL_Rect* currentClip = &gSpriteClips[frame];
        gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, degrees, NULL, flipType);

        //Update screen
        SDL_RenderPresent(gRenderer);

        //Go to next frame
        frameSlower += 1;
        if (frameSlower % 2 == 0) {
            frame++;
        }

        //Cycle animation
        if (frame >= WALKING_ANIMATION_FRAMES)
        {
            frame = 0;
            frameSlower = 0;
        }
    }
    close();

    return 0;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    int x = 0;
    if (gSpriteSheetTexture.loadFromFile("Image/Walk.png")) {
        printf("Failed to load walking animation texture!\n");
        success = false;
    }
    else {
        for (int i = 0; i < 20; i++) {//109 - ширина, 159,5 - высота
            //Set sprite clips
            gSpriteClips[i].x = x;
            gSpriteClips[i].y = 0;
            gSpriteClips[i].w = 109;
            gSpriteClips[i].h = 158;

            x += 109;
        }
    }

    Background = loadTexture("Image/background.png");

    return success;
}

void close()
{
    gSpriteSheetTexture.free();

    //Destroy window    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 222, 222, 222, 222);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

SDL_Texture* loadTexture(std::string path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}