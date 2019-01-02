/*
	Buffer for writing
*/
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include "BufferWrite.hh"

void BufferWrite::flushOnlyBuffer(){
	size_t size = write(fn, buffer, BUFFER_SIZE);
	//copy the stuff in postbuffer to buffer
	for(int i = 0; i < phead - postbuffer; i++)
		buffer[i] = postbuffer[i];
	phead -= BUFFER_SIZE;
}

void BufferWrite::writeG0(double x, double y) {
	*this << "G0 F4800 X" << x << " Y" << y << '\n';
	if(phead >= postbuffer)
		flushOnlyBuffer();
}
void BufferWrite::writeG1(double x, double y, double e) {
	*this << "G1 F2000 X" << x << " Y" << y << " E" << e << '\n';
	if(phead >= postbuffer)
		flushOnlyBuffer();
}
void BufferWrite::flush(){
	uint32_t sizeToWrite = phead - buffer;
	size_t size = write(fn, buffer, sizeToWrite);
	phead = buffer;
}
