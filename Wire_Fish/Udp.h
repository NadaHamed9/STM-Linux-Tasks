#ifndef UDP_H
#define UDP_H

#include "Ip.h"

class Udp : public Ip {
	public:
		//constructor
		Udp(char *dev);
		//overrides packet handler in Ip class which is inherited from
		void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) override;
		//Destructor
		~Udp();
};

#endif

