/*
 * wgwave.hpp
 *
 *  Created on: 9 Aug 2014
 *      Author: matt
 */

#ifndef WGWAVE_HPP_
#define WGWAVE_HPP_

#include "parsable.h"


class wgwave : public parsable{
private:
	unsigned int wavesteps;
	float* pbuffer;
public:
	wgwave();
	void make_sin(unsigned int steps);
	float get_step(unsigned int step);
	float get_phase(float phase);
	float get_phase_interp(float phase);
	bool parse_variable(string varstr, string valstr);
};



#endif /* WGWAVE_HPP_ */
