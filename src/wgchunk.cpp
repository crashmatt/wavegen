/*
 * wgchunk.cpp
 *
 *  Created on: 17 Aug 2014
 *      Author: matt
 */

#include "wgchunk.h"

wgchunk::wgchunk() {
	for(int i=0; i<sizeof(buffer); i++){
		buffer[i] = 0.0;
	}
}

wgchunk::~wgchunk() {
	// TODO Auto-generated destructor stub
}
