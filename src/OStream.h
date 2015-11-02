// Robin Heinemann Copyright (C) 2015

#ifndef _OSTREAM_H
#define _OSTREAM_H

#include <vector>
#include <iostream>

class OStream {
public:
	OStream();
	int write(float * data, int count);
	float * read();
private:
	std::vector<float> m_buffer;
};

#endif 
