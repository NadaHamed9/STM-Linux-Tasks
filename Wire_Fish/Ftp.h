#ifndef FTP_H
#define FTP_H

#include "Tcp.h"

class Ftp : public Tcp {
	public:
		//Constructor
		Ftp(char *dev);
		//override function from Tcp class
		void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) override;
		//Destructor
		~Ftp();
};

#endif

