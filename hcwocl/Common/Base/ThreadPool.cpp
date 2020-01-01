/*
		2011 Takahiro Harada
*/
#include <Common/Base/ThreadPool.h>

ThreadPool::ThreadPool(int nThreads)
: m_nThreads( nThreads ), m_deleteSignal( false ), m_taskHead( 0 ), m_taskTail( 0 )
{
	m_threads = new Thread[nThreads];
	for(int i=0; i<nThreads; i++)
		m_threads[i].init(i, this);

	resetThreadTimer();
}

ThreadPool::~ThreadPool()
{
	m_deleteSignal = true;
	start();
	wait();
	for(int i = 0; i < m_nThreads; ++i)
		m_threads[i].m_thread.join();
	delete [] m_threads;

	m_threads = 0;
	m_nThreads = 0;
	m_taskHead = 0;
	m_taskTail = 0;
}

void ThreadPool::resetThreadTimer()
{
	m_startTime = std::chrono::system_clock::now();
	for(int i=0; i<m_nThreads; i++)
	{
		m_threads[i].timestampReset();
	}
}

void ThreadPool::start(bool resetTimestamp)
{
	if( resetTimestamp )
		resetThreadTimer();

	for(int i=0; i<m_nThreads; i++)
	{
		m_threads[i].start();
	}
}

void ThreadPool::wait()
{
	for(int i = 0; i < m_nThreads; ++i)
		m_threads[i].m_finSignal.get_future().wait();
	for(int i=0; i<m_nThreads; i++)
		m_threads[i].m_finSignal = {};
}

void ThreadPool::pushBack(Task* task)
{
	std::lock_guard<std::mutex> guard{m_mutex};
	CLASSERT( m_taskHead != ((m_taskTail+1)&TASK_MASK) ); // full
	m_tasks[m_taskTail] = task;
	m_taskTail = (m_taskTail+1) & TASK_MASK;
}

ThreadPool::Task* ThreadPool::pop()
{
	Task* task = NULL;
	std::lock_guard<std::mutex> guard{m_mutex};
	if( m_taskHead != m_taskTail )
	{
		task = m_tasks[ m_taskHead ];
		m_taskHead = (m_taskHead+1)&TASK_MASK;
	}
	return task;
}

ThreadPool::Thread::Thread()
{

}

void ThreadPool::Thread::init(int idx, ThreadPool* threadPool)
{
	m_args.m_threadPool = threadPool;
	m_args.m_idx = idx;
	m_thread = std::thread(run, &m_args);
}

ThreadPool::Thread::~Thread()
{

}

void ThreadPool::Thread::start()
{
	m_runSignal.set_value();
}

void ThreadPool::Thread::timestampReset()
{
	m_nTimestamps = 0;
}

u32 ThreadPool::Thread::run(void* args)
{
	ThreadArgs* tArgs = (ThreadArgs*)args;
	ThreadPool* threadPool = tArgs->m_threadPool;
	int idx = tArgs->m_idx;
	Thread* th = &threadPool->m_threads[idx];

	volatile bool& deleteSignal = threadPool->m_deleteSignal;
	while(!deleteSignal)
	{
		th->m_runSignal.get_future().wait();
		th->m_runSignal = {};
		if( deleteSignal )
		{
			break;
		}

		{
			Task* task = threadPool->pop();
			while( task )
			{
				std::chrono::system_clock::time_point s,e;
				s = std::chrono::system_clock::now();
				task->run( idx );
				e = std::chrono::system_clock::now();
				using T = std::chrono::duration<float, std::milli>;
				float start, end;
				start = std::chrono::duration_cast<T>(s - threadPool->m_startTime).count();
				end = std::chrono::duration_cast<T>(e - threadPool->m_startTime).count();
				th->pushBackTimeStamp( task->getType(), start, end );

				delete task;

				task = threadPool->pop();
			}
		}
		th->m_finSignal.set_value();
	}
	th->m_finSignal.set_value();
	return 0;
}

void ThreadPool::Thread::pushBackTimeStamp(u16 type, float s, float e)
{
	if( m_nTimestamps < MAX_TIMESTAMPS-1 )
	{
		m_timestamps[ m_nTimestamps ].m_type = type;
		m_timestamps[ m_nTimestamps ].m_start = s;
		m_timestamps[ m_nTimestamps ].m_end = e;
		m_nTimestamps++;
	}
}

