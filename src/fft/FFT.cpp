#include "FFT.h"

FFT::FFT(int size) : m_size(size) {
	m_indata = (double *) malloc(sizeof(double) * size);
	m_outdata = (complex * ) malloc(sizeof(complex) * size);

	char filename[512];

	sprintf(filename, "fftw_wisdom/r2c%d", size);
	
	fftw_import_wisdom_from_filename(filename);
	
	m_plan = fftw_plan_dft_r2c_1d(size, m_indata, m_outdata, FFTW_EXHAUSTIVE);

	fftw_export_wisdom_to_filename(filename);    
}

FFT::FFT(int size, const char * filename) {
	m_indata = (double *) malloc(sizeof(double) * size);
	m_outdata = (complex *) malloc(sizeof(complex) * size);

	std::cout << "wisdom import: " << fftw_import_wisdom_from_filename(filename) << std::endl;

	m_plan = fftw_plan_dft_r2c_1d(size, m_indata, m_outdata, FFTW_EXHAUSTIVE);
}

complex * FFT::execute(double * indata, complex * outdata) {
	fftw_execute_dft_r2c(m_plan, indata, outdata);
	
	return outdata;
}

FFT::~FFT() {
	free(m_indata);
	free(m_outdata);
}
