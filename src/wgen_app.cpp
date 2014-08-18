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
#include <string>
#include <cstring>
#include <pthread.h>
#include <queue>

//Named pipes
#include <sys/types.h>  // mkfifo
#include <sys/stat.h>   // mkfifo
#include <fcntl.h>
#include <errno.h>

using namespace std;


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

    wgen->parse_all();

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = wgen->get_waveout();  /* left */
        *out++ = wgen->get_waveout();  /* right */
    	wgen->time_step();
    }
    return 0;
}

/*******************************************************************/
//static paTestData data;
int main(void);
int main(void)
{
	/* this variable is our reference to the waveform generator thread */
//	pthread_t chunkgen_thread;

	wavegen wgen = wavegen();
    PaStream *stream;
    PaError err;

    char mesg[1000];

    // named pipe
    int num, fifo, status;
    struct stat   buffer;
//    char* line = NULL;
    FILE * pfifoFile = NULL;
//    size_t len = 0;
//    ssize_t read;

    char* pPath = getenv ("HOME");
    strcpy(mesg, pPath);
    strcat(mesg,"/wavegen.fifo");

    fprintf(stderr, "fifo path : ", mesg, "/n");
    fprintf(stderr, mesg);
    fprintf(stderr, "\n");

    status = stat(mesg, &buffer);
    if(status != 0){
        status = mkfifo(mesg, 0666);
        if (status < 0) {
        	printf("error creating fifo at :", mesg, "/n");
        }
    }

#if (DISABLE_SOUND != 1)
    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    /* create a thread executing wave generation */
//    if(pthread_create(&chunkgen_thread, NULL, chunk_generator, 0)) {
//    	fprintf(stderr, "Error declared creating chunck generatorthread\n");
//    	return 1;
//    }

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                2,          /* stereo output */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                FRAME_SIZE, /* frames per buffer */
                                patestCallback,
                                &wgen );
    if( err != paNoError ) goto error;


    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

#endif //(DISABLE_SOUND != 1)

    printf("opening fifo :", mesg);
    pfifoFile = fopen(mesg, "r");

    while(pfifoFile != NULL){
    	if(fgets(mesg, 100, pfifoFile) != NULL){
//       	printf("received messge : ");
        	printf(mesg);
        	wgen.push_command(mesg);
    	}
    	else{
    		//Wait 10ms before checking fifo again
    		Pa_Sleep(10);
    	}


    }

    /* wait for the second thread to finish */
//    if(pthread_join(chunkgen_thread, NULL)) {
//    	fprintf(stderr, "Error joining wave communication thread\n");
//    }


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


