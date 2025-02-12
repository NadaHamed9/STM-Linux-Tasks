#include "Ip.h"
#include "Tcp.h"
#include "Udp.h" 

Ip::Ip(char* dev): PacketSniffer(dev)
{
	printf("Ip constructor is created\n");
}

void Ip::Capture()
{
	// Capture packets indefinitely
	printf("starting packet capture... \n");
	pcap_loop(handle, 0, Ip::packet_handler_converter, (u_char*)this);

}

void Ip::packet_handler_converter(u_char *user,const struct pcap_pkthdr *pkthdr,const u_char *packet){

	//cast user to Ip object
	Ip *obj = (Ip*)user;
	obj->packet_handler(pkthdr,packet);
}

void Ip::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	struct iphdr *ip = (struct iphdr*)(packet + 14); // Skip Ethernet header (14 bytes)

	printf("Packet captured:\n");
	printf("IP Header:\n");
	printf("   |-IP Version        : %d\n", (unsigned int)ip->version);
	printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)ip->ihl, ((unsigned int)(ip->ihl)) * 4);
	printf("   |-Type Of Service   : %d\n", (unsigned int)ip->tos);
	printf("   |-IP Total Length   : %d Bytes(Size of Packet)\n", ntohs(ip->tot_len));
	printf("   |-Identification    : %d\n", ntohs(ip->id));
	printf("   |-TTL      : %d\n", (unsigned int)ip->ttl);
	printf("   |-Protocol : %d\n", (unsigned int)ip->protocol);
	printf("   |-Checksum : %d\n", ntohs(ip->check));
	printf("   |-Source IP        : %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr));
	printf("   |-Destination IP   : %s\n", inet_ntoa(*(struct in_addr *)&ip->daddr));

	// Call appropriate handler for TCP or UDP which they are inherited classes from Ip class
	if (ip->protocol == IPPROTO_TCP) 
	{
		Tcp tcp_sniffer(dev);
		tcp_sniffer.packet_handler(pkthdr, packet);
	} 
	else if (ip->protocol == IPPROTO_UDP) 
	{
		Udp udp_sniffer(dev);
		udp_sniffer.packet_handler(pkthdr, packet);
	}

}
//Destructor
Ip::~Ip()
{
	printf("Ip sniffer destructed\n");

}

