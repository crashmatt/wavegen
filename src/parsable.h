/*
 * parsable.h
 *
 *  Created on: 11 Aug 2014
 *      Author: matt
 */

#ifndef PARSABLE_H_
#define PARSABLE_H_

#include <string>

using namespace std;

class parsable {
public:
	parsable();
	virtual ~parsable();
	virtual bool parse_variable(string varstr, string valstr);
	string get_name() {return name;};;
protected:
	string name;
};

#endif /* PARSABLE_H_ */
