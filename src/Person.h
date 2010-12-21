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

///Base class for Assistant, Barmaid, Customer, Landlord
class Person {
protected:

	///name of the person
	string name;

public:

	///gets the name of the person
	///@return	the name of the person
	string getName() {
		return name;
	}

	///virtual placeholder for lastOrder() method
	virtual void lastOrder()
	{
		return;
	}

	///virtual placeholder for greetLandlord() method
	///@param leaving	true if person is leaving the bar, false if entering
	virtual void greetLandlord(bool leaving)
	{
		return;
	}

	///virtual placeholder for closeUp() method
	virtual void closeUp()
	{
		return;
	}
};


#endif /* PERSON_H_ */
