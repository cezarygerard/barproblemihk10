/*
 * Person.h
 *
 *  Created on: Dec 5, 2010
 *      Author: hoodles
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <string>

using namespace std;

//Base class for Assistant, Barmaid, Customer, Landlord
class Person {
protected:
	string name;
public:
	string getName() {
		return name;
	}

	//virtual void run() =0;
};


#endif /* PERSON_H_ */
