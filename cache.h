/*
 * cache.h
 *
 *  Created on: Nov 3, 2012
 *      Author: vr178
 */

#ifndef CACHE_H_
#define CACHE_H_

class cache {
public:

	   //cache set holds the cache elements
	    set<int> cache_set;
	    //cache count,
	    map<int,int>  cache_count_map;
	cache();
	virtual ~cache();
};

#endif /* CACHE_H_ */
