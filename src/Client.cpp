#include "Client.h"

Client::Client(const char * ip, unsigned short port, unsigned int &mic_count) {
	m_socket = new sf::TcpSocket;

	std::cout << "connecting to " << ip << ":" << port << std::endl;

	while(m_socket->connect(sf::IpAddress(ip), port) != sf::Socket::Done) {
		//NOOP
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //std::cerr << "could not connect to " << ip << ":" << port << std::endl;
	}

	std::size_t recieved_size;

	m_socket->receive(&m_mic_count, sizeof(unsigned int), recieved_size);

	mic_count = m_mic_count;
}

double * Client::recive(int count) {
	double * samples = (double *) malloc(sizeof(double) * m_mic_count * count);
	std::size_t recieved_size;

	if(m_socket->receive(samples, sizeof(double) * m_mic_count * count, recieved_size) != sf::Socket::Done) {
		return nullptr;
	}

	return samples;
}
