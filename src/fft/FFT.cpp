#include "FFT.h"

FFT::FFT(int size, int count) : m_size(size) {
	m_indata = (double *) malloc(sizeof(double) * size * count);
	m_outdata = (complex * ) malloc(sizeof(complex) * size * count);

	char filename[512];

	sprintf(filename, "fftw_wisdom/dr2c%dx%d", size, count);

	fftw_import_wisdom_from_filename(filename);

	int n[] = {size};

	m_plan = fftw_plan_many_dft_r2c(1, n, count, m_indata, n, count, 1, m_outdata, n, count, 1, FFTW_EXHAUSTIVE);

//	m_plan = fftw_plan_dft_r2c_1d(size, m_indata, m_outdata, FFTW_EXHAUSTIVE);

	fftw_export_wisdom_to_filename(filename);    
}

FFT::FFT(int size, const char * filename) {
/*	m_indata = (double *) malloc(sizeof(double) * size);
	m_outdata = (complex *) malloc(sizeof(complex) * size);

	std::cout << "wisdom import: " << fftw_import_wisdom_from_filename(filename) << std::endl;

	m_plan = fftw_plan_dft_r2c_1d(size, m_indata, m_outdata, FFTW_EXHAUSTIVE);
*/
}

complex * FFT::execute(double * indata, complex * outdata) {
	fftw_execute_dft_r2c(m_plan, indata, outdata);
	
	return outdata;
}

FFT::~FFT() {
	free(m_indata);
	free(m_outdata);
}
