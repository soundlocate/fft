// Robin Heinemann Copyright (C) 2015

#include "IStream.h"

IStream::IStream(const char * filename) {
	m_file = fopen(filename, "r");
}

float * IStream::read(int count) {
	unsigned int size = count * sizeof(float);

	if(count > m_buffersize) {
		m_buffer = (float *) malloc(count * sizeof(float));
	}
	
	if(fread(m_buffer, sizeof(float), count, m_file) == count * sizeof(float))
		return m_buffer;
	else
		return nullptr;
	
}

bool IStream::open() {
	return feof(m_file);
}
