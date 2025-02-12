#ifndef DNS_H
#define DNS_H

#include "Udp.h"

class Dns : public Udp {
	public:
		//Constructor
		Dns(char *dev);
		//override function from Udb class
		void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) override;
		//Destructor
		~Dns();
};

#endif

