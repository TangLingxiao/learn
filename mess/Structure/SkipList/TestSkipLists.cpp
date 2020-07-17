
#include <stdio.h>

#include "SkipList.h"

#define TESTSIZE 10000

void* Keys[TESTSIZE][2];

int main()
{
	SkipList sl;
	int i, n=TESTSIZE;
	printf("initializing\n");
	for(i=0; i<n; i++) {
		Keys[i][0] = new int;
		Keys[i][1] = new int;
	}
	printf("inserting\n");
	for(i=0; i<n; i++)
		sl.insert(Keys[i][0], Keys[i][1]);
	printf("finding\n");
	for(i=0; i<n; i++)
		if(sl.search(Keys[i][0]) != Keys[i][1])
			printf("skiplist failed on Key %d\n", i);
	printf("done\n");
	return 0;
}