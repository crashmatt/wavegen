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
	string module, variable, value;
	size_t pos = 0;

	printf("parser parsing string : ");
	printf(str.c_str());

	pos = str.find(':');
	module = str.substr(0, pos);
	str.erase(0, pos+1);
	pos = str.find(':');
	variable = str.substr(0, pos);
	str.erase(0, pos+1);
	value = str;

	for(i=0; i<parse_list.size(); i++ ){
		if(parse_list[i]->get_name() == module){
			parse_list[i]->parse_variable(variable, value);
		}
	}
	return true;
}


void parser::add_parsable(parsable* pparsable){
	parse_list.push_back(pparsable);
}


