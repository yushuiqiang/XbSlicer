#pragma once

#include "Components.hh"

template<typename T>
class LinkedList {
private:
public:
	class Node {
	public:
		Node * next;
		T* val;
		Node(T* val) : val(val), next(nullptr) {}
		Node(T* val, Node* next) : val(val), next(next) {}
		//T* getVal() { return val; }
	};
	Node* head;
	LinkedList() : head(nullptr) {}
	~LinkedList() {
		Node* temp = head;
		while (temp != nullptr) {
			head = temp->next;
			delete temp;
			temp = head;
		}
	}
	void addStart(T* v) {
		Node* temp = new Node(v, head);
		head = temp;
	}
	T* removeStrat() {
		if (head == nullptr)
			return nullptr;
		Node* temp = head;
		T* v = temp->val;
		head = temp->next;
		delete temp;
		return v;
	}
	T* findVal(const T* v) {
		Node* temp = head;
		while (temp != nullptr) {
			if (temp->val->equal(v))
				return temp->val;
			temp = temp->next;
		}
		return nullptr;
	}
};
