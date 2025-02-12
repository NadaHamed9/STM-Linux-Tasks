#ifndef TCP_H
#define TCP_H

#include "Ip.h"

class Tcp : public Ip {
	public:
		//Constructor
		Tcp(char *dev);
		//overrides packet handler in Ip class which is inherited from 
		void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) override;
		//Destructor
		~Tcp();
};

#endif
