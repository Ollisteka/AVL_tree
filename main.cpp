#include <iostream>
#include <memory>
#include <vector>
#include <list>

using namespace std;

template<class T>
class AvlTree {
    typedef shared_ptr<AvlTree> PTree;

    struct Node {
        T key;

        PTree left = nullptr;
        PTree right = nullptr;


        Node(T k, PTree l, PTree r) {
            key = k;
            left = l;
            right = r;
        }

        explicit Node(T k) {
            key = k;
        }
    };

    typedef shared_ptr<Node> PNode;

    PNode RootNode = nullptr;

    explicit AvlTree(PNode node) {
        RootNode = node;
    }

    int GetDepth(PNode node) const {
        if (node == nullptr) {
            return 0;
        }

        int leftDepth = node->left == nullptr ? 0 : GetDepth(node->left->RootNode);
        int rightDepth = node->right == nullptr ? 0 : GetDepth(node->right->RootNode);
        return max(leftDepth, rightDepth) + 1;
    }

    int GetBalance(Node *p1, Node *p2) const {
        return GetDepth(p1) - GetDepth(p2);
    }

    PTree InsertTree(PNode node, PTree tree) const {
        if (tree == nullptr) {
            return make_shared<AvlTree>(AvlTree(node));
        }
        return make_shared<AvlTree>(InsertTree(node, tree->RootNode));
    }

    AvlTree InsertTree(PNode node, PNode root) const {
        if (root == nullptr) {
            return AvlTree(node);
        }

        if (node->key <= root->key) {
            PTree left = InsertTree(node, root->left);
            Node newRoot = Node(root->key, left, root->right);
            PNode n = make_shared<Node>(newRoot);
            return AvlTree(n);
        } else {
            PTree right = InsertTree(node, root->right);
            Node newRoot = Node(root->key, root->left, right);
            PNode n = make_shared<Node>(newRoot);
            return AvlTree(n);
        }

    }

    vector<T> Traverse(PTree tree) const {
        if (tree == nullptr)
            return vector<T>();
        return Traverse(tree->RootNode);
    }

    vector<T> Traverse(PNode node) const {
        if (node == nullptr)
            return vector<T>();
        vector<T> left = Traverse(node->left);
        vector<T> right = Traverse(node->right);
        left.push_back(node->key);
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }


public:
    AvlTree() = default;

    AvlTree Insert(T value) const {
        Node node = Node(value);
        PNode n = make_shared<Node>(node);
        return InsertTree(n, RootNode);
    }


    vector<T> Traverse() {
        return Traverse(RootNode);
    }

    void PrintElements() {
        auto elements = Traverse();
        for (int i = 0; i < elements.size(); ++i) {
            cout << to_string(elements[i]) + " ";
        }
        cout << endl;
    }

    int GetHeight() const {
        return GetDepth(RootNode);
    }
};


int main() {
    AvlTree<int> tree = AvlTree<int>();
    cout << tree.GetHeight() << endl;
    tree = tree.Insert(2);
    tree.PrintElements();
    cout << tree.GetHeight() << endl;
    tree = tree.Insert(1);
    tree.PrintElements();
    tree = tree.Insert(3);
    tree.PrintElements();
    tree = tree.Insert(5).Insert(8).Insert(3).Insert(9);
    cout << tree.GetHeight() << endl;
    tree.PrintElements();
    // tree = tree.Insert(2);
    // cout << tree.Size() << endl;
    return 0;
}