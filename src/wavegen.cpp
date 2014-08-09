//============================================================================
// Name        : wavegen.cpp
// Author      : crashmatt
// Version     :
// Copyright   : 
// Description : audio wave function generator
//============================================================================

#include <portaudio.h>
#include <pa_linux_alsa.h>

#define NUM_SECONDS   	(10)

#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <cmath>
using namespace std;

#include "wgmodulator.hpp"

#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#include "setup.hpp"

#define uint unsigned int

#define PI 3.14159265

#include "wgwave.hpp"



class wgchunk{
public:
	wgchunk();
	float buffer[CHANNELS*FRAME_SIZE];
};

wgchunk::wgchunk()
{
	uint i;
	for(i=0; i<sizeof(buffer); i++){
		buffer[i] = 0.0;
	}
}


class wavegen{
private:
	float phase;
	float frequency;
	float phase_step;	// step for a given sample rate and frequency
	float amplitude;
	float output;

	float modulation_time;
	queue <wgchunk> chunks;
	wgwave wave;
	wgmodulator modulator;

public:
	float left_phase;
	float right_phase;
	wavegen();
	void time_step();
	void set_frequency(float freq);
	float get_waveout();
};


wavegen::wavegen(){
	output = 0.0;
	phase = 0;
	amplitude = 0.1;

	modulation_time = 0;

	set_frequency(250.0);
	wave.make_sin(TABLE_LENGTH);
	modulator.set_pulse_shape(0.2, 0.01, 0.05, 0.01);
	modulator.set_pulsing();
}

void wavegen::set_frequency(float freq){
	frequency = freq;
	phase_step = frequency / (float) SAMPLE_RATE;
}

void wavegen::time_step(){
	output = wave.get_phase(phase) * amplitude * modulator.get_modulation();

	phase += phase_step;
	if(phase > 1.0) phase -= 1.0;

	modulator.timestep();
}

float wavegen::get_waveout() {return output;};

//Prototype for soundgen thread
void *wave_gen(void *);


/*
typedef struct
{
    float left_phase;
    float right_phase;
}
paTestData;
*/



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

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = wgen->get_waveout();  /* left */
        *out++ = wgen->get_waveout();  /* right */
    	wgen->time_step();
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
//        wgen->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
//        if( wgen->left_phase >= 1.0f ) wgen->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
//        wgen->right_phase += 0.03f;
//        if( wgen->right_phase >= 1.0f ) wgen->right_phase -= 2.0f;
    }
    return 0;
}

/*******************************************************************/
//static paTestData data;
int main(void);
int main(void)
{
	/* this variable is our reference to the waveform generator thread */
	pthread_t wg_thread;

	wavegen wgen = wavegen();
    PaStream *stream;
    PaError err;

    printf("PortAudio Test: output sawtooth wave.\n");

    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    /* create a second thread which executes inc_x(&x) */
    if(pthread_create(&wg_thread, NULL, wave_gen, 0)) {

    fprintf(stderr, "Error declared creating thread\n");
    return 1;

    }

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

    /* Sleep for several seconds. */
    Pa_Sleep(NUM_SECONDS*1000);

    /* wait for the second thread to finish */
    if(pthread_join(wg_thread, NULL)) {
    	fprintf(stderr, "Error joining wave gen thread\n");
    }

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    Pa_Terminate();
    printf("Test finished.\n");
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}


void *wave_gen(void *void_ptr)
{
	printf("Start wave generator\n");
	sleep(5);
	printf("End wave generator\n");
}
