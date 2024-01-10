//
// Created by toky on 31/3/23.
//

#ifndef KITER_STORAGEDISTRIBUTIONTREE_H
#define KITER_STORAGEDISTRIBUTIONTREE_H

#include <algorithms/dse/StorageDistribution.h>


class TreeNode {
public:
    TreeNode(const StorageDistribution& dist) : data(dist) {}
    ~TreeNode() {}

    StorageDistribution getData() const { return data; }
    void setData(const StorageDistribution& dist) { data = dist; }

    std::vector<TreeNode*>& getChildren() { return children; }

private:
    StorageDistribution data;
    std::vector<TreeNode*> children;
};
class StorageDistributionTree {
public:
    StorageDistributionTree() : root(nullptr) {}
    ~StorageDistributionTree() { deleteTree(root); }

    void addDistribution(const StorageDistribution& dist) {
        TreeNode* node = new TreeNode(dist);
        addDistributionHelper(node, root);
    }


    std::vector<StorageDistribution> getDistributions() const {
        std::vector<StorageDistribution> result;
        getDistributionsHelper(root, result);
        return result;
    }

private:
    TreeNode* root;

    void addDistributionHelper(TreeNode* node, TreeNode*& curr) {
        if (curr == nullptr) {
            curr = node;
            return;
        }

        if (node->getData().dominates(curr->getData())) {
            // Replace the current node with the new node and make the current node a child of the new node.
            TreeNode* oldCurr = curr;
            curr = node;
            curr->getChildren().push_back(oldCurr);
        } else {
            bool childAdded = false;
            for (TreeNode*& child : curr->getChildren()) {
                if (node->getData().dominates(child->getData())) {
                    addDistributionHelper(node, child);
                    childAdded = true;
                    break;
                }
            }
            if (!childAdded) {
                curr->getChildren().push_back(node);
            }
        }
    }


    void getDistributionsHelper(TreeNode* node, std::vector<StorageDistribution>& result) const {
        if (node == nullptr) {
            return;
        }
        result.push_back(node->getData());
        for (TreeNode* child : node->getChildren()) {
            getDistributionsHelper(child, result);
        }
    }

    void deleteTree(TreeNode* node) {
        if (node == nullptr) {
            return;
        }
        for (TreeNode* child : node->getChildren()) {
            deleteTree(child);
        }
        delete node;
    }

    void printHelper(TreeNode* node, std::string prefix, bool isTail, std::stringstream& output) const {
        if (node == nullptr) {
            return;
        }
        output << prefix;
        output << (isTail ? "└── " : "├── ");
        output << "Throughput: " << node->getData().getThroughput() << " Channel Sizes: " << node->getData().getQuantitiesStr() << std::endl;
        for (size_t i = 0; i < node->getChildren().size(); i++) {
            TreeNode* child = node->getChildren()[i];
            std::string newPrefix = prefix + (isTail ? "    " : "│   ");
            printHelper(child, newPrefix, i == node->getChildren().size() - 1, output);
        }
    }

public:
    std::string print() const {
        std::stringstream output;
        output << "Storage Distribution Tree:" << std::endl;
        printHelper(root, "", true, output);
        return output.str();
    }
};




#endif //KITER_STORAGEDISTRIBUTIONTREE_H
