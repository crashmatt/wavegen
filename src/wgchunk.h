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
	float buffer[FRAME_SIZE];
//	float buffer[CHANNELS*FRAME_SIZE];
//	wgchunk operator=(const wgchunk& other);
};

#endif /* WGCHUNK_H_ */

