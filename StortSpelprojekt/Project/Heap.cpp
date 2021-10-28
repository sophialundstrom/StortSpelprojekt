#include "Heap.h"

template<typename T>
Heap<T>::Heap(int maxHeapSize)
{
	items.reserve(maxHeapSize);
}

template<typename T>
void Heap<T>::Add(T *element)
{
	element->heapIndex = currentItemCount;
	items[currentItemCount] = element;
	SortUp(element);
	currentItemCount++;
}

template<class T>
T* Heap<T>::RemoveFirst()
{
	T* firstElement = items[0];
	currentItemCount--;
	items[0] = items[currentItemCount];
	items[0]->heapIndex = 0;
	SortDown(items[0]);
	return firstElement;
}

template<class T>
void Heap<T>::SortUp(T* element)
{
	int parentIndex = (element->heapIndex - 1) / 2;
	while (true)
	{
		T* parentElement = items[parentIndex];
		if(element->Compare(parentElement) > 0)
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

template<class T>
void Heap<T>::Swap(T* elemA, T* elemB)
{
	items[elemA->heapIndex] = elemB;
	items[elemB->heapIndex] = elemA;
	int tmpI = elemA->heapIndex;
	elemA->heapIndex = elemB->heapIndex;
	elemB->heapIndex = tmpI;
}

template<class T>
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
				if (items[leftChildIndex]->Compare(items[rightChildIndex]) < 0)
				{
					swapIndex = rightChildIndex;
				}
			}

			if (element->Compare(items[swapIndex]) < 0)
			{
				Swap(element, items[swapIndex]);
			}
			else
			{
				return;
			}


		}
		return;
	}
}

