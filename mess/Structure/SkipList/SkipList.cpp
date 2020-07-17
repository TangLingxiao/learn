/*
	File: SkipList.C
	Author: Bruno Grossniklaus, 13.11.97
	Version: 1.0
	History:
	13.11.97; Gro; Version 1.0
	9/98 C++'ified and added iterator by Daniel Green - dgreen@superliminal.com
*/

#include <limits.h> // INT_MAX
#include <assert.h>

#include "SkipList.h"


#include "Log/Logger.h"
#include <limits.h> // INT_MAX

// Big hack for now since original code was int centric - DG
#define VAL_MAX ((Value)INT_MAX)




////////////////////////////////////////////////////////////////////////////////
// SKIPLIST
////////////////////////////////////////////////////////////////////////////////

/*
 * get new element level using given probability
 */
static long getNewLevel(long maxLevel, float probability)
{
	long newLevel = 0;
#ifndef drand48
extern double genrand();
#define drand48 genrand
#endif
	
	while ( (newLevel < maxLevel - 1) && (drand48() < probability) ) { // fast hack. fix later
		newLevel++;
	}

	return(newLevel);
}

SkipList::SkipList(float probability)
	: myProbability(probability)
{
	myHeader = new SkipListElement(0, VAL_MAX, 0); // get memory for header element
	//myHeader->setKey(VAL_MAX);
}

SkipList::SkipList()
{
	myProbability = 0.5;
	myHeader = new SkipListElement(0, VAL_MAX, 0); // get memory for header element
	//myHeader->setKey(VAL_MAX);
}

SkipList::~SkipList()
{
	empty(); // delete all data containers
	delete myHeader; // free memory for header element
}

void SkipList::insert(const Key searchKey, const Value value)
{
	int i;
	long newLevel;
	SkipListElement* element;
	SkipListElement* nextElement;
	SkipListElement	update(SKIPLIST_MAXLEVEL, nullptr, nullptr);

	// scan all levels while key < searchKey
	// starting with header in his level
	element = myHeader;
	for(i=myHeader->getLevel(); i>=0; i--) {
		nextElement = element->getElement(i);
		while( (nextElement != nullptr) && (nextElement->getKey() < searchKey) ) {
			element=nextElement;
			nextElement = element->getElement(i);
		}
		update.setElement(i, element); // save level pointer
	}

	element=element->getElement(0); // key is < searchKey

	// key exists. set new value
	if( (element != nullptr) && (element->getKey() == searchKey) ) {
		//cout << "*"; // print * to cout. remove later!
		element->setValue(value);
	}
	
	// new key. add to list
	else {
		// get new level and fix list level
		newLevel = getNewLevel(SKIPLIST_MAXLEVEL, myProbability); // get new level
		if (newLevel > myHeader->getLevel() ) { // adjust header level
			for (i=myHeader->getLevel() + 1; i<=newLevel; i++) {
				update.setElement(i, myHeader); // adjust new pointer of new element
			}
			myHeader->setLevel(newLevel); // set new header level
		}
		
		// make new element
		element = new SkipListElement(newLevel, searchKey, value);
		for (i=0; i<= newLevel; i++ ) { // scan all levels
			// set next pointer of new element
			element->setElement(i, update.getElement(i)->getElement(i));
		 update.getElement(i)->setElement(i, element);
		}
		
		/*
		// fix level of element
		if(newLevel < update.getLevel()) {
			update.setLevel(newLevel);
		}
		*/
	}
}

Value SkipList::search(const Key searchKey) const
{
	int i;
	SkipListElement* element;
	SkipListElement* nextElement;

	element = myHeader;
	for(i=myHeader->getLevel(); i>=0; i--) {
		nextElement = element->getElement(i);
		while( (nextElement != nullptr) && (nextElement->getKey() < searchKey) ) {
			element=nextElement;
			nextElement = element->getElement(i);
		}
	}

	element=element->getElement(0); // key is < searchKey

	// if key exists return value else ERROR
	if( (element != nullptr) && (element->getKey() == searchKey) )
		return(element->getValue());
	else
		return(SKIPLIST_NOT_FOUND);
}


void SkipList::free(const Key searchKey)
{
	int i;
	SkipListElement* element;
	SkipListElement* nextElement;
	SkipListElement	update(SKIPLIST_MAXLEVEL, nullptr, nullptr);

	// scan all levels while key < searchKey
	// starting with header in this level
	element = myHeader;
	for(i=myHeader->getLevel(); i>=0; i--) {
		nextElement = element->getElement(i);
		while( (nextElement != nullptr) && (nextElement->getKey() < searchKey) ) {
			element=nextElement;
			nextElement = element->getElement(i);
		}
		update.setElement(i, element); // save level pointer
	}

	element=element->getElement(0);	// key is < searchKey

	// if key exists
	if( (element != nullptr) && (element->getKey() == searchKey) ) {		
		for(i=0; i<=myHeader->getLevel(); i++) { // save next pointers
			if (update.getElement(i)->getElement(i) == element) {
				update.getElement(i)->setElement(i, element->getElement(i));
			}
		}
		
		delete (element); // free memory of element

		// set new header level
		while ( (myHeader->getLevel() > 0) && (myHeader->getElement(myHeader->getLevel()) == nullptr) ) {
			myHeader->setLevel(myHeader->getLevel()-1);
		}
	}
}

void SkipList::empty() {
	SkipListElement* element;
	while((element = myHeader->getElement(0)) != nullptr)
		free(element->getKey());
}

SkipListIterator* SkipList::getIterator()
{
	return new SkipListIterator(this);
}


////////////////////////////////////////////////////////////////////////////////
// SKIPLIST ELEMENT
////////////////////////////////////////////////////////////////////////////////

SkipListElement::SkipListElement(long level, Key key, Value value)
	: myLevel(level), myKey(key), myValue(value)
{
	int i;

	// init pointers to next elements
	for(i=0; i<SKIPLIST_MAXLEVEL; i++) {
		myNext[i] = nullptr;
	}
}

/*
SkipListElement::~SkipListElement()
{
}
*/

SkipListElement* SkipListElement::getElement(long level)
{
	if (level > myLevel) {
		LOG_ERROR("Error in: SkipListElement::getElement() level:" << level << ", my level:"<<myLevel << ", max level:"<< SKIPLIST_MAXLEVEL);
		return(this);
	} else {
		return(myNext[level]);
	}
}

void SkipListElement::setElement(long level, SkipListElement* element)
{
	if (level > myLevel) {
		LOG_ERROR("Error in: SkipListElement::setElement() level:" << level << ", my level:"<<myLevel << ", max level:"<< SKIPLIST_MAXLEVEL);
	} else {
		myNext[level]=element;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SKIPLIST ITERATOR
////////////////////////////////////////////////////////////////////////////////

SkipListIterator::SkipListIterator(SkipList* SL) 
{
	element = SL->myHeader;
}

bool SkipListIterator::next()
{
	if(!element)
		return false; // finished
	element = element->getElement(0);
	return element != nullptr;
}

Key SkipListIterator::getKey() const
{
	assert(element);
	return element->getKey();
}

Value SkipListIterator::getValue() const
{
	assert(element);
	if(!element) {
		LOG_DEBUG("SkipListIterator::getValue: null element\n");
		return(nullptr); // not really an error value
	}
	return element->getValue();
}
