#include "SDL.h"
#include <stdio.h>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define AUDIO_FREQ 22050
#define AUDIO_CHANNELS 1
#define AUDIO_SAMPLES 1024
#define WAV_BUFFER_LEN 10000

void DrawStuff(SDL_Renderer *renderer, SDL_Texture *texture);
void FillWaveBuffer();
void SDLCALL FillAudioBufferCallback(void *unused, Uint8 * stream, int len);
void SDLCALL FillAudioBufferCallback2(void *unused, Uint8 * stream, int len);
struct WavData {
    float* buffer;
    int length;
    int curIndex;
    int remain;
} waveData;

void printAudioStuff(SDL_AudioSpec* spec){
    printf("SDL_AudioSpec: \n");
    printf("freq: %d\n", spec->freq);
    switch(spec->format)
    {
        case AUDIO_S8:
            printf("format: AUDIO_S8\n");
        break;
        case AUDIO_U8:
            printf("format: AUDIO_U8\n");
        break;
        case AUDIO_S16LSB:
            printf("format: AUDIO_S16LSB\n");
        break;        
        case AUDIO_S16MSB:
            printf("format: AUDIO_S16MSB\n");
        break;        
        //case AUDIO_S16SYS:
        //    printf("format: AUDIO_S16SYS\n");
        //break;        
        //case AUDIO_S16:
        //    printf("format: AUDIO_S16\n");
        //break;        
        case AUDIO_U16LSB:
            printf("format: AUDIO_U16LSB\n");
        break;        
        case AUDIO_U16MSB:
            printf("format: AUDIO_U16MSB\n");
        break;        
        //case AUDIO_U16SYS:
         //   printf("format: AUDIO_U16SYS\n");
        //break;        
        //case AUDIO_U16:
        //    printf("format: AUDIO_U16\n");
        //break;        
        case AUDIO_S32LSB:
            printf("format: AUDIO_S32LSB\n");
        break;        
        case AUDIO_S32MSB:
            printf("format: AUDIO_S32MSB\n");
        break;        
       // case AUDIO_S32SYS:
       //     printf("format: AUDIO_S32SYS\n");
        //break;        
        //case AUDIO_S32:
        //    printf("format: AUDIO_S32\n");
        //break;        
        case AUDIO_F32LSB:
            printf("format: AUDIO_F32LSB\n");
        break;        
        case AUDIO_F32MSB:
            printf("format: AUDIO_F32MSB\n");
        break;        
        //case AUDIO_F32SYS:
        //    printf("format: AUDIO_F32SYS\n");
        //break;
        //case AUDIO_F32:
        //    printf("format: AUDIO_F32\n");
        //break;
    }
    printf("channels: %d\n", spec->channels);
    printf("silence: %d\n", spec->silence);
    printf("samples: %d\n", spec->samples);
    printf("size: %d\n", spec->size);
    printf("callback: %x\n", spec->callback);
    printf("userdata: %x\n", spec->userdata);
    
}

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer
    SDL_Rect r;
    SDL_Event event;
    int endLoop = 0;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_AudioSpec audioSpec;
    audioSpec.freq = AUDIO_FREQ;
    audioSpec.format = AUDIO_S16LSB;
    audioSpec.channels = AUDIO_CHANNELS;
    audioSpec.samples = WAV_BUFFER_LEN;
    audioSpec.callback = FillAudioBufferCallback2;
    
    waveData.buffer = malloc(WAV_BUFFER_LEN*4);
    printf("waveData.buffer %x\n", waveData.buffer);
    waveData.length = WAV_BUFFER_LEN;
    waveData.curIndex = 0;
    waveData.remain = WAV_BUFFER_LEN;
    FillWaveBuffer();
    FILE* pFile = fopen("data2.csv", "w+");
    for (int i = 0; i<waveData.length; i++)
    {
        fprintf(pFile, "%f\n", waveData.buffer[i]);
    }
    fclose(pFile);
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return (1);
    }

    if (SDL_OpenAudio(&audioSpec, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open audio: %s\n", SDL_GetError());
    }
    printAudioStuff(&audioSpec);
    
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

void SDLCALL FillAudioBufferCallback2(void *data, Uint8 * stream, int len)
{
    printf("len %d\n", len);
    int toCopy = 0;
    len = len/2; // values are 2 bytes a piece
    int value = 0;
    int streamIndex = 0;
    printf("len %d\n", len);
    int *pBuffer = (int*)stream;
    
    FILE* pFile = fopen("stream.csv", "w+");
    for (int i = 0; i<waveData.length; i++)
    {
        fprintf(pFile, "%d\n", waveData.buffer[i]);
    }
    fclose(pFile);
    while (len > 0)
    {
        if (waveData.curIndex + len >= waveData.length){
            toCopy = waveData.length-waveData.curIndex;
            //memcpy(stream, waveData, 10000);
            //SDL_memcpy(stream, waveData.buffer + waveData.curIndex, toCopy*2);
            
            for (int i =0; i<toCopy; i++){
                value = (int)waveData.buffer[i+waveData.curIndex];
                pBuffer[streamIndex] = value;
                streamIndex++;
            }
            //for (int i = 0; i<len; i++){ stream[i] = 0; }
            FILE* pFile = fopen("stream.csv", "w+");
            streamIndex = 0;
            for (int i = 0; i<toCopy; i++)
            {
                fprintf(pFile, "%d\n", pBuffer[streamIndex]);
            }
            fclose(pFile);
            

            len -= toCopy;
            waveData.curIndex = 0;
            printf("if copied %d samples %d\n", toCopy, len);
        }
        else{
            toCopy = len;
            //SDL_memcpy(stream, waveData.buffer + waveData.curIndex, toCopy*2);
            len -= toCopy;
            waveData.curIndex += toCopy;
            printf("else copied %d samples %d\n", toCopy, len);
        }
    }
    printf("end\n");
}

void SDLCALL FillAudioBufferCallback(void *data, Uint8 * stream, int len)
{
    //printf("requesting sample len %d %x %x\n", len, data, stream);
    
    int copyLen = 0;
    int underflowLength = 0;
    len = len/2;
    
    if (len < waveData.remain){
        copyLen = len;
    }
    else
    {
        // partially filled the buffer because we ran out of data
        copyLen = waveData.remain;
        //SDL_memcpy(stream, waveData.buffer+waveData.curIndex, copyLen);
       for (int i =0;i<copyLen;i++){
            stream[i] = waveData.buffer+waveData.curIndex+i;
       }

        
        // loop back to the beginning of the samples and finish filling
        // the buffer with remaining bytes
        // buffer remain = samples-copyLen
        // reset the buffer pointers back to 0
        underflowLength = len-copyLen;
        printf("underflow remaining in buffer %d  to pull from front %d\n",
            waveData.remain,
            underflowLength);
        waveData.curIndex = 0;
        waveData.remain = waveData.length;
        // SDL_memcpy(stream+copyLen, waveData.buffer, underflowLength);
        for (int i = copyLen; i<underflowLength; i++){
            stream[i] = waveData.buffer+(waveData.curIndex + i);
        }
        
        waveData.curIndex += underflowLength;
        waveData.remain -= underflowLength;
        return;
    }
    
    for (int i =0;i<copyLen;i++){
        stream[i] = waveData.buffer+(waveData.curIndex+i);
    }
    // SDL_memcpy(stream, waveData.buffer+waveData.curIndex, copyLen);

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
    // log so we can see the graph in Excel
    FILE *log = fopen("data.csv", "w+");
    if (log==NULL)
    {
        printf("log file is null\n");
    }
    else{
        fprintf(log, "index,data\n");
    }
    //printf("filling wav buffer %d %x\n", waveData.length, waveData.buffer);
    for (int i = 0; i<waveData.length; i++)
    {
        float f = (/*amplitude of*/100)
            * (sin(
                (/*frequency in herz*/20 
                    * /* 2 pi radians per cycle*/ 2 * 3.14 
                    * /* cycle counter*/i)
                / /* sample rate in samples per cycle (e.g. 220500)*/ AUDIO_FREQ)) + 150;
        
        waveData.buffer[i] = f;
        //printf("%d\n",waveData.buffer[i]);
        //printf("%f\n", f);
        if (log != NULL){
            fprintf(log, "%d,%f\n",i, waveData.buffer[i]);
        }
    }
    
    if (log != NULL){
        fclose(log);
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