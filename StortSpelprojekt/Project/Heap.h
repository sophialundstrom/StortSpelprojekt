#include <algorithm>
#include <vector>
#include "Grid.h"

template <typename T> 
class Heap 
{
public:
	Heap(int maxHeapSize);
	virtual ~Heap();

	void Add(T *element);
	T* RemoveFirst();
	void SortUp(T* element);
	void SortDown(T* element);
	void Swap(T* elemA, T* elemB);
	bool Contains(T* element);
	int GetSize();
	T* GetItem(int i);

private:
	std::vector<T*> items;
	int currentItemCount;


};


template<typename T>
Heap<T>::Heap(int maxHeapSize)
{
	items.reserve(maxHeapSize);
	//for (int i = 0; i < 32 *32; i++)
	//{
	//	items.push_back(new Node);
	//}
}

template<typename T>
Heap<T>::~Heap()
{
}

template<typename T>
void Heap<T>::Add(T* element)
{
	element->heapIndex = currentItemCount;
	items.push_back(element);
	//items[currentItemCount] = element; // remove
	SortUp(element);
	currentItemCount++;
}

template<typename T>
T* Heap<T>::RemoveFirst()
{
	T* firstElement = items[0];
	currentItemCount--;
	items[0] = items[currentItemCount];
	items[0]->heapIndex = 0;
	SortDown(items[0]);
	return firstElement;
}

template<typename T>
void Heap<T>::SortUp(T* element)
{
	int parentIndex = (element->heapIndex - 1) / 2;
	while (true)
	{
		T* parentElement = items[parentIndex];
		if (element->CompareTo(parentElement) > 0)
		{
			Swap(element, parentElement);
		}
		else
		{
			break; // try to put in a condition in the while loop instead of breaking
		}
		//update the parents index because of the swapping
		parentIndex = (element->heapIndex - 1) / 2;
	}
}

template<typename T>
void Heap<T>::Swap(T* elemA, T* elemB)
{
	items[elemA->heapIndex] = elemB;
	items[elemB->heapIndex] = elemA;
	int tmpI = elemA->heapIndex;
	elemA->heapIndex = elemB->heapIndex;
	elemB->heapIndex = tmpI;
}

template<typename T>
void Heap<T>::SortDown(T* element)
{
	while (true)
	{
		int leftChildIndex = element->heapIndex * 2 + 1;
		int rightChildIndex = element->heapIndex * 2 + 2;
		int swapIndex = 0;

		if (leftChildIndex < items.size())
		{
			swapIndex = leftChildIndex;
			if (rightChildIndex < items.size())
			{
				if (items[leftChildIndex]->CompareTo(items[rightChildIndex]) < 0)
				{
					swapIndex = rightChildIndex;
				}
			}

			if (element->CompareTo(items[swapIndex]) < 0)	{
				Swap(element, items[swapIndex]);
			}
			else{
				return;
			}
		}
		else
		{
			return;
		}
	}
}

template<typename T>
bool Heap<T>::Contains(T* element)
{
	if (element->heapIndex < 0)
		return false;
	return items[element->heapIndex] == element;
}

template<typename T>
int Heap<T>::GetSize()
{
	return currentItemCount;
}

template<typename T>
T* Heap<T>::GetItem(int i)
{
	return items[i];
}