/*
	Buffer for writing
*/

#pragma once

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include "DoubleToString.hh"
//#include "Components.hh"


class BufferWrite {
private:
	char* postbuffer;	//just after buffer
	char* buffer;
	char* phead;
	int fn;
	uint32_t BUFFER_SIZE = 32768;	//32K
	uint32_t POSTBUFFER_SIZE = 256;
	void flushOnlyBuffer();
public:
	void writeG0(double x, double y);
	void writeG1(double x, double y, double e);
	void flush();

	BufferWrite(std::string& fileTarget) {
		buffer = new char[BUFFER_SIZE + POSTBUFFER_SIZE];
		postbuffer = buffer + BUFFER_SIZE;
    phead = buffer;
    remove(fileTarget.c_str());
    creat(fileTarget.c_str(), S_IREAD | S_IWRITE);
		fn = open(fileTarget.c_str(), O_WRONLY | O_APPEND);
	}
	~BufferWrite() {
		flush();
		delete[] buffer;
    close(fn);
	}
  friend BufferWrite& operator <<(BufferWrite& buf, const char* ch){
    //check buffer is full or not. If full, only flush the buffer
		for(int i = 0; ch[i] != '\0'; i++, buf.phead++)
			*buf.phead = ch[i];
		return buf;
  }
	friend BufferWrite& operator <<(BufferWrite& buf, char ch){
		*buf.phead = ch;
		buf.phead++;
		return buf;
  }
	friend BufferWrite& operator <<(BufferWrite& buf, std::string& str){
		return buf << str.c_str();
  }
	friend BufferWrite& operator <<(BufferWrite& buf, double val){
#if 0
		//TODO: translating double to string REALLY take so much time!!!
		//TODO: make our own doubleToString()
		//snprintf(buf.phead, 9, "%lf", val);
		//buf.phead += 9;
		std::string str = std::to_string(val);
		return buf << str;
#endif
#if 1	//faster way for DoubleToString
		buf.phead = formatDouble(val, buf.phead);
		return buf;
#endif
	}
};
