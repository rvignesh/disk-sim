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
extern "C" {
    #include "genzip.h"
}
#define CACHE_POLICY 0
#define EVICT_POLICY 0
using namespace std;

int find_max_index(int* array,int N,int w,set<int>);
int find_min_index(int* array,int N,int w,set<int>);
int find_min(map<int,int> ma,bool evict_page);
int memory_write=0;
int host_write=0;
// Page numbering and block numbering starts at zero.




double print_writeampl(int host_write,int memory_write)
{

  return (double)host_write/memory_write;

}
int find_last_accessed(int *values,set<int> cache_set,int start_index);
int main()
{

     ofstream myfile;
   myfile.open("Min.txt");
    queue<int> free_block_queue;
    set<int> free_set;
 //    static int physical_disk[1024];
    // Number of blocks in the total memory
    const int no_blocks=32;
    // Number of pages in the total memory
    const int no_pages=1024;
    // Number of pages per block
    const int block_size=32;
    // Write pointer points the page number that is written to
    // Multiply block_size times block number to get write pointer to start of block. starts are zero
    int write_pointer=0;
    // free count for all the blocks
    int free_block_count=31;
    // the number of pages at which the garbage collection is triggered.
    int garbage_treshold=5;

    //evict flag.. specifies if a page is evicted from cache
    bool evict_flag=false;

	//look ahead on the zipf distribution.
	static int look_ahead[1024];

    // currently evicted page
    int evict_page;
	

	bool gc_flag=false;


    // page that is updated in cache
    int page_no;
    //cache set holds the cache elements
    set<int> cache_set;

    //cache count,
    map<int,int>  cache_count_map;
    // Garbage collection flag
    bool garbage_collect=false;
    static int dirty_count[no_blocks];

    map<int,int> ad_map;
    map<int,int> block_map;


    int simulation_count=0;
    int reclaim_block=0;
    int num_total_write=0;
    double alpha=1;
    int* zipf_values=NULL;

		zipf_values=NULL;
     zipf_values=generate_zipf(10000,alpha,32.0*32*27,434);
	  for(int i=1;i<32;i++)
	  {
		  free_block_queue.push(i);
		  free_set.insert(i);
	  }	
 

	while(num_total_write<10000)
    {
    //simulation runs until the user breaks it.
    int log_address=zipf_values[simulation_count];
    int page_no=log_address/32;
	evict_page=page_no;

	// IF the page is found in the cache.
    if(cache_set.find(page_no)!=cache_set.end())
    {
          if(cache_count_map.find(page_no)!=cache_count_map.end()){

            map<int,int>::iterator it=cache_count_map.find(page_no);
            it->second=it->second+1;
            evict_flag=false;
          }
          else{
          cache_count_map.insert(pair<int,int>(page_no,1));
          evict_flag=false;
          }
        }
    else
	{
        if(cache_set.size()<32)
        {
            cache_set.insert(page_no);
            cache_count_map.insert(pair<int,int>(page_no,1));
            evict_flag=false;
        }
        else
        {
            evict_flag=true;
            //find the index of the minimum element in the map
			evict_page=find_last_accessed(zipf_values,cache_set,simulation_count+1);
			//assert(evict_page!=0);
            cout<<"Evicted Page "<<evict_page<<endl;
            cache_set.erase(evict_page);
            cache_set.insert(page_no);
            cache_count_map.erase(evict_page);
            cache_count_map.insert(pair<int,int>(page_no,1));
            // remove the element at that index from count map and also the set.
            //do something else
        }
    }
   if(evict_flag==true)
   {
	  
        evict_flag=false;
	if(write_pointer%block_size==0 && write_pointer!=0)
    {

        free_block_count--;
        write_pointer=free_block_queue.front()*block_size;
        free_set.erase(free_block_queue.front());
		free_block_queue.pop();
        //print_disk(physical_disk,no_pages);
        //print_disk(dirty_count,block_size,simulation_count);
        //cout<<"Write Pointer"<<write_pointer<<endl;

    }

	  if(ad_map.find(evict_page)!=ad_map.end())
    {
      map<int,int>::iterator it=ad_map.find(evict_page);
	  int block_address=it->second;
	  assert(block_map.find(block_address)!=block_map.end());
	  block_map.erase(block_address);
	  dirty_count[(block_address)/no_blocks]++;
      ad_map.erase(evict_page);
    }
    memory_write++;
    host_write++;
	assert(ad_map.size()==block_map.size());
    ad_map.insert(pair<int,int>(evict_page,write_pointer));
	if(block_map.find(write_pointer)!=block_map.end())
		cout<<"Failed here!"<<write_pointer<<endl;
    block_map.insert(pair<int,int>(write_pointer,evict_page));
    write_pointer++;
	if(free_block_queue.size()<5)
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
   }
        num_total_write++;

    simulation_count++;
	
   }
	simulation_count=0;
	num_total_write=0;
    num_total_write=0;
    myfile<<alpha<<"\t"<<print_writeampl(host_write,memory_write)<<endl;
    host_write=memory_write=0;
    alpha=alpha+0.1;
	 while(!free_block_queue.empty())
			  free_block_queue.pop();
	 free_set.clear();
	 ad_map.clear();
	 block_map.clear();
	
 
	      myfile.close();
		 
}


int find_max_index(int* array,int N,int w,set<int> free_set)
{
int i,k = N-1;
int max = 0;
for (i = N-1; i >= 0 ; --i)
{
    if (array[i] >= max && i!=w && free_set.find(i)==free_set.end())
    {
        max = (int)array[i];
        k = i;
    }
}
if(max==0)
	cout<<"Garbage Collected a Block with zero dirty Pages";
return k;
}
int find_min_index(int* array,int N,int w,set<int> free_set)
{

int i,k = 0;
int min = 33;

for (i = 0; i < N; ++i)
{
    if (array[i] < min && array[i]>1 && i!=w && free_set.find(i)==free_set.end())
    {
        min = (int)array[i];
        k = i;
    }
}
return k;
}
int find_last_accessed(int *values,set<int> cache_set,int start_index)
{

	set<int> access_list;
	int last_index=-1;
	while(access_list.size()<32 && start_index<10000)
	{
		if(cache_set.count(values[start_index]/32)!=0 && access_list.count(values[start_index]/32)==0)
		{
			access_list.insert(values[start_index]/32);
			last_index=start_index;
		}
		if(access_list.size()==32)
			return values[last_index]/32;
		start_index++;
	}
	
	for(set<int>::iterator it=cache_set.begin();it!=cache_set.end();it++)
		if(access_list.count((*it))==0)
		{
			return *it;
		}

}

int find_min(map<int,int> frequencyCount,bool rand_flag)
{

int currentMax = 9999;

int rand_no=rand()%frequencyCount.size();



int arg_max = 0;
int c=0;
for(map<int,int>::iterator it = frequencyCount.begin(); it != frequencyCount.end(); ++it,c++ )
	{
     if(rand_flag==1 && c==rand_no)
	 {
        return it->first;
     }
    if(it ->second < currentMax) {
        arg_max = it->first;
        currentMax = it->second;
	  }
	}
return arg_max;

}
