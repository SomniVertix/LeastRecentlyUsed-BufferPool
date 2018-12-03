/*
* File:   BufferPoolADT.h
* Author: Clifford A. Shaffer, Dept of CompSci, Virginia Tech
*         Page 286 of Data Structures and Algorithm Analysis
*         Edition 3.2 (C++ Version)
* Modified by Prof Terri Sipantzi, Dept of Computational Sciences,
*		   Liberty University, 3/28/2014
*
* Created on June 5, 2012
*/

#ifndef LRUBUFFERPOOL_H
#define	LRUBUFFERPOOL_H
#include "BufferPoolADT.h"
#include "BufferBlock.h"
#include <fstream>
#include <string>
using namespace std;

// ADT for buffer pools using the message-passing style


class LRUBufferpool : public BufferPoolADT {
private:
	//The buffer pool consists of X number of buffer blocks
	BufferBlock LRUBufferPool[POOL_SIZE];
	int MRU; // Hold the int for Most Recently Used
	fstream input;
public:
	//Constructor gets the filename of the file to be buffered,
	//opens the file, and instantiates poolSize buffer blocks by
	//reading the file and filling the blocks in order.  When the constructor
	//is done the buffer pool blocks should be full with the beginning
	//contents of the input file.
	LRUBufferpool();
	LRUBufferpool(string filename, int poolSize = 5, int blockSize = 4096);
	virtual ~LRUBufferpool();

	// Copy "sz" bytes from position "pos" of the buffered
	//   storage to "space".
	virtual void getBytes(char* space, int sz, int pos) override;

	// Print the order of the buffer blocks using the block id
	//	 numbers.
	virtual void printBufferBlockOrder() override;

	// Get the block id number of the least recently used 
	//	 buffer block.
	virtual int getLRUBlockID() override;
};

LRUBufferpool::LRUBufferpool(){}
LRUBufferpool::LRUBufferpool(string filename, int poolSize , int blockSize )
{
	char * set = new char[blockSize + 1];
	//loop through the text, and make a block of text, setting that into a temp buffer block then append that to an arrayof blocks
	input.open(filename.c_str(), fstream::in | fstream::binary);
	for (int i = 0 ; i < poolSize;)
	{
		input.read(set, blockSize);
		BufferBlock temp(set, blockSize); // Temp variable to hold block information
		LRUBufferPool[i].setBlock(temp.getBlock());
		LRUBufferPool[i].setID(i);
		i++;
		input.seekg(blockSize * (i));
	}
}
LRUBufferpool::~LRUBufferpool() { }
inline void LRUBufferpool::getBytes(char* space, int sz, int pos) 
{
	// 3 Steps :
		//1 - Find data (copy from "pos" to "pos" + "sz" from BufferPool and put in "space"
		//2 - If not found, find data in the raw file
		//3 - Apply the LRU part
	bool found = false; // Bool for Steps 1 & 2
	bool switched = false; // Bool for Step 3
	int section = pos / BLOCKSIZE;
	int selectionStart = pos % BLOCKSIZE;
	BufferBlock holder;
	// Use data
	for (int i = 0; i < POOL_SIZE  && found == false; i++) 
	{
		if (LRUBufferPool[i].getID() == section)
		{
			LRUBufferPool[i].getData(selectionStart, sz, space);
			holder.setBlock(LRUBufferPool[i].getBlock());
			holder.setID(section);
			MRU = i;
			found = true;
		}
	}

	// If data not found, find data in file
	if ((!found)) 
	{
		char * set = new char[BLOCKSIZE + 1];
		input.seekg(BLOCKSIZE * section);
		input.read(set, BLOCKSIZE);
		BufferBlock temp(set, BLOCKSIZE);

		holder.setBlock(temp.getBlock());
		holder.setID(section);
		holder.getData(selectionStart, sz, space);
	}
	
	// Apply the LRU part
	if (holder.getID() == LRUBufferPool[0].getID()) // Front case
		switched = true;
	else if (found)
	{
		for (int j = MRU; j > 0; j--) // middle and end cases
		{
			LRUBufferPool[j].setBlock(LRUBufferPool[j- 1].getBlock());
			LRUBufferPool[j].setID(LRUBufferPool[j- 1].getID());
		}
		LRUBufferPool[0].setBlock(holder.getBlock());
		LRUBufferPool[0].setID(holder.getID());
		switched = true;
	}
	if (switched == false) // Case of new number
	{
		for (int j = POOL_SIZE-1; j > 0; j--)
		{
			LRUBufferPool[j].setBlock(LRUBufferPool[j - 1].getBlock());
			LRUBufferPool[j].setID(LRUBufferPool[j - 1].getID());
		}
		LRUBufferPool[0].setBlock(holder.getBlock());
		LRUBufferPool[0].setID(holder.getID());
	}

}

inline void LRUBufferpool::printBufferBlockOrder() // loops through printing the buffer block order
{
	cout << "My buffer block order from most recently used to LRU is: " << endl << "        ";
	for (int i = 0; i < POOL_SIZE; i++) 
	{
		if (i != POOL_SIZE -1)
			cout << " " << LRUBufferPool[i].getID() << ",";
		else
			cout << " " << LRUBufferPool[i].getID() << ".";
	}
	cout << endl;
}
inline int LRUBufferpool::getLRUBlockID() { return LRUBufferPool[POOL_SIZE].getID(); } // return the LRU's ID
#endif
