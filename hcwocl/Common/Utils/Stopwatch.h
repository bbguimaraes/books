/*
		2011 Takahiro Harada
*/
#ifndef STOPWATCH_H
#define STOPWATCH_H

class Stopwatch
{
	public:
		__inline
		void start();
		__inline
		void split();
		__inline
		void stop();
		__inline
		float getMs();
		__inline
		void getMs( float* times, int capacity );
		__inline
		int getNIntervals() const { return m_idx-1; }

	private:
		enum
		{
			CAPACITY = 64,
		};
		std::chrono::system_clock::time_point m_t[CAPACITY];
	public:
		int m_idx;
};

__inline
void Stopwatch::start()
{
	m_idx = 0;
	split();
}

__inline
void Stopwatch::split()
{
	m_t[m_idx++] = std::chrono::system_clock::now();
}

__inline
void Stopwatch::stop()
{
	split();
}

__inline
float Stopwatch::getMs()
{
	using T = std::chrono::duration<float, std::milli>;
	return std::chrono::duration_cast<T>(m_t[1] - m_t[0]).count();
}

__inline
void Stopwatch::getMs(float* times, int capacity)
{
	for(int i=0; i<capacity; i++) times[i] = 0.f;
	using T = std::chrono::duration<float, std::milli>;
	for(int i=0; i<min2(capacity, m_idx-1); i++)
		times[i] = std::chrono::duration_cast<T>(m_t[i+1] - m_t[i]).count();
}



#endif

