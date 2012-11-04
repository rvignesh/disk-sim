/*
 * disk.h
 *
 *  Created on: Nov 3, 2012
 *      Author: vr178
 */

#ifndef DISK_H_
#define DISK_H_

class disk {
public:
    static int free_block_count=31;
    queue<int> free_block_queue;
    //Parallel Structures to maintain a queue and a set. Breaks the laws, but l
    set<int> free_set;
    static int dirty_count[no_blocks];
    // Number of blocks in the total memory
    const int no_blocks=32;
    // Number of pages in the total memory
    const int no_pages=1024;
    // Number of pages per block
    const int block_size=32;

    map<int,int> ad_map;
    map<int,int> block_map;

	disk();
	virtual ~disk();

	int get_free_block();
	// Return a block after garbage collection.
	void grabage_collect();
	// Index of the block with maximum dirty pages.
	int find_max_index();
	// Random index returned.
	int find_random_index();
	// Insert page
	void insert_page_no();





};

#endif /* DISK_H_ */
