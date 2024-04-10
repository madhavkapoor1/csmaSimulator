#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip> 

struct Node {
    int id;
    int backoff;
    int collisionCount;
    bool isTransmitting;
    int packetLength;

    Node(int nodeId) : id(nodeId), backoff(0), collisionCount(0), isTransmitting(false), packetLength(0) {}
};

std::vector<Node> nodes;
int R = 4; // Initial range for random backoff
const int maxCollisionCount = 6; // Maximum retransmission attempts
int totalSimulationTime = 0;
int successfulTransmissions = 0;

void initializeSimulation(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        char parameter;
        int value;
        iss >> parameter;
        if (parameter == 'N') {
            iss >> value;
            for (int i = 0; i < value; ++i) {
                nodes.emplace_back(i);
            }
        } else if (parameter == 'L') {
            iss >> value;
            for (auto& node : nodes) {
                node.packetLength = value;
            }
        } else if (parameter == 'R') {
            iss >> R;
        } else if (parameter == 'T') {
            iss >> totalSimulationTime;
        }
    }
}

void outputResults() {
    std::ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        double linkUtilizationRate = static_cast<double>(successfulTransmissions) / totalSimulationTime;
        outputFile << std::fixed << std::setprecision(2) << linkUtilizationRate << std::endl;
        outputFile.close();
    } else {
        std::cerr << "Unable to open output file." << std::endl;
    }
}


void simulate() {
    int currentTime = 0;

    while (currentTime < totalSimulationTime) {
        bool channelOccupied = false;
        int transmittingNodeCount = 0;
        std::vector<Node*> readyToTransmitNodes;

        // First, check if any node is ready to transmit at this tick
        for (auto& node : nodes) {
            if (!node.isTransmitting && node.backoff == 0) {
                readyToTransmitNodes.push_back(&node);
            }
        }
        
        transmittingNodeCount = readyToTransmitNodes.size();

        // Collision detection and handling
        if (transmittingNodeCount > 1) {
            // Collision detected
            for (auto* node : readyToTransmitNodes) {
                node->collisionCount++;
                if (node->collisionCount > maxCollisionCount) {
                    node->collisionCount = 0; // Reset collision count
                    node->backoff = (node->id + currentTime) % R; // Set backoff after dropping the packet
                } else {
                    R = std::min(R*2, 1024); // Double R, but do not exceed the maximum value
                    node->backoff = (node->id + currentTime) % R; // Recalculate backoff
                }
            }
        } else if (transmittingNodeCount == 1) {
            // Successful transmission
            auto* transmittingNode = readyToTransmitNodes.front();
            transmittingNode->isTransmitting = true;
            transmittingNode->packetLength = 2; // Assuming a fixed packet length for simplification
            successfulTransmissions++;
            channelOccupied = true;
        }

        // Decrement backoff for nodes not ready to transmit
        for (auto& node : nodes) {
            if (!node.isTransmitting && node.backoff > 0) {
                node.backoff--;
            }
        }

        // Handle ongoing transmissions
        for (auto& node : nodes) {
            if (node.isTransmitting) {
                node.packetLength--;
                if (node.packetLength == 0) {
                    // Transmission complete
                    node.isTransmitting = false;
                    node.collisionCount = 0; // Reset collision count after successful transmission
                    R = 4; // Reset R to the initial value
                    node.backoff = (node.id + currentTime) % R; // Calculate new backoff for the next transmission
                }
            }
        }

        currentTime++;
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <inputfilename>" << std::endl;
        return 1;
    }

    std::string inputFileName(argv[1]);
    initializeSimulation(inputFileName);
    simulate();
    outputResults();

    return 0;
}
