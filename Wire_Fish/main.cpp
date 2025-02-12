#include "Ip.h"

int main() {
    char dev[] = "wlo1"; // Change to your network interface

    // Start IP Sniffer 
    Ip ip_sniffer(dev);
    ip_sniffer.Capture();

    return 0;
}

