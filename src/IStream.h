// Robin Heinemann Copyright (C) 2015

#ifndef _ISTREAM_H
#define _ISTREAM_H

#include <cstdio>
#include <iostream>

class IStream {
public:
  IStream() {};
  IStream(const char * filename);
  float * read(int count);
  bool open();
private:
  float * m_buffer;
  int m_buffersize = 0;
  FILE * m_file;
};

#endif
