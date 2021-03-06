//============================================================================
// Name        : wavegen.cpp
// Author      : crashmatt
// Version     :
// Copyright   : 
// Description : audio wave function generator
//============================================================================

#include "setup.hpp"

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


using namespace std;

#include "wavegen.h"

#include <math.h>
#include "portaudio.h"

#include "parser.h"

#define uint unsigned int

#define PI 3.14159265


//Prototype for wave chunk generator thread
void *chunk_generator(void *);


wavegen::wavegen(){
	name = "wavegen";

	pthread_mutex_init(&chunks_mutex, NULL);
	pthread_mutex_init(&parseq_mutex, NULL);

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
    pthread_mutex_destroy(&parseq_mutex);
    pthread_mutex_destroy(&chunks_mutex);
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
    pthread_mutex_lock(&parseq_mutex);
	parseq.push(string(line));
    pthread_mutex_unlock(&parseq_mutex);
}

void wavegen::parse_all(void){
	string cmd;

    pthread_mutex_lock(&parseq_mutex);
	while(parseq.empty() == false){
		cmd = parseq.front();
		parseq.pop();
		wg_parser.parse(cmd);
	}
    pthread_mutex_unlock(&parseq_mutex);
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

void wavegen::push_chunk(wgchunk* pcnk){
	pthread_mutex_lock(&chunks_mutex);
	chunks.push(*pcnk);
	pthread_mutex_unlock(&chunks_mutex);
}


wgchunk* wavegen::get_first_chunk(){
//	pthread_mutex_lock(&chunks_mutex);
	return &chunks.front();
//	pthread_mutex_unlock(&chunks_mutex);
};

void wavegen::pop_chunk() {
	pthread_mutex_lock(&chunks_mutex);
	chunks.pop();
	pthread_mutex_unlock(&chunks_mutex);
};


void *chunk_generator(void *void_ptr)
{
	wavegen* pwgen = (wavegen*) void_ptr;
	wgchunk cnk;

	printf("Start wave generator\n");
	while(pwgen->get_exit_request() == false){
		if(pwgen->get_chunks_size() <= CHUNK_QUEUE_SIZE){
			pwgen->parse_all();
			for(int i = 0; i < FRAME_SIZE; i++){
				pwgen->time_step();
				cnk.buffer[i] = pwgen->get_waveout();
			}
			pwgen->push_chunk(&cnk);
		}
		Pa_Sleep(10);
	}
	printf("End wave generator\n");
	return NULL;
}

