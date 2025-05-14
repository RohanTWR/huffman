#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>
using namespace std;

int global_id = 0;


struct Node {
    int id;
    char ch;
    int freq;
    bool isLeaf;
    vector<Node*> children; 
    
    Node(char _ch, int _freq, bool _isLeaf){
        
        ch = _ch;
        freq = _freq;
        isLeaf = _isLeaf;
        id = global_id++;
    }
};

class Graph {
public:
    unordered_map<int, list<pair<int,int>>> adjList;
    
    void addEdge(int u, int v, int weight) {
        adjList[u].push_back({v, weight});
    }
};

struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
public:
    int d; 
    unordered_map<char, int> freqMap;      
    unordered_map<char, string> hCodes;      
    Graph treeGraph;                         
    Node* root = nullptr;                    

    void buildFrequency(const string &s) {
        for (char c : s) {
            freqMap[c]++;
        }
    }

    void buildTree() {
        int n = freqMap.size();
        int splits = (n-1+d-1) / (d-1);
        int dummyCount = (splits*(d-1)+1) - n;
        
        cout<<"Number of dummy nodes:"<<dummyCount<<"\n";
        priority_queue<Node*, vector<Node*>, NodeCompare> pq;
        
        for (auto &p : freqMap) {
            Node* node = new Node(p.first, p.second, true);
            pq.push(node);
        }
        
        for (int i = 0; i < dummyCount; i++) {
            Node* dummy = new Node('$', 0, true);
            pq.push(dummy);
        }
        
        while (pq.size() > 1) {
            vector<Node*> children;
            int sum = 0;
            for (int i = 0; i < d; i++) {
                
                Node* temp = pq.top();
                pq.pop();
                children.push_back(temp);
                sum += temp->freq;
            }
            Node* newNode = new Node('\0', sum, false);
            newNode->children = children;
            
            for (int i = 0; i < children.size(); i++) {
                treeGraph.addEdge(newNode->id, children[i]->id, i);
            }
            
            pq.push(newNode);
        }
        root = pq.top();
    }

    void generateCodes(Node* node, string code) {
        if (!node) return;
        if (node->isLeaf && node->freq > 0) {
            hCodes[node->ch] = code;
        }
        for (int i = 0; i < node->children.size(); i++) {
            generateCodes(node->children[i], code + char('0' + i));
        }
    }

    string encode(const string &s) {
        string encoded = "";
        for (char c : s) {
            encoded += hCodes[c];
        }
        return encoded;
    }

    string decode(const string &encoded) {
        string decoded = "";
        Node* current = root;
        for (char digit : encoded) {
            int index = digit - '0';
            if(index < current->children.size())
                current = current->children[index];
            if(current->isLeaf) {
                if(current->freq > 0) 
                    decoded.push_back(current->ch);
                current = root;
            }
        }
        return decoded;
    }
};

int main() {
    string input;
    cout << "Enter the string: " << endl;
    getline(cin, input);
    
    int d;
    cout << "Enter the value of d: " << endl;
    cin >> d;

    
    Huffman h;
    h.d = d;
    h.buildFrequency(input);
    h.buildTree();
    h.generateCodes(h.root, "");
    
    cout << "\nHuffman Codes for each symbol:" << endl;
    for(auto &p : h.hCodes) {
        cout << p.first << " : " << p.second << endl;
    }
    
    string encoded = h.encode(input);
    cout << "\nEncoded string:\n" << encoded << endl;
    
    string decoded = h.decode(encoded);
    cout << "\nDecoded string:\n" << decoded << endl;
    
    return 0;
}
