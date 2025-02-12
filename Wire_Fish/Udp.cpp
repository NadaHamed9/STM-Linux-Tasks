#include "Udp.h"
//#include "Dns.h"
// Constructor implementation
Udp::Udp(char *dev) : Ip(dev) {
	printf("UDP Sniffer initialized\n");
}

//handle udp headers
void Udp::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	struct iphdr *ip = (struct iphdr*)(packet + 14); // Skip Ethernet header
	struct udphdr *udp = (struct udphdr*)(packet + 14 + ip->ihl * 4);

	printf("UDP Header:\n");
        printf("   |-Source Port      : %u\n", ntohs(udp->source));
        printf("   |-Destination Port : %u\n", ntohs(udp->dest));


	// Check if it's a DNS request port 53
/*	if (ntohs(udp->source) == 53 || ntohs(udp->dest) == 53) {
		//create obj of Dns class
		Dns dns_sniffer(dev);
		dns_sniffer.packet_handler(pkthdr, packet);
	}*/
}

// Destructor
Udp::~Udp() {
	printf("UDP Sniffer destroyed\n");
}

