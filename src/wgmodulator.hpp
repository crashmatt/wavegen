//============================================================================
// Name        : wavegen.cpp
// Author      : crashmatt
// Version     :
// Copyright   : 
// Description : audio wave function generator
//============================================================================

#ifndef WGMODULATOR_HPP
#define WGMODULATOR_HPP

#include "parsable.h"

//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <queue>
#include <cmath>
using namespace std;


class wgmodulator : public parsable{
private:
	enum modstate
	{
		MS_CONSTANT = 0,
	    MS_ATTACK,
	    MS_HOLD,
	    MS_DECAY,
	    MS_END,
	};

	float 	mod_period;
	float 	attack_time;
	float 	decay_time;
	float 	hold_time;

	float 	attack_step;
	float	decay_step;

	float 	modulation;		// modulation output
	float 	mod_time;		// time into the modulation period
	modstate mod_state;		// state

	void modulate(void);

public:
	wgmodulator(void);
	void timestep(void);
	float get_modulation(void);
	void set_attack(float time);
	void set_decay(float time);
	void set_hold(float time);
	void set_period(float time);
	void set_pulse_shape(float period, float attack, float hold, float decay);
	void set_pulsing(void);
	void set_constant(void);
	bool parse_variable(string varstr, string valstr);
};

#endif	// WGMODULATOR_HPP

