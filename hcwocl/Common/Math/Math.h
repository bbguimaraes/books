/*
		2011 Takahiro Harada
*/
#ifndef CL_MATH_H
#define CL_MATH_H

#include <cstdlib>
#include <math.h>
#include <float.h>
#include <xmmintrin.h>


#include <Common/Base/Error.h>
#include <algorithm>
#define pxSort std::sort

#define PI       3.14159265358979323846f
#define NEXTMULTIPLEOF(num, alignment) (((num)/(alignment) + (((num)%(alignment)==0)?0:1))*(alignment))

#define _MEM_ALIGNED_ALLOCATOR16 \
	void* operator new(size_t size) { return std::aligned_alloc(16, size); } \
	void operator delete(void *p) { std::free(p); } \
	void* operator new[](size_t size) { return std::aligned_alloc(16, size); } \
	void operator delete[](void *p) { std::free(p); } \
	void* operator new(size_t size, void* p) { return p; } \
	void operator delete(void *p, void* pp) {}

template<class T>
T nextPowerOf2(T n)
{
	n -= 1;
	for(int i=0; i<sizeof(T)*8; i++)
		n = n | (n>>i);
	return n+1;
}

struct alignas(16) float4
{
	_MEM_ALIGNED_ALLOCATOR16;
	union
	{
		struct
		{
			float x,y,z,w;
		};
		struct
		{
			float s[4];
		};
		__m128 m_quad;
	};
};

struct alignas(16) int4
{
	_MEM_ALIGNED_ALLOCATOR16;
	union
	{
		struct
		{
			int x,y,z,w;
		};
		struct
		{
			int s[4];
		};
	};
};

struct int2
{
	union
	{
		struct
		{
			int x,y;
		};
		struct
		{
			int s[2];
		};
	};
};

struct float2
{
	union
	{
		struct
		{
			float x,y;
		};
		struct
		{
			float s[2];
		};
	};
};


typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;



#include <Common/Math/float4.inl>




template<typename T>
void swap2(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}


__inline
void randSeed(int seed)
{
	srand( seed );
}

template<typename T>
__inline
T randRange(const T& minV, const T& maxV)
{
	float r = (rand()%10000)/10000.f;
	T range = maxV - minV;
	return (T)(minV + r*range);
}

template<>
__inline
float4 randRange(const float4& minV, const float4& maxV)
{
	float4 r = make_float4( (rand()%10000)/10000.f, (rand()%10000)/10000.f, (rand()%10000)/10000.f, (rand()%10000)/10000.f );
	float4 range = maxV - minV;
	return (minV + r*range);
}


struct SortData
{
	union
	{
		u32 m_key;
		struct { u16 m_key16[2]; };
	};
	u32 m_value;

	friend bool operator <(const SortData& a, const SortData& b)
	{
		return a.m_key < b.m_key;
	}
};



template<typename T>
T* addByteOffset(void* baseAddr, u32 offset)
{
	return static_cast<T*>(
		static_cast<void*>(static_cast<char*>(baseAddr) + offset));
}


struct Pair32
{
	Pair32(){}
	Pair32(u32 a, u32 b) : m_a(a), m_b(b){}

	u32 m_a;
	u32 m_b;
};

struct PtrPair
{
	PtrPair(){}
	PtrPair(void* a, void* b) : m_a(a), m_b(b){}
	template<typename T>
	PtrPair(T* a, T* b) : m_a((void*)a), m_b((void*)b){}

	void* m_a;
	void* m_b;
};

#endif
