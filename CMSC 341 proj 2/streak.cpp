//
//  streak.cpp
//  prjoj2
//
//  Created by Elijah Busse on 3/4/26.
//

// UMBC - CMSC 341 - Spring 2026 - Proj2
#include "streak.h"

Streak::Streak(){
    
    //Initialize all variables
    m_root = nullptr;
    m_gridID = 0;
    m_left = nullptr;
    m_right = nullptr;
    
    m_treeHeight = -1;
    nodeCount = 0;
    
}

Streak::Streak(int grid, Tiger tigers[], int population){
    
    //Initialize grid ID and root pointers
    m_gridID = grid;
    m_root = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    m_treeHeight = -1;
    nodeCount = 0;
    
    //Iterate through the array and add the tigers the to the tree
    int counter = 0;
    while(counter < population){
        insert(tigers[counter]);
        counter++;
    }
    
    m_treeHeight = getHeight(m_root);
    
}

Streak::~Streak(){
    
    clear();
    
}

//This function returns the balance factor of any particular node
int Streak::balanceFactor(Tiger * aTiger){
    
    //If empty
    if(aTiger == nullptr){return -1;}
    
    int balFact = 0;
    
    Tiger * leftTiger = aTiger->getLeft();
    Tiger * rightTiger = aTiger->getRight();
    
    //Calculate balance factor
    balFact = getHeight(leftTiger) - getHeight(rightTiger);
    
    return(balFact);
    
    
}

//Returns the maximum value of two integers
int Streak::maximumValue(int a, int b){
    
    if(a >= b){
        return a;
    }
    else{
        return b;
    }
}

//Function returns the height of a node
int Streak::getHeight(Tiger * tiger){
    if(tiger == nullptr){return -1;}
    return tiger->getHeight();
}

//Helper function for rebalancing right rotate
Tiger* Streak::rotateRight(Tiger* aTiger){
    Tiger * x = aTiger->getLeft();
    Tiger * z = x->getRight();
    
    x->setRight(aTiger);
    aTiger->setLeft(z);
    
    aTiger->setHeight(1 + maximumValue(getHeight(aTiger->getLeft()), getHeight(aTiger->getRight())));
    x->setHeight(1 + maximumValue(getHeight(x->getLeft()), getHeight(x->getRight())));
    
    return x;
}

//Helper function for reblanacing left rotate
Tiger* Streak::rotateLeft(Tiger* aTiger){
    
    Tiger * x = aTiger->getRight();
    Tiger * z = x->getLeft();
    
    x->setLeft(aTiger);
    aTiger->setRight(z);
    
    aTiger->setHeight(1 + maximumValue(getHeight(aTiger->getLeft()), getHeight(aTiger->getRight())));
    x->setHeight(1 + maximumValue(getHeight(x->getLeft()), getHeight(x->getRight())));
    
    return x;
}

void Streak::insert(const Tiger& tiger){
    
    //Check ID is within valid range
    if(!(MINID <= tiger.getID() && tiger.getID() <= MAXID)){return;}
    
    //Check if ID already exists
    if(findTiger(tiger.getID())){return;}
    
    //Create the node
    Tiger * theTiger = new Tiger(tiger.getID(), tiger.getAge(), tiger.getGender(), tiger.getState());
    
    //Iterate the counter
    nodeCount++;
    
    //Set its height to 0, as new nodes always start at height 0
    theTiger->setHeight(0);
    
    //Call to recursive helper function
    m_root = insertHelper(m_root, theTiger);
    
    //Update total tree height
    m_treeHeight = getHeight(m_root);
    
}

//This helper function provides recursive ability to insert function
Tiger* Streak::insertHelper(Tiger * r, Tiger * tiger){
    
    //Empty case, or end of leaf case
    if(r == nullptr){return tiger;}
    
    //Inserting node is less than root node
    if(tiger->getID() < r->getID()){
        r->setLeft(insertHelper(r->getLeft(), tiger));
    }
    
    //Inserting node is greater than root node
    else if(tiger->getID() > r->getID()){
        r->setRight(insertHelper(r->getRight(), tiger));
    }
    
    else{
        return r;
    }
    
    //Update the height
    r->setHeight(1 + maximumValue(getHeight(r->getLeft()), getHeight(r->getRight())));
    
    //Variable to hold balance factor for reblanacing
    int bf = 0;
    
    bf = balanceFactor(r);
    
    //Rotation cases for various balance factors
    
    //Left Left Inbalance
    if (bf > 1 && tiger->getID() < r->getLeft()->getID()) {
        return rotateRight(r);
    }

    //Right Right Inbalance
    if (bf < -1 && tiger->getID() > r->getRight()->getID()) {
        return rotateLeft(r);
    }

    //Left Right Inbalance
    if (bf > 1 && tiger->getID() > r->getLeft()->getID()) {
        r->setLeft(rotateLeft(r->getLeft()));
        return rotateRight(r);
    }

    //Right Left Inbalance
    if (bf < -1 && tiger->getID() < r->getRight()->getID()) {
        r->setRight(rotateRight(r->getRight()));
        return rotateLeft(r);
    }

    return r;
    
}

void Streak::clear(){
    
    clearHelper(m_root);
    m_root = nullptr;
    
}

void Streak::clearHelper(Tiger * node){
    
    if(node == nullptr){
        return;
    }
    
    clearHelper(node->m_left);
    clearHelper(node->m_right);
    
    delete node;
    
}

void Streak::remove(int id){
    
    m_root = removeHelper(m_root, id);
    m_treeHeight = getHeight(m_root);
}

//Recursice helper function for remove
Tiger * Streak::removeHelper(Tiger * root, int id){
    
    //Empty case
    if(root == nullptr){return root;}
    
    //ID smaller than current node
    else if(id < root->getID()){
        root->setLeft(removeHelper(root->getLeft(), id));
    }
    
    //ID greater than current node
    else if(id > root->getID()){
        root->setRight(removeHelper(root->getRight(), id));
    }
    
    //Value matches
    else{
        //If deleting a leaf node
        if(root->getLeft() == nullptr && root->getRight() == nullptr){
            delete root;
            return nullptr;
        }
        
        //If node has no left child
        else if(root->getLeft() == nullptr){
            Tiger * temp = root->getRight();
            delete root;
            return temp;
        }
        
        //If node has no right child
        else if(root->getRight() == nullptr){
            Tiger * temp = root->getLeft();
            delete root;
            return temp;
        }
        
        //Node has two children
        else{
            //Find in order successor
            Tiger * succ = findMin(root->getRight());
            
            //Copy successor content into current node
            root->setAge(succ->getAge());
            root->setState(succ->getState());
            root->setGender(succ->getGender());
            root->setHeight(succ->getHeight());
            root->setID(succ->getID());
            
            //Delete the successor
            root->setRight(removeHelper(root->getRight(), root->getID()));
        }
    }
    
    //Update the height
    root->setHeight(1 + maximumValue(getHeight(root->getLeft()), getHeight(root->getRight())));
    
    //Variable to hold balance factor for reblanacing
    int bf = 0;
    
    bf = balanceFactor(root);
    
    //Rotation cases for various balance factors
    
    //Left Left Inbalance
    if (bf > 1 && balanceFactor(root->getLeft()) >= 0) {
        return rotateRight(root);
    }

    //Right Right Inbalance
    if (bf < -1 && balanceFactor(root->getRight()) <= 0) {
        return rotateLeft(root);
    }

    //Left Right Inbalance
    if (bf > 1 && balanceFactor(root->getLeft()) < 0) {
        root->setLeft(rotateLeft(root->getLeft()));
        return rotateRight(root);
    }

    //Right Left Inbalance
    if (bf < -1 && balanceFactor(root->getRight()) > 0) {
        root->setRight(rotateRight(root->getRight()));
        return rotateLeft(root);
    }

    return root;
}

//This function finds the minimum value node under the passed node r
Tiger* Streak::findMin(Tiger* r){
    return findMinHelper(r);
}

Tiger* Streak::findMinHelper(Tiger* r){
    
    if(r->getLeft() == nullptr){return r;}
    
    r = findMinHelper(r->getLeft());
    
    return r;
    
}

void Streak::listTigers() const{
    
    listHelper(m_root);
    
}

//This function allows for in order traversal of the list using recursion
void Streak::listHelper(Tiger* r) const{
    
    if(r == nullptr){return;}
    
    //In order traversal
    listHelper(r->getLeft());
    cout << r->getID() << ":" << r->getAgeStr() << ":" << r->getGenderStr() << ":" << r->getStateStr() << endl;
    listHelper(r->getRight());
    
}

bool Streak::setState(int id, STATE state){
    
    //Call to recursively traverse the tree, and attempt to change the state
    return setStateHelper(m_root, id, state);
    
}

bool Streak::setStateHelper(Tiger * r, int id, STATE state){
    
    if(r == nullptr){
         return false;
     }
     
     if(id < r->getID()){
         return setStateHelper(r->getLeft(), id, state);
     }
     else if(id > r->getID()){
         return setStateHelper(r->getRight(), id, state);
     }
     else{
         //found the tiger
         if(r->getState() == state){
             return false;
         }
         r->setState(state);
         return true;
     }
}

void Streak::removeDead(){
    
    Tiger* temp = removeDeadHelper(m_root);
    
    while(temp != nullptr){
        
        remove(temp->getID());
        temp = removeDeadHelper(m_root);
        
    }
}

Tiger * Streak::removeDeadHelper(Tiger* r){
    
    if(r == nullptr){return nullptr;}
    
    Tiger* foundDeadLeft = removeDeadHelper(r->getLeft());
    if(foundDeadLeft != nullptr){
        return foundDeadLeft;
    }
    
    Tiger* foundDeadRight = removeDeadHelper(r->getRight());
    if(foundDeadRight != nullptr){
        return foundDeadRight;
    }
    
    if(r->getState() == DEAD){
        return r;
    }
    
    return nullptr;
}

bool Streak::findTiger(int id) const {
    //Make check pointer
    Tiger * test = m_root;
    
    //Check the other nodes
    while(test != nullptr){
        if(test->m_id == id){
            return true;
        }
        //Move right if bigger
        if(id > test->m_id){
            test = test->m_right;
        }
        //Move left if smaller
        else{
            test = test->m_left;
        }
    }
    return false;
}

int Streak::count(AGE age) const{
    //Call helper function to search recursively
    int count = countHelper(m_root, age);
    return count;
}

int Streak::countHelper(Tiger* r, AGE age) const{
    
    int count = 0;
    
    if(r == nullptr){return 0;}
    
    //Search the subtrees recursively
    count += countHelper(r->getLeft(), age);
    count += countHelper(r->getRight(), age);
    
    if(r->getAge() == age){
        return count + 1;
    }
    
    return count;
}

int Streak::count(STATE state) const{
    //Call helper function to search recursively
    int count = countHelperState(m_root, state);
    return count;
}

int Streak::countHelperState(Tiger* r, STATE state) const{
    
    int count = 0;
    
    if(r == nullptr){return 0;}
    
    //Search the subtrees recursively
    count += countHelperState(r->getLeft(), state);
    count += countHelperState(r->getRight(), state);
    
    if(r->getState() == state){
        return count + 1;
    }
    
    return count;
}

void Streak::dumpTree() const {dump(m_root);}

void Streak::dump(Tiger* aTiger) const{
    if (aTiger != nullptr){
        cout << "(";
        dump(aTiger->m_left);//first visit the left child
        cout << aTiger->m_id << ":" << aTiger->m_height;//second visit the node itself
        dump(aTiger->m_right);//third visit the right child
        cout << ")";
    }
}

//////////////////////////////////////////////////////////////////////
Grid::Grid(){
    m_root = nullptr;
}

Grid::~Grid(){
    clear();
}

void Grid::clear(){
    
    clearHelper(m_root);
    m_root = nullptr;
    
}

void Grid::clearHelper(Streak* node){
    
    if(node == nullptr){
        return;
    }
    
    clearHelper(node->m_left);
    clearHelper(node->m_right);
    
    delete node;
    
}

bool Grid::findStreak(int id) const{
    
    //Make check pointer
    Streak * test = m_root;
    
    //Check the other nodes
    while(test != nullptr){
        if(test->m_gridID == id){
            return true;
        }
        //Move right if bigger
        if(id > test->m_gridID){
            test = test->m_right;
        }
        //Move left if smaller
        else{
            test = test->m_left;
        }
    }
    return false;
    
    
}

bool Grid::insert(int grid, Tiger tigers[], int population){
    
    //Check to avoid duplicates
    if(findStreak(grid)){return false;}
    
    //Create a new streak and load in the tigers
    Streak* newNode = new Streak(grid, tigers, population);
    
    //Temp pointers to traverse the tree
    Streak* temp = m_root;
    Streak* parent = nullptr;
    
    while(temp != nullptr){
        //Assign parent to temp
        parent = temp;
    
        //Check move left or right
        if(newNode->m_gridID < temp->m_gridID){
            temp = temp->m_left;
        }else{
            temp = temp->m_right;
        }
    }
    
    //Insert node in correct location
    if(parent == nullptr){
        m_root = newNode;
    }
    else if(newNode->m_gridID < parent->m_gridID){
        parent->m_left = newNode;
    }else{
        parent->m_right = newNode;
    }
    
    splay(newNode);
    
    return true;
}

//THis function performs right rotations for splay function
Streak* Grid::rightRotate(Streak* streak){
    
    Streak * x = streak->m_left;
    Streak * z = x->m_right;
    
    x->m_right = streak;
    streak->m_left = z;
    
    return x;
}

//THis function performs left rotations for splay function
Streak* Grid::leftRotate(Streak* streak){
    Streak * x = streak->m_right;
    Streak * z = x->m_left;
    
    x->m_left = streak;
    streak->m_right = z;
    
    return x;
}

//This is a helper function for splay, that finds the parent node of the node passed
Streak* Grid::findParent(Streak* child){
    
    //Make check pointer
    Streak * test = m_root;
    Streak* parent = nullptr;
    
    //Check the other nodes
    while(test != child){
        parent = test;
        
        //Move right if bigger
        if(child->m_gridID > test->m_gridID){
            test = test->m_right;
        }
        //Move left if smaller
        else{
            test = test->m_left;
        }
    }
    return parent;
    
}

//This function performs the splay operation for the tree
bool Grid::splay(Streak* streak){
    
    //If node inserted is root
    if(streak == m_root){return true;}
    
    //Create the parent and grandparent pointers to use in the splay
    Streak* parent = nullptr;
    Streak* grandParent = nullptr;
    Streak* greatGrandParent = nullptr;
    
    
    //Iterative loop to splay the tree
    while(streak != m_root){
        
        //Update parent
        parent = findParent(streak);
        
        //Bool used to check for great grand parent
        bool GGPexists = false;
        bool GPisLeftChild = true;
        
        //streak is one step down
        if(parent == m_root){
            if(streak->m_gridID < parent->m_gridID){
                m_root = rightRotate(parent);
            }else{
                m_root = leftRotate(parent);
            }
        }else{
            //Find grandparent and great grandparent
            grandParent = findParent(parent);
            greatGrandParent = findParent(grandParent);
            
            //If a grandparent exists
            //Check if great grand parent exists
            if(greatGrandParent != nullptr){
                GGPexists = true;
            }
            
            //Check if grandparent is left or right child
            if(GGPexists){
                if(greatGrandParent->m_right == grandParent){
                    GPisLeftChild = false;
                }
            }
            
            //Both streak and parent are left children
            if(streak == parent->m_left && parent == grandParent->m_left){
                //If great grand parent exists
                if(GGPexists){
                    if(GPisLeftChild){
                        greatGrandParent->m_left = rightRotate(rightRotate(grandParent));
                    }else{
                        greatGrandParent->m_right = rightRotate(rightRotate(grandParent));
                    }
                }else{
                    //If great grand parent does not exist
                    m_root = rightRotate(rightRotate(grandParent));
                }
            }
            //Both streak and parent are right children
            else if(streak == parent->m_right && parent == grandParent->m_right){
                //If great grand parent exists
                if(GGPexists){
                    if(GPisLeftChild){
                        greatGrandParent->m_left = leftRotate(leftRotate(grandParent));
                    }else{
                        greatGrandParent->m_right = leftRotate(leftRotate(grandParent));
                    }
                }else{
                    //If great grand parent does not exist
                    m_root = leftRotate(leftRotate(grandParent));
                }
            }
            //Streak is left child, parent is right child
            else if(streak == parent->m_left && parent == grandParent->m_right){
                //First right rotate on parent
                grandParent->m_right = rightRotate(parent);
                
                //If great grand parent exists
                if(GGPexists){
                    if(GPisLeftChild){
                        greatGrandParent->m_left = leftRotate(grandParent);
                    }else{
                        greatGrandParent->m_right = leftRotate(grandParent); //WE ARE HERE
                    }
                }else{
                    //If great grand parent does not exist
                    m_root = leftRotate(grandParent);
                }
            }
            //Streak is right child, parent is left child
            else{
                //First leftt rotate on parent
                grandParent->m_left = leftRotate(parent);
                
                //If great grand parent exists
                if(GGPexists){
                    if(GPisLeftChild){
                        greatGrandParent->m_left = rightRotate(grandParent);
                    }else{
                        greatGrandParent->m_right = rightRotate(grandParent); //WE ARE HERE
                    }
                }else{
                    //If great grand parent does not exist
                    m_root = rightRotate(grandParent);
                }
            }
        }
    }
    
    return true;
}

int Grid::count(int grid, STATE state){
    
    //pointer initialized
    Streak* temp = m_root;
    
    //Search the grid for the streak
    while(temp != nullptr && temp->m_gridID != grid){
        if(temp->m_gridID < grid){
            temp = temp->m_right;
        }else{
            temp = temp->m_left;
        }
    }
    
    //Don't you go dereferencing nullptr buddy
    if(temp == nullptr){return 0;}
    
    splay(temp);

    return temp->count(state);
}

int Grid::count(int grid, AGE age){
    
    //pointer initialized
    Streak* temp = m_root;
    
    //Search the grid for the streak
    while(temp != nullptr && temp->m_gridID != grid){
        if(temp->m_gridID < grid){
            temp = temp->m_right;
        }else{
            temp = temp->m_left;
        }
    }
    
    //Don't you go dereferencing nullptr buddy
    if(temp == nullptr){return 0;}
    
    splay(temp);
    
    return temp->count(age);

}

bool Grid::removeTiger(int grid, int tiger, bool all){
    
    //pointer initialized
    Streak* temp = m_root;
    
    //Search the grid for the streak
    while(temp != nullptr && temp->m_gridID != grid){
        if(temp->m_gridID < grid){
            temp = temp->m_left;
        }else{
            temp = temp->m_right;
        }
    }
    
    //Don't you go dereferencing nullptr buddy
    if(temp == nullptr){return false;}
    
    //Remove all dead if all flag is true
    if(all){
        temp->removeDead();
        return true;
    }else{
        //Remove just the tiger with the id otherwise
        //Find out if the tiger exists in the streak
        if(temp->findTiger(tiger)){
            temp->remove(tiger);
            return true;
        }else{
            return false;
        }
    }
}

int Grid::getGridHeight(int grid){
    
    //If grid empty return -1
    if(m_root == nullptr){
        return -1;
    }
    
    //pointer initialized
    Streak* temp = m_root;
    
    //Search the grid for the streak
    while(temp != nullptr && temp->m_gridID != grid){
        if(temp->m_gridID < grid){
            temp = temp->m_left;
        }else{
            temp = temp->m_right;
        }
    }
    
    //Don't you go dereferencing nullptr buddy
    if(temp == nullptr){return -1;}
    
    return temp->m_treeHeight;
    
}

bool Grid::setState(int grid, int tiger, STATE state){
    //If grid empty return false
    if(m_root == nullptr){
        return false;
    }
    
    //pointer initialized
    Streak* temp = m_root;
    
    //Search the grid for the streak
    while(temp != nullptr && temp->m_gridID != grid){
        if(temp->m_gridID < grid){
            temp = temp->m_right;
        }else{
            temp = temp->m_left;
        }
    }
    
    //Don't you go dereferencing nullptr buddy
    if(temp == nullptr){return false;}
    
    //Splay the tree
    splay(temp);
    
    //Change the state and return the boolean
    return temp->setState(tiger, state);
    
}

void Grid::dump(bool verbose) const{
    dumpHelper(m_root, verbose);
    cout << endl;
}

void Grid::dumpHelper(Streak* root, bool verbose) const{
    if (root != nullptr){
        {
            cout << "(";
            dumpHelper( root->m_left, verbose );
            if (verbose)
                cout << root->m_gridID << ":" << root->m_root->m_id;
            else
                cout << root->m_gridID;
            dumpHelper( root->m_right, verbose );
            cout << ")";
        }
    }
}
