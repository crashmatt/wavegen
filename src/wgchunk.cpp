/*
 * wgchunk.cpp
 *
 *  Created on: 17 Aug 2014
 *      Author: matt
 */

#include "wgchunk.h"

wgchunk::wgchunk() {
	for(int i=0; i<(FRAME_SIZE*CHANNELS); i++){
		buffer[i] = 0.0;
	}
}

wgchunk::~wgchunk() {
	// TODO Auto-generated destructor stub
}

//wgchunk wgchunk::operator=(const wgchunk&  other){
//	memcpy( buffer, other.buffer, sizeof(float)*(FRAME_SIZE*CHANNELS) );
//	return this;
//}
