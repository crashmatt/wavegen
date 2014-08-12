/*
 * parsable.cpp
 *
 *  Created on: 11 Aug 2014
 *      Author: matt
 */

#include <iostream>
#include <stdio.h>

using namespace std;

#include "parsable.h"

parsable::parsable() {
	name = "null";
}

parsable::~parsable() {
}

bool parsable::parse_variable(string varstr, string valstr){
	printf("no variable parser available/n");
	return false;
}
