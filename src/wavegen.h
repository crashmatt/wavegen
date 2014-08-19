/*
 * wavegen.h
 *
 *  Created on: 11 Aug 2014
 *      Author: matt
 */

#ifndef WAVEGEN_H_
#define WAVEGEN_H_

#include <queue>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <mutex>

using namespace std;

#include "parsable.h"
#include "parser.h"


#include "wgwave.hpp"
#include "wgmodulator.hpp"
#include "wgchunk.h"


class wavegen : public parsable{
private:
	float phase;
	float frequency;
	float phase_step;	// step for a given sample rate and frequency
	float amplitude;
	float output;

	float modulation_time;
	queue <string> parseq;
	pthread_mutex_t parseq_mutex;

	wgwave wave;
	wgmodulator modulator;

	parser wg_parser;

	/* this variable is our reference to the waveform generator thread */
	pthread_mutex_t chunks_mutex;
	pthread_t chunkgen_thread;
	queue <wgchunk> chunks;
	bool chunkgen_exit;

public:
	wavegen();
	virtual ~wavegen();
	void time_step();
	void set_frequency(float freq);
	float get_waveout();
	void push_command(char* line);
	void parse_all(void);
	bool parse_variable(string varstr, string valstr);
	bool get_exit_request() {return chunkgen_exit;};

	int get_chunks_size(void) {return chunks.size();};
	void push_chunk(wgchunk* pcnk);
	wgchunk* get_first_chunk();
	void pop_chunk();
};


#endif /* WAVEGEN_H_ */
