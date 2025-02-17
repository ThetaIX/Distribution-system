#include "packet_processor.h"
#include <cstring>
#include <pcap.h>

void distributePackets(const char* filename, Processor1& p1, Processor2& p2, Processor3& p3) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_offline(filename, errbuf);
    if (!handle) {
        std::cerr << "Ошибка открытия файла: " << errbuf << std::endl;
        return;
    }

    struct pcap_pkthdr header;
    const u_char* packet;
    while ((packet = pcap_next(handle, &header))) {
        struct in_addr dstIP;
        memcpy(&dstIP, packet + 30, 4);
        uint16_t dstPort = ntohs(*(uint16_t*)(packet + 36));

        if (ntohl(dstIP.s_addr) >= ntohl(inet_addr("11.0.0.3")) &&
            ntohl(dstIP.s_addr) <= ntohl(inet_addr("11.0.0.200"))) {
            p1.enqueue({packet, header});
        } else if (ntohl(dstIP.s_addr) >= ntohl(inet_addr("12.0.0.3")) &&
                   ntohl(dstIP.s_addr) <= ntohl(inet_addr("12.0.0.200")) &&
                   dstPort == 8080) {
            p2.enqueue({packet, header});
        } else {
            p3.enqueue({packet, header});
        }
    }

    pcap_close(handle);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <pcap-файл>\n";
        return 1;
    }

    Processor1 p1;
    Processor2 p2;
    Processor3 p3;

    p1.start();
    p2.start();
    p3.start();

    distributePackets(argv[1], p1, p2, p3);

    p1.stopProcessing();
    p2.stopProcessing();
    p3.stopProcessing();

    return 0;
}
