#ifndef __RECTSTASH__
#define __RECTSTASH__

#include <vector>
#include "Rect.h"

class RectStash
{
	std::vector<Rect> storage;

public:
	typedef std::vector<Rect>::size_type iterator;

	~RectStash()
	{
		// future cleanup operations
	}

	void add(Rect r)
	{
		storage.push_back(r);
	}

	Rect& fetch(iterator index)
	{
		return storage[index];
	}

	iterator count()
	{
		return storage.size();
	}

	void promote(iterator index)
	{
		Rect r = storage[index];
		storage.erase(storage.begin() + index);
		storage.insert(storage.begin(), r);
	}
};

#endif
