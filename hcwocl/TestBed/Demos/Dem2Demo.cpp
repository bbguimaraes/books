/*
		2011 Takahiro Harada
*/
#include <Demos/Dem2Demo.h>
#include <Common/Utils/Stopwatch.h>
#include <Common/Math/RadixSortHost.h>
#include <Common/Base/ThreadPool.h>

__inline
int4 ugConvertToGridCrd(const float4& pos, float gridScale)
{
	int4 g;
	g.x = (int)floor(pos.x*gridScale);
	g.y = (int)floor(pos.y*gridScale);
	g.z = (int)floor(pos.z*gridScale);
	return g;
}

__inline
int ugGridCrdToGridIdx(const int4& g, int nCellX, int nCellY, int nCellZ)
{
	return g.x+g.y*nCellX+g.z*nCellX*nCellY;
}

Dem2Demo::Dem2Demo(const DeviceDataBase* deviceData)
: Demo( 2 ), m_grid( NULL )
{
	m_enableAlphaBlending = true;
	m_backgroundColor = make_float4(1,1,1,0.f);


	INITIALIZE_DEVICE_DATA( deviceData, this );

	m_numSParticles = 128*4*1;
	m_numLParticles = 38;
	m_numFParticles = 16;

	m_numSParticles*=7;
	m_numLParticles*=2;


	m_scale = 1.f;//50.f;

	m_posL = new float4[m_numLParticles+m_numFParticles];
	m_velL = new float4[m_numLParticles+m_numFParticles];
	m_forceL = new float4[m_numLParticles+m_numFParticles];
	m_posS = new float4[m_numSParticles];
	m_velS = new float4[m_numSParticles];
	m_forceS = new float4[m_numSParticles];
	m_forceSHost = new float4[m_numSParticles];

#ifdef NO_INTERACTION
	DeviceBufferBase::Type type = DeviceBufferBase::BUFFER;
#else
	DeviceBufferBase::Type type = DeviceBufferBase::BUFFER_CPU_GPU;
#endif

	int numMaxParticles = max2(m_numSParticles, 0 );//256*1024);
	m_posD.allocate<float4>( m_deviceData, numMaxParticles, type );
	m_velD.allocate<float4>( m_deviceData, numMaxParticles, type );
	m_forceD.allocate<float4>( m_deviceData, numMaxParticles, type );
	m_forceDInt.allocate<float4>( m_deviceData, numMaxParticles, type );
	m_sBuffer.allocate<float4>( m_deviceData, numMaxParticles, DeviceBufferBase::BUFFER_STAGING );

	m_constBuffer.allocate<ConstBuffer>( m_deviceData, 1, DeviceBufferBase::BUFFER_CONST );

	{
		const char *option = "-I ../";
		DKernelBuilder builder( m_deviceData, "Demos/Dem2Demo", option, true );
		builder.createKernel("CollideGridKernel", m_collisionKernel );
		builder.createKernel("IntegrateKernel", m_integrateKernel );

	}

#ifndef NO_INTERACTION
	if( USE_ZERO_COPY )
	{
		m_posD.map<float4>( m_deviceData, m_numSParticles, m_posMapped, &m_sBuffer );
		m_velD.map<float4>( m_deviceData, m_numSParticles, m_velMapped, &m_sBuffer );
		m_forceDInt.map<float4>( m_deviceData, m_numSParticles, m_forceMapped, &m_sBuffer );
		DUtils::waitForCompletion( m_deviceData );
	}
#endif
}

Dem2Demo::~Dem2Demo()
{
#ifndef NO_INTERACTION
	if( USE_ZERO_COPY )
	{
		m_posD.unmap( m_deviceData, m_numSParticles, m_posMapped, &m_sBuffer );
		m_velD.unmap( m_deviceData, m_numSParticles, m_velMapped, &m_sBuffer );
		m_forceDInt.unmap( m_deviceData, m_numSParticles, m_forceMapped, &m_sBuffer );
	}
#endif

	delete [] m_posL;
	delete [] m_velL;
	delete [] m_forceL;
	delete [] m_posS;
	delete [] m_velS;
	delete [] m_forceS;
	delete [] m_forceSHost;

	m_posD.deallocate( m_deviceData );
	m_velD.deallocate( m_deviceData );
	m_forceD.deallocate( m_deviceData );
	m_forceDInt.deallocate( m_deviceData );
	m_sBuffer.deallocate( m_deviceData );

	m_constBuffer.deallocate( m_deviceData);


	DKernelBuilder::deleteKernel( m_deviceData, m_collisionKernel );
	DKernelBuilder::deleteKernel( m_deviceData, m_integrateKernel );

	if( m_grid ) 
	{
		int nCells = m_grid->m_gProps.m_nCells.x*m_grid->m_gProps.m_nCells.y*m_grid->m_gProps.m_nCells.z;
		m_grid->m_grid.unmap( m_deviceData, nCells*MAX_IDX_PER_GRID, m_gridMapped, &m_sBuffer );
		m_grid->m_gridCounter.unmap( m_deviceData, nCells, m_gridCounterMapped, &m_sBuffer );
		delete m_grid;
	}

	DESTROY_DEVICE_DATA( this );
}

void Dem2Demo::init()
{

}

void Dem2Demo::reset()
{
	m_gridIsDirty = true;
	float s = 1.f;

	float density = 100.f;
	float rL = 0.2f;
	float rS = 0.025f;
	rL /= 2.5f;
	rS /= 2.5f;


	for(int i=0; i<m_numLParticles+m_numFParticles; i++)
	{
		m_posL[i] = make_float4( randRange(-s+rL,s-rL),
			randRange(-s+rL,s-rL), 
			0 )*m_scale;
		float r = rL*m_scale;
		r *= randRange( 0.1f, 1.f );
		m_velL[i] = make_float4(0,0,0,r*r*PI*density);
		m_posL[i].w = r;

		if( i>=m_numLParticles )
		{
			m_velL[i] = make_float4(0,0,0,FLT_MAX);
			m_posL[i].w *= 1.5f;
		}
	}

	for(int i=0; i<m_numSParticles; i++)
	{
		m_posS[i] = make_float4( randRange(-s+rS,s-rS),
			randRange(-s+rS,s-rS), 
			0 )*m_scale;
		float r = rS*m_scale;
		m_velS[i] = make_float4(0,0,0,r*r*PI*density);
		m_posS[i].w = r;
	}

	{
		m_planes[0] = make_float4(0,1,0,m_scale);
		m_planes[1] = make_float4(-1,0,0,m_scale);
		m_planes[2] = make_float4(1,0,0,m_scale);
		m_planes[3] = make_float4(0,-1,0,m_scale);
	}

	for(int i=0; i<m_numLParticles; i++)
	{
		m_forceL[i] = make_float4(0,0,0,0);
	}
	for(int i=0; i<m_numSParticles; i++)
	{
		m_forceS[i] = make_float4(0,0,0,0);
		m_forceSHost[i] = make_float4(0,0,0,0);
	}

	{	//	create grid

		if( m_grid == NULL )
		{
			float cellSize = rS*m_scale*2.f;
			Aabb space;
			space.m_min = make_float4(-s, -s, 0, 0 )*m_scale;
			space.m_min -= rS*m_scale;
			space.m_max = make_float4(s, s, 0, 0 )*m_scale;
			space.m_max += rS*m_scale;
			space.m_min.z = 0.f;
			space.m_max.z = 0.f;
#ifdef NO_INTERACTION
			m_grid = new UniformGrid<DBuffer, DKernel>( m_deviceData, space, cellSize );
#else
			m_grid = new UniformGrid<DBuffer, DKernel>( m_deviceData, space, cellSize, true );
#endif
			int nCells = m_grid->m_gProps.m_nCells.x*m_grid->m_gProps.m_nCells.y*m_grid->m_gProps.m_nCells.z;
			if( USE_ZERO_COPY )
			{
			m_grid->m_grid.map<int>( m_deviceData, nCells*MAX_IDX_PER_GRID, m_gridMapped, &m_sBuffer );
			m_grid->m_gridCounter.map<int>( m_deviceData, nCells, m_gridCounterMapped, &m_sBuffer );
			}
		}
	}


	{
		{	//	sort by grid
			SortData* sortData = new SortData[ m_numSParticles ];
			SortData* sortData1 = new SortData[ m_numSParticles ];
			for(int i=0; i<m_numSParticles; i++)
			{
				int4 gCrd = ugConvertToGridCrd( m_posS[i]-m_grid->m_gProps.m_min, m_grid->m_gProps.m_gridScale );
				sortData[i].m_key = ugGridCrdToGridIdx( gCrd, m_grid->m_gProps.m_nCells.x, m_grid->m_gProps.m_nCells.y, m_grid->m_gProps.m_nCells.z );
				sortData[i].m_value = i;
			}

			RadixSortHost::sort( sortData, sortData1, m_numSParticles );

			float4* p = new float4[m_numSParticles];
			memcpy( p, m_posS, m_numSParticles*sizeof(float4) );

			for(int i=0; i<m_numSParticles; i++)
			{
				int dstIdx = sortData[i].m_value;
				m_posS[ dstIdx ] = p[ i ];
			}

			delete [] p;

			delete [] sortData;
			delete [] sortData1;
		}

		m_posD.write<float4>( m_deviceData, m_numSParticles, m_posS );
		m_velD.write<float4>( m_deviceData, m_numSParticles, m_velS );
		m_forceDInt.write<float4>( m_deviceData, m_numSParticles, m_forceSHost );
		DUtils::waitForCompletion( m_deviceData );
	}
}

__inline
float4 calcForce(const float4& x_i, const float4& x_j, const float4& v_i, const float4& v_j, float r_i, float r_j, float m_i, float m_j, float dt,
				 float e = 0.7f)
{
	float4 f = make_float4(0,0,0,0);
	float sCoeff, dCoeff;

	{
		float dtInv = 1.f/dt;
		float m = (m_i*m_j)/(m_i+m_j);
		sCoeff = m*dtInv*dtInv;
		dCoeff = m*dtInv*(1.f-e);
	}

	float4 x_ij = x_j-x_i;
	float dist2 = dot3F4( x_ij, x_ij );

	if( dist2 < pow( r_i+r_j, 2.f ) )
	{
		float dist = sqrtf( dist2 );
		f -= sCoeff*(r_i+r_j-dist)*x_ij/dist;
		f += dCoeff*(v_j - v_i);
	}
	return f;
}

void calcInteractions1(float4* p0, float4* v0, float4* fOut0, int n0,
					  float4* p1, float4* v1, int n1,
					  float dt, float e)
{
	for(int i=0; i<n0; i++)
	{
		float4 f = make_float4(0,0,0,0);

		for(int j=0; j<n1; j++)
		{
			if( p0[i].x == p1[j].x && p0[i].y == p1[j].y && p0[i].z == p1[j].z ) continue;

			f += calcForce( p0[i], p1[j], v0[i], v1[j], p0[i].w, p1[j].w, v0[i].w, v1[j].w, dt, e );
		}
		fOut0[i] += f;
	}
}

void calcInteractions(float4* p0, float4* v0, float4* fOut0, int n0,
					  float4* p1, float4* v1, float4* fOut1, int n1,
					  float dt, float e, UniformGrid<Dem2Demo::DBuffer, Dem2Demo::DKernel>* grid,
					  Dem2Demo::DMapBuffer* gridCounterM, Dem2Demo::DMapBuffer* gridDataM)
{
	if( grid )
	{
		const int4& nCells = grid->m_gProps.m_nCells;
		int* gridCounter = gridCounterM->getPtr<int>();
		int* gridData = gridDataM->getPtr<int>();
		float rS = p1[0].w;

		for(int iIdx=0; iIdx<n0; iIdx++)
		{
			const float4& x_i = p0[iIdx];
			const float4& v_i = v0[iIdx];
			float r_i = x_i.w;

			int4 iGridCrdMin = ugConvertToGridCrd( x_i-grid->m_gProps.m_min-r_i-rS, grid->m_gProps.m_gridScale );
			int4 iGridCrdMax = ugConvertToGridCrd( x_i-grid->m_gProps.m_min+r_i+rS, grid->m_gProps.m_gridScale );

			for(int i=iGridCrdMin.x; i<=iGridCrdMax.x; i++)
			for(int j=iGridCrdMin.y; j<=iGridCrdMax.y; j++)
			for(int k=iGridCrdMin.z; k<=iGridCrdMax.z; k++)
			{
				int4 gridCrd = make_int4( i,j,k,0 );

				if( gridCrd.x < 0 || gridCrd.x >= nCells.x
					|| gridCrd.y < 0 || gridCrd.y >= nCells.y
					|| gridCrd.z < 0 || gridCrd.z >= nCells.z ) continue;
				
				int gridIdx = ugGridCrdToGridIdx( gridCrd, nCells.x, nCells.y, nCells.z );
				int numElem = gridCounter[gridIdx];

				numElem = min2(MAX_IDX_PER_GRID, numElem);

				for(int ie=0; ie<numElem; ie++)
				{
					int jIdx = gridData[ MAX_IDX_PER_GRID*gridIdx+ie ];

					const float4& x_j = p1[jIdx];
					const float4& v_j = v1[jIdx];

					float4 f = calcForce( x_i, x_j, v_i, v_j, x_i.w, x_j.w, v_i.w, v_j.w, dt, e );

					fOut0[iIdx] += f;
					fOut1[jIdx] -= f;
				}
			}
		}
	}
	else
	{
		for(int i=0; i<n0; i++)
		{
			for(int j=0; j<n1; j++)
			{
				if( p0[i].x == p1[j].x && p0[i].y == p1[j].y && p0[i].z == p1[j].z ) continue;

				float4 f = calcForce( p0[i], p1[j], v0[i], v1[j], p0[i].w, p1[j].w, v0[i].w, v1[j].w, dt, e );

				fOut0[i] += f;
				fOut1[j] -= f;
			}
		}
	}
}

void calcBoundary(float4* p0, float4* v0, float4* fOut0, int n0, float dt, float e,
							float4* planes, int NUM_PLANES)
{
	for(int i=0; i<n0; i++)
	{
		float4 f = make_float4(0,0,0,0);
		float sCoeff, dCoeff;
		{
			float m = v0[i].w/2.f;
			sCoeff = m/(dt*dt);
			dCoeff = m/dt*(1.f-e);
		}
		for(int j=0; j<NUM_PLANES; j++)
		{
			const float4& eqn = planes[j];
			float dist = dot3w1( p0[i], eqn );
			float r_i = p0[i].w;
			if( dist < r_i )
			{
				f += sCoeff*(r_i-dist)*eqn;
				f += dCoeff*(-v0[i]);
			}
		}
		fOut0[i] += f;
	}
}

void integration(float4* p0, float4* v0, float4* f0, int n0, float dt, const float4& g)
{
	for(int i=0; i<n0; i++)
	{

		if( v0[i].w == FLT_MAX ) continue;


		float4 x = p0[i];
		float4 v = v0[i];

		v += f0[i]*dt/v.w+g;
		x += v*dt;

		p0[i] = make_float4(x.x, x.y, x.z, p0[i].w);
		v0[i] = make_float4(v.x, v.y, v.z, v0[i].w);
		f0[i] = make_float4(0,0,0,0);
	}
}

struct GPUCollideTask : public ThreadPool::Task
{
	typedef Dem2Demo::DKernel DKernel;
	typedef Dem2Demo::DBuffer DBuffer;
	typedef Dem2Demo::DKernelLauncher DKernelLauncher;
	typedef Dem2Demo::DUtils DUtils;

	u16 getType(){ return 0; }

	void run(int tIdx)
	{
		DKernelLauncher launcher( m_deviceData, *m_collisionKernel );
		launcher.pushBackR( *m_pos );
		launcher.pushBackR( *m_vel );
		launcher.pushBackR( *m_grid );
		launcher.pushBackR( *m_gridCounter );
		launcher.pushBackRW( *m_force );
		launcher.setConst( *m_constBuffer, m_constDatas );
		launcher.launch1D( m_numParticles );
		DUtils::waitForCompletion( m_deviceData );
	}

	const DeviceDataBase* m_deviceData;
	DKernel* m_collisionKernel;
	DBuffer* m_pos;
	DBuffer* m_vel;
	DBuffer* m_grid;
	DBuffer* m_gridCounter;
	DBuffer* m_force;
	DBuffer* m_constBuffer;
	Dem2Demo::ConstBuffer* m_constDatas;
	int m_numParticles;
};

struct GPUIntegrateTask : public ThreadPool::Task
{
	typedef Dem2Demo::DKernel DKernel;
	typedef Dem2Demo::DBuffer DBuffer;
	typedef Dem2Demo::DKernelLauncher DKernelLauncher;
	typedef Dem2Demo::DUtils DUtils;

	u16 getType(){ return 0; }

	void run(int tIdx)
	{
		DKernelLauncher launcher( m_deviceData, *m_integrateKernel );
		launcher.pushBackRW( *m_pos );
		launcher.pushBackRW( *m_vel );
		launcher.pushBackRW( *m_force );
		launcher.pushBackRW( *m_forceInt );
		launcher.setConst( *m_constBuffer, m_constDatas );
		launcher.launch1D( m_numParticles );
//				dsw.stop();
		*m_gridIsDirty = false;
		m_grid->clearAndBuild( *m_pos, m_numParticles );

		DUtils::waitForCompletion( m_deviceData );
	}

	const DeviceDataBase* m_deviceData;
	DKernel* m_integrateKernel;
	DBuffer* m_pos;
	DBuffer* m_vel;
	DBuffer* m_force;
	DBuffer* m_forceInt;
	DBuffer* m_constBuffer;
	Dem2Demo::ConstBuffer* m_constDatas;
	int m_numParticles;
	UniformGrid<DBuffer, DKernel>* m_grid;
	bool* m_gridIsDirty;
};

struct CPUCollideTask : public ThreadPool::Task
{
	u16 getType(){ return 1; }

	void run(int tIdx)
	{
		calcInteractions1( m_posL, m_velL, m_forceL, m_numLParticles, m_posL, m_velL, m_numLParticles, m_dt, m_e );
		calcBoundary( m_posL, m_velL, m_forceL, m_numLParticles, m_dt, m_e, m_planes, m_numPlanes );
	}

	float4* m_posL;
	float4* m_velL;
	float4* m_forceL;
	int m_numLParticles;
	float m_dt;
	float m_e;
	float4* m_planes;
	int m_numPlanes;
};

struct CPUInteractTask : public ThreadPool::Task
{
	typedef Dem2Demo::DMapBuffer DMapBuffer;
	typedef Dem2Demo::DKernel DKernel;
	typedef Dem2Demo::DBuffer DBuffer;

	u16 getType(){ return 1; }

	void run(int tIdx)
	{
		float4* posSHostPtr = m_posMapped->getPtr<float4>();
		float4* velSHostPtr = m_velMapped->getPtr<float4>();
		float4* forceSHostPtr = m_forceMapped->getPtr<float4>();

		{
			memcpy( m_posS, posSHostPtr, sizeof(float4)*m_numSParticles );
			memcpy( m_velS, velSHostPtr, sizeof(float4)*m_numSParticles );
			
			calcInteractions( m_posL, m_velL, m_forceL, m_numLParticles, m_posS, m_velS, m_forceSHost, m_numSParticles, m_dt, m_e,
				m_grid, m_gridCounterMapped, m_gridMapped );
		}

		memcpy( forceSHostPtr, m_forceSHost, sizeof(float4)*m_numSParticles );
	}

	float4* m_posL;
	float4* m_velL;
	float4* m_forceL;
	float4* m_posS;
	float4* m_velS;
	float4* m_forceS;
	float4* m_forceSHost;
	int m_numLParticles;
	int m_numSParticles;
	float m_dt;
	float m_e;

	UniformGrid<DBuffer, DKernel>* m_grid;
	DMapBuffer* m_posMapped;
	DMapBuffer* m_velMapped;
	DMapBuffer* m_forceMapped;
	DMapBuffer* m_gridMapped;
	DMapBuffer* m_gridCounterMapped;
};




struct CPUIntegrateTask : public ThreadPool::Task
{
	u16 getType(){ return 1; }

	void run(int tIdx)
	{
		for(int i=0; i<m_numSParticles; i++)	//	sync
		{
			m_forceSHost[i] = make_float4(0,0,0,0);
		}
		integration( m_posL, m_velL, m_forceL, m_numLParticles, m_dt, *m_g );
	}

	float4* m_posL;
	float4* m_velL;
	float4* m_forceL;
	float4* m_forceSHost;
	int m_numLParticles;
	int m_numSParticles;
	float m_dt;
	float4* m_g;
};

#define UNMAP_ALL int nCells = m_grid->m_gProps.m_nCells.x*m_grid->m_gProps.m_nCells.y*m_grid->m_gProps.m_nCells.z; \
				m_grid->m_grid.unmap( m_deviceData, nCells*MAX_IDX_PER_GRID, m_gridMapped, &m_sBuffer ); \
				m_grid->m_gridCounter.unmap( m_deviceData, nCells, m_gridCounterMapped, &m_sBuffer ); \
				m_posD.unmap( m_deviceData, m_numSParticles, m_posMapped, &m_sBuffer ); \
				m_velD.unmap( m_deviceData, m_numSParticles, m_velMapped, &m_sBuffer ); \
				m_forceDInt.unmap( m_deviceData, m_numSParticles, m_forceMapped, &m_sBuffer ); \
				DUtils::waitForCompletion( m_deviceData );

#define MAP_ALL	int nCells = m_grid->m_gProps.m_nCells.x*m_grid->m_gProps.m_nCells.y*m_grid->m_gProps.m_nCells.z;\
				m_grid->m_grid.map<int>( m_deviceData, nCells*MAX_IDX_PER_GRID, m_gridMapped, &m_sBuffer );\
				m_grid->m_gridCounter.map<int>( m_deviceData, nCells, m_gridCounterMapped, &m_sBuffer );\
				m_posD.map<float4>( m_deviceData, m_numSParticles, m_posMapped, &m_sBuffer );\
				m_velD.map<float4>( m_deviceData, m_numSParticles, m_velMapped, &m_sBuffer );\
				m_forceDInt.map<float4>( m_deviceData, m_numSParticles, m_forceMapped, &m_sBuffer );\
				DUtils::waitForCompletion( m_deviceData );


void Dem2Demo::step( float dt )
{
	int nIter = 2;

	nIter *= 2;

	float e = 0.85f;
	float4 g = make_float4(0.f, -9.8f, 0.f, 0.f) * 0.5f;

	dt/=1200.f;

	ConstBuffer cBuffer;
	{
		cBuffer.m_g = g;
		cBuffer.m_dt = dt;
		cBuffer.m_numParticles = m_numSParticles;
		cBuffer.m_scale = m_scale;
		cBuffer.m_e = e;
		cBuffer.m_nCells = m_grid->m_gProps.m_nCells;
		cBuffer.m_spaceMin = m_grid->m_gProps.m_min;
		cBuffer.m_gridScale = m_grid->m_gProps.m_gridScale;
	}

	//	todo. have to flush DX pipe. 
	ThreadPool& threadPool = *m_threads;

	for(int iter=0; iter<nIter; iter++)
	{
		Stopwatch swGlobal;
		swGlobal.start();

		//	this shouldn't be here
		if( m_gridIsDirty )
		{
			m_grid->clearAndBuild( m_posD, m_numSParticles );
			DUtils::waitForCompletion( m_deviceData );
		}

		CPUCollideTask* cpuCollideTask = new CPUCollideTask;
		{
			GPUCollideTask* gpuCollideTask = new GPUCollideTask;
			{
				gpuCollideTask->m_deviceData = m_deviceData;
				gpuCollideTask->m_collisionKernel = &m_collisionKernel;
				gpuCollideTask->m_pos = &m_posD;
				gpuCollideTask->m_vel = &m_velD;
				gpuCollideTask->m_grid = &m_grid->m_grid;
				gpuCollideTask->m_gridCounter = &m_grid->m_gridCounter;
				gpuCollideTask->m_force = &m_forceD;
				gpuCollideTask->m_constBuffer = &m_constBuffer;
				gpuCollideTask->m_constDatas = &cBuffer;
				gpuCollideTask->m_numParticles = m_numSParticles;
			}
			{
				cpuCollideTask->m_posL = m_posL;
				cpuCollideTask->m_velL = m_velL;
				cpuCollideTask->m_forceL = m_forceL;
				cpuCollideTask->m_numLParticles = m_numLParticles+m_numFParticles;
				cpuCollideTask->m_dt = dt;
				cpuCollideTask->m_e = e;
				cpuCollideTask->m_planes = m_planes;
				cpuCollideTask->m_numPlanes = NUM_PLANES;
			}

			if( !USE_ZERO_COPY )
			{
				threadPool.pushBack( gpuCollideTask );
				threadPool.pushBack( cpuCollideTask );
				threadPool.start( (iter==nIter-2 || iter==0)? true : false );
				threadPool.wait();

				MAP_ALL;
			}

			CPUInteractTask* cpuInteractTask = new CPUInteractTask;
			{
				cpuInteractTask->m_posL = m_posL;
				cpuInteractTask->m_velL = m_velL;
				cpuInteractTask->m_forceL = m_forceL;
				cpuInteractTask->m_posS = m_posS;
				cpuInteractTask->m_velS = m_velS;
				cpuInteractTask->m_forceS = m_forceS;
				cpuInteractTask->m_forceSHost = m_forceSHost;
				cpuInteractTask->m_numLParticles = m_numLParticles+m_numFParticles;
				cpuInteractTask->m_numSParticles = m_numSParticles;
				cpuInteractTask->m_dt = dt;
				cpuInteractTask->m_e = e;
				cpuInteractTask->m_grid = m_grid;
				cpuInteractTask->m_posMapped = &m_posMapped;
				cpuInteractTask->m_velMapped = &m_velMapped;
				cpuInteractTask->m_forceMapped = &m_forceMapped;
				cpuInteractTask->m_gridMapped = &m_gridMapped;
				cpuInteractTask->m_gridCounterMapped = &m_gridCounterMapped;
			}

			if( USE_ZERO_COPY )	//	1.	collide small(gpuCollideTask), collide large and small(cpuInteractTask).
			{
				threadPool.pushBack( gpuCollideTask );
				threadPool.pushBack( cpuInteractTask );
				threadPool.start((iter==nIter-2 || iter==0)? true : false);
				threadPool.wait();
			}
			else
			{
				threadPool.pushBack( cpuInteractTask );
				threadPool.start(false);
				threadPool.wait();

				UNMAP_ALL;
			}
		}
		//	2. sync GPU and CPU
		{
			GPUIntegrateTask* gpuIntegrateTask = new GPUIntegrateTask;
			CPUIntegrateTask* cpuIntegrateTask = new CPUIntegrateTask;
			{
				gpuIntegrateTask->m_deviceData = m_deviceData;
				gpuIntegrateTask->m_integrateKernel = &m_integrateKernel;
				gpuIntegrateTask->m_pos = &m_posD;
				gpuIntegrateTask->m_vel = &m_velD;
				gpuIntegrateTask->m_force = &m_forceD;
				gpuIntegrateTask->m_forceInt = &m_forceDInt;
				gpuIntegrateTask->m_constBuffer = &m_constBuffer;
				gpuIntegrateTask->m_constDatas = &cBuffer;
				gpuIntegrateTask->m_numParticles = m_numSParticles;
				gpuIntegrateTask->m_grid = m_grid;
				gpuIntegrateTask->m_gridIsDirty = &m_gridIsDirty;
			}

			if( !USE_ZERO_COPY )
			{
				threadPool.pushBack( gpuIntegrateTask );
				threadPool.start(false);
				threadPool.wait();

				MAP_ALL;
			}

			{
				cpuIntegrateTask->m_posL = m_posL;
				cpuIntegrateTask->m_velL = m_velL;
				cpuIntegrateTask->m_forceL = m_forceL;
				cpuIntegrateTask->m_forceSHost = m_forceSHost;
				cpuIntegrateTask->m_numLParticles = m_numLParticles;
				cpuIntegrateTask->m_numSParticles = m_numSParticles;
				cpuIntegrateTask->m_dt = dt;
				cpuIntegrateTask->m_g = &g;
			}

			if( USE_ZERO_COPY )//	3.	collide big(cpuCollideTask), integrate small(gpuIntegrateTask), integrate large(cpuIntegrateTask)
			{
				threadPool.pushBack( cpuCollideTask );
				threadPool.pushBack( gpuIntegrateTask );
				threadPool.pushBack( cpuIntegrateTask );
				threadPool.start(false);
				threadPool.wait();
			}
			else
			{
				threadPool.pushBack( cpuIntegrateTask );
				threadPool.start(false);
				threadPool.wait();

				UNMAP_ALL;
			}
			
			//	4. sync GPU and CPU
		}

		swGlobal.stop();

		if( iter==nIter-1 )
		{
			float tg = swGlobal.getMs();

			m_nTxtLines = 0;
			{
				std::snprintf(m_txtBuffer[m_nTxtLines++], LINE_CAPACITY, "S:%d, L:%d", m_numSParticles, m_numLParticles);
				std::snprintf(m_txtBuffer[m_nTxtLines++], LINE_CAPACITY, "Total : %3.3f", tg);
			}
		}
	}

	{
		m_posD.read<float4>( m_deviceData, m_numSParticles, m_posS, &m_sBuffer );
		m_velD.read<float4>( m_deviceData, m_numSParticles, m_velS, &m_sBuffer );
		DUtils::waitForCompletion( m_deviceData );
	}
}


__inline
void renderCircle(const float4& c, float r)
{
	float4 vec[] = {make_float4(0,1,0), make_float4(1,0,0), make_float4(0,-1,0), make_float4(-1,0,0)};

	for(int idir=0; idir<4; idir++)
	{
		float4 wDir;
		wDir = vec[idir]*r*0.95f;
		pxDrawLine( c, c+wDir, (idir==0)? make_float4(0,1,0,1): make_float4(1,1,1,1) );

		wDir = normalize3(vec[idir]+vec[(idir+1)%4])*r*0.95f;
		pxDrawLine( c, c+wDir, make_float4(1,1,1,1) );
	}
}

void Dem2Demo::render()
{
	for(int i=0; i<m_numLParticles; i++)
	{
		renderCircle( m_posL[i]/m_scale, m_posL[i].w/m_scale );
	}

	for(int i=0; i<m_numSParticles; i++)
	{
		renderCircle( m_posS[i]/m_scale, m_posS[i].w/m_scale );
	}
}

