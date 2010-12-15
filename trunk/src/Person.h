/*
 * Person.h
 *
 *  Created on: Dec 5, 2010
 *      Author: hoodles
 */

#ifndef PERSON_H_
#define PERSON_H_

#include "Common.h"

using namespace std;

//Base class for Assistant, Barmaid, Customer, Landlord
class Person {
protected:
	string name;
public:
	string getName() {
		return name;
	}
	virtual void lastOrder()
	{
		return;
	}
	virtual void greetLandlord(bool leaving)
	{
		return;
	}
	virtual void closeUp()
	{
		return;
	}
};


#endif /* PERSON_H_ */
