#include <iostream>
using namespace std;

class Node {
public:
    int value;
    Node* left;  
    Node* right;
    Node* parent;

    Node(int num) : value(num), left(nullptr), right(nullptr), parent(nullptr) {}
};

void rewireNode(Node* new_parent_Node, Node* new_child_Node, int direction){
    if (new_parent_Node == nullptr){
            new_child_Node->parent = nullptr;
            return;
    }
    if (direction == 0){
        if (new_child_Node == nullptr){
            new_parent_Node->left = nullptr;
            return;
        }
        new_parent_Node->left = new_child_Node;
        new_child_Node->parent = new_parent_Node;
    }
    else{
        if (new_child_Node == nullptr){
            new_parent_Node->right = nullptr;
            return;
        }
        new_parent_Node->right = new_child_Node;
        new_child_Node->parent = new_parent_Node;
    }
}

class BinaryTree {
private:
    Node* findRecursive(Node* curr_Node, int num) {
        if (curr_Node == nullptr) {
            return nullptr;
        }

        if (curr_Node->value == num){
            return curr_Node;
        }

        if (curr_Node->left != nullptr){
            if (curr_Node->left->value == num){
                return curr_Node->left;
            }
        }

        if (curr_Node->right != nullptr){
            if (curr_Node->right->value == num){
                return curr_Node->right;
            }
        }

        Node* leftResult = findRecursive(curr_Node->left, num);
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        return findRecursive(curr_Node->right, num);
    }

    Node* findMinSuccessor(Node* curr_Node){
        if (curr_Node->right != nullptr){
            curr_Node = curr_Node->right;
        }
        else if (curr_Node->left != nullptr){
            return curr_Node->left;
        }
        else{
            return nullptr;
        }

        while (1){
            if (curr_Node->left != nullptr){
                curr_Node = curr_Node->left;
            }
            else{
                return curr_Node;
            }
        }
    }

    bool isMirror(Node* leftNode, Node* rightNode) {
        if (leftNode == nullptr && rightNode == nullptr){ 
            return true;
        }

        if (leftNode == nullptr || rightNode == nullptr || leftNode->value != rightNode->value) {
            return false;
        }

        return isMirror(leftNode->left, rightNode->right) && isMirror(leftNode->right, rightNode->left);
    }

public:
    Node* root;

    BinaryTree(Node* starting_node) : root(starting_node) {}

    void addNode(int num){
        Node* newNode = new Node(num);

        if (root == nullptr){
            root = newNode;
            return;
        }

        Node* curr_Node = root;
        while (1){
            if (num >= curr_Node->value){
                if (curr_Node->right == nullptr){
                    rewireNode(curr_Node, newNode, 1);
                    return;
                }
                else{
                    curr_Node = curr_Node->right;
                }
            }
            else{
                if (curr_Node->left == nullptr){
                    rewireNode(curr_Node, newNode, 0);
                    return;
                }
                else{
                    curr_Node = curr_Node->left;
                }
            }
        }
    }

    void delNode(int num){
        Node* deletion_Node = findRecursive(root, num);

        if (deletion_Node == nullptr){
            cout << "node not found" << endl;
            return;
        }
        // cout << deletion_Node->value << endl;

        Node* deletion_Node_parent = deletion_Node->parent;
        // cout << deletion_Node_parent->value << endl;

        int deletion_Node_direction = 0;
        if (deletion_Node_parent != nullptr){
            if (deletion_Node_parent->right == deletion_Node){
                deletion_Node_direction = 1;
            }
        }
        // cout << deletion_Node_direction << endl;

        Node* successor_Node = findMinSuccessor(deletion_Node);
        // cout << successor_Node->value << endl;

        if (successor_Node == nullptr){
            if (deletion_Node->parent == nullptr){
                root = nullptr;
                delete deletion_Node;
                return;
            }
            else{
                rewireNode(deletion_Node_parent, nullptr, deletion_Node_direction);
                delete deletion_Node;
                return;
            }
        }

        Node* successor_Node_parent = successor_Node->parent;
        // cout << successor_Node_parent->value << endl;

        deletion_Node->value = successor_Node->value;
        if (deletion_Node->right == successor_Node){
            rewireNode(deletion_Node, successor_Node->right, 1);
        }
        else{
            rewireNode(successor_Node_parent, successor_Node->right, 0);
        }
        delete successor_Node;
    }

    void printTree(Node* node, string prefix = "") {
        if (node != nullptr){

            cout << prefix;
            cout << node->value << endl;    

            printTree(node->left, prefix + "    ");
            printTree(node->right, prefix + "    ");
        }
    }

    bool isSymmetric(Node* root) {
        if (root == nullptr) 
            return true;
        
        return isMirror(root->left, root->right);
    }

    void clearTree(Node* node){
        if (node == nullptr){
            return;
        } 

        clearTree(node->left);
        clearTree(node->right);
        
        if (node->parent == nullptr){
            root = nullptr;
        }
        else if (node->parent->left == node){
            rewireNode(node->parent, nullptr, 0);
        }
        else{
            rewireNode(node->parent, nullptr, 1);
        }
        delete node;
    }
};

int main(){
    BinaryTree tree(nullptr);

    string command;

    cout << "========The tree of life protocol========" << endl;
    cout << "add <num>" << endl;
    cout << "remove <num>" << endl;
    cout << "print" << endl;
    cout << "free" << endl;
    cout << "symmetric" << endl; 
    cout << "build_symmetric" << endl;
    cout << "exit" << endl;
    cout << "=========================================" << endl;

    while (true){
        cin >> command;

        if (command == "add"){
            int value;
            cin >> value;
            if (cin.fail()) {
                cout << "Please enter number" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            tree.addNode(value);
        }
        else if (command == "remove"){
            int value;
            cin >> value;
            if (cin.fail()) {
                cout << "Please enter number" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            tree.delNode(value);
        }
        else if (command == "print"){
            if (tree.root == nullptr){
                cout << "Tree is empty" << endl;
            } 
            else{
                tree.printTree(tree.root);
            }
        }
        else if (command == "free"){
            tree.clearTree(tree.root);
        }
        else if (command == "symmetric"){
            if (tree.isSymmetric(tree.root)){
                cout << "Tree is symmetric" << endl;
            } 
            else{
                cout << "Tree is NOT symmetric" << endl;
            }
        }
        else if (command == "build_symmetric"){
            tree.clearTree(tree.root);

            tree.root = new Node(1);

            Node* left = new Node(3);
            Node* right = new Node(3);
            Node* left_child = new Node(4);
            Node* right_child = new Node(4);

            rewireNode(tree.root, left, 0);
            rewireNode(tree.root, right, 1);

            rewireNode(left, left_child, 0);
            rewireNode(right, right_child, 1);
        }
        else if (command == "exit"){
            tree.clearTree(tree.root);
            break;
        }
        else{
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}