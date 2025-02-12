#include "PacketSniffer.h"

PacketSniffer::PacketSniffer(char *dev) {
	printf("Ip constructor is created\n");
	this->dev = dev ;
	// Open the device for packet capture
	handle = pcap_open_live(this->dev, BUFSIZ, 1, 1000, errbuf);

	if (handle == NULL)
	{
		fprintf(stderr, "Couldn't open device %s: %s\n", this->dev, errbuf);
	}

}

PacketSniffer::~PacketSniffer() {
	if (handle) {
		pcap_close(handle);
	}
	printf("PacketSniffer object destroyed\n");
	printf("---------------------------------------------------\n");
}

