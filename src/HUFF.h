#include <queue>
struct Node {
    char data;
    int freq;
    Node *left, *right;
};

Node* createNode(char data, int freq, Node* left, Node* right) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq; 
    }
};

Node* BuildTree(std::map<char,int> CharFreq){
    std::priority_queue<Node*, std::vector<Node*>, Compare> queue;
    for(auto& pair:CharFreq){
        Node* Leaf=createNode(pair.first, pair.second, nullptr, nullptr);
        queue.push(Leaf);
    }
    while(queue.size()>1){
        Node* left = queue.top();
        queue.pop();
        Node* right = queue.top();
        queue.pop();

        int sum = left->freq + right->freq;
        Node* Dad=createNode('\0', sum, left, right);
        queue.push(Dad);
    }
    return queue.top();
}