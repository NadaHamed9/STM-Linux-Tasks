#include "Dns.h"
#include <ctype.h>

// Constructor
Dns::Dns(char *dev) : Udp(dev) {
	printf("DNS Sniffer initialized\n");
}

// Extract DNS request headers and data
void Dns::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	struct iphdr *ip = (struct iphdr *)(packet + 14);           // Skip Ethernet header
	struct udphdr *udp = (struct udphdr*)(packet + 14 + ip->ihl * 4); // UDP  Header
	const u_char *payload = packet + 14 + ip->ihl * 4 + sizeof(struct udphdr); // DNS Payload 

	// Print DNS Query
	printf("   |- DNS Query: ");
	int i=12;
	while(payload[i] !=0) {

		//check if char printable or not 
		if (isprint(payload[i])) {
			printf("%c", payload[i]);
		} else {
			printf(".");
		}

		i++;
	}
	printf("\n");

}

// Destructor
Dns::~Dns() {
	printf("DNS Sniffer destroyed\n");
}
