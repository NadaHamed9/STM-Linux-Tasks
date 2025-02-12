#include "Http.h"
#include <ctype.h>

// Constructor
Http::Http(char *dev) : Tcp(dev) {
	printf("HTTP Sniffer initialized\n");
}

// Extract HTTP request headers and data
void Http::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	struct iphdr *ip = (struct iphdr *)(packet + 14);           // Skip Ethernet header
	struct tcphdr *tcp = (struct tcphdr *)(packet + 14 + ip->ihl * 4);   // TCP Header
	const u_char *payload = packet + 14 + ip->ihl * 4 + tcp->doff * 4;  // HTTP Payload

	//get payload size
	int payload_size = pkthdr->len - (14 + ip->ihl * 4 + tcp->doff * 4);

	// Print HTTP payload
	printf("   |- HTTP Data: ");
	for (int i = 0; i < payload_size; i++) {

		//check if char printable or not 
		if (isprint(payload[i])) {
			printf("%c", payload[i]);
		} else {
			printf(".");
		}
	}
	printf("\n");

}

// Destructor
Http::~Http() {
	printf("HTTP Sniffer destroyed\n");
}

