/*
		2011 Takahiro Harada
*/
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <future>
#include <mutex>
#include <thread>

#include <Common/Math/Math.h>

class ThreadPool
{
	public:
		struct Task
		{
			virtual ~Task() = default;
			virtual u16 getType() = 0;
			virtual void run(int tIdx) = 0;
		};

		ThreadPool(int nThreads);
		~ThreadPool();
		void resetThreadTimer();
		void start(bool resetTimestamp = true);
		void wait();
		void pushBack(Task* task);
		Task* pop();

	public:
		struct ThreadArgs
		{
			ThreadPool* m_threadPool;
			int m_idx;
		};

		class Thread
		{
			public:
				Thread();
				~Thread();

				void init(int idx, ThreadPool* threadPool);
				void start();
				void timestampReset();

				static
				u32 run(void* args);

				struct Timestamp
				{
					float m_start;
					float m_end;
					u16 m_type;
				};
			 
				void pushBackTimeStamp(u16 type, float s, float e);

			public:
				std::thread m_thread;
				std::promise<void> m_runSignal;
				std::promise<void> m_finSignal;
				ThreadArgs m_args;

				enum
				{
					MAX_TIMESTAMPS = 256,
				};

				Timestamp m_timestamps[MAX_TIMESTAMPS];
				int m_nTimestamps;
		};

		int m_nThreads;
		bool m_deleteSignal;

		std::mutex m_mutex;
		Thread* m_threads;
		std::chrono::system_clock::time_point m_startTime;

		enum
		{
			MAX_TASKS = 256,
			TASK_MASK = MAX_TASKS-1,
		};

		Task* m_tasks[MAX_TASKS];
		int m_taskHead;
		int m_taskTail;
};

#endif
