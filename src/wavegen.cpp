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
#define DISABLE_SOUND 0

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <cmath>
#include <string>
#include <cstring>

// sockets
//#include <sys/socket.h>
//#include <netinet/in.h>

//Named pipes
//#include <sys/types.h>  // mkfifo
//#include <sys/stat.h>   // mkfifo
//#include <fcntl.h>
//#include <errno.h>

using namespace std;

#include "wavegen.h"

#include <math.h>
#include "portaudio.h"

#include "parser.h"
#include "setup.hpp"

#define uint unsigned int

#define PI 3.14159265


//Prototype for wave chunk generator thread
void *chunk_generator(void *);



wavegen::wavegen(){
	name = "wavegen";

	output = 0.0;
	phase = 0;
	amplitude = 0.1;

	modulation_time = 0;

	set_frequency(250.0);
	wave.make_sin(TABLE_LENGTH);
	modulator.set_pulse_shape(5.0, 0.01, 0.05, 0.01);
	modulator.set_pulsing();

	wg_parser.add_parsable(this);
	wg_parser.add_parsable(&modulator);
	wg_parser.add_parsable(&wave);

	chunkgen_exit = false;

	/* create a thread executing wave generation */
    if(pthread_create(&chunkgen_thread, NULL, chunk_generator, this)) {
    	fprintf(stderr, "Error creating chunk generator thread\n");
    }

}

wavegen::~wavegen(){
	chunkgen_exit = true;

    /* wait for the second thread to finish */
    if(pthread_join(chunkgen_thread, NULL)) {
    	fprintf(stderr, "Error joining chunk generation thread\n");
    }
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


void wavegen::push_command(char* line){
	parseq.push(string(line));
}

void wavegen::parse_all(void){
	string cmd;

	while(parseq.empty() == false){
		cmd = parseq.front();
		parseq.pop();
		wg_parser.parse(cmd);
	}

}

bool wavegen::parse_variable(string varstr, string valstr){

	if(varstr == "amplitude"){
		amplitude = strtof(valstr.c_str(), NULL );
		return true;
	}
	else if(varstr == "frequency"){
		set_frequency(strtof(valstr.c_str(), NULL ));
		return true;
	}
	else
		return false;
}



void *chunk_generator(void *void_ptr)
{
	wavegen* pwgen = (wavegen*) void_ptr;
	wgchunk cnk;

	float step = 0;

	printf("Start wave generator\n");
	while(pwgen->get_exit_request() == false){
		if(pwgen->chunks.size() < 2){
			pwgen->parse_all();

			for(int i = 0; i < FRAME_SIZE; i++){
				pwgen->time_step();
				cnk.buffer[i*2] = pwgen->get_waveout();
				cnk.buffer[(i*2)+1] = pwgen->get_waveout();
			}
			pwgen->chunks.push(cnk);
		}
		Pa_Sleep(10);
	}
	printf("End wave generator\n");
	return NULL;
}

