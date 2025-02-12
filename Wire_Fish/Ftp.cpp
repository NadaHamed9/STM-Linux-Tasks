#include "Ftp.h"
#include <ctype.h>

// Constructor
Ftp::Ftp(char *dev) : Tcp(dev) {
        printf("FtP Sniffer initialized\n");
}

// Extract FTP request headers and data
void Ftp::packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
        struct iphdr *ip = (struct iphdr *)(packet + 14);           // Skip Ethernet header
        struct tcphdr *tcp = (struct tcphdr *)(packet + 14 + ip->ihl * 4);   // TCP Header
        const u_char *payload = packet + 14 + ip->ihl * 4 + tcp->doff * 4;  // Ftp Payload

        //get payload size
        int payload_size = pkthdr->len - (14 + ip->ihl * 4 + tcp->doff * 4);

        // Print FTP payload
        printf("   |- FTP Data: ");
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
Ftp::~Ftp() {
        printf("FTP Sniffer destroyed\n");
}
