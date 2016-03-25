#include "FFT.h"

FFT::FFT(int size, int count) : m_size(size), m_count(count) {
	m_indata = (double *) malloc(sizeof(double) * size * count);
	m_outdata = (complex * ) malloc(sizeof(complex) * size * count);

	char filename[512];

	sprintf(filename, "fftw_wisdom/dr2c%dx%d", size, count);
	fftw_init_threads();
	fftw_plan_with_nthreads(4);
	fftw_import_wisdom_from_filename(filename);

	int n = size;

//	fftw_plan_many_dft_r2c(int rank, const int *n, int howmany, double *in, const int *inembed, int istride, int idist, fftw_complex *out, const int *onembed, int ostride, int odist, unsigned int flags)
	m_plan = fftw_plan_many_dft_r2c(1, &n, count, m_indata, nullptr, count, 1, m_outdata, nullptr, count, 1, FFTW_PATIENT);

//	m_plan = fftw_plan_dft_r2c_1d(size, m_indata, m_outdata, FFTW_EXHAUSTIVE);

	// build hamming window table
	m_window = (double *) malloc(sizeof(double) * size);

	for(int i = 0; i < size; i++) {
	    m_window[i] = 0.54 - 0.46 * cos((2.0 * M_PI * i) / ((double) size - 1.0));
	}

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
    memcpy(m_indata, indata, sizeof(double) * m_size * m_count);

	for(int i = 0; i < m_size; i++) {
		for(int j = 0; j < m_count; j++) {
			m_indata[m_count * i + j] *= m_window[i];
		}
	}

	fftw_execute_dft_r2c(m_plan, m_indata, outdata);
	
	return outdata;
}

FFT::~FFT() {
	free(m_indata);
	free(m_outdata);
}
