#include "Client.h"

Client::Client(const char * ip, unsigned short port, unsigned int &mic_count) {
	m_socket = new sf::TcpSocket;
	m_socket->setBlocking(true);

	std::cout << "blocking: " << (m_socket->isBlocking() ? "true" : "false") << std::endl;
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
	std::size_t recieved_size = 0, to_recieve = m_mic_count * count * sizeof(double), recieved_total = 0;
    char * samples = (char *) malloc(to_recieve);

	while(recieved_total != to_recieve) {
		if(m_socket->receive(samples + recieved_total, to_recieve - recieved_total, recieved_size) == sf::Socket::Disconnected) {
			return nullptr;
		}

		recieved_total += recieved_size;

//		std::cout << "recieved " << (float) recieved_total / (m_mic_count * sizeof(double)) << " samples" << std::endl;
	}

	return (double *) samples;
}
