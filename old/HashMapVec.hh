#pragma once

#include "LinkedList.hh"
#include "Components.hh"

template<typename T>
class HashMapVec {
private:
	LinkedList<T> * table;
	uint32_t size;
	uint32_t used;
	uint32_t hash(const T* v) {
		uint64_t sum = v->sum();
		return sum & (size - 1);
	}
	void grow() {
		uint32_t oldsize = size;
		LinkedList<T> *old = table;
		size *= 2;
		table = new LinkedList<T>[size];
		used = 0;
		for (int i = 0; i < oldsize; i++) {
			T* v = old[i].removeStrat();
			while (v) {
				addHash(v);
				v = old[i].removeStrat();
			}
		}
		delete[] old;
	}
public:
	HashMapVec(uint32_t initialSize = 1024) :size(initialSize), used(0) {
		int digit = 0;
		for (int i = 0; i < 32; i++) {
			digit += size & 1;
			size = size >> 1;
		}
		size = initialSize;
		if (digit != 1 && !(size & 0x80000000)) {
			int c;
			for (c = 0; c < 32 && !(size & 0x80000000); c++) {
				size = size << 1;
			}
			size = 0x80000000;
			c--;
			while (c) {
				size = size >> 1;
				c--;
			}
		}
		table = new LinkedList<T>[size];
	}
	~HashMapVec() {
		delete[] table;
	}
	int getSize() { return size; }
	LinkedList<T>* getLinkedList(int index) {
		if (index >= size)
			return nullptr;
		return &table[index];
	}

	void addHash(T* v) {
		if (v == nullptr)
			return;
		uint32_t index = hash(v);
		table[index].addStart(v);
		used++;
		if (used * 4 > size)
			grow();
	}
	T* findHash(const T* v) {
		if (!v)
			return nullptr;
		uint32_t index = hash(v);
		return table[index].findVal(v);
	}
	T* check(T* v) {
		if (!v)
			return nullptr;
		uint32_t index = hash(v);
		T* res = table[index].findVal(v);
		if (res != nullptr) {
			delete v;
			return res;
		}
		else {
			addHash(v);
			return v;
		}
	}
};
