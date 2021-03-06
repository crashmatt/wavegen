//============================================================================
// Name        : wavegen.cpp
// Author      : crashmatt
// Version     :
// Copyright   : 
// Description : audio wave function generator
//============================================================================

#include "setup.hpp"
#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <cmath>
#include <string>
#include <stdlib.h>
using namespace std;


#include <math.h>

#include "wgmodulator.hpp"

#define uint unsigned int

#define PI 3.14159265

wgmodulator::wgmodulator(void){
	name = "modulator";

	modulation = 0.0;
	mod_time = 0.0;
	mod_period = 1.0;
	mod_state = MS_CONSTANT;
	hold_time = 0.1;

	set_attack(0.0);
	set_decay(0.0);
}

float wgmodulator::get_modulation(void) {return modulation;};

void wgmodulator::set_attack(float time){
	attack_time = time;
	if(attack_time <= 1/SAMPLE_RATE) {
		attack_step = 1.0;
	}
	else{
		attack_step = SAMPLE_RATE / time;
	}
}

void wgmodulator::set_decay(float time){
	decay_time = time;
	if(decay_time <= 1/SAMPLE_RATE) {
		decay_step = 1.0;
	}
	else{
		decay_step = SAMPLE_RATE / time;
	}
}

void wgmodulator::set_pulse_shape(float period, float attack, float hold, float decay){
	mod_period = period;
	set_attack(attack);
	set_decay(decay);
	hold_time = hold;
}

void wgmodulator::set_pulsing(void){
	if( (mod_state == MS_CONSTANT) or (mod_state == MS_MUTE) ) mod_state = MS_PULSE_PAUSE;
}

void wgmodulator::set_constant(void){
	mod_state = MS_CONSTANT;
}

void wgmodulator::set_mute(void){
	mod_state = MS_MUTE;
}


void wgmodulator::modulate(void){
	float off_time = mod_period - attack_time - decay_time - hold_time;

	if( (mod_state == MS_PULSE_PAUSE) and (mod_time >= off_time ) ){
		mod_state = MS_ATTACK;
		mod_time -= off_time;
	}

	if(mod_state == MS_ATTACK){
		modulation += attack_step;
		if(modulation >= 1.0){
			mod_state = MS_HOLD;
			modulation = 1.0;
			mod_time -= attack_time;
		}
	}

	if( (mod_state == MS_HOLD) and (mod_time >= hold_time) ){
		mod_state = MS_DECAY;
		mod_time -= hold_time;
	}

	if( mod_state == MS_DECAY){
		modulation -= decay_step;
		if(modulation <= 0.0){
			modulation = 0.0;
			mod_state = MS_PULSE_PAUSE;
			mod_time = 0;
		}
	}

	if( mod_state == MS_MUTE){
		modulation -= decay_step;
		if(modulation <= 0.0){
			modulation = 0.0;
		}
	}

	if( mod_state == MS_CONSTANT){
		modulation += attack_step;
		if(modulation >= 1.0){
			modulation = 1.0;
		}
	}

}

void wgmodulator::timestep(void){
	modulate();

	mod_time += (1 / (float) SAMPLE_RATE);
	if(mod_time > mod_period) {
		mod_time -= mod_period;
	}
}

void wgmodulator::set_period(float time){ mod_period = time;};


bool wgmodulator::parse_variable(string varstr, string valstr){
	float val;
	if(varstr == "attack"){
		val = strtof(valstr.c_str(), NULL );	//
		set_attack(val);
		return true;
	}
	else if(varstr == "decay"){
		val = strtof(valstr.c_str(), NULL );	//
		set_decay(val);
		return true;
	}
	else if( (varstr == "sustain") or (varstr == "hold")){
		hold_time = strtof(valstr.c_str(), NULL );	//
		return true;
	}
	else if(varstr == "period"){
		mod_period = strtof(valstr.c_str(), NULL );	//
		return true;
	}
	else if(varstr == "pulsing"){
		set_pulsing();
		return true;
	}
	else if(varstr == "constant"){
		set_constant();
		return true;
	}
	else if(varstr == "mute"){
		set_mute();
		return true;
	}
	else
		return false;
}
