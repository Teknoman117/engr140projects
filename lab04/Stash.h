#ifndef STASH_H
#define STASH_H

#include <cassert>
#include <iostream>

struct Stash {
	int size;      // Size of each space
	int quantity;  // Number of storage spaces
	int next;      // Next empty space
	int reallocsize;
	int numreallocs;
  
	// Dynamically allocated array of bytes:
	unsigned char* storage;
  
	// Functions!
	void initialize(int sz){
		size = sz;
		quantity = 0;
		storage = 0;
		next = 0;
		reallocsize = 100;
		numreallocs = 0;
	}
  
	void cleanup(){
		if(storage != 0) {
		  std::cout << "freeing storage" << std::endl;
		  delete []storage;
		}
	}
  
	int add(const void* element){
		if(next >= quantity) // Enough space left?
		  inflate(reallocsize);
	  
		// Copy element into storage,
		// starting at next empty space:
		int startBytes = next * size;
		unsigned char* e = (unsigned char*)element;
	  
		for(int i = 0; i < size; i++)
		  storage[startBytes + i] = e[i];
		next++;
	  
		return(next - 1); // Index number
	}

	void* fetch(int index){
		// Check index boundaries:
		assert(0 <= index);
  
		if(index >= next)
			return 0; // To indicate the end
  
		// Produce pointer to desired element:
		return &(storage[index * size]);
	}
  
	int count() {
		return next; // Number of elements in CStash
	}
  
	void inflate(int increase){
		assert(increase > 0);
		
		numreallocs++;
		int newQuantity = quantity + increase;
		int newBytes = newQuantity * size;
		int oldBytes = quantity * size;
		unsigned char* b = new unsigned char[newBytes];
		
		for(int i = 0; i < oldBytes; i++)
			b[i] = storage[i]; // Copy old to new
		
		delete []storage; // Old storage
		storage = b; // Point to new memory
		quantity = newQuantity;
	}
}; ///:~

#endif
