#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <algorithm>
#include <iterator>
#include <fstream>

#include <SFML/Audio.hpp>

#include "fft/FFT.h"

int main(int argc, char ** argv) {
	sf::InputSoundFile audio_in;

	audio_in.openFromFile("testSound.wav");

	int samples = atoi(argv[1]);

	auto planningTime = std::chrono::high_resolution_clock::now();

	FFT fft(samples);

	auto finalTime = std::chrono::duration<double, std::ratio<1l, 1000l>>(std::chrono::high_resolution_clock::now() - planningTime);
	
	std::cout << "Planning took: " << finalTime.count() << "ms" << std::endl;
	
	double * in  =  (double *) malloc(sizeof(double) * samples);
	int16_t * in_tmp = (int16_t *) malloc(sizeof(int16_t) * samples);
	complex * out = (complex *) malloc(sizeof(complex) * samples);

	audio_in.read(in_tmp, samples);
	
	const int16_t int16_max = std::numeric_limits<int16_t>::max();
	
	for (int i = 0; i < samples; i++) {
		in[i] = (double) in_tmp[i] / (double) int16_max;
	}

	auto fftTime = std::chrono::high_resolution_clock::now();
	
	fft.execute(in, out);

	finalTime = std::chrono::duration<double, std::ratio<1l, 1000l>>(std::chrono::high_resolution_clock::now() - fftTime);

	std::cout << "FFT took: " << finalTime.count() << "ms" << std::endl;
	
	std::ofstream in_file;
	in_file.open("in.txt");
	std::ofstream out_file;
	out_file.open("out.txt");

	for (int i = 0; i < samples; i++) {
		in_file << i << "\t" << in[i] << std::endl;
		if (i <= samples / 2 + 1) 
			//out_file << i << "\t" << out[i][0] << "\t" << out[i][1] << std::endl;
			out_file << i << "\t" << sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / (double) ((double) samples / 2.0) << "\t" << atan(out[i][0] / out[i][1])  << std::endl;
	}

	std::cout << "samples: " << samples << std::endl;
	
	in_file.close();
	out_file.close();
}
