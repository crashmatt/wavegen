/*
 * wgchunk.h
 *
 *  Created on: 17 Aug 2014
 *      Author: matt
 */

#ifndef WGCHUNK_H_
#define WGCHUNK_H_

#include "setup.hpp"

class wgchunk {
public:
	wgchunk();
	virtual ~wgchunk();
	float buffer[CHANNELS*FRAME_SIZE];
};

#endif /* WGCHUNK_H_ */

