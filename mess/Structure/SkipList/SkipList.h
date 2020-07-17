
/*
                                 SKIPLIST.H
DESCRIPTION:
  Interface for skip list
  See William Pugh's paper: 
    Skip Lists: A Probabilistic Alternative to Balanced Trees
  This is a good generic implementation of the SkipList method.
  A SkipList is a container class which associates key/value pairs.
  Construct an empty SkipList and add arbitrary key/value pairs using
  the insert method and then search for inserted values by key.
  Iterators can also be constructed from a SkipList instance which
  can be used to walk all the entries of that instance.

HISTORY:
  Original C version by Bruno Grossniklaus, 11/13/97
  C++ version with Iterator by Daniel Green - dgreen@superliminal.com 4/3/99
*/

#ifndef _SkipList_H
#define _SkipList_H

typedef void* Value;
typedef void* Key;

// returned by search method
#define SKIPLIST_NOT_FOUND ((Value)-1)


class SkipList {
public:
	SkipList();
	SkipList(float probability);
	~SkipList();
	void insert(const Key searchKey, const Value value);
	Value search(const Key searchKey) const;
	void free(const Key searchKey);
	void empty();
	class SkipListIterator* getIterator();
private:
	float myProbability;
	class SkipListElement* myHeader;
	friend class SkipListIterator;
};

class SkipListIterator {
public:
	SkipListIterator(SkipList* SL);
	bool next();
	Key getKey() const;
	Value getValue() const;
private:
	class SkipListElement* element;
};


#define SKIPLIST_MAXLEVEL 6 // maximum node level

class SkipListElement {
public:
	SkipListElement(long level, Key key, Value value);
	void setElement(long level, SkipListElement* element);
	SkipListElement* getElement(long level);
	void setKey(Key key) { myKey = key; }
	Key getKey() const { return myKey; }
	void setLevel(long level) { myLevel = level; }
	long getLevel() const { return myLevel; }
	void setValue(Value value) { myValue = value; }
	Value getValue() const { return myValue; }
private:
	long myLevel;
	Key myKey;
	Value myValue;
	SkipListElement* myNext[SKIPLIST_MAXLEVEL]; // pointers to next elements
};

#endif