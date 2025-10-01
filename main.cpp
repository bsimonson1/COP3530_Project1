#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <algorithm>

class BST
{
private:
    struct TreeNode
    {
        int node_ID;
        std::string node_name;
        TreeNode *left;
        TreeNode *right;
        TreeNode(std::string name, int ID) : node_name(name), node_ID(ID), left(nullptr), right(nullptr) {}
    };

    TreeNode* root = nullptr;
    BST::TreeNode* helperInsert(TreeNode* helpRoot, std::string, int name);
    BST::TreeNode* rotateLeft(TreeNode* helpRoot);
    BST::TreeNode* rotateRight(TreeNode* helpRoot);
    BST::TreeNode* rotateLeftRight(TreeNode* helpRoot);
    BST::TreeNode* rotateRightLeft(TreeNode* helpRoot);
    BST::TreeNode* findMinimumNode(TreeNode* helpRoot);
    BST::TreeNode*removeIDHelper(TreeNode* helpRoot, int ID);
    void searchIDHelper(TreeNode* helpRoot, int ID);
    bool searchNameHelper(TreeNode* helpRoot, std::string name);
    void recursiveTraversalInOrder(TreeNode* helpRoot, std::vector<std::string> &values);
    std::vector<std::string> printInOrderHelper(TreeNode* helpRoot);
    std::vector<std::string> printPreOrderHelper(TreeNode* helpRoot);
    void recursiveTraversalPreOrder(TreeNode* helpRoot, std::vector<std::string> &values);
    std::vector<std::string> printPostOrderHelper(TreeNode* helpRoot);
    void recursiveTraversalPostOrder(TreeNode* helpRoot, std::vector<std::string> &values);
    int printLevelCountHelper(TreeNode* helpRoot);
    void recursiveTraversalInOrderID(TreeNode* helpRoot, std::vector<int> &values);
    std::vector<int> findInOrder(TreeNode* helpRoot);
    bool removeInOrderHelper(TreeNode*& helpRoot, int& N);
    int removeInOrderHelperTwo(TreeNode*& helpRoot, int& N);


    // referenced in dsa lecture slides, with balance factor and height, balance factor can call height to get the necessary values, but I also use height in my level count method
    int getHeight(BST::TreeNode* helpRoot) {
        if (helpRoot == nullptr) {
            return 0;
        }
        return std::max(getHeight(helpRoot->left), getHeight(helpRoot->right)) + 1; // max of the passed in left sub tree "root" node left versus right, then add one to account for this root node, this is the same for right; max of the passed in right sub tree "root" node left versus right
    }

    // lecture 4 page 26, var is used for balance factor of the AVL tree to determine what rotations to do
    int balanceFactor(BST::TreeNode* helpRoot) {
        if (helpRoot == nullptr) {
            return 0;
        }
        return (getHeight(helpRoot->left) - getHeight(helpRoot->right));
    }

public:
    std::vector<std::string> inorder();
    std::vector<std::string> preorder();
    std::vector<std::string> postorder();
    void searchID(int ID);
    bool searchName(std::string name);
    void insert(std::string name, int ID);
    int printlevelcount();
    void removeID(int ID);
    bool removeInOrder(int N);
    int removeInOrderFirst(int N);
};

//used lecture content for rotate left Lecture 4 AVL Tree page 14
BST::TreeNode* BST::rotateLeft(BST::TreeNode* helpRoot) {
    BST::TreeNode* grandchild = helpRoot->right->left;
    BST::TreeNode* newParent = helpRoot->right;
    newParent->left = helpRoot;
    helpRoot->right = grandchild;
    return newParent;
}

//used lecture content for rotate left Lecture 4 AVL Tree page 14 just the opposite to perform the right rotation rather than left
BST::TreeNode* BST::rotateRight(BST::TreeNode* helpRoot) {
    BST::TreeNode* grandchild = helpRoot->left->right;
    BST::TreeNode* newParent = helpRoot->left;
    newParent->right = helpRoot;
    helpRoot->left = grandchild;
    return newParent;
}

BST::TreeNode* BST::rotateLeftRight(BST::TreeNode* helpRoot) {
    // first perform the left rotation by passing in the child left subtree node
    helpRoot->left = rotateLeft(helpRoot->left);
    // then return the right rotation of the previous left rotation
    return rotateRight(helpRoot);
    // the above logic was implemented after my AVL discussion section
}

BST::TreeNode* BST::rotateRightLeft(BST::TreeNode* helpRoot) {
    // inverse of the rotateLeftRight function, rotate right is done first and then return the left rotation of that performed right rotation
    helpRoot->right = rotateRight(helpRoot->right); // pass in the child right subtree node and then rotate the parent in the subsequent line
    return rotateLeft(helpRoot);
    // the above logic was implemented after my AVL discussion section
}

BST::TreeNode* BST::helperInsert(TreeNode* helpRoot, std::string name, int ID)
{
    if (helpRoot == nullptr) {
        return new TreeNode(name, ID);
    }
    if (helpRoot->node_ID > ID) {
        helpRoot->left = helperInsert(helpRoot->left, name, ID);
    }
        // this is for the right subtree
    else if (helpRoot->node_ID < ID) {
        helpRoot->right = helperInsert(helpRoot->right, name, ID);
    }
    else {
        // if the above conditions are not met then there is an issue
        std::cout << "unsuccessful" << std::endl;
        // this also checks for unsuccessful id insertion (ie if equal)
        return helpRoot;
    }
    // now i need to update the heights of the tree for balance factor (page 28 of lecture 4)
    int balance = balanceFactor(helpRoot);

    if (balance > 1 && ID < helpRoot->left->node_ID) {
        // this is a left left case so i do a right rotation to fix it
        return rotateRight(helpRoot);
    }
    if (balance < -1 && ID > helpRoot->right->node_ID) {
        // this is a right right case so i do a right rotation to fix it
        return rotateLeft(helpRoot);
    }
    if (balance > 1 && ID > helpRoot->left->node_ID) {
        // this is a left right rotation case
        return rotateLeftRight(helpRoot);
    }
    if (balance < -1 && ID < helpRoot->right->node_ID) {
        // this is a right left rotation case
        return rotateRightLeft(helpRoot);
    }
    // no balancing needed
    return helpRoot;

    // else if (ID < helpRoot->node_ID)
    //     helpRoot->left = helperInsert(helpRoot->left, name, ID);
    // else
    //     helpRoot->right = helperInsert(helpRoot->right, name, ID);

    // return helpRoot;
}

void BST::searchIDHelper(TreeNode* helpRoot, int ID) {
    // purely returning true or false if the ID is there
    if (helpRoot == nullptr) {
        // just return false and the unsuccessful will be printed in the main function
        std::cout << "unsuccessful" << std::endl;;
        return;
    }
    if (helpRoot->node_ID == ID) {
        std::cout << helpRoot->node_name << std::endl;
    }
        // since IDs are sorted by value we can improve the efficiency of the search by comparing the values of the Nodes ID to the ID we are searching for and search efficiently
    else if (ID < helpRoot->node_ID) {
        searchIDHelper(helpRoot->left, ID);
    }
    else if (ID > helpRoot->node_ID) {
        searchIDHelper(helpRoot->right, ID);
    }
    else {
        std::cout << "unsuccessful" << std::endl;
    }
}

bool BST::searchNameHelper(TreeNode* helpRoot, std::string name) {
    // purely returning true or false if the name is there
    if (helpRoot == nullptr) {
        // just return false and the unsuccessful will be printed in the main function
        return false;
    }
    if (helpRoot->node_name == name) {
        std::cout << helpRoot->node_ID << std::endl;
        return true;
    }

    // Check both left and right subtrees
    bool foundInLeft = searchNameHelper(helpRoot->left, name);
    bool foundInRight = searchNameHelper(helpRoot->right, name);

    // Return true if found in either left or right subtree
    return foundInLeft || foundInRight;
}

BST::TreeNode* BST::findMinimumNode(TreeNode* helpRoot) {
    while (helpRoot->left != nullptr) {
        helpRoot = helpRoot->left;
    }
    return helpRoot;
}

BST::TreeNode* BST::removeIDHelper(TreeNode* helpRoot, int ID) {
    // return the updated root
    // need to handle root deletion, no child deletion, 1 child deletion, 2 child deletion
    if (helpRoot == nullptr) {
        std::cout << "unsuccessful" << std::endl;
        return helpRoot;
    }
    // traverse the tree looking for the ID
    // node has not been found but the ID value is lesser than the current nodes ID value thus go left
    if (ID < helpRoot->node_ID) {
        // recall the function with the updated root
        helpRoot->left = removeIDHelper(helpRoot->left, ID);
    }
        // node has no been found but the ID value is greater than the current nodes ID value thus go right
    else if (ID > helpRoot->node_ID) {
        // recall the function with the updated root
        helpRoot->right = removeIDHelper(helpRoot->right, ID);
    }
    else {
        // node to remove has been found, do removal logic here
        // address node with two children here
        if (helpRoot->right != nullptr && helpRoot->left != nullptr) {
            // need to find the inorder successor to replace the current node
            BST::TreeNode* successor = findMinimumNode(helpRoot->right); // successor node to copy the info
            helpRoot->node_ID = successor->node_ID; // copy info from successor into the target node
            helpRoot->node_name = successor->node_name; // copy info from successor into the target node
            helpRoot->right = removeIDHelper(helpRoot->right, successor->node_ID); // remove the successor node as it has been moved
        }
            // node has zero children
        else if (helpRoot->right == nullptr && helpRoot->left == nullptr) {
            delete helpRoot;
            helpRoot = nullptr;
        }
            // node has right child
        else if (helpRoot->right != nullptr && helpRoot->left == nullptr) {
            TreeNode* temp = helpRoot;
            helpRoot = helpRoot->right;
            delete temp;
        }
            // node has left child
        else if (helpRoot->left != nullptr && helpRoot->right == nullptr) {
            TreeNode* temp = helpRoot;
            helpRoot = helpRoot->left;
            delete temp;
        }
    }
    return helpRoot;
}

// aspects of this traversal were implemented from a similar project I did last year, functioning as a helper method for the in order printing by recursively calling itself
void BST::recursiveTraversalInOrder(TreeNode* helpRoot, std::vector<std::string> &values) {
    if (helpRoot == nullptr) {
        return; // this either means there is no root, or, we are done recursively calling this function to find the left, root and right nodes
    }
    recursiveTraversalInOrder(helpRoot->left, values); // once we execute the first if statement then the value will be pushed as the left has been reached
    values.push_back(helpRoot->node_name); // push the node value onto it
    recursiveTraversalInOrder(helpRoot->right, values); // now do the same for the right
}

std::vector<std::string> BST::printInOrderHelper(TreeNode* helpRoot) {
    std::vector<std::string> values; // container for the values of the nodes we visit
    if (helpRoot->left == nullptr && helpRoot->right == nullptr) {
        values.push_back(helpRoot->node_name);
        return values;
    }
    // print the values; visit left sub, visit root, visit right
    recursiveTraversalInOrder(helpRoot, values);
    return values;
}
// aspects of this traversal were implemented from a similar project I did last year, functioning as a helper method for the pre order printing by recursively calling itself
void BST::recursiveTraversalPreOrder(TreeNode* helpRoot, std::vector<std::string> &values) {
    if (helpRoot == nullptr) {
        return; // this either means there is no root, or, we are done recursively calling this function to find the left, root and right nodes
    }
    values.push_back(helpRoot->node_name); // push the node value onto it
    recursiveTraversalPreOrder(helpRoot->left, values); // once we execute the first if statement then the value will be pushed as the left has been reached
    recursiveTraversalPreOrder(helpRoot->right, values); // now do the same for the right
}

std::vector<std::string> BST::printPreOrderHelper(TreeNode* helpRoot) {
    // print the values; visit root, visit left, visit right
    std::vector<std::string> values; // container for the values of the nodes we visit
    if (helpRoot->left == nullptr && helpRoot->right == nullptr) {
        values.push_back(helpRoot->node_name);
        return values;
    }
    recursiveTraversalPreOrder(helpRoot, values);
    return values;
}
// aspects of this traversal were implemented from a similar project I did last year, functioning as a helper method for the post order printing by recursively calling itself
void BST::recursiveTraversalPostOrder(TreeNode* helpRoot, std::vector<std::string> &values) {
    if (helpRoot == nullptr) {
        return; // this either means there is no root, or, we are done recursively calling this function to find the left, root and right nodes
    }
    recursiveTraversalPostOrder(helpRoot->left, values); // once we execute the first if statement then the value will be pushed as the left has been reached
    recursiveTraversalPostOrder(helpRoot->right, values); // now do the same for the right
    values.push_back(helpRoot->node_name); // push the node value onto it
}

std::vector<std::string> BST::printPostOrderHelper(TreeNode* helpRoot) {
    // print the values; visit left, visit right, visit root
    std::vector<std::string> values; // container for the values of the nodes we visit
    if (helpRoot->left == nullptr && helpRoot->right == nullptr) {
        values.push_back(helpRoot->node_name);
        return values;
    }
    recursiveTraversalPostOrder(helpRoot, values);
    return values;
}

// root is passed in and then traversed assuming it exists. It calls get height
int BST::printLevelCountHelper(TreeNode* helpRoot) {
    // print the values
    if (helpRoot == nullptr) {
        // if no tree then we print 0
        return 0;
    }
    else {
        // otherwise we get the height of the tree and print that value
        return getHeight(helpRoot); // only successful if we are not already printing something thus no need for successful here
    }
}

void BST::recursiveTraversalInOrderID(TreeNode* helpRoot, std::vector<int> &values) {
    if (helpRoot == nullptr) {
        return; // this either means there is no root, or, we are done recursively calling this function to find the left, root and right nodes
    }
    recursiveTraversalInOrderID(helpRoot->left, values); // once we execute the first if statement then the value will be pushed as the left has been reached
    values.push_back(helpRoot->node_ID); // push the node ID value onto it
    recursiveTraversalInOrderID(helpRoot->right, values); // now do the same for the right
}

// new spin on previous in order method, now I am calling a recursive traversal method that wiull find and push the IDs onto the vector
std::vector<int> BST::findInOrder(TreeNode* helpRoot) {
    // print the values; visit left sub, visit root, visit right
    std::vector<int> values; // container for the values of the nodes we visit
    recursiveTraversalInOrderID(helpRoot, values);
    return values;
}

int BST::removeInOrderHelperTwo(TreeNode*& helpRoot, int& N) {
    std::vector<int> inorder_vals = findInOrder(helpRoot); // this gets the IDs that have been pushed via the inorder traversal

    // Check if N is out of range
    if (N < 1 || N > inorder_vals.size()) {
        return false;
    }
    // MADE A HUGE MISTAKE HERE, N IS ALREADY 0 INDEXED, I WAS DOING N-1 THINKING IT WAS N INDEXED!!!!!!!!!!!!!!!!!!
    int ID_to_remove = inorder_vals[N]; // now I can reference the passed in Nth node to remove and remove it VIA the ID value
    return ID_to_remove;
}

bool BST::removeInOrderHelper(TreeNode*& helpRoot, int& N) {
    std::vector<int> inorder_vals = findInOrder(helpRoot); // this gets the IDs that have been pushed via the inorder traversal

    // Check if N is out of range
    if (N < 1 || N > inorder_vals.size()) {
        return false;
    }
    // MADE A HUGE MISTAKE HERE, N IS ALREADY 0 INDEXED, I WAS DOING N-1 THINKING IT WAS N INDEXED!!!!!!!!!!!!!!!!!!
    int ID_to_remove = inorder_vals[N]; // now I can reference the passed in Nth node to remove and remove it VIA the ID value
    helpRoot = removeIDHelper(helpRoot, ID_to_remove);
    return true;
}

std::vector<std::string> BST::inorder() {
    return printInOrderHelper(this->root);
}

std::vector<std::string> BST::preorder() {
    return printPreOrderHelper(this->root);
}

std::vector<std::string> BST::postorder() {
    return printPostOrderHelper(this->root);
}

int BST::printlevelcount() {
    return printLevelCountHelper(this->root);
}

void BST::insert(std::string name, int ID)
{
    this->root = helperInsert(this->root, name, ID);
}

void BST::removeID(int ID) {
    this->root = removeIDHelper(this->root, ID);
}

void BST::searchID(int ID) {
    searchIDHelper(this->root, ID);
}

bool BST::searchName(std::string name) {
    return searchNameHelper(this->root, name);
}

bool BST::removeInOrder(int N) {
    return removeInOrderHelper(this->root, N);
}

int BST::removeInOrderFirst(int N) {
    return removeInOrderHelperTwo(this->root, N);
}

/* WILL NEED TO ADD HEIGHT AND BALANCE ATTRIBUTES ONCE THIS IS COVERED
 * Balance is left minus right. There is a balance for each node. Balance is based on the height of the left subtree - the height of the right subtree.
 * Need a height function to get the height of each node being evaluated.
 * height will be called by the balance function to see if balancing needs to occur.
 * four rotations, left, right, left-right, right-left
 * all operations must be O(1)
*/

// method to confirm that the ID is valid
bool isValidID(const std::string &id) {
    //checks the length of the ID (as a string)
    if (id.length() != 8) {
        return false;
    }
    //checks that each id is an integer string character
    for (char element : id) {
        if (!isdigit(element)) {
            return false;
        }
    }
    // if true then it will convert the string Id into an integer ID
    return true;
}

// similar implementation to above except with isalpha
bool isValidName(const std::string &name) {
    //checks that each char is an alphabetical letter
    if (name.length() < 1) {
        return false;
    }
    for (char element : name) {
        if (!isalpha(element) && !isspace(element)) { // checks for both letters and the spaces
            return false;
        }
    }
    // if true then it will then continue the code execution
    return true;
}

bool repeatedUFID(const std::vector<std::string> &IDS, const std::string &intermediate_ID) {
    if (IDS.size() == 0) {
        return true;
    }

    //set is best for checking for duplicates
    std::set<std::string> uniqueIDs(IDS.begin(), IDS.end());

    // Check if intermediate_ID is already in the set, if it is we are returning false, which will be notted to true thus unsuccessful
    if (uniqueIDs.find(intermediate_ID) != uniqueIDs.end()) {
        return false;
    }
    // No duplicates found
    return true;
}

void removalOfID(std::vector<std::string> &IDS, std::vector<std::string> &IDS_to_remove) {
    auto isToRemove = [&IDS_to_remove](const std::string &value) {
        return std::find(IDS_to_remove.begin(), IDS_to_remove.end(), value) != IDS_to_remove.end();
    };

    IDS.erase(std::remove_if(IDS.begin(), IDS.end(), isToRemove), IDS.end());
    IDS_to_remove.clear();
}


int main()
{
    BST myBST;
    int numberOfInstructions = 0; // number of instructions for the for loop
    int ID; // integer ID value that we will be inserting
    std::string line; // one line of instructions at a time
    std::string instruction; // this will be the first word of the line and dictates what functions to call
    std::string name; // this will be the second word, contained by " ", and is part of the information I am inserting
    std::string intermediate_ID; // this is a placeholder just to confirm the length of the passed in id string is 8, it'll be converted to an integer once this is confirmed
    std::vector<std::string> IDS; // I am using this to store repeated IDs
    std::cin >> numberOfInstructions; // get the number of instructions for the for loop
    std::cin.ignore(); // remove whitespace
    bool removalTracker = false;
    std::vector<std::string> IDS_to_remove;

    if (numberOfInstructions > 1000 || numberOfInstructions < 1) {
        // check to see if the number of instructions is valid or not
        std::cout << "unsuccessful" << std::endl;
        return 0;
    }

    for (int i = 0; i < numberOfInstructions; i++) {
        getline(std::cin, line);

        // regex idea came from an internship I did over the summer, we extensively used regex for parsing through excel files and subsequently building yaml files with desired pieces of information
        // I decided to use individual regex patterns rather than a regex to check for the instruction and then handle it for simplicity, now I can have individual if and else if statements based on the instruction match
        std::regex insertPattern("^insert\\s+\"([^\"]*)\"\\s+(\\d+)$"); // regex for each instruction, non capturing group for the instruction and then capturing groups for the name and ID
        std::regex removePattern("^remove\\s+(\\d+)$"); // regex for each instruction, non capturing group for the instruction and then capturing group for ID
        //regex searchIDPattern("^searchID\\s+(\\d+)$"); // regex for each instruction, non capturing group for the instruction and then capturing group ID
        //regex searchNamePattern("^searchName\\s+\"([^\"]*)\"$"); // regex for each instruction, non capturing group for the instruction and then capturing group for name
        std::regex searchPattern("^search\\s+(\\d+|\"[^\"]+\")$");
        std::regex printInorderPattern("^printInorder$"); // regex for each instruction, non capturing group for the instruction and no capturing group since there are no parameters to analyze
        std::regex printPreorderPattern("^printPreorder$"); // regex for each instruction, non capturing group for the instruction and no capturing group since there are no parameters to analyze
        std::regex printPostorderPattern("^printPostorder$"); // regex for each instruction, non capturing group for the instruction and no capturing group since there are no parameters to analyze
        std::regex printLevelCountPattern("^printLevelCount$"); // regex for each instruction, non capturing group for the instruction and no capturing group since there are no parameters to analyze
        std::regex removeInorderPattern("^removeInorder\\s+(\\d+)$"); // regex for each instruction, non capturing group for the instruction and then capturing group for the removal of the Nth node

        std::smatch match;

        if (regex_match(line, match, insertPattern)) {
            name = match[1];
            intermediate_ID = match[2];
            if (!isValidID(intermediate_ID)) {
                std::cout << "unsuccessful" << std::endl;
                continue;
            }
            if (removalTracker) {
                removalTracker = false;
                removalOfID(IDS, IDS_to_remove);
            }
            if (repeatedUFID(IDS, intermediate_ID)) {
                IDS.push_back(intermediate_ID);
            }
            else {
                //cout << "unsuccessful " << name << endl; // this is just to check to ensure that the name associated with the ID is in line since the terminal window messes up the output
                std::cout << "unsuccessful" << std::endl;
                continue;
            }
            ID = std::stoi(intermediate_ID);
            if (!isValidName(name)) {
                std::cout << "unsuccessful" << std::endl;
                continue;
            }

            // if entered I have the first instruction, insert, thus call the insert method
            // auto newRoot = insertNameID(root, name, ID);
            // root = newRoot;
            myBST.insert(name, ID);
            std::cout << "successful" << std::endl;
        }
        else if (regex_match(line, match, removePattern)) {
            intermediate_ID = match[1];

            if (!isValidID(intermediate_ID)) {
                std::cout << "unsuccessful" << std::endl;
                continue;
            }
            ID = std::stoi(intermediate_ID);

            // if entered I have the first instruction, remove, thus call the remove method
            // auto newRoot = removeID(root, ID);
            // root = newRoot;
            removalTracker = true; // this will tell us that the ID needs to be removed and it will be removed
            IDS_to_remove.push_back(intermediate_ID); // this stores the IDS that need to be removed just in case insert is not called for a while we want want to remove the ids from my vector of ids before checking if there are repeated IDS
            myBST.removeID(ID);
            std::cout << "successful" << std::endl;
        }
        else if (regex_match(line, match, searchPattern)) {
            std::string string_or_int = match[1]; // intermediate value to check if the matched value has digits or is a string

            if (isdigit(string_or_int[0])) { // if we find that there is a digit then we assume that we are seaching ID
                if (!isValidID(string_or_int)) {  // this covers the check if the passed in id is 1A since we will be hear as we assume it is an id but then realize it is invalid
                    std::cout << "unsuccessful" << std::endl;
                    continue;
                }
                // now that the check is done we can convert the string in to the id
                ID = std::stoi(string_or_int);
                myBST.searchID(ID); // perform the search
            }
            else {
                std::string new_string; // new string variable to add to since the match capture group captures the " "
                for (char element : string_or_int) {
                    if (element != '"') {
                        new_string += element;
                    }
                }
                // new string now has the name with the " " removed
                // now we can check if it is a valid name
                if (!isValidName(new_string)) {
                    std::cout << "unsuccessful" << std::endl;
                    continue;
                }
                if (myBST.searchName(new_string)) {
                    // I am returning a boolean so if it returns true that means the name was found and I print successful
                    //cout << "successful" << endl;
                    continue;
                }
                else {
                    // I am returning a boolean so if it returns false that means the name was not found and I print unsuccessful
                    //cout << string_or_int << endl;
                    std::cout << "unsuccessful" << std::endl;
                    continue;
                }
            }
        }
        else if (regex_match(line, match, printInorderPattern)) {
            // just call the print function, it is a void method and the logic as well as the printing will be done there
            std::vector<std::string> values;
            values = myBST.inorder();
            if (!values.empty()) {
                for (int i = 0; i < values.size(); i++) {
                    if (i == values.size() - 1) { // printing a comma separated "list" of the names. if this the last name I do not want to print a comma with it
                        std::cout << values[i] << std::endl;
                    }
                    else { // if the above check is not met then there are still names to print out WITH the commas
                        std::cout << values[i] << ", ";
                    }
                }
            }
            else {
                std::cout << "0" << std::endl;
            }
        }
        else if (regex_match(line, match, printPreorderPattern)) {
            std::vector<std::string> values;
            // just call the print function, it is a void method and the logic as well as the printing will be done there
            values = myBST.preorder();
            if (!values.empty()) {
                for (int i = 0; i < values.size(); i++) {
                    if (i == values.size() -
                             1) { // printing a comma separated "list" of the names. if this the last name I do not want to print a comma with it
                        std::cout << values[i] << std::endl;
                    } else { // if the above check is not met then there are still names to print out WITH the commas
                        std::cout << values[i] << ", ";
                    }
                }
            }
            else {
                std::cout << "0" << std::endl;
            }
        }
        else if (regex_match(line, match, printPostorderPattern)) {
            std::vector<std::string> values;
            // just call the print function, it is a void method and the logic as well as the printing will be done there
            values = myBST.postorder();
            if (!values.empty()) {
                for (int i = 0; i < values.size(); i++) {
                    if (i == values.size() -
                             1) { // printing a comma separated "list" of the names. if this the last name I do not want to print a comma with it
                        std::cout << values[i] << std::endl;
                    } else { // if the above check is not met then there are still names to print out WITH the commas
                        std::cout << values[i] << ", ";
                    }
                }
            }
            else {
                std::cout << "0" << std::endl;
            }
        }
        else if (regex_match(line, match, printLevelCountPattern)) {
            int level;
            // just call the print function, it is a void method and the logic as well as the printing will be done there
            level = myBST.printlevelcount();
            std::cout << level << std::endl;
        }
        else if (regex_match(line, match, removeInorderPattern)) {
            intermediate_ID = match[1];
            // Parse the N value from intermediate_ID
            int N = std::stoi(intermediate_ID);
            // Call removeInOrder with the root and N
            std::string value_to_remove = std::to_string(myBST.removeInOrderFirst(N));
            if (myBST.removeInOrder(N)) {
                IDS_to_remove.push_back(value_to_remove);
                removalOfID(IDS, IDS_to_remove);
                std::cout << "successful" << std::endl;
            }
            else {
                std::cout << "unsuccessful" << std::endl;
            }
        }
        else {
            std::cout << "unsuccessful" << std::endl;
            continue;
        }
    }
}
