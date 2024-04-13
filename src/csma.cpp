#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstdint>

using namespace std;

/**
 * @brief Represents a node in the CSMA simulator.
 * 
 * This struct stores information about a node in the CSMA (Carrier Sense Multiple Access) simulator.
 * It includes the node's ID, backoff value, time count, backoff count, and collision count.
 */
struct Node{
    uint32_t id;
    uint32_t backoff;
    uint32_t time_count;
    uint32_t backoff_count;
    uint32_t collision_count;
};

uint32_t success = 0;

/**
 * Simulates the CSMA/CD (Carrier Sense Multiple Access with Collision Detection) algorithm.
 * 
 * @param nodes A reference to a vector of Node objects representing the network nodes.
 * @param T The current time slot.
 * @param R_values A vector of integers representing the backoff values for each node.
 * @param col_count The maximum number of collisions allowed before resetting the backoff values.
 * @return Returns the ID of the selected node if only one node is available for transmission,
 *         -2 if there are multiple nodes available but a collision occurs,
 *         -1 if no node is available for transmission.
 */
int simulate(vector<Node> &nodes, int T, vector<int> R_values, uint32_t col_count){
    vector<int> selected_nodes;
    for(int i = 0; i < nodes.size(); i++){
        if(nodes[i].time_count == 0){
            selected_nodes.push_back(nodes[i].id);
        }
    }          
    if(selected_nodes.size() > 1){
        for(int count = 0; count < selected_nodes.size(); count++){
            if(nodes[selected_nodes[count]].collision_count > col_count - 1){
                nodes[selected_nodes[count]].backoff_count = 0;
                nodes[selected_nodes[count]].time_count = (T + nodes[selected_nodes[count]].id + 1) % R_values[0];
                nodes[selected_nodes[count]].backoff = R_values[0];
                nodes[selected_nodes[count]].collision_count = 0;
            }
            else{
                nodes[selected_nodes[count]].backoff = R_values[nodes[selected_nodes[count]].backoff_count + 1];
                nodes[selected_nodes[count]].backoff_count++;
                nodes[selected_nodes[count]].time_count = (T + nodes[selected_nodes[count]].id + 1) % nodes[selected_nodes[count]].backoff;
                nodes[selected_nodes[count]].collision_count++;
            }
        }
        return -2;
    }else if(selected_nodes.size() == 1){
        return selected_nodes[0];
    }else{
        for(int i = 0; i < nodes.size(); i++){
            if(nodes[i].time_count > 0){
                nodes[i].time_count--;
            }
        }
    }
    return -1;
}

/**
 * @brief The main function of the program.
 * 
 * This function reads input from a file specified as a command-line argument and performs a CSMA simulation based on the input parameters.
 * It calculates the success rate of the simulation and writes it to an output file named "output.txt".
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 if the program executed successfully, 1 if there was an error opening the input file.
 */
int main(int argc, char *argv[]){
    if(argc != 2){
        cout << "Usage: ./csma <input_file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if(!file.is_open()){
        cout << "Error: Could not open file" << endl;
        return 1;
    }

    int N, L, M, T;
    vector<int> R_values;
    string inputType;
    while(file >> inputType){
        if(inputType == "N") file >> N;
        else if(inputType == "L") file >> L;
        else if(inputType == "M") file >> M;
        else if(inputType == "T") file >> T;
        else if(inputType == "R"){
            int R;
            while(file >> R){
                R_values.push_back(R);
                if(file.peek() == '\n') break;
            }
        }
    }

    vector<Node> nodes;
    for(int i = 0; i < N; i++){
        Node node;
        node.id = i;
        node.backoff = R_values[0];
        node.backoff_count = 0;
        node.time_count = node.id % node.backoff;
        nodes.push_back(node);
    }

    for(int count = 0; count < T; count++){
        int suc = simulate(nodes, count, R_values, M);
        if(suc != -1 && suc != -2){
            if(count + L < T){
                success += L;
            }else {
                success += T - count;
            }
            // Ensuring the transmission time is considered
            count += L; // -1 because for loop will increment count next
            count--; // -1 because for loop will increment count next
            nodes[suc].time_count = (count + nodes[suc].id + 1) % nodes[suc].backoff;
        }
        if(suc == -2 && count == T - 2) break;
    }
    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%.2lf", (double)success/(double)T);
    fclose(output);
    return 0;
}
