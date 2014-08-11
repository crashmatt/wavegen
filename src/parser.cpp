/*
 * parser.cpp
 *
 *  Created on: 11 Aug 2014
 *      Author: matt
 */
#include <iostream>
#include <stdio.h>
#include <list>

#include "parser.h"
#include "parsable.h"

using namespace std;


parser::parser() {
}

parser::~parser() {
	// TODO Auto-generated destructor stub
}


bool parser::parse(string str){
	int i;

	printf("parser parsing string : ");
	printf(str.c_str());

	for(i=0; i<parse_list.size(); i++ ){
		parse_list[i]->parse(str);
	}
	return true;
}


void parser::add_parsable(parsable* pparsable){
	parse_list.push_back(pparsable);
}


