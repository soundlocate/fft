#include "Client.h"

Client::Client(const char * ip, unsigned short port, unsigned int &mic_count) {
	m_socket = new sf::TcpSocket;
	m_socket->setBlocking(true);

	std::cout << "connecting to " << ip << ":" << port << std::endl;

	while(m_socket->connect(sf::IpAddress(ip), port) != sf::Socket::Done) {
		//NOOP
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //std::cerr << "could not connect to " << ip << ":" << port << std::endl;
	}

	std::size_t received_size;

	m_socket->receive(&m_mic_count, sizeof(unsigned int), received_size);

	mic_count = m_mic_count;
}

void Client::reconnect(const char * ip, unsigned short port, unsigned int &mic_count) {
	m_socket->disconnect();

	std::cout << "reconnecting to " << ip << ":" << port << std::endl;

	while(m_socket->connect(sf::IpAddress(ip), port) != sf::Socket::Done) {
		//NOOP
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //std::cerr << "could not connect to " << ip << ":" << port << std::endl;
		m_socket->disconnect();
	}

	std::size_t received_size;

	m_socket->receive(&m_mic_count, sizeof(unsigned int), received_size);

	mic_count = m_mic_count;
}

double * Client::receive(int count) {
	std::size_t received_size = 0, to_receive = m_mic_count * count * sizeof(double), received_total = 0;
    char * samples = (char *) malloc(to_receive);

	while(received_total != to_receive) {
		if(m_socket->receive(samples + received_total, to_receive - received_total, received_size) == sf::Socket::Disconnected) {
			return nullptr;
		}

		received_total += received_size;

//		std::cout << "recieved " << (float) received_total / (m_mic_count * sizeof(double)) << " samples" << std::endl;
	}

	return (double *) samples;
}
