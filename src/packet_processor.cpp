#include "packet_processor.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <thread>

PacketProcessor::PacketProcessor(int id, const std::string& filename) 
    : processorID(id), outputFilename(filename) {}

void PacketProcessor::enqueue(Packet packet) {
    std::unique_lock<std::mutex> lock(queueMutex);
    packetQueue.push(packet);
    cv.notify_one();
}

void PacketProcessor::start() {
    std::thread([this]() { this->process(); }).detach();
}

void PacketProcessor::stopProcessing() {
    stop = true;
    cv.notify_all();
}

void Processor1::process() {
    std::ofstream output(outputFilename, std::ios::binary);
    while (!stop) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this]() { return !packetQueue.empty() || stop; });

        while (!packetQueue.empty()) {
            Packet packet = packetQueue.front();
            packetQueue.pop();
            const u_char* data = packet.data;
            uint16_t dstPort = ntohs(*(uint16_t*)(data + 36));

            if (dstPort == 7070) {
                std::cout << "Обработчик 1: пакет игнорируется\n";
            } else {
                output.write(reinterpret_cast<const char*>(data), packet.header.caplen);
            }
        }
    }
}

void Processor2::process() {
    std::ofstream output(outputFilename, std::ios::binary);
    while (!stop) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this]() { return !packetQueue.empty() || stop; });

        while (!packetQueue.empty()) {
            Packet packet = packetQueue.front();
            packetQueue.pop();
            std::vector<u_char> modifiedData(packet.data, packet.data + packet.header.caplen);

            for (size_t i = 0; i < modifiedData.size(); i++) {
                if (modifiedData[i] == 'x') {
                    modifiedData.resize(i + 1);
                    break;
                }
            }

            output.write(reinterpret_cast<const char*>(modifiedData.data()), modifiedData.size());
        }
    }
}

void Processor3::process() {
    std::ofstream output(outputFilename, std::ios::binary);
    while (!stop) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this]() { return !packetQueue.empty() || stop; });

        while (!packetQueue.empty()) {
            Packet packet = packetQueue.front();
            packetQueue.pop();
            const u_char* data = packet.data;
            uint16_t srcPort = ntohs(*(uint16_t*)(data + 34));
            uint16_t dstPort = ntohs(*(uint16_t*)(data + 36));
            uint8_t protocol = data[23];

            if (protocol == IPPROTO_TCP) {
                sleep(2);
                if (time(nullptr) % 2 == 0) {
                    output.write(reinterpret_cast<const char*>(data), packet.header.caplen);
                }
            } else if (protocol == IPPROTO_UDP && srcPort == dstPort) {
                std::cout << "Обработчик 2: Найдено совпадение port = " << srcPort << "\n";
                output.write(reinterpret_cast<const char*>(data), packet.header.caplen);
            }
        }
    }
}
