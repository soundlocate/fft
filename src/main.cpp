#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cstring>

#include <Stopwatch.h>

#include <SFML/Network.hpp>

#include "fft/FFT.h"
#include "Client.h"
#include "Server.h"

FFT * fft;
double * in, * out_converted;
complex * out;

constexpr unsigned int samplerate = 192000;

int update_buffer(double * base_buffer, double * new_data, unsigned int base_count, unsigned int new_count) {
	unsigned int offset = sizeof(double) * new_count;
	unsigned int base_size = sizeof(double) * base_count;

	memmove(base_buffer, base_buffer + new_count, base_size - offset);
    memcpy(base_buffer + (base_count -  new_count), new_data, offset);

	return 0;
}


int init(int samples, int mic_count) {
	auto planningTime = std::chrono::high_resolution_clock::now();

	fft = new FFT(samples, mic_count);

	in  =  (double *) malloc(sizeof(double) * samples * mic_count);
    out = (complex *) malloc(sizeof(complex) * samples * mic_count);
    out_converted = (double *) malloc(sizeof(double) * samples * mic_count * 3);

	auto finalTime = std::chrono::duration<double, std::ratio<1l, 1000l>>(std::chrono::high_resolution_clock::now() - planningTime);

	std::cout << "Planning took: " << finalTime.count() << "ms" << std::endl;

	return 0;
}

int cleanup() {
	free(in);
	free(out);
	free(out_converted);

	return 0;
}

int convert_output(complex * out, double * out_converted, int samples, int mic_count) {
	for(int i = 0; i < samples; i++) {
		for(int j = 0; j < mic_count; j++) {
			double real = out[i * mic_count + j][0];
			double complex = out[i * mic_count + j][1];

			//            (samplerate / size of fft)
			double freq = i * (samplerate / samples);
			double phase = atan(real / complex);
			double amplitude = (sqrt(real * real + complex * complex)) / (0.5 * samplerate);

			out_converted[3 * mic_count * i + 3 * j] = freq;
			out_converted[3 * mic_count * i + 3 * j + 1] = phase;
			out_converted[3 * mic_count * i + 3 * j + 2] = amplitude;
		}
	}

	return 0;
}

int main(int argc, char ** argv) {
//	int samples = atoi(argv[1]);
	int block_size = 9600;
	int samples = 192000;

	unsigned int mic_count;

	Client client(argv[1], atoi(argv[2]), mic_count);
	Server server(atoi(argv[3]), [&mic_count](sf::TcpSocket * socket){
			socket->send(&mic_count, sizeof(unsigned int));
		});

	std::cout << "mic_count: " << mic_count << std::endl;

	double * in_new;

	init(samples, mic_count);

	Stopwatch::getInstance().setCustomSignature(32434);

	while(true) {
		auto fftTime = std::chrono::high_resolution_clock::now();
		TICK("fft_total");

		STOPWATCH("fft_execute_fft",
				  fft->execute(in, out);
			);

		auto finalTime = std::chrono::duration<double, std::ratio<1l, 1000l>>(std::chrono::high_resolution_clock::now() - fftTime);

//		std::cout << "FFT took: " << finalTime.count() << "ms" << std::endl;

		STOPWATCH("fft_convert_output",
				  convert_output(out, out_converted, samples, mic_count);
			);

		STOPWATCH("fft_send_data",
				  //server.send(out_converted, samples * mic_count * 3);
				  server.send(out_converted, 210 * mic_count * 3);
			);

		STOPWATCH("fft_recieve_data",
				  in_new = client.recive(block_size);
			);

		if(in_new == nullptr) {
			std::cout << "server disconnected, trying reconnect" << std::endl;

			client = Client(argv[1], atoi(argv[2]), mic_count);

			std::cout << "reconnected :)" << std::endl;
			std::cout << "new mic_count: " << mic_count << std::endl;
			std::cout << "rebuilding fftw_plan" << std::endl;

			cleanup();
			init(samples, mic_count);
		}

/*		for(int i = 0; i < block_size; i++) {
			std::cout << in[4 * i] << std::endl;
			std::cout << in[4 * i + 1] << std::endl;
			std::cout << in[4 * i + 2] << std::endl;
			std::cout << in[4 * i + 3] << std::endl;
		}
*/
		STOPWATCH("fft_remap_buffer",
				  update_buffer(in, in_new, samples * mic_count, block_size * mic_count);
			);

		TOCK("fft_total");

		Stopwatch::getInstance().sendAll();
	}
}
