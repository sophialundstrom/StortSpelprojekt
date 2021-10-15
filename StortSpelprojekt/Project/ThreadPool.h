#include <functional>
#include <vector>
#include <thread>
#include <Windows.h>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <chrono>
#include <ctime>
#include <utility>

class ThreadPool {

public:

	using Task = std::function<void()>;

	explicit ThreadPool(UINT numThreads)
	{
		Start(numThreads);
	}

	~ThreadPool()
	{
		Stop();
	}

	void Enqueue(Task task)
	{
		{
			std::unique_lock<std::mutex>lock(eventMutex);
			tasks.emplace(std::move(task));
		}

		// Move means it doesn't send in by copy / value. It is sent in by reference ( type "rvalue-reference" / && )
		eventVar.notify_one(); // When a task has been added one of the threads gets notified and woken up.
	}

private:
	std::vector<std::thread> threads;

	std::condition_variable eventVar;
	std::mutex eventMutex; // To protect the queue of tasks. A.K.A. make it thread safe.
	bool stopping = false;

	std::queue<Task> tasks;

	void Start(UINT numThreads) // Function called in contructor of ThreadPool object.
	{
		for (UINT i = 0; i < numThreads; i++) // For each thread.
		{
			// Emplace_back a lambda function into the thread. 
			// Using emplace_back means it will not make unnecessary copies. 
			// [=] means the lambda function will be able to access variables in the current scope. In this case it will be able to reach all member variables.
			threads.emplace_back([=] {

				while (true) // Infinite while loop until stopping variable is true.
				{

					Task task;

					{ // Scope here so the mutex won't lock the variables while the task is executing (below this scope).

						std::unique_lock<std::mutex> lock(eventMutex);

						eventVar.wait(lock, [=] { return stopping || !tasks.empty(); });
						// each thread waits until it is notified or spatial wakup.
						// lambda function control so spatial wakeup doesn't let the thread continue.
						// if true the lock is reaquired and the thread continues.
						// when enqueue is run ONE waiting thread will be notified.
						// because the queue tasks isn't empty when one is notified by the Enqueue function the thread will continue.

						if (stopping && tasks.empty())
						{
							break;
						}

						task = std::move(tasks.front()); // Taking first task from the queue.
						tasks.pop(); // Removing the first task from the queue.

					}

					task(); // Executing the task.

				}

				});
		}
	}

	void Stop() noexcept
	{
		{
			std::unique_lock<std::mutex>lock(eventMutex);
			stopping = true;
		}

		eventVar.notify_all(); // Notifies all threads waiting 

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

};