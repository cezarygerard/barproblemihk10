/*
 * Assistant.h
 *
 *  Created on: Dec 3, 2010
 *      Author: hoodles
 */

#ifndef ASSISTANT_H_
#define ASSISTANT_H_

#include "Person.h"

class Assistant: public Person{
public:
	Assistant();
	virtual ~Assistant();
	void run();
};

#endif /* ASSISTANT_H_ */
