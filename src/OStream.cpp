// Robin Heinemann Copyright (C) 2015

#include "OStream.h"

OStream::OStream() {
	
}

int OStream::write(float * data, int count) {
	for (int i = 0; i < count; i++) {
		m_buffer.push_back(data[i]);
	}

}

float * OStream::read() {
	float * ret = (float *) malloc(m_buffer.size() * sizeof(float));
	
	for (int i = 0; i < m_buffer.size(); i++) {
		ret[i] = m_buffer[i];
	}

	m_buffer.clear();
	
	return ret;
}
