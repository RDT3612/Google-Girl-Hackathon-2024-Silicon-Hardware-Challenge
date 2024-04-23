#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <random>
//threading library: python requests 
// Define constants
const int DEFAULT_BUFFER_SIZE = 100; // Default buffer size
const double DEFAULT_ARBITER_WEIGHT = 0.5; // Default arbiter weight
const int READ_LATENCY = 1; //in cycles 
const int WRITE_LATENCY = 2; //in cycles 

// Define constants for request types
const int READ_REQUEST = 0;
const int WRITE_REQUEST = 1;

// Request structure
struct Request {
    int type; // 0 for read, 1 for write
    int address;
    int data;
};

// System Memory class
class SystemMemory {
public:
    // Method to read data from memory with latency
    void readDataWithLatency(int address, int latency) {
        // Simulate read operation with latency
        std::cout << "Reading data from memory at address " << address << " with latency of " << latency << " cycles." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(latency)); // Simulate read latency
    }

    // Method to write data to memory with latency
    void writeDataWithLatency(int address, int data, int latency) {
        // Simulate write operation with latency
        std::cout << "Writing data " << data << " to memory at address " << address << " with latency of " << latency << " cycles." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(latency)); // Simulate write latency
    }
};

// CPU class
class CPU {
private:
    std::queue<Request> requestBuffer; // Request buffer for CPU

public:
    // Method to send read request to memory
    void sendReadRequest(SystemMemory& memory, int address) {
        Request request;
        request.type = READ_REQUEST;
        request.address = address;
        requestBuffer.push(request);
    }

    // Method to send write request to memory
    void sendWriteRequest(SystemMemory& memory, int address, int data) {
        Request request;
        request.type = WRITE_REQUEST;
        request.address = address;
        request.data = data;
        requestBuffer.push(request);
    }

    // Method to process pending requests
    void processRequests(SystemMemory& memory) {
        while (!requestBuffer.empty()) {
            Request request = requestBuffer.front();
            requestBuffer.pop();
            if (request.type == READ_REQUEST) {
                memory.readDataWithLatency(request.address, READ_LATENCY);
            } else if (request.type == WRITE_REQUEST) {
                memory.writeDataWithLatency(request.address, request.data, WRITE_LATENCY);
            }
        }
    }
};

// IO class
class IO {
private:
    std::queue<Request> requestBuffer; // Request buffer for IO

public:
    // Method to send read request to memory
    void sendReadRequest(SystemMemory& memory, int address) {
        Request request;
        request.type = READ_REQUEST;
        request.address = address;
        requestBuffer.push(request);
    }

    // Method to send write request to memory
    void sendWriteRequest(SystemMemory& memory, int address, int data) {
        Request request;
        request.type = WRITE_REQUEST;
        request.address = address;
        request.data = data;
        requestBuffer.push(request);
    }

    // Method to process pending requests
    void processRequests(SystemMemory& memory) {
        while (!requestBuffer.empty()) {
            Request request = requestBuffer.front();
            requestBuffer.pop();
            if (request.type == READ_REQUEST) {
                memory.readDataWithLatency(request.address, READ_LATENCY);
            } else if (request.type == WRITE_REQUEST) {
                memory.writeDataWithLatency(request.address, request.data, WRITE_LATENCY);
            }
        }
    }
};

class NoC {
private:
    std::unordered_map<int, std::queue<Request>> requestQueues; // Queue for each agent
    std::unordered_map<int, int> bufferSizes; // Map buffer ID to buffer size
    std::unordered_map<std::string, double> arbiterWeights; // Arbiter weights for each agent
    int operatingFrequency; // Operating frequency of NoC
    bool memoryAccessInProgress; // Flag to indicate if memory access is in progress

public:
    NoC(int frequency) : operatingFrequency(frequency), memoryAccessInProgress(false) {}

    void setMaxBufferSize(int buffer_id, int num_entries) {
        bufferSizes[buffer_id] = num_entries;
    }

    void setArbiterWeights(std::string agent_type, double weight) {
        arbiterWeights[agent_type] = weight;
    }

    void throttle() {
        for (const auto& [agent_type, queue] : requestQueues) {
            double queue_utilization = static_cast<double>(queue.size()) / bufferSizes[agent_type];
            if (queue_utilization >= 0.9) {
                operatingFrequency *= 0.9; // Decrease by 10%
                return; // Adjusting frequency once is sufficient
            }
        }
        operatingFrequency = 1000; // Return to normal frequency
    }

    std::string selectNextAgent() {
        std::vector<std::string> agents;
        for (const auto& [agent_type, weight] : arbiterWeights) {
            for (int i = 0; i < weight * 10; ++i) { // Multiply by 10 for finer granularity
                agents.push_back(agent_type);
            }
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, agents.size() - 1);
        return agents[dis(gen)];
    }

    void sendRequest(SystemMemory& memory, std::string agentType, int address, int data = 0) {
        if (memoryAccessInProgress) {
            std::cout << "Memory access in progress. Waiting for previous operation to complete..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10)); 
        }

        int latency = (agentType == "CPU") ? READ_LATENCY : WRITE_LATENCY;

        // Simulate memory access with latency
        if (data == 0) {
            memory.readDataWithLatency(address, latency); 
        } else {
            memory.writeDataWithLatency(address, data, latency); 
        }

        memoryAccessInProgress = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(latency)); 
        memoryAccessInProgress = false;
    }
};

int main() {
    // Create instances of SystemMemory, CPU, IO, and NoC
    SystemMemory memory;
    CPU cpu;
    IO io;
    NoC noc(1000); // Operating frequency of 1000 MHz

    // Example usage of NoC APIs
    noc.setMaxBufferSize(1, DEFAULT_BUFFER_SIZE); // Set buffer 1 size
    noc.setArbiterWeights("CPU", DEFAULT_ARBITER_WEIGHT); // Set arbiter weight for CPU

    // Simulate memory access with read latency
    noc.sendRequest(memory, "CPU", 0x1234); // CPU sends a read request to memory

    // Simulate memory access with write latency
    noc.sendRequest(memory, "IO", 0x5678, 42); // IO sends a write request to memory

    return 0;
}

// add threads throttle 