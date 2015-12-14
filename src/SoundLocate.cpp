#include "SoundLocate.h"

SoundLocate::SoundLocate(IStream & in, OStream & out, int blocksize) {
	m_blocksize = blocksize;
	m_in = in;
	m_out = out;
}

int SoundLocate::start() {
	#pragma omp task
	m_start();
}

int SoundLocate::m_start() {
	float * samples = nullptr;
	std::vector<float> outdata;
	
	while(m_in.open()) {
		while(!samples) {
			samples = m_in.read(m_blocksize);
		}
		
		m_out.write(samples, m_blocksize);
	}
}
