/*
 * parser.h
 *
 *  Created on: 11 Aug 2014
 *      Author: matt
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "parsable.h"
#include <vector>
#include <string>

using namespace std;

class parser {
public:
	parser();
	virtual ~parser();
	bool parse(string str);
	void add_parsable(parsable* pparsable);
	void bluff(void);
private:
	vector<parsable*> parse_list;
};

#endif /* PARSER_H_ */
