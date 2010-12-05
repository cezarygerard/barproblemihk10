/*
 * Person.h
 *
 *  Created on: Dec 5, 2010
 *      Author: hoodles
 */

#ifndef PERSON_H_
#define PERSON_H_

//Base class for Assistant, Barmaid, Customer, Landlord
class Person {
protected:
	const char* name;
	int tid;
public:
	const char* getName() {
		return name;
	}
	int getTid() {
		return tid;
	}
	virtual void run() =0;
};


#endif /* PERSON_H_ */
