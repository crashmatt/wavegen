/*
 * wgwave.hpp
 *
 *  Created on: 9 Aug 2014
 *      Author: matt
 */

#ifndef WGWAVE_HPP_
#define WGWAVE_HPP_


class wgwave{
private:
	unsigned int wavesteps;
	float* pbuffer;
public:
	wgwave();
	void make_sin(unsigned int steps);
	float get_step(unsigned int step);
	float get_phase(float phase);
	float get_phase_interp(float phase);
};



#endif /* WGWAVE_HPP_ */
