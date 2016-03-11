#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <iostream>

struct LinkedList
{
	struct Link 
	{
		void* data;
		Link* next;
		
		Link(void* dat, Link* nxt)
		{
			data = dat;
			next = nxt;
		}
	}* head;

	LinkedList(void* dat, Link* nxt)
	{
		head = new Link(dat, nxt);
	}

	void add(LinkedList::Link* l, int n)
	{
		destroy(l->next);
		l->next = NULL;
		for(int i = n; i > 0; i--)
		{
			l->next = new Link(new double(i-1), l->next);
		}
	}

	void insert(LinkedList::Link* l, int n)
	{
		for(int i = n; i > 0; i--)
		{
			l->next = new Link(new double(i-1), l->next);
		}
	}

	void print()
	{
		Link* current = head;
		while(current != NULL)
		{
			std::cout << *static_cast<double *>(current->data) << std::endl;
			current = current->next;
		}
	}

	void destroy(LinkedList::Link* current)
	{
		while(current != NULL)
		{
			delete static_cast<double *>(current->data);
			Link* n = current->next;
			delete current;
			current = n;
		}
	}

	void cleanup()
	{
		destroy(head);
		head = NULL;
	}
};

#endif
