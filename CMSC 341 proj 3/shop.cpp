// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"
Shop::Shop(){ // empty object
  
    m_shopID = 0;
    m_heap = nullptr;
    m_size = 0;
    
}

Shop::Shop(prifn_t priFn, HEAPTYPE heapType,
          STRUCTURE structure, int regPrior,
          int id)
{
    //Initialize all private member variables
    m_shopID = id;
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
    m_regPrior = regPrior;
}

Shop::~Shop()
{
    
    clear();
  
}

void Shop::clear() {
    
    clearHelper(m_heap);
    
    //Initialize private member variables
    m_shopID = 0;
    m_heap = nullptr;
    m_size = 0;
    
}

void Shop::clearHelper(Order* node){
    
    if(node == nullptr){return;}
    
    clearHelper(node->m_left);
    
    clearHelper(node->m_right);
    
    delete node;
    
}

Shop::Shop(const Shop& rhs)
{
    //No self assignment on my watch
    if(&rhs == this){
        return;
    }
    
    //Copy all the member variables (except size and heap pointer)
    m_shopID = rhs.m_shopID;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_regPrior = rhs.m_regPrior;
    m_size = rhs.m_size;
    
    //Iterate through rhs, copy and add to current heap
    m_heap = copyHelper(rhs.m_heap);
    
}

Order* Shop::copyHelper(Order* node){
    
    if(node == nullptr){return nullptr;}
    
    Order* newOrder = new Order(node->m_item, node->m_count,
                                 node->m_membership, node->m_points,
                                 node->m_customerID, node->m_orderID);
    
    //Make sure to copy NPL
    newOrder->m_npl = node->m_npl;
    
    newOrder->m_left = copyHelper(node->m_left);
    newOrder->m_right = copyHelper(node->m_right);
    
    return newOrder;
    
}

Shop& Shop::operator=(const Shop& rhs) {
    //Aint nobody gonna self assign ovah here yo
    if(&rhs == this){return *this;}
    //If this object has shtuff, it needs to not
    clear(); //bye //Copy all the member variables (except size and heap pointer)
    m_shopID = rhs.m_shopID;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_regPrior = rhs.m_regPrior;
    m_size = rhs.m_size;
    //Iterate through rhs, copy and add to current heap
    m_heap = copyHelper(rhs.m_heap);
    return *this;
}


void Shop::mergeWithQueue(Shop& rhs) {
    
    //Check to make sure heaps are compatible
    if (this == &rhs) {
        return;
    }
    if(rhs.m_priorFunc != m_priorFunc){
        throw domain_error("Incompatible priority functions");
    }
    else if(rhs.m_structure != m_structure){
        throw domain_error("Incompatible structures");
    }
    else if(rhs.m_heapType != m_heapType){
        throw domain_error("Incompatible heap type");
    }
    else{
        m_heap = mergeHelper(m_heap, rhs.m_heap);
        m_size += rhs.m_size;
        rhs.m_heap = nullptr;
        rhs.m_size = 0;
    }
}

Order* Shop::mergeHelper(Order* p1, Order* p2){
    if(p1 == nullptr){
        return p2;
    }
    else if(p2 == nullptr){
        return p1;
    }
    
    //This part depends on MIN or MAX heap
    if(m_heapType == MAXHEAP){ //MAXHEAP CASE
        if(m_priorFunc(*p1) < m_priorFunc(*p2)){
            //Swap p1 and p2
            Order* temp = p1;
            p1 = p2;
            p2 = temp;
        }
    }
    else if(m_heapType == MINHEAP){ //MINHEAP CASE
        if(m_priorFunc(*p1) > m_priorFunc(*p2)){
            //Swap p1 and p2
            Order* temp = p1;
            p1 = p2;
            p2 = temp;
        }
    }
    else{
        cout << "no struct" << endl;
    }
    
    //This part depends on SKEW vs LEFTIST heap
    if (m_structure == SKEW) {
           Order* temp = p1->m_left;
           p1->m_left = p1->m_right;
           p1->m_right = temp;

           p1->m_left = mergeHelper(p1->m_left, p2);
       }
   else if (m_structure == LEFTIST) {
       p1->m_right = mergeHelper(p1->m_right, p2);

       //if left NPL is less than right NPL, swap children
       if(getNPL(p1->m_left) < getNPL(p1->m_right)){
           Order* temp = p1->m_left;
           p1->m_left = p1->m_right;
           p1->m_right = temp;
       }
       
       p1->m_npl = getNPL(p1->m_right) + 1;
   }
    
    return p1;
    
}

int Shop::getNPL(Order* node) {
    if (node == nullptr) return -1;
    return node->m_npl;
}

bool Shop::insertOrder(const Order& order) {
    
    //Wait for TA response to figure out this bit
    if(m_priorFunc(order) == 0){
        return false;
    }
    
    Order* newOrder = new Order(order);
    newOrder->m_npl = 0;
    newOrder->m_left = nullptr;
    newOrder->m_right = nullptr;
    m_heap = mergeHelper(m_heap, newOrder);
    m_size++;
    return true;
 
}

int Shop::numOrders() const
{
    return m_size;
}

prifn_t Shop::getPriorityFn() const {
    return m_priorFunc;
}

Order Shop::getNextOrder() {
    if(m_heap == nullptr){
        throw out_of_range("Heap is empty");
    }else{
        //Remove the root and return it
        Order* root = m_heap;
        Order* leftSub = m_heap->m_left;
        Order* rightSub = m_heap->m_right;
        
        root->m_left = nullptr;
        root->m_right = nullptr;

        Order result = *root;
        
        m_heap = mergeHelper(leftSub, rightSub);

        delete root;
        m_size--;

        return result;
    }
}

void Shop::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    
    //Set new prior function and heap type
    m_priorFunc = priFn;
    m_heapType = heapType;
    
    //Make new pointer to current data to free m_heap
    Order* oldHeap = m_heap;
    m_heap = nullptr;
    
    //Now rebuild the heap
    rebuildHeap(oldHeap);
    
}

void Shop::rebuildHeap(Order* node){
    //Return if reached end
    if (node == nullptr) return;

    //Store left and right nodess
    Order* left = node->m_left;
    Order* right = node->m_right;

    //Detach le node
    node->m_left = nullptr;
    node->m_right = nullptr;
    node->m_npl = 0;

    //Merge single node in
    m_heap = mergeHelper(m_heap, node);

    //Recursive call to traverse tree
    rebuildHeap(left);
    rebuildHeap(right);
}

void Shop::setStructure(STRUCTURE structure){
    m_structure = structure;
    
    //Make new pointer to current data to free m_heap
    Order* oldHeap = m_heap;
    m_heap = nullptr;
    
    //Now rebuild the heap
    rebuildHeap(oldHeap);
    
}

HEAPTYPE Shop::getHeapType() const {
    return m_heapType;
}

STRUCTURE Shop::getStructure() const {
    return m_structure;
}

void Shop::printOrdersQueue() const {
  
    cout << "Preorder traversal of the nodes in the highest priority shop:" << endl;
    cout << "Contents of the queue: " << endl;
    
    //Now traverse and output
    preOrderPrint(m_heap);
    
}

string Shop::itemToString(ITEM item) const{
    switch(item){
        //enum ITEM {COFFEE, LATTE, SOFTDRINK, MILK, WATER, ICEDTEA};// use with MinHeap
        case COFFEE: return "COFFEE";
        case LATTE: return "LATTE";
        case SOFTDRINK: return "SOFTDRINK";
        case MILK: return "MILK";
        case WATER: return "WATER";
        case ICEDTEA: return "ICEDTEA";
        default: return "UNKNOWN";
    }
}

void Shop::preOrderPrint(Order* node) const{
    
    if(node == nullptr){return;}
    
    cout << "["<< m_priorFunc(*node) <<"] Order ID: " << node->m_orderID << ", item: " << itemToString(node->m_item) << ", count: " << node->m_count << endl;
    preOrderPrint(node->m_left);
    preOrderPrint(node->m_right);
    
}

void Shop::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    cout << "Shop " << m_regPrior << ": => ";
    dump(m_heap);
  }
  cout << endl;
}

void Shop::dump(Order *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID;
    else
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

int Shop::getRegPrior(){
    return m_regPrior;
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order ID: " << order.getOrderID()
        << ", item: " << order.getItem()
        << ", count: " << order.getCount();
  return sout;
}

//////////////////////////////////////////////////////////////
Region::Region(int size){
  
    //Set capcity to the size
    m_capacity = size;
    
    //No elements yet
    m_size = 0;
    
    //Empty
    m_heap = nullptr;
    
    //Create the array with the capacity
    m_heap = new Shop[m_capacity + 1];
    
}
Region::~Region(){
    delete [] m_heap;
}

bool Region::addShop(Shop & aShop){
    
    //No negative priorities??
    if(aShop.getRegPrior() < 0){
        return false;
    }
    
    //Make sure the size is large enough
    if(m_capacity == m_size){
        
        cout << "we in the large zone" << endl;
        
        //Declare temp array pointer cuz SCOPE ISSUES grrrrrr
        Shop* temp = nullptr;
        
        //Make a new array that is double the size
        if(m_capacity == 0){
            temp = new Shop[2];
        }else{
            temp = new Shop[2*m_capacity + 1];
        }
        
        //Load everything into the new array
        for(int i = 1; i < m_size + 1; i++){
            temp[i] = m_heap[i];
        }
        
        //Swap and clean up
        delete[] m_heap;
        m_heap = temp;
        if(m_capacity == 0){
            m_capacity = 1;
        }else{
            m_capacity *= 2;
        }
        temp = nullptr;
    }
    
    //Add shop to end of array
    m_heap[m_size + 1] = aShop;
    m_size += 1;
    
    //Bubble up (I love bubbles, my girlfriend has a bubble machine and her cat loves watching them. It's adorable.)
    //Create pointer to just inserted node
    int i = m_size;
    
    while(i > 1){
        
        //Calculate parent index
        int parentIndex = i/2;
        
        //If the parent is greater than the newly inserted node, swap them
        if(m_heap[parentIndex].getRegPrior() > m_heap[i].getRegPrior()){
            //Swap them
            Shop temp = m_heap[i];
            m_heap[i] = m_heap[parentIndex];
            m_heap[parentIndex] = temp;
        }
        
        //Update i
        i = parentIndex;
        
    }
    return true;
}

bool Region::getShop(Shop & aShop){
    
    //CHeck empty
    if(m_size == 0){
        return false;
    }
    
    //Save the highest prio shop in the parameter variable
    aShop = m_heap[1];
    
    //DEQUEUE
    //Move bottom element to index 0
    m_heap[1] = m_heap[m_size];
    
    //Decrease the size
    m_size -= 1;
    
    //Bubble down
    bool bubbleIncomplete = true;
    int i = 1; //Variable for the index
    while(bubbleIncomplete){
        //Compute left and right child indicies
        int leftChild = 2*i;
        int rightChild = 2*i + 1;
        
        //If there is no left child (AKA no children cuz complete on bobo)
        if(leftChild > m_size){
            bubbleIncomplete = false;
        //If there is no right child
        }else if(rightChild > m_size){
            if(m_heap[i].getRegPrior() > m_heap[leftChild].getRegPrior()){
                //Swap with child
                Shop temp = m_heap[leftChild];
                m_heap[leftChild] = m_heap[i];
                m_heap[i] = temp;
                bubbleIncomplete = false;
            }else{
                bubbleIncomplete = false;
            }
        //There are 2 children (it could be twins even! So exciting. I'll plan the baby shower.)
        }else{
            //Cureent node larger than both children
            if(m_heap[i].getRegPrior() > m_heap[leftChild].getRegPrior() && m_heap[i].getRegPrior() > m_heap[rightChild].getRegPrior()){
                //Swap with smallest index child
                if(m_heap[leftChild].getRegPrior() <= m_heap[rightChild].getRegPrior()){
                    //Swap with child
                    Shop temp = m_heap[leftChild];
                    m_heap[leftChild] = m_heap[i];
                    m_heap[i] = temp;
                    i = leftChild;
                }else{
                    //Swap with child
                    Shop temp = m_heap[rightChild];
                    m_heap[rightChild] = m_heap[i];
                    m_heap[i] = temp;
                    i = rightChild;
                }
            //Current node larger than just 1 child
            }else if(m_heap[i].getRegPrior() > m_heap[leftChild].getRegPrior() || m_heap[i].getRegPrior() > m_heap[rightChild].getRegPrior()){
                //Swap
                if(m_heap[leftChild].getRegPrior() <= m_heap[rightChild].getRegPrior()){
                    //Swap with child
                    Shop temp = m_heap[leftChild];
                    m_heap[leftChild] = m_heap[i];
                    m_heap[i] = temp;
                    i = leftChild;
                }else{
                    //Swap with child
                    Shop temp = m_heap[rightChild];
                    m_heap[rightChild] = m_heap[i];
                    m_heap[i] = temp;
                    i = rightChild;
                }
            }else{
                bubbleIncomplete = false;
            }
        }
    }
    return true;
}

bool Region::getNthShop(Shop & aShop, int n){
    
    //Check that nth shop exists
    if(m_size == 0){
        return false;
    }
    if(n > m_size || n <=0){
        return false;
    }
    
    //Make temporary array to store the shops
    Shop* temp = new Shop[n];
    
    //Pop off the shops (it rhymes hehe)
    for(int i = 0; i < n; i++){
        Shop tempShop;
        getShop(tempShop);
        temp[i] = tempShop;
    }
    
    //Load the nth shop into the parameter
    aShop = temp[n-1];
    
    //Put them shops back
    for(int i = 0; i < n - 1; i++){
            addShop(temp[i]);
        }
    
    delete[] temp;
    
    return true;
}

bool Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){
    
    //Check that nth shop exists
    if(m_size == 0){
        return false;
    }
    if(n > m_size || n <=0){
        return false;
    }
    
    //Make temporary array to store the shops
    Shop* temp = new Shop[n];
    
    //Pop off the shops (it rhymes hehe)
    for(int i = 0; i < n; i++){
        Shop tempShop;
        getShop(tempShop);
        temp[i] = tempShop;
    }
    
    //Make the changes on the nth shop
    temp[n-1].setPriorityFn(priFn, heapType);
    
    //Put them shops back
    for(int i = 0; i < n; i++){
            addShop(temp[i]);
        }
    
    delete[] temp;
    
    return true;
}

bool Region::setStructure(STRUCTURE structure, int n){
    
    //Check that nth shop exists
    if(m_size == 0){
        return false;
    }
    if(n > m_size || n <=0){
        return false;
    }
    
    //Make temporary array to store the shops
    Shop* temp = new Shop[n];
    
    //Pop off the shops (it rhymes hehe)
    for(int i = 0; i < n; i++){
        Shop tempShop;
        getShop(tempShop);
        temp[i] = tempShop;
    }
    
    //Make the changes on the nth shop
    temp[n-1].setStructure(structure);
    
    //Put them shops back
    for(int i = 0; i < n; i++){
            addShop(temp[i]);
        }
    
    delete[] temp;
    
    return true;
}
bool Region::getOrder(Order & order){
    
    //Check for empty heap
    if(m_heap == nullptr || m_size == 0){
        return false;
    }
    
    //Dequeue the highest prio order from highest prio shop
    //If top is empty, remove until gives actual order
    bool gotOrder = false;
    while(!gotOrder && m_size > 0){
        try {
            order = m_heap[1].getNextOrder();
            gotOrder = true;
            return true;
        } catch (const out_of_range&) {
            cout << "exception" << endl;
            Shop temp;
            getShop(temp);
        }
    }
    
    return false;
    
}
void Region::dump(){
    dump(ROOTINDEX);
    cout << endl;
}
void Region::dump(int index){
  if (index <= m_size){
    cout << "(";
    dump(index*2);
    cout << m_heap[index].m_regPrior;
    dump(index*2 + 1);
    cout << ")";
  }
}
