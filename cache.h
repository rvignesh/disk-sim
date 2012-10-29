/*
 * cache.h
 *
 *  Created on: Oct 29, 2012
 *      Author: vignesh
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
	bool check_hit(int page_no);
	bool insert_new_entry(int page_no);
	bool evict_page(int page_no);

};

#endif /* CACHE_H_ */
