// robin Copyright (C) 2015

#ifndef _FFT_FFT_H
#define _FFT_FFT_H

#include <fftw3.h>
#include <cstdlib>
#include <iostream>

typedef fftw_complex complex;

class FFT {
public:
	FFT(int, int);
	FFT(int, const char *);
	
	complex * execute(double * indata, complex * outdata);

	~FFT();
private:
	int m_size;

	fftw_plan m_plan;

	double * m_indata = nullptr;
	complex * m_outdata = nullptr;
};

#endif
