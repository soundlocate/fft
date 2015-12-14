#ifndef _CLIENT_H
#define _CLIENT_H

#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <thread>

class Client  {
public:
	Client(const char * ip, unsigned short port, unsigned int &mic_count);

	double * recive(int count);
private:
	sf::TcpSocket * m_socket = nullptr;
	unsigned int m_mic_count = 0;
};

#endif
