#ifndef PACKET_SNIFFER_H
#define PACKET_SNIFFER_H

#include <pcap.h>
#include <string>
#include <stdio.h>
/****************** ABSTRACT CLASS ***********************/
class PacketSniffer {
protected:
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

public:
    PacketSniffer(char *dev);
    /* pure virtual function will be implemented in inherited class Ip*/
    virtual void Capture() = 0;
    virtual ~PacketSniffer();
};

#endif

