/*
	Buffer for reading binary STL file
*/
#pragma once

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include "Components.hh"

class BufferReadBinary {
private:
	char* prebuffer;	//just before buffer
	char* buffer;
	char* phead;
	int fn;
	uint32_t BUFFER_SIZE = 32768;	//32K
	uint32_t PREBUFFER_SIZE = 64;
	uint32_t readIn;
	uint32_t remain;	//bytes that left in buffer, not enough for one triangle
	uint32_t numTrianglesInFile;
	uint32_t numTrianglesInBlock;
public:
	BufferReadBinary(std::string& fileTarget): remain(0), readIn(0), numTrianglesInBlock(0) {
		prebuffer = new char[PREBUFFER_SIZE + BUFFER_SIZE];
		buffer = prebuffer + PREBUFFER_SIZE;
		fn = open(fileTarget.c_str(), O_RDONLY);
		readIn = read(fn, buffer, BUFFER_SIZE);
		phead = buffer + 80;
		numTrianglesInFile = *(uint32_t*)phead;
		phead += sizeof(uint32_t);
		readIn -= 84;
		numTrianglesInBlock = readIn / 50;
		remain = readIn % 50;
	}
	~BufferReadBinary() {
		delete[] prebuffer;
		close(fn);
	}
	uint32_t getNumTrianglesInFile() { return numTrianglesInFile; }
	uint32_t getNumTrianglesInBlock(){return numTrianglesInBlock;}
	void step2Bytes() {phead += 2;}
	void stepFloat() {phead += sizeof(float);}
	float getFloat() {return *(float*)phead;}
	void readBuffer(){
		char* pd = buffer - remain;
		char* pf = buffer + BUFFER_SIZE - remain;
		for(int i = remain; i > 0; i--, pd++, pf++)
			*pd = *pf;
		readIn = read(fn, buffer, BUFFER_SIZE);
		phead = buffer - remain;
		numTrianglesInBlock = (readIn + remain) / 50;
		remain = (readIn + remain) % 50;
	}
};
