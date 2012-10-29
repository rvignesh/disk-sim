/*
 * disk.h
 *
 *  Created on: Oct 29, 2012
 *      Author: vignesh
 */
#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <fstream>
#include <queue>
#include <cassert>
#define NO_BLOCKS 32
#define BLOCK_SIZE 32
#define NO_PAGES 32
using namespace std;

#ifndef DISK_H_
#define DISK_H_

class disk {
public:
const int no_blocks;
const int no_pages;
const int block_size;
int free_block_count;
static int dirty_count[no_blocks];

map<int,int> ad_map;
map<int,int> block_map;
queue<int> free_block_queue;
set<int> free_set;

public:
	disk();
	virtual ~disk();
	int get_new_block();
	int garbage_collect();

};

#endif /* DISK_H_ */
