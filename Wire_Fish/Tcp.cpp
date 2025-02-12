#include "Tcp.h"
//#include "Http.h"

//constuctor implementation
Tcp::Tcp(char *dev) : Ip(dev) {
	printf("TCP sniffer initialized\n");
}

void Tcp::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	struct iphdr *ip = (struct iphdr *)(packet + 14);
	struct tcphdr *tcp = (struct tcphdr*)(packet + 14 + ip->ihl * 4);

	printf("TCP Header:\n");
	printf("   |-Source Port      : %u\n", ntohs(tcp->source));
	printf("   |-Destination Port : %u\n", ntohs(tcp->dest));

	// Check for HTTP Traffic (Port 80 or 443)
/*	if (ntohs(tcp->source) == 80 || ntohs(tcp->dest) == 80) {
		//create object of Http class
		Http http_sniffer(dev);
		http_sniffer.packet_handler(pkthdr,packet);
	}*/
}
Tcp::~Tcp() {
	printf("TCP Sniffer destroyed\n");
}

