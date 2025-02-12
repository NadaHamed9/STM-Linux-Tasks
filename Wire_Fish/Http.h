#ifndef HTTP_H
#define HTTP_H

#include "Tcp.h"

class Http : public Tcp {
	public:
		//Constructor
		Http(char *dev);
		//override function from Tcp class
		void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) override;
		//Destructor
		~Http();
};

#endif

