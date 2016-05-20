// robin Copyright (C) 2015

#ifndef _FFT_FFT_H
#define _FFT_FFT_H

#include <fftw3.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>

typedef fftw_complex complex;

class FFT {
public:
	FFT(int, int, int);
	FFT(int, const char *);
	
	complex * execute(double * indata, complex * outdata);

	~FFT();
private:
	int m_size;
	int m_count;

	fftw_plan m_plan;

	double * m_window = nullptr;
	double * m_indata = nullptr;
	complex * m_outdata = nullptr;
};

#endif
