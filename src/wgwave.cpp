/*
 * wgwave.cpp
 *
 *  Created on: 9 Aug 2014
 *      Author: matt
 */

#include <iostream>
#include "wgwave.hpp"
#include <cmath>
using namespace std;

#define PI 3.14159265

wgwave::wgwave(){
	wavesteps = 0;
	pbuffer = NULL;
};

void wgwave::make_sin(unsigned int steps){
	unsigned int i;

	if(pbuffer != NULL){
		if(sizeof(pbuffer) != steps){
			delete pbuffer;
			pbuffer = NULL;
		}
	};

	if(pbuffer == NULL){
		pbuffer = new(float[steps]);
	};

	for(i=0; i<steps; i++){
		pbuffer[i] = sin(i * (2 * PI) / steps);
	};
	wavesteps = steps;
};

float wgwave::get_step(unsigned int step){
	if(step >= wavesteps) return 0.0;

	if(pbuffer != NULL){
		return pbuffer[step];
	}
	else return 0.0;
}


float wgwave::get_phase(float phase){
	if(pbuffer == NULL) return 0.0;
	int step = int(phase * wavesteps);
	if(step >= (int) wavesteps) step = 0;
	if(step < 0) step = wavesteps - 1;
	return pbuffer[step];
};



