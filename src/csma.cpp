#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstdint>

using namespace std;

struct Node{
    uint32_t id;
    uint32_t backoff;
    uint32_t time_count;
    uint32_t backoff_count;
};

uint32_t success = 0;

int simulate(vector<Node> &nodes, int T, vector<int> R_values){
    vector<int> selected_nodes;
    for(int i = 0; i < nodes.size(); i++){
        if(nodes[i].time_count == 0){
            selected_nodes.push_back(nodes[i].id);
        }
    }          
    if(selected_nodes.size() > 1){
        for(int count = 0; count < selected_nodes.size(); count++){
            nodes[selected_nodes[count]].backoff = R_values[nodes[selected_nodes[count]].backoff_count + 1];
            nodes[selected_nodes[count]].backoff_count++;
            nodes[selected_nodes[count]].time_count = (T + nodes[selected_nodes[count]].id + 1) % nodes[selected_nodes[count]].backoff;
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
        int suc = simulate(nodes, count, R_values);
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
    fprintf(output, "%.2lf\n", (double)success/(double)T);
    fclose(output);
    return 0;
}
