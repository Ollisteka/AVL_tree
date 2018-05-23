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

    explicit AvlTree(T key) {
        RootNode = MakeShared(Node(key));
    }

    int GetHeight(PNode node) const {
        if (node == nullptr)
            return 0;

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
        return make_shared<Node>(node);
    }

    PTree MakeShared(AvlTree tree) const {
        return make_shared<AvlTree>(tree);
    }

    int CompareKey(T key, PNode node) const {
        if (node == nullptr)
            return 0;
        if (key < node->key)
            return -1;
        return 1;
    }

    AvlTree InsertTree(T key, PNode root) const {
        if (root == nullptr)
            return AvlTree(key);

        if (key == root->key)
            return AvlTree(root);

        if (key < root->key)
            root->left = MakeShared(InsertTree(key, root->left->RootNode));
        else
            root->right = MakeShared(InsertTree(key, root->right->RootNode));

        return BalanceAfterInsertion(key, root);
    }

    AvlTree BalanceAfterInsertion(T key, PNode node) const {
        int balance = GetBalance(node);

        if (balance > 1 && CompareKey(key, node->left->RootNode) < 0)
            return AvlTree(RotateRight(node));
        if (balance < -1 && CompareKey(key, node->right->RootNode) > 0)
            return AvlTree(RotateLeft(node));
        if (balance > 1 && CompareKey(key, node->left->RootNode) > 0) {
            node->left->RootNode = RotateLeft(node->left->RootNode);
            return AvlTree(RotateRight(node));
        }
        if (balance < -1 && CompareKey(key, node->right->RootNode) < 0) {
            node->right->RootNode = RotateRight(node->right->RootNode);
            return AvlTree(RotateLeft(node));
        }
        return AvlTree(node);
    }

    AvlTree DeleteTree(T key, PNode root) const {
        if (!root)
            return AvlTree();

        if (key < root->key) {
            root->left = MakeShared(DeleteTree(key, root->left->RootNode));
        } else if (key > root->key) {
            root->right = MakeShared(DeleteTree(key, root->right->RootNode));
        } else {
            if (root->left->IsEmpty() || root->right->IsEmpty()) {
                PNode child = !root->left->IsEmpty() ? root->left->RootNode : root->right->RootNode;
                if (child == nullptr)
                    root = nullptr;
                else
                    root = child;

            } else {
                PNode smallestChild = GetMinNode(root->right->RootNode);
                root->key = smallestChild->key;
                root->right = MakeShared(DeleteTree(smallestChild->key, root->right->RootNode));

            }
        }

        if (root == nullptr)
            return AvlTree();

        return BalanceAfterDeletion(root);

    }

    PNode GetMinNode(PNode node) const {
        if (node == nullptr)
            return nullptr;

        PNode current = node;
        while (!current->left->IsEmpty())
            current = current->left->RootNode;

        return current;
    }

    AvlTree BalanceAfterDeletion(PNode node) const {
        int balance = GetBalance(node);

        if (balance > 1 && GetBalance(node->left->RootNode) >= 0)
            return AvlTree(RotateRight(node));

        if (balance > 1 && GetBalance(node->left->RootNode) < 0) {
            node->left->RootNode = RotateLeft(node->left->RootNode);
            return AvlTree(RotateRight(node));
        }

        if (balance < -1 && GetBalance(node->right->RootNode) <= 0)
            return AvlTree(RotateLeft(node));

        if (balance < -1 && GetBalance(node->right->RootNode) > 0) {
            node->right->RootNode = RotateRight(node->right->RootNode);
            return AvlTree(RotateLeft(node));
        }
        return AvlTree(node);
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
        return InsertTree(value, RootNode);
    }

    AvlTree Delete(T value) const {
        return DeleteTree(value, RootNode);
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

    bool IsEmpty() const {
        return RootNode == nullptr;
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
    tree = tree.Delete(5);
    tree.PrintElements();
    tree = tree.Delete(1);
    tree.PrintElements();
    tree = tree.Delete(1);
    tree.PrintElements();
    // tree = tree.Insert(2);
    // cout << tree.Size() << endl;
    return 0;
}