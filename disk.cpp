/*
 * disk.cpp
 *
 *  Created on: Nov 3, 2012
 *      Author: vr178
 */

#include "disk.h"

disk::disk() {
	// TODO Auto-generated constructor stub
	// Insert page
	int get_free_block(){

	     	free_block_count--;
	        int write_pointer=free_block_queue.front()*block_size;
	        free_set.erase(free_block_queue.front());
			free_block_queue.pop();
			if(free_block_queue.size()<5){
				garbage_collect();


			}
			return write_pointer;
	}
	int get_free_block_count()
	{
		return free_block_count;
	}
	void increment_free_block_count()
	{
		free_block_count++;
	}
	void decrement_free_block_count()
	{


        if(CACHE_POLICY==0)
            reclaim_block=find_max_index(dirty_count,no_blocks,write_pointer/no_blocks,free_set);
        if(CACHE_POLICY==1)
            reclaim_block=find_min_index(dirty_count,no_blocks,write_pointer/no_blocks,free_set);
        if(CACHE_POLICY==2)
        {
            int k=rand()%32;
        while(k==(write_pointer/no_blocks) || free_set.find(k)!=free_set.end())
        k=rand()%32;
        reclaim_block=k;
        }
        cout<<"reclaim Block"<<reclaim_block<<"Free Block queue"<<free_block_queue.size();

            free_block_count++;

            dirty_count[reclaim_block]=0;


			for(int j=0;j<block_size;j++)
	        {
				if(write_pointer%block_size==0)
				{
			    free_block_count--;
				 write_pointer=free_block_queue.front()*block_size;
				 free_set.erase(free_block_queue.front());
					free_block_queue.pop();
	        }



                map<int,int>::iterator it=block_map.find((reclaim_block*block_size)+j);
            if(it!=block_map.end())
            {
              int mem_address=it->second;
			  assert(ad_map.find(it->second)!=ad_map.end());
               ad_map.erase(it->second);
			   block_map.erase(reclaim_block*block_size+j);
               ad_map.insert(pair<int,int>(mem_address,write_pointer));

               block_map.insert(pair<int,int>(write_pointer,mem_address));
            write_pointer++;
            host_write++;
		  	}
			block_map.erase(reclaim_block*block_size+j);




		}
			   free_block_queue.push(reclaim_block);
            free_set.insert(reclaim_block);


	}

	void garbage_collect(){

	}
}

disk::~disk() {
	// TODO Auto-generated destructor stub
}

