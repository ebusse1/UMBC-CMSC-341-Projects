// UMBC - CMSC 341 - Spring 2026 - Proj2
#include "streak.h"
#include <vector>
#include <random>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{ // Tester class to implement test functions
    
    public:
    
    //This function of test class generates a streak object with random tigers of the requested size
    Streak generateStreak(int size){
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = size;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            if (i == streakSize / 2) tempID = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }
        return streak;
    }
    
    //This function tests that the AVL tree is still balanced after 100 insertions
    bool testAvlBalancedNormal(){
        
        //Create a streak
        Streak streak = generateStreak(100);
        
        //Check that it is balanced via recursion
        return checkBalanced(streak.m_root);
    }
    
    //This function tests that the AVL tree is still balanced after 1 insertions
    bool testAvlBalancedEdge(){
        
        //Create a streak
        Streak streak = generateStreak(1);
        
        //Check that it is balanced via recursion
        return checkBalanced(streak.m_root);
    }
    
    //Recursive helper function for textAvlBalanced
    bool checkBalanced(Tiger* r){
        
        if(r == nullptr){return true;}
        
        int leftHeight = testHeight(r->getLeft());
        int rightHeight = testHeight(r->getRight());
        
        if(leftHeight - rightHeight > 1 || leftHeight - rightHeight < -1){
            return false;
        }
        
        if(leftHeight > rightHeight){
            if(r->getHeight() != 1 + leftHeight){
                return false;
            }
        }else{
            if(r->getHeight() != 1 + rightHeight){
                return false;
            }
        }
        
        return checkBalanced(r->getLeft()) && checkBalanced(r->getRight());
    }
    
    //This function computes the height of a node in an AVL tree for more accurate testing
    int testHeight(Tiger* r){
        
        //if empty or at the end return -1
        if(r == nullptr){return -1;}
        
        //Recursive check all subtrees
        int leftHeight = testHeight(r->getLeft());
        int rightHeight = testHeight(r->getRight());
        
        if(leftHeight > rightHeight){
            return 1 + leftHeight;
        }else{
            return 1 + rightHeight;
        }
    }
    
    //This function tests that the BST property is preserved after 100 insertions
    bool testBSTNormal(){
        //Create a streak and insert 100 tigers
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = 100;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            if (i == streakSize / 2) tempID = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
            //After each insertion check that BST property remains
            if(!checkBST(streak.m_root)){
                return false;
            }
        }
        return true;
    }
    
    //This is a helper funtion that checks the tree maintains BST property
    bool checkBST(Tiger* r){
        return checkBSTHelper(r, MINID -1, MAXID +1);
    }
    
    //This is a helper function for the helper function to allow for recursive search
    bool checkBSTHelper(Tiger * r, int min, int max){
        //Return true if reach end of list
        if(r == nullptr){return true;}
        
        //If id of of right is smaller than current, or if id of left is larger than current, return false
        if((r->m_id < min) || (r->m_id > max)){return false;}
        
        //Recursive call if passed
        return (checkBSTHelper(r->m_left, min, r->m_id) && checkBSTHelper(r->m_right, r->m_id, max));
    }
    
    //This function tests that the splay tree continues to maintain splay and BST properties after many insertions
    bool testSplayInsertion(){
        //Create a grid and insert tigers
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Grid aGrid;
        for (int i=0; i<50; i++){
            Tiger tigers[10];
            int streakSize = 1;
            //int ID = 0;
            for(int i=0;i<streakSize;i++){
                Tiger tiger(idGen.getRandNum(),
                            static_cast<AGE>(ageGen.getRandNum()),
                            static_cast<GENDER>(genderGen.getRandNum()));
                tigers[i] = tiger;
            }
            //Save the id of the most recent insertion for checking
            int id = idGen.getRandNum();
            
            //Insert the tigers into the grid
            aGrid.insert(id, tigers, 1);
            
            //Check that the most recent ID is now the root
            if(aGrid.m_root->m_gridID != id){return false;}
            
            //Check that the BST property is preserved after each insertion
            if(!checkBST(aGrid.m_root)){
                return false;
            }
        }
        
        return true;
    }
    
    //This is a helper funtion that checks the tree maintains BST property for the grid
    bool checkBST(Streak* r){
        return checkBSTHelper(r, MINID -1, MAXID +1);
    }
    
    //This is a helper function for the helper function to allow for recursive search for the grid
    bool checkBSTHelper(Streak * r, int min, int max){
        //Return true if reach end of list
        if(r == nullptr){return true;}
        
        //If id of of right is smaller than current, or if id of left is larger than current, return false
        if((r->m_gridID < min) || (r->m_gridID > max)){return false;}
        
        //Recursive call if passed
        return (checkBSTHelper(r->m_left, min, r->m_gridID) && checkBSTHelper(r->m_right, r->m_gridID, max));
    }
    
    //This function tests where the AVL tree is balanced after multiple removals
    bool testAvlRemovalsBalance(){
        //Create a streak with 100 elements
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = 100;
        int ID = 0;
        int IDsToDelete[100];
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            //Store half of the IDs in an array to delete afterwards
            if (i % 2 == 0) IDsToDelete[i] = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }
        
        //Delete half of the streak
        for(int i = 0; i < 100; i += 2){
            streak.remove(IDsToDelete[i]);
        }
        
        //Now check that balance is preserved
        if(!checkBalanced(streak.m_root)){
            return false;
        }
         
        return true;
    }
    
    //This function tests where the AVL tree is balanced after multiple removals
    bool testAvlRemovalsBST(){
        //Create a streak with 100 elements
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = 100;
        int ID = 0;
        int IDsToDelete[100];
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            //Store half of the IDs in an array to delete afterwards
            if (i % 2 == 0) IDsToDelete[i] = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }
        
        //Delete half of the streak
        for(int i = 0; i < 100; i += 2){
            streak.remove(IDsToDelete[i]);
        }
        
        //Now check that BST is preserved
        if(!checkBST(streak.m_root)){
            return false;
        }
         
        return true;
    }
    
    //THis function tests whether remove dead functions for 50 removals from a streak, and that BST and AVL are preserved
    bool testRemoveDead(){
        
        //Create a streak with 100 elements
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = 100;
        int ID = 0;
        int IDsToDelete[100];
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            //Store half of the IDs in an array to delete afterwards
            if (i % 2 == 0) IDsToDelete[i] = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }
        
        //Set half of the tigers to dead
        for(int i = 0; i < 100; i += 2){
            streak.setState(IDsToDelete[i], DEAD);
        }
        
        //Call remove dead
        streak.removeDead();
        
        //Check that all tigers assigned dead were removed
        for(int i = 0; i < 100; i += 2){
            if(streak.findTiger(IDsToDelete[i])){return false;}
        }
        
        //Now check that BST is preserved
        if(!checkBST(streak.m_root)){
            return false;
        }
        
        //Now check that balance is preserved
        if(!checkBalanced(streak.m_root)){
            return false;
        }
         
        return true;
    }
    
    //THis function tests that find tiger functions for 50 searches, and that AVL and BST are preserved
    bool testFindTiger(){
        //Create a streak with 100 elements
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Streak streak;
        int streakSize = 100;
        int ID = 0;
        int IDsToDelete[100];
        for(int i=0;i<streakSize;i++){
            ID = idGen.getRandNum();
            //Store half of the IDs in an array to delete afterwards
            if (i % 2 == 0) IDsToDelete[i] = ID;//we store this ID for later use
            Tiger tiger(ID,
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()));
            streak.insert(tiger);
        }
        
        //Call find tiger on half of the tigers
        for(int i = 0; i < 100; i += 2){
            if(!streak.findTiger(IDsToDelete[i])){return false;}
        }
        
        
        //Now check that BST is preserved
        if(!checkBST(streak.m_root)){
            return false;
        }
        
        //Now check that balance is preserved
        if(!checkBalanced(streak.m_root)){
            return false;
        }
         
        return true;
        
    }
    
    //This function tests that Grid::count functions properly and that splay and BST are preserved
    bool testGridCount(){
        //Create a grid and insert tigers
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Random stateGen(0,1);
        Grid aGrid;
        
        //Add set of 50 tigers with known values for testing
        Tiger tigers2[50];
        for(int i=0;i<50;i++){
            Tiger tiger(idGen.getRandNum(),
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()),
                        static_cast<STATE>(i % 2 == 0 ? DEAD : ALIVE));
                        tigers2[i] = tiger;
        }
        
        //While loop for case of random duplicate ID
        int finalID = idGen.getRandNum();
        
        while(aGrid.findStreak(finalID)){
            finalID = idGen.getRandNum();
        }
        
        //Insert the final tiger streak
        aGrid.insert(finalID, tigers2, 50);
        
        //Now add more streaks to test splay
        for (int i=0; i<50; i++){
            Tiger tigers[10];
            int streakSize = 10;
            //int ID = 0;
            for(int i=0;i<streakSize;i++){
                Tiger tiger(idGen.getRandNum(),
                            static_cast<AGE>(ageGen.getRandNum()),
                            static_cast<GENDER>(genderGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
                            tigers[i] = tiger;
            }
            
            //Insert the tigers into the grid
            aGrid.insert(idGen.getRandNum(), tigers, 10);
        }
        
        //Count the number of dead, should be 25.
        int result = aGrid.count(finalID, DEAD);
        
        //Test result is valid
        if(result != 25){return false;}
        
        //Test splay occured
        if(aGrid.m_root->m_gridID != finalID){return false;}
        
        //Test BST preserved
        if(!checkBST(aGrid.m_root)){return false;}
      
        return true;
        
    }
    
    //This function tests that Grid::setState functions properly and that splay and BST are preserved
    bool testGridSetState(){
        //Create a grid and insert tigers
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Random stateGen(0,1);
        Grid aGrid;
        
        //Add set of 50 tigers with known values for testing
        Tiger tigers2[50];
        for(int i=0;i<50;i++){
            Tiger tiger(idGen.getRandNum(),
                        static_cast<AGE>(ageGen.getRandNum()),
                        static_cast<GENDER>(genderGen.getRandNum()),
                        static_cast<STATE>(i % 2 == 0 ? DEAD : ALIVE));
                        tigers2[i] = tiger;
        }
        
        //While loop for case of random duplicate ID
        int finalID = idGen.getRandNum();
        
        while(aGrid.findStreak(finalID)){
            finalID = idGen.getRandNum();
        }
        
        //Insert the final tiger streak
        aGrid.insert(finalID, tigers2, 50);
        
        //Now add more streaks to test splay
        for (int i=0; i<50; i++){
            Tiger tigers[10];
            int streakSize = 10;
            //int ID = 0;
            for(int i=0;i<streakSize;i++){
                Tiger tiger(idGen.getRandNum(),
                            static_cast<AGE>(ageGen.getRandNum()),
                            static_cast<GENDER>(genderGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
                            tigers[i] = tiger;
            }
            
            //Insert the tigers into the grid
            aGrid.insert(idGen.getRandNum(), tigers, 10);
        }
        
        //Change the state of one of the tigers that is dead to alive, we set state of all even index tigers in the array so select even number
        int tigerToChange = 40;
        
        //Check that set state works.
        if(!aGrid.setState(finalID, tigers2[tigerToChange].m_id, ALIVE)){return false;}
        
        //Check that splay occured
        if(aGrid.m_root->m_gridID != finalID){return false;}
        
        return true;
    }
    
    //This function tests that the grid preserves BST after 50 splay operations
    bool testSplayBST(){
        //Create a grid and insert tigers
        Random idGen(MINID,MAXID);
        Random ageGen(0,2);
        Random genderGen(0,2);
        Grid aGrid;
        int IDS[50];
        for (int i=0; i<50; i++){
            Tiger tigers[10];
            int streakSize = 1;
            //int ID = 0;
            for(int i=0;i<streakSize;i++){
                Tiger tiger(idGen.getRandNum(),
                            static_cast<AGE>(ageGen.getRandNum()),
                            static_cast<GENDER>(genderGen.getRandNum()));
                tigers[i] = tiger;
            }
            //Save the id of the most recent insertion for checking
            IDS[i] = idGen.getRandNum();
            
            //Insert the tigers into the grid
            aGrid.insert(IDS[i], tigers, 1);
            
            //Check that the BST property is preserved after each insertion
            if(!checkBST(aGrid.m_root)){
                return false;
            }
        }
        
        //Call count on every streak in the grid
        for(int i = 0; i < 50; i++){
            aGrid.count(IDS[i], DEAD);
        }
        
        //Check that the BST property is preserved after all of the splays
        if(!checkBST(aGrid.m_root)){
            return false;
        }
        
        return true;
    }
    
};
int main(){
   
    Tester test;
    
    if(test.testAvlBalancedNormal()){
        cout << "testAvlBalancedNormal has passed " << endl;
    }else{
        cout << "testAvlBalancedNormal has failed " << endl;
    }
    
    if(test.testAvlBalancedEdge()){
        cout << "testAvlBalancedEdge has passed " << endl;
    }else{
        cout << "testAvlBalancedEdge has failed " << endl;
    }
    
    if(test.testBSTNormal()){
        cout << "testBSTNormal has passed " << endl;
    }else{
        cout << "testBSTNormal has failed " << endl;
    }
    
    if(test.testSplayInsertion()){
        cout << "testSplayInsertion has passed " << endl;
    }else{
        cout << "testSplayInsertion has failed " << endl;
    }
    
    if(test.testAvlRemovalsBalance()){
        cout << "testAvlRemovalsBalance has passed " << endl;
    }else{
        cout << "testAvlRemovalsBalance has failed " << endl;
    }
    
    if(test.testAvlRemovalsBST()){
        cout << "testAvlRemovalsBST has passed " << endl;
    }else{
        cout << "testAvlRemovalsBST has failed " << endl;
    }
    
    if(test.testRemoveDead()){
        cout << "testRemoveDead has passed " << endl;
    }else{
        cout << "testRemoveDead has failed " << endl;
    }
    
    if(test.testFindTiger()){
        cout << "testFindTiger has passed " << endl;
    }else{
        cout << "testFindTiger has failed " << endl;
    }
    
    if(test.testGridCount()){
        cout << "testGridCount has passed " << endl;
    }else{
        cout << "testGridCount has failed " << endl;
    }
    
    if(test.testGridSetState()){
        cout << "testGridSetState has passed " << endl;
    }else{
        cout << "testGridSetState has failed " << endl;
    }
    
    if(test.testSplayBST()){
        cout << "testSplayBST has passed " << endl;
    }else{
        cout << "testSplayBST has failed " << endl;
    }
    
    return 0;
}
