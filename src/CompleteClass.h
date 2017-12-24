/*
 * CompleteClass.h
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#include "GraphClass.h"

#ifndef COMPLETECLASS_H_
#define COMPLETECLASS_H_

template <typename Graph>
class CompleteClass {


public:
	CompleteClass();
	virtual ~CompleteClass();
	Graph get(int n);

};

#endif /* COMPLETECLASS_H_ */
