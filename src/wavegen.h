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
	wgwave wave;
	wgmodulator modulator;

	parser wg_parser;

	/* this variable is our reference to the waveform generator thread */
	pthread_t chunkgen_thread;
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

	queue <wgchunk> chunks;
};


#endif /* WAVEGEN_H_ */
