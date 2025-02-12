#ifndef IP
#define IP

#include "PacketSniffer.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

class Ip : public PacketSniffer
{
	public:
	//constructor
	Ip(char* dev);
	//Function to start capturing data
	void Capture() override;

	static void packet_handler_converter(u_char *user,const struct pcap_pkthdr *pkthdr,const u_char *packet);
	// Callback function called by pcap for each captured packet
	virtual void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet);
	//Destructor 
	~Ip();

};

#endif

