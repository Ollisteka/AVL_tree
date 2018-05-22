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

    explicit AvlTree(Node node) {
        RootNode = MakeShared(node);
    }

    AvlTree(T k, PTree l, PTree r) {
        RootNode = MakeShared(Node(k, l, r));
    }

    int GetDepth(PNode node) const {
        if (node == nullptr) {
            return 0;
        }

        int leftDepth = GetDepth(node->left->RootNode);
        int rightDepth = GetDepth(node->right->RootNode);
        return max(leftDepth, rightDepth) + 1;
    }

    int GetBalance(PTree t1, PTree t2) const {
        int left = GetDepth(t1->RootNode);
        int right = GetDepth(t2->RootNode);
        return left - right;
    }

    //balanceLL (Node v (Node lv ll lr) r) = (Node lv ll (Node v lr r))
    PNode BalanceLL(PNode node) const {
        PNode l = node->left->RootNode;
        PTree right = MakeShared(AvlTree(node->key, l->right, node->right));
        return MakeShared(Node(l->key, l->left, right));
    }

    //balanceLR (Node v (Node lv ll (Node rlv rll rrr)) r) = (Node rlv (Node lv ll rll) (Node v rrr r))
    PNode BalanceLR(PNode node) const {
        PNode l = node->left->RootNode;
        PNode rl = l->right->RootNode;
        PTree left = MakeShared(AvlTree(l->key, l->left, rl->left));
        PTree right = MakeShared(AvlTree(node->key, rl->right, node->right));
        return MakeShared(Node(rl->key, left, right));
    }

//    balanceRL (Node v l (Node rv (Node lrv lrl lrr) rr)) = (Node lrv (Node v l lrl) (Node rv lrr rr))
    PNode BalanceRL(PNode node) const {
        PNode r = node->right->RootNode;
        PNode lr = r->left->RootNode;
        PTree left = MakeShared(AvlTree(node->key, node->left, lr->left));
        PTree right = MakeShared(AvlTree(r->key, lr->right, r->right));
        return MakeShared(Node(lr->key, left, right));
    }

//    balanceRR (Node v l (Node rv rl rr)) = (Node rv (Node v l rl) rr)
    PNode BalanceRR(PNode node) const {
        PNode r = node->right->RootNode;
        PTree left = MakeShared(AvlTree(node->key, node->left, r->left));
        return MakeShared(Node(r->key, left, r->right));
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
        if (root == nullptr) {
            return AvlTree(node);
        }
        if (node->key == root->key)
            return AvlTree(root);

        PTree left_i = MakeShared(InsertTree(node, root->left->RootNode));
//        if (node->key < root->key) {
//            if (GetBalance(left_i, root->right) == 2 && CompareKey(node, root->left->RootNode) < 0) {
//                return AvlTree(BalanceLL(MakeShared(Node(node->key, left_i, root->right))));
//            }
//            if (GetBalance(left_i, root->right) == 2 && CompareKey(node, root->left->RootNode) > 0) {
//                return AvlTree(BalanceLR(MakeShared(Node(node->key, left_i, root->right))));
//            }
//        }
        PTree right_i = MakeShared(InsertTree(node, root->right->RootNode));
//        if (node->key > root->key) {
//            if (GetBalance(root->left, right_i) == -2 && CompareKey(node, root->right->RootNode) < 0) {
//                return AvlTree(BalanceRL(MakeShared(Node(node->key, root->left, right_i))));
//            }
//            if (GetBalance(root->left, right_i) == -2 && CompareKey(node, root->right->RootNode) > 0) {
//                return AvlTree(BalanceRR(MakeShared(Node(node->key, root->left, right_i))));
//            }
//        }

        if (node->key < root->key) {
            Node newRoot = Node(root->key, left_i, root->right);
            return AvlTree(newRoot);
        } else {
            Node newRoot = Node(root->key, root->left, right_i);
            return AvlTree(newRoot);
        }

    }

    vector<T> Traverse(PNode node) const {
        if (node == nullptr)
            return vector<T>();
        vector<T> left = Traverse(node->left->RootNode);
        vector<T> right = Traverse(node->right->RootNode);
        left.push_back(node->key);
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }


public:
    AvlTree() = default;

    AvlTree Insert(T value) const {
        Node node = Node(value);
        PNode n = make_unique<Node>(node);
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
    tree = tree.Insert(5);
    tree = tree.Insert(8).Insert(3).Insert(9);
    cout << tree.GetHeight() << endl;
    tree.PrintElements();
    // tree = tree.Insert(2);
    // cout << tree.Size() << endl;
    return 0;
}