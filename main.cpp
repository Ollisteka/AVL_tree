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
        PTree left = make_shared<AvlTree>(AvlTree());
        PTree right = make_shared<AvlTree>(AvlTree());

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

    int GetHeight(PNode node) const {
        if (node == nullptr) {
            return 0;
        }

        int leftDepth = GetHeight(node->left->RootNode);
        int rightDepth = GetHeight(node->right->RootNode);
        return max(leftDepth, rightDepth) + 1;
    }

    int GetBalance(PTree t1, PTree t2) const {
        int left = GetHeight(t1->RootNode);
        int right = GetHeight(t2->RootNode);
        return left - right;
    }

    int GetBalance(PNode node) const {
        if (node == nullptr)
            return 0;
        return GetBalance(node->left, node->right);
    }


    PNode RotateRight(PNode z) const {
        PNode y = z->left->RootNode;
        PNode x = y->right->RootNode;
        y->right = MakeShared(AvlTree(z));
        z->left = MakeShared(AvlTree(x));
        return y;
    }

    PNode RotateLeft(PNode z) const {
        PNode y = z->right->RootNode;
        PNode x = y->left->RootNode;
        y->left = MakeShared(AvlTree(z));
        z->right = MakeShared(AvlTree(x));
        return y;
    }

    PNode MakeShared(Node node) const {
        return make_unique<Node>(node);
    }

    PTree MakeShared(AvlTree tree) const {
        return make_unique<AvlTree>(tree);
    }

    int CompareKey(PNode first, PNode second) const {
        if (second == nullptr || first == nullptr)
            return 0;
        if (first->key < second->key)
            return -1;
        return 1;
    }

    AvlTree InsertTree(PNode node, PNode root) const {
        if (root == nullptr)
            return AvlTree(node);

        if (node->key == root->key)
            return AvlTree(root);

        PNode newRoot = MakeShared(Node(root->key, root->left, root->right));
        if (node->key < root->key) {
            PTree left = MakeShared(InsertTree(node, root->left->RootNode));
            newRoot = MakeShared(Node(root->key, left, root->right));
        } else {
            PTree right = MakeShared(InsertTree(node, root->right->RootNode));
            newRoot = MakeShared(Node(root->key, root->left, right));
        }

        int balance = GetBalance(newRoot);

        if (balance > 1 && CompareKey(node, newRoot->left->RootNode) < 0)
            return AvlTree(RotateRight(newRoot));
        if (balance < -1 && CompareKey(node, newRoot->right->RootNode) > 0)
            return AvlTree(RotateLeft(newRoot));
        if (balance > 1 && CompareKey(node, newRoot->left->RootNode) > 0) {
            newRoot->left->RootNode = RotateLeft(newRoot->left->RootNode);
            return AvlTree(RotateRight(newRoot));
        }
        if (balance < -1 && CompareKey(node, newRoot->right->RootNode) < 0) {
            newRoot->right->RootNode = RotateRight(newRoot->right->RootNode);
            return AvlTree(RotateLeft(newRoot));
        }

        return AvlTree(newRoot);
    }

    vector<T> TraverseInOrder(PNode node) const {
        if (node == nullptr)
            return vector<T>();
        vector<T> left = TraverseInOrder(node->left->RootNode);
        vector<T> right = TraverseInOrder(node->right->RootNode);
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


    vector<T> TraverseInOrder() {
        return TraverseInOrder(RootNode);
    }

    void PrintElements() {
        auto elements = TraverseInOrder();
        for (int i = 0; i < elements.size(); ++i) {
            cout << to_string(elements[i]) + " ";
        }
        cout << endl;
    }

    int GetHeight() const {
        return GetHeight(RootNode);
    }
};


int main() {
    AvlTree<int> tree = AvlTree<int>();
    //  cout << tree.GetHeight() << endl;
    tree = tree.Insert(2);
    tree.PrintElements();
    //  cout << tree.GetHeight() << endl;
    tree = tree.Insert(1);
    tree.PrintElements();
    tree = tree.Insert(3);
    tree.PrintElements();
    tree = tree.Insert(5);
    tree.PrintElements();
    tree = tree.Insert(8);
    tree.PrintElements();
    tree = tree.Insert(3).Insert(9).Insert(-1);
    cout << tree.GetHeight() << endl;
    tree.PrintElements();
    // tree = tree.Insert(2);
    // cout << tree.Size() << endl;
    return 0;
}