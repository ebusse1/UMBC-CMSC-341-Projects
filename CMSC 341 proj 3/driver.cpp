// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_unidist = uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_uniReal = uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = mt19937(10);// 10 is the fixed seed value
        m_unidist = uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
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
        result = floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    random_device m_device;
    mt19937 m_generator;
    normal_distribution<> m_normdist;//normal distribution
    uniform_int_distribution<> m_unidist;//integer uniform distribution
    uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

// the followings are sample priority functions to be used by Shop class
// Priority functions compute an integer priority for a order.
int priorityFn1(const Order &order);// works with a MAXHEAP
int priorityFn2(const Order &order);// works with a MINHEAP

class Tester{
    public:
    
    //The function tests the normal insertion case for a skew minheap
    bool insertMinNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn2, MINHEAP, SKEW);
        
        //Check with helper function to verify heap property
        return checkHeap(aShop);
    };
    
    //The function tests the normal insertion case for a skew maxheap
    bool insertMaxNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        //Check with helper function to verify heap property
        return checkHeap(aShop);
    };
    
    //The function tests the normal insertion case for a leftist minheap
    bool insertMinNormalLeftist(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn2, MINHEAP, LEFTIST);
        
        //Check with helper function to verify heap property
        if(checkHeap(aShop) && checkNPL(aShop) && checkLeftist(aShop)){
            return true;
        }else{
            return false;
        }
    };
    
    //The function tests the normal insertion case for a leftist maxheap
    bool insertMaxNormalLeftist(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, LEFTIST);
        
        //Check with helper function to verify heap property
        if(checkHeap(aShop) && checkNPL(aShop) && checkLeftist(aShop)){
            return true;
        }else{
            return false;
        }
    };
    
    //This tests the normal case of removing nodes in the correct order from a minheap
    bool minRemovalNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn2, MINHEAP, SKEW);
        
        return checkRemovals(aShop);
    };
    
    //This tests the normal case of removing nodes in the correct order from a maxheap
    bool maxRemovalNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        return checkRemovals(aShop);
    };
    
    //This function checks for correct NPL values in leftist heap
    bool checkNPLNormal(){
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, LEFTIST);
        return checkNPL(aShop);
    };
    
    //This function checks for leftistness
    bool checkLeftistNormal(){
        Shop aShop = makeShop(300, priorityFn2, MINHEAP, LEFTIST);
        if(checkNPL(aShop) && checkLeftist(aShop)){
            return true;
        }else{
            return false;
        }
    };
    
    //This function checks that a shop maintains heap properties and all correct orders upon priority funciton change
    bool checkPrioChange(){
        
        //Make our orders and load a copy of each order into an array
        //All the randomness
        Random shopGen(10,30); // this generates same priority numbers too
        Random orderIDGen(MINORDERID,MAXORDERID);
        Random countGen(ONE, DOZEN);
        Random customerIDGen(MINCUSTID,MAXCUSTID);
        Random membershipGen(TIER1,TIER6);
        Random pointsGen(MINPOINTS,MAXPOINTS);
        Random itemGen(COFFEE,ICEDTEA);
        Random rndShopID(SHOPMINID, SHOPMAXID);
        
        //Give shop random properties
        int rndShop = shopGen.getRandNum();
        int shopID = rndShopID.getRandNum();
        
        //Number of orders being tests
        int orders = 300;
        
        Order orderArray[orders];
        
        // create a Shop object
        Shop aShop(priorityFn2, MINHEAP, SKEW, rndShop, shopID);
        
        //Load up with orders
        for (int i=0;i<orders;i++){
            // create multiple Order objects
            Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                          static_cast<COUNT>(countGen.getRandNum()),
                          static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                          pointsGen.getRandNum(),
                          customerIDGen.getRandNum(),
                          orderIDGen.getRandNum());
            
            //Copy the order and load it into the array
            Order copyOrder = anOrder;
            orderArray[i] = copyOrder;
            
            // insert orders into shop
            aShop.insertOrder(anOrder);
        }
        
        //Change the priority function
        aShop.setPriorityFn(priorityFn1, MAXHEAP);
        
        //Now test that heap properties are viable
        if(!checkHeap(aShop)){
            return false;
        }
        
        //Now check all orders still exist and are correct
        bool found = false;
        
        for(int i = 0; i<orders; i++){
            //Dequeue top order
            Order checkOrder = aShop.getNextOrder();
            
            int checked = 0;
            while(checked < orders){
                if(compareOrders(checkOrder, orderArray[checked])){
                    found = true;
                    break;
                }
                else{
                    found = false;
                }
                checked++;
            }
        }
        
        return found;
    };
    
    //This function compares two orders to see if they have the same values
    bool compareOrders(Order &order1, Order &order2){
        
        if(order1.m_item != order2.m_item){return false;}
        if(order1.m_count != order2.m_count){return false;}
        if(order1.m_membership != order2.m_membership){return false;}
        if(order1.m_points != order2.m_points){return false;}
        if(order1.m_customerID != order2.m_customerID){return false;}
        if(order1.m_orderID != order2.m_orderID){return false;}
        
        return true;
    };
    
    //This function tests the edge case of merging an empty queue with a full one
    bool mergeEdge(){
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        Shop aShopEmpty = makeShop(0, priorityFn1, MAXHEAP, SKEW);
        
        aShop.mergeWithQueue(aShopEmpty);
        
        if(checkHeap(aShop) && checkRemovals(aShop)){
            return true;
        }else{
            return false;
        }
    }
    
    //This function tests the normal case of the copy constructor
    bool copyNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        //Make a shop using copy constructor
        Shop aShopCopy(aShop);
        
        while(true){
            try {
                Order a = aShop.getNextOrder();
                Order b = aShopCopy.getNextOrder();

                if(!compareOrders(a, b)) return false;
            } catch (const out_of_range& e) {
                return true;
            }
        }
        return true;
    };
    
    //This function tests the edge case of copying an empty shop
    bool copyEdge(){
        //Make an empty shop
        Shop aShop = makeShop(0, priorityFn1, MAXHEAP, SKEW);
        
        //Make a shop using copy constructor
        Shop aShopCopy(aShop);
        
        //Check that both shops are empty
        if(aShop.m_heap != nullptr || aShopCopy.m_heap != nullptr){
            return false;
        }
        
        //Check that shops have same priority function
        if(aShop.m_priorFunc != aShopCopy.m_priorFunc){
            return false;
        }
        
        //Check that shops have same structure
        if(aShop.m_structure != aShopCopy.m_structure){
            return false;
        }
        
        return true;
    };
    
    //This function tests the normal case of the copy constructor
    bool assignNormal(){
        //Make a shop
        Shop aShop = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        Shop aShopCopy = makeShop(300, priorityFn1, MAXHEAP, SKEW);

        aShopCopy = aShop;
        
        while(true){
            try {
                Order a = aShop.getNextOrder();
                Order b = aShopCopy.getNextOrder();

                if(!compareOrders(a, b)) return false;
            } catch (const out_of_range& e) {
                return true;
            }
        }
        return true;
    };
    
    //This function tests the edge case of copying an empty shop with assignment operator
    bool assignEdge(){
        //Make an empty shop
        Shop aShop = makeShop(0, priorityFn1, MAXHEAP, SKEW);
        
        Shop aShopCopy = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        //Make a shop using assignment operator
        aShopCopy = aShop;
        
        //Check that both shops are empty
        if(aShop.m_heap != nullptr || aShopCopy.m_heap != nullptr){
            return false;
        }
        
        //Check that shops have same priority function
        if(aShop.m_priorFunc != aShopCopy.m_priorFunc){
            return false;
        }
        
        //Check that shops have same structure
        if(aShop.m_structure != aShopCopy.m_structure){
            return false;
        }
        
        return true;
    };
    
    bool dequeueException(){
        //Make empty shop
        Shop aShop = makeShop(0, priorityFn2, MINHEAP, SKEW);
        
        //Attempt to dequeue
        try {
            aShop.getNextOrder();
        } catch (const out_of_range& e) {
            return true;
        }
        
        return false;
    }
    
    bool domainError(){
        //Make two shops with different prio functions
        Shop aShop = makeShop(300, priorityFn2, MINHEAP, SKEW);
        Shop aShop2 = makeShop(300, priorityFn1, MAXHEAP, SKEW);
        
        //Attempt to merge them
        try {
            aShop.mergeWithQueue(aShop2);
        } catch (const domain_error& e) {
            return true;
        }
        
        return false;
    }
    
    bool checkRemovals(Shop &aShop){
        //Holds priority of previously dequeued order
        int previousPrio = 0;
        
        int nodesRemoved = 0;
        
        prifn_t prioFn = aShop.getPriorityFn();
        
        //Check if min or max heap
        if(aShop.m_heapType == MAXHEAP){
            previousPrio = 5050;
            while(true){
                try {
                    Order newOrder = aShop.getNextOrder();
                    if(prioFn(newOrder) > previousPrio){
                        return false;
                    }else{
                        previousPrio = prioFn(newOrder);
                        nodesRemoved ++;
                    }
                } catch (const out_of_range& e) {
                    if(nodesRemoved == 300){
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        
        if(aShop.m_heapType == MINHEAP){
            previousPrio = 0;
            while(true){
                try {
                    Order newOrder = aShop.getNextOrder();
                    if(prioFn(newOrder) < previousPrio){
                        return false;
                    }else{
                        previousPrio = prioFn(newOrder);
                        nodesRemoved ++;
                    }
                } catch (const out_of_range& e) {
                    if(nodesRemoved == 300){
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        
        return true;
    }
    
    Shop makeShop(int orders, prifn_t priFn, HEAPTYPE heapType,
                  STRUCTURE structure){
        
        //All the randomness
        Random shopGen(10,30); // this generates same priority numbers too
        Random orderIDGen(MINORDERID,MAXORDERID);
        Random countGen(ONE, DOZEN);
        Random customerIDGen(MINCUSTID,MAXCUSTID);
        Random membershipGen(TIER1,TIER6);
        Random pointsGen(MINPOINTS,MAXPOINTS);
        Random itemGen(COFFEE,ICEDTEA);
        Random rndShopID(SHOPMINID, SHOPMAXID);
        
        //Give shop random properties
        int rndShop = shopGen.getRandNum();
        int shopID = rndShopID.getRandNum();
        
        // create a Shop object
        Shop aShop(priFn, heapType, structure, rndShop, shopID);
        
        //Load up with orders
        for (int i=0;i<orders;i++){
            // create multiple Order objects
            Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
                          static_cast<COUNT>(countGen.getRandNum()),
                          static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                          pointsGen.getRandNum(),
                          customerIDGen.getRandNum(),
                          orderIDGen.getRandNum());
            // insert orders into shop
            aShop.insertOrder(anOrder);
        }
        
        return aShop;
    };
    
    //This function checks that a shop has maintained it's heap property
    bool checkHeap(Shop& aShop){
        
        HEAPTYPE heaptype = aShop.getHeapType();
    
        if(heaptype == MAXHEAP){
            return isMaxHeap(aShop.m_heap, aShop.getPriorityFn());
        }
        if(heaptype == MINHEAP){
            return isMinHeap(aShop.m_heap, aShop.getPriorityFn());
        }

        return true;
    };
    
    //Recursive checker for max heap
    bool isMaxHeap(Order* node, prifn_t priFn){
        if (node == nullptr) {
                return true;
            }

            if (node->m_left != nullptr && priFn(*node) < priFn(*node->m_left)) {
                return false;
            }

            if (node->m_right != nullptr && priFn(*node) < priFn(*node->m_right)) {
                return false;
            }

            return isMaxHeap(node->m_left, priFn) && isMaxHeap(node->m_right, priFn);
    };
    
    //Recursive checker for min heap
    bool isMinHeap(Order* node, prifn_t priFn){
        if (node == nullptr) {
                return true;
            }

            if (node->m_left != nullptr && priFn(*node) > priFn(*node->m_left)) {
                return false;
            }

            if (node->m_right != nullptr && priFn(*node) > priFn(*node->m_right)) {
                return false;
            }

            return isMinHeap(node->m_left, priFn) && isMinHeap(node->m_right, priFn);
    };
    
    //This function checks that a leftist heap has correct NPL values
    bool checkNPL(Shop &aShop){
        bool correct = true;
        calcNPL(aShop.m_heap, correct);
        return correct;
    };
    
    
    //Recursive helper function to calculate NPL values
    int calcNPL(Order* node, bool& correct){
        if(node == nullptr){
            return -1;
        }
        
        int leftNPL = calcNPL(node->m_left, correct);
        int rightNPL = calcNPL(node->m_right, correct);
        
        //Assume left is smaller
        int min = leftNPL;
        
        //No? Swap it
        if(leftNPL > rightNPL){
            min = rightNPL;
        }
        
        int expected = 1 + min;
        
        if(node->m_npl != expected){
            correct = false;
        }
        
        return expected;
        
    };
    
    //This function verifies tree abides by leftist properties (assumes NPLS are correct, so run calcNPL first to confirm they are)
    bool checkLeftist(Shop &aShop){
        return checkLeftistHelper(aShop.m_heap);
    };
    
    //Recursive helper for checkLeftist
    bool checkLeftistHelper(Order* node){
        if(node == nullptr){return true;}
        
        if (getNPLHelper(node->m_left) < getNPLHelper(node->m_right)) {
            return false;
        }

        return checkLeftistHelper(node->m_left) && checkLeftistHelper(node->m_right);
    };
    
    //NPL value helper to avoid dereferencing nullptr
    int getNPLHelper(Order* node){
        if(node == nullptr){
            return -1;
        }else{
            return node->m_npl;
        }
    };
    
    
    
    private:
};

int main(){
    
    Tester test;
    
    //Test insertion for a normal case of min-heap.
    if(test.insertMinNormal()){
        cout << "insertMinNormal has passed" << endl;
    }else{
        cout << "insertMinNormal has failed" << endl;
    }
    //Test insertion for a normal case of max-heap.
    if(test.insertMaxNormal()){
        cout << "insertMaxNormal has passed" << endl;
    }else{
        cout << "insertMaxNormal has failed" << endl;
    }
    
    //Test removal for a normal case of min-heap.
    if(test.minRemovalNormal()){
        cout << "minRemovalNormal has passed" << endl;
    }else{
        cout << "minRemovalNormal has failed" << endl;
    }
    
    //Test removal for a normal case of max-heap
    if(test.maxRemovalNormal()){
        cout << "maxRemovalNormal has passed" << endl;
    }else{
        cout << "maxRemovalNormal has failed" << endl;
    }
    
    //Test all nodes in a leftist heap have the correct NPL value.
    if(test.checkNPLNormal()){
        cout << "checkNPLNormal has passed" << endl;
    }else{
        cout << "checkNPLNormal has failed" << endl;
    }
    
    //Test a leftist heap preserves the property of such a heap
    if(test.checkLeftistNormal()){
        cout << "checkLeftistNormal has passed" << endl;
    }else{
        cout << "checkLeftistNormal has failed" << endl;
    }
    
    //Test whether after changing the priority function a correct heap is rebuilt
    if(test.checkPrioChange()){
        cout << "checkPrioChange has passed" << endl;
    }else{
        cout << "checkPrioCHange has failed" << endl;
    }
    
    //Test merge of an empty queue (an edge case) with a normal queue.
    if(test.mergeEdge()){
        cout << "mergeEdge has passed" << endl;
    }else{
        cout << "mergeEdge has failed" << endl;
    }
    
    //Test the Shop class copy constructor for a normal case.
    if(test.copyNormal()){
        cout << "copyNormal has passed" << endl;
    }else{
        cout << "copyNormal has failed" << endl;
    }
    
    //Test the Shop class copy constructor for an edge case.
    if(test.copyEdge()){
        cout << "copyEdge has passed" << endl;
    }else{
        cout << "copyEdge has failed" << endl;
    }
    
    //Test the Shop class assignment operator for an edge case.
    if(test.assignEdge()){
        cout << "assignEdge has passed" << endl;
    }else{
        cout << "assignEdge has failed" << endl;
    }
    
    //Test the Shop class copy constructor for an edge case.
    if(test.assignNormal()){
        cout << "assignNormal has passed" << endl;
    }else{
        cout << "assignNormal has failed" << endl;
    }
    
    //Test that attempting to dequeue an empty queue throws an out_of_range exception.
    if(test.dequeueException()){
        cout << "dequeueException has passed" << endl;
    }else{
        cout << "dequeueException has failed" << endl;
    }
    
    //Test that attempting to merge queues with different priority functions throws a domain_error exception.
    if(test.domainError()){
        cout << "domainError has passed" << endl;
    }else{
        cout << "domainError has failed" << endl;
    }
    
    
    return 0;
}

int priorityFn1(const Order &order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-5003]
    //the highest priority would be 3+5000 = 5003
    //the lowest priority would be 0+0 = 0 => 1
    //the larger value means the higher priority
    int priority = static_cast<int>(order.getCount()) + order.getPoints();
    if (priority == 0 ) priority = 1;
    return priority;
}

int priorityFn2(const Order &order) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //the highest priority would be 0+0 = 0 => 1
    //the lowest priority would be 5+5 =10
    //the smaller value means the higher priority
    int priority = static_cast<int>(order.getItem()) + static_cast<int>(order.getMemebership());
    if (priority == 0 ) priority = 1;
    return priority;
}
