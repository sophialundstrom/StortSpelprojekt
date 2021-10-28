#include <algorithm>
#include <vector>
#include "Node.h"

template <class T> 
class Heap 
{
public:
	Heap(int maxHeapSize);
	~Heap();

	void Add(T *element);
	T* RemoveFirst();
	void SortUp(T* element);
	void Swap(T* elemA, T* elemB);
	void SortDown(T* element);

private:
	std::vector<T*> items;
	int currentItemCount;


};


