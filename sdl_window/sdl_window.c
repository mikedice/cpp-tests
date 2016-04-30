#include "SDL.h"
#include <stdio.h>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define AUDIO_FREQ 22050
#define AUDIO_CHANNELS 1
#define AUDIO_SAMPLES 1024
#define WAV_BUFFER_LEN 8192

void DrawStuff(SDL_Renderer *renderer, SDL_Texture *texture);
void FillWaveBuffer();
void SDLCALL FillAudioBufferCallback(void *unused, Uint8 * stream, int len);
struct WavData {
    short* buffer;
    int length;
    int curIndex;
    int remain;
} waveData;

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer
    SDL_Rect r;
    SDL_Event event;
    int endLoop = 0;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_AudioSpec audioSpec;
    audioSpec.freq = AUDIO_FREQ;
    audioSpec.format = AUDIO_S8;
    audioSpec.channels = AUDIO_CHANNELS;
    audioSpec.samples = AUDIO_SAMPLES;
    audioSpec.callback = FillAudioBufferCallback;
    
    waveData.buffer = malloc(WAV_BUFFER_LEN);
    waveData.length = WAV_BUFFER_LEN;
    waveData.curIndex = 0;
    waveData.remain = WAV_BUFFER_LEN;
    FillWaveBuffer();
    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return (1);
    }

    if (SDL_OpenAudio(&audioSpec, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
    }
    
    SDL_PauseAudio(0);


    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        SCREEN_WIDTH,                               // width, in pixels
        SCREEN_HEIGHT,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    r.w = 100;
    r.h = 50;
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);
 
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                  printf("Keydown\n");
                  endLoop = 1;
                break;
            }
        }

        if (endLoop > 0){
            printf("loop ended\n");
            break;
        }
        
        DrawStuff(renderer, texture);
    }
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}



void SDLCALL FillAudioBufferCallback(void *data, Uint8 * stream, int len)
{
    //printf("requesting sample len %d %x %x\n", len, data, stream);
    
    int copyLen = 0;
    int samples = len; // we were passed a byte array and our samples are 2 bytes long
    
    if (samples < waveData.remain){
        copyLen = samples;
    }
    else if (samples == waveData.remain)
    {
        copyLen = samples;
    }
    else if (samples > waveData.remain)
    {
        copyLen = waveData.remain;
    }
    SDL_memcpy(stream, waveData.buffer+waveData.curIndex, copyLen);

    printf("size %d  curIndex %d  remain %d   len %d  copyLen %d\n", WAV_BUFFER_LEN, 
        waveData.curIndex, 
        waveData.remain,
        len,
        copyLen);
     
    waveData.curIndex += copyLen;
    waveData.remain -= copyLen;
    if (waveData.remain <= 0)
    {
        waveData.curIndex = 0;
        waveData.remain = waveData.length;
    }
}

void FillWaveBuffer()
{
    for (int i = 0; i<waveData.length; i++)
    {
        float f = (100)*(sin((200 * 2 * 3.14 * i)/AUDIO_FREQ));
       
        waveData.buffer[i] = f;
        printf("%d\n",waveData.buffer[i]);
        //printf("%f\n", f);
    }
}

void DrawStuff(SDL_Renderer *renderer, SDL_Texture *texture)
{
    // clear screen
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( renderer );
    
    //Render red filled quad
    SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );        
    SDL_RenderFillRect(renderer, &fillRect );
    SDL_RenderPresent(renderer);
}