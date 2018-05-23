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
        if (!node) {
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
        if (!node)
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

    int CompareKey(T key, PNode second) const {
        if (second == nullptr)
            return 0;
        if (key < second->key)
            return -1;
        return 1;
    }

    AvlTree InsertTree(T key, PNode root) const {
        if (root == nullptr)
            return AvlTree(key);

        if (key == root->key)
            return AvlTree(root);

        if (key < root->key) {
            AvlTree leftSubTree = InsertTree(key, root->left->RootNode);
            PTree left = MakeShared(leftSubTree);
            Node nen = Node(root->key, left, root->right);
            root = MakeShared(nen);
        } else {
            AvlTree rightSubTree = InsertTree(key, root->right->RootNode);
            PTree right = MakeShared(rightSubTree);
            Node newNode = Node(root->key, root->left, right);
            root = MakeShared(newNode);
        }
        return Balance(key, root);
    }

    AvlTree Balance(T key, PNode node) const {
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

    PNode GetMinNode(PNode node) const {
        if (node == nullptr)
            return nullptr;

        PNode current = MakeShared(Node(node->key, node->left, node->right));

        /* loop down to find the leftmost leaf */
        while (current->left->RootNode != nullptr)
            current = current->left->RootNode;

        return current;
    }

    AvlTree DeleteTree(T key, PNode root) const {
        if (!root)
            return AvlTree();

        if (key < root->key) {
            PTree deleted = MakeShared(DeleteTree(key, root->left->RootNode));
            root->left = deleted;
        } else if (key > root->key) {
            PTree rightTree = root->right;
            PNode rightNode = rightTree->RootNode;
            PTree deleted = MakeShared(DeleteTree(key, rightNode));
            //auto t = deleted == NULL;
            auto tt = deleted->RootNode == nullptr;
            root->right = deleted;
        } else {
            // 1 или 0 потомков
            if (root->left->RootNode == nullptr || root->right->RootNode == nullptr) {
                PNode child = root->left->RootNode != nullptr ? root->left->RootNode : root->right->RootNode;
                //0 детей
                if (child == nullptr) {
                    child = MakeShared(Node(root->key, root->left, root->right));
                    root = nullptr;
                } else //1 ребёнок
                {
                    root = MakeShared(Node(child->key, child->left, child->right));
                }
                //child.reset();
            } else {
                PNode smallestChild = GetMinNode(root->right->RootNode);

                root->key = smallestChild->key;
                root->right = MakeShared(DeleteTree(smallestChild->key, root->right->RootNode));

            }
        }

        if (root == nullptr)
            return AvlTree();

        int balance = GetBalance(root);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && GetBalance(root->left->RootNode) >= 0)
            return AvlTree(RotateRight(root));

        // Left Right Case
        if (balance > 1 && GetBalance(root->left->RootNode) < 0) {
            root->left->RootNode = RotateLeft(root->left->RootNode);
            return AvlTree(RotateRight(root));
        }

        // Right Right Case
        if (balance < -1 && GetBalance(root->right->RootNode) <= 0)
            return AvlTree(RotateLeft(root));

        // Right Left Case
        if (balance < -1 && GetBalance(root->right->RootNode) > 0) {
            root->right->RootNode = RotateRight(root->right->RootNode);
            return AvlTree(RotateLeft(root));
        }
        return AvlTree(root);
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
    // tree = tree.Insert(2);
    // cout << tree.Size() << endl;
    return 0;
}