/*
* File:   BufferBlock.hpp
* Author: Professor Terri Sipantzi
*
* Created on August 25, 2012, 8:49 AM
*/

#ifndef BUFFERBLOCK_H
#define	BUFFERBLOCK_H

#include <iostream>
#include <string>
#include "BufferBlockADT.h"
using namespace std;

class BufferBlock : public BufferblockADT{
private:
	//Instance variables:
	int blockID;
	char* block;

public:

	//sz is the size of the character array data
	//points to.
	BufferBlock() {}
	BufferBlock(char* data, int sz = 4096) 
	{
		block = new char[sz ];
		for (int i = 0; i <= sz-1; i++)
		{
			block[i] = data[i];
		}
		block[sz] = '\0'; // Last char has to be null
	}
	virtual ~BufferBlock() {}

	//read the block from pos to pos + sz-1 (or to the end of the block)
	virtual void getData(int pos, int sz, char* data) override;

	//setID
	virtual void setID(int id) override;

	//getID
	virtual int getID() const override;

	//getBlocksize
	virtual int getBlocksize() const override;

	//return the block
	virtual char* getBlock() const override;

	//set the block
	virtual void setBlock(char* blk) override;
};	


void BufferBlock::getData(int pos, int sz, char* data) 
{
	int b = 0;
	for (int i = pos; i <= (pos + (sz - 1)); i++) // Loop out the data into data[b]
	{
		data[b] = block[i];
		b++;
	}
	data[sz] = '\0'; //Last char has to be null
}

void BufferBlock::setID(int id) { blockID = id; }
int BufferBlock::getID() const { return blockID; }
int BufferBlock::getBlocksize() const { return strlen(block); }
char* BufferBlock::getBlock() const { return block; }
void BufferBlock::setBlock(char* blk) 
{
	block = new char[BLOCKSIZE];
	for (int i = 0; i < BLOCKSIZE; i++) 
	{
		block[i] = blk[i];
	}
	block[BLOCKSIZE] = '\0'; //Last char set to null
}

#endif	/* BUFFERBLOCKADT_H */


