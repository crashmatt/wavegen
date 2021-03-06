/*
 * wgen_app.cpp
 *
 *  Created on: 18 Aug 2014
 *      Author: matt
 */

#include <portaudio.h>
#include <pa_linux_alsa.h>

#include "wavegen.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cstring>
#include <queue>

//Named pipes
#include <sys/types.h>  // mkfifo
#include <sys/stat.h>   // mkfifo
#include <fcntl.h>
#include <errno.h>

using namespace std;

time_t timeout = 0;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    wavegen *wgen = (wavegen*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    wgchunk* pcnk;
    if(wgen->get_chunks_size() > 0){
    	pcnk = wgen->get_first_chunk();
//    	for(i=0; i < CHANNELS*FRAME_SIZE; i++){
    	for(i=0; i < FRAME_SIZE; i++){
    		out[i] = pcnk->buffer[i];
    	}
    	wgen->pop_chunk();
    }
    else
    {
    	memset(outputBuffer, 0, sizeof(float)*FRAME_SIZE);
    	fprintf(stderr, "error: no chunks in chunk queue\n");
//    	memset(outputBuffer, 0, sizeof(float)*CHANNELS*FRAME_SIZE);
    }

    return 0;
}

/*******************************************************************/
//static paTestData data;
int main(void);
int main(void)
{
	wavegen wgen = wavegen();
    PaStream *stream;
    PaError err;

    char mesg[1000];

    // named pipe
    int status;
    struct stat   buffer;
    FILE * pfifoFile = NULL;

    char* pPath = getenv ("HOME");
    strcpy(mesg, pPath);
    strcat(mesg,"/wavegen.fifo");

    fprintf(stderr, "fifo path : ");
    fprintf(stderr, mesg);
    fprintf(stderr, "\n");

    status = stat(mesg, &buffer);
    if(status != 0){
        status = mkfifo(mesg, 0666);
        if (status < 0) {
        	fprintf(stderr, "error creating fifo at :");
        	fprintf(stderr , mesg);
        	fprintf(stderr , "/n");
        }
    }

#if (DISABLE_SOUND != 1)
    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;


    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                1,   /* channels */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                FRAME_SIZE, /* frames per buffer */
                                patestCallback,
                                &wgen );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

#endif //(DISABLE_SOUND != 1)

    fprintf(stderr, "opening fifo :");
    fprintf(stderr, mesg);
    fprintf(stderr, "\n");
    pfifoFile = fopen(mesg, "r");

    while(pfifoFile != NULL){
    	if(fgets(mesg, 100, pfifoFile) != NULL){
        	wgen.push_command(mesg);
    	}
    	else{
    		//Wait 10ms before checking fifo again
    		Pa_Sleep(10);
    	}


    }



#if (DISABLE_SOUND != 1)
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    Pa_Terminate();
#endif //(DISABLE_SOUND != 1)

    printf("Test finished.\n");
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}


