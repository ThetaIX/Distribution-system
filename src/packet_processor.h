#ifndef PACKET_PROCESSOR_H
#define PACKET_PROCESSOR_H

#include <pcap.h>
#include <pthread.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <vector>

struct Packet {
    const u_char* data;
    struct pcap_pkthdr header;
};

// Общая структура для обработчиков
class PacketProcessor {
protected:
    std::queue<Packet> packetQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop = false;
    int processorID;
    std::string outputFilename;
    
public:
    PacketProcessor(int id, const std::string& filename);
    void enqueue(Packet packet);
    virtual void process() = 0;
    void start();
    void stopProcessing();
};

class Processor1 : public PacketProcessor {
public:
    Processor1() : PacketProcessor(1, "result_1.pcap") {}
    void process() override;
};

class Processor2 : public PacketProcessor {
public:
    Processor2() : PacketProcessor(2, "result_2.pcap") {}
    void process() override;
};

class Processor3 : public PacketProcessor {
public:
    Processor3() : PacketProcessor(3, "result_3.pcap") {}
    void process() override;
};

#endif
