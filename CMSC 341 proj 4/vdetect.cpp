// CMSC 341 - Spring 26 - Project 4
#include "vdetect.h"
VDetect::VDetect(int size, hash_fn hash, prob_t probing = DEFPOLCY){
    //Handle the size (YOU CAN'T HANDLE THE TRUTH) p.s. I DID IT IN ONE LINE LETS GO!
    if(!isPrime(size) || size > MAXPRIME ||size < MINPRIME)size = findNextPrime(size);
    
    //Initialize the variables
    m_hash = hash;              // hash function
    m_newPolicy = probing;      // stores the change of policy request

    m_currentTable = nullptr;  // hash table
    m_currentCap = size;        // hash table size (capacity)
    m_currentSize = 0;          // current number of entries
                                // m_currentSize includes deleted entries
    m_currNumDeleted = 0;       // number of deleted entries
    m_currProbing = probing;    // collision handling policy

    m_oldTable = nullptr;       // hash table
    m_oldCap = 0;;              // hash table size (capacity)
    m_oldSize = 0;              // current number of entries
                                // m_oldSize includes deleted entries
    m_oldNumDeleted = 0;        // number of deleted entries
    m_oldProbing = probing;     // collision handling policy

    m_transferIndex = 0;        // this can be used as a temporary place holder
                                // during incremental transfer to scanning the table

    //Create the memory
    m_currentTable = new Virus*[size];
    for (int i = 0; i < size; i++) {
        m_currentTable[i] = nullptr; //Make sure all nullptr
    }
    
}

VDetect::~VDetect(){
    
    //Delete the current table
    for (int i = 0; i < m_currentCap; i++) {
        if (m_currentTable[i] != nullptr) {
            delete m_currentTable[i];
        }
    }
    delete[] m_currentTable;
    
    //Delete the old table
    for (int i = 0; i < m_oldCap; i++) {
        if (m_oldTable[i] != nullptr) {
            delete m_oldTable[i];
        }
    }
    delete[] m_oldTable;
}

void VDetect::changeProbPolicy(prob_t policy){
    m_newPolicy = policy;
}

bool VDetect::insert(Virus virus){
    
    //Confirm valid ID
    if(virus.m_id > MAXID || virus.m_id < MINID){return false;}
    
    //Confirm virus is unique
    Virus dupe = getVirus(virus.m_key, virus.m_id);
    if(dupe.m_id != 0){return false;} //Return false if this is a duplicate
    
    //Now we have a valid virus
    int insertionIndex = m_hash(virus.m_key) % m_currentCap; //Calculate insertion index
    
    //Check if collision
    if(m_currentTable[insertionIndex] != nullptr){
        //Find new index using probing function
        if (m_currProbing == LINEAR) {
            insertionIndex = probeLinear(insertionIndex, true);
        }
        else if (m_currProbing == QUADRATIC) {
            insertionIndex = probeQuadratic(insertionIndex, true);
        }
        else if (m_currProbing == DOUBLEHASH) {
            insertionIndex = probeDouble(insertionIndex, true);
        }
    }
    
    //Insert the node
    m_currentTable[insertionIndex] =  new Virus(virus);
    //Update the size
    m_currentSize ++;
    
    //Check if load factor becomes greater than 0.5 (make sure old table is drained)
    if(lambda() > 0.5 && m_oldTable == nullptr){
        reHash();
    }
    if(deletedRatio() > 0.8 && m_oldTable == nullptr){
        reHash();
    }
    
    //Check if transfer is necessary
    if(m_oldTable != nullptr){
        transferData();
    }
    
    return true;
}

void VDetect::reHash(){
    //update EVERYTHING
    //Old table stuff
    m_oldTable = m_currentTable;
    m_oldCap = m_currentCap;
    m_oldSize = m_currentSize;
    m_oldNumDeleted = m_currNumDeleted;
    m_oldProbing = m_currProbing;

    //Find new capacity and allocated new memory
    m_currentCap = findNextPrime((m_currentSize - m_currNumDeleted) * 4);
    m_currentTable = new Virus*[m_currentCap]();
    m_currentSize = 0;
    m_currNumDeleted = 0;

    //Update probing
    m_currProbing = m_newPolicy;
    
}

void VDetect::transferData(){
    int chunkSize = m_oldCap / 4;
    
    //Handle the oddness and the floorness of our 25% jam
    int remaining = m_oldCap - m_transferIndex;
    int end;

    if (remaining <= 3 + chunkSize) {
        end = m_oldCap;
    } else {
        end = m_transferIndex + chunkSize;
    }
    
    for(int i = m_transferIndex; i < end; i++){
        //If found valid virus that we need to transfer
        if(m_oldTable[i] != nullptr){
            if(m_oldTable[i]->m_used){
                insertCurrent(*m_oldTable[i]);
            }
            delete m_oldTable[i];
            m_oldTable[i] = nullptr;
            m_oldSize --;
        }
    }
    
    m_transferIndex = end;
    
    if (m_transferIndex == m_oldCap) {
        delete[] m_oldTable;
        m_oldTable = nullptr;
        m_oldCap = 0;
        m_oldSize = 0;
        m_oldNumDeleted = 0;
        m_transferIndex = 0;
    }
}

bool VDetect::insertCurrent(Virus virus){
    
    //Now we have a valid virus
    int insertionIndex = m_hash(virus.m_key) % m_currentCap; //Calculate insertion index
    
    //Check if collision
    if(m_currentTable[insertionIndex] != nullptr){
        //Find new index using probing function
        if (m_currProbing == LINEAR) {
            insertionIndex = probeLinear(insertionIndex, true);
        }
        else if (m_currProbing == QUADRATIC) {
            insertionIndex = probeQuadratic(insertionIndex, true);
        }
        else if (m_currProbing == DOUBLEHASH) {
            insertionIndex = probeDouble(insertionIndex, true);
        }
    }
    
    //Insert the node
    m_currentTable[insertionIndex] =  new Virus(virus);
    //Update the size
    m_currentSize ++;
    
    return true;
}

//Linear probing function
int VDetect::probeLinear(int index, bool insert){
    int i = (index+1) % m_currentCap;
    
    while(i != index && m_currentTable[i] != nullptr && m_currentTable[i]->m_used){
        i = (i+1) % m_currentCap;
    }

    return i;
}

//remove helper funciton
int VDetect::probeLinearRemove(int index, const int ide) const{
    //Check current table first
    //Check the index given
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && m_currentTable[index]->m_used){
        return index;
    }
    //No need to keep searching if found and is not being used
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && !(m_currentTable[index]->m_used)){
        return -1;
    }
    
    //If not found at first index, iterate through to search
    int i = (index+1) % m_currentCap;
    while(i != index && m_currentTable[i] != nullptr){
        //If found and is currently being used
        if(m_currentTable[i]->m_id == ide && m_currentTable[i]->m_used){
            return i;
        }
        //If found and is not being used no need to keep searching
        if(m_currentTable[i]->m_id == ide && !(m_currentTable[i]->m_used)){
            return -1;
        }
        i = (i+1) % m_currentCap;
    }
    return -1;
}

//Remove helper function to search old table
int VDetect::probeLinearRemoveOld(int index, const int ide) const{
    if(m_oldTable == nullptr){return -1;}
    
    //Check the index given
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && m_oldTable[index]->m_used){
        return index;
    }
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && !(m_oldTable[index]->m_used)){
        return -1;
    }
    
    //If not found at first index, iterate through to search
    int i = (index+1) % m_oldCap;
    while(i != index && m_oldTable[i] != nullptr){
        //If found return the index
        if(m_oldTable[i]->m_id == ide && m_oldTable[i]->m_used){
            return i;
        }
        //If found but not being used, no need to keep searching
        if(m_oldTable[i]->m_id == ide && !(m_oldTable[i]->m_used)){
            return -1;
        }
        i = (i+1) % m_oldCap;
    }
    
    return -1;
}


//Probing function for quadratic
int VDetect::probeQuadratic(int index, bool insert){
    int  i = (index + 1) % m_currentCap;
    int iteration = 1;
    

    while(i != index && m_currentTable[i] != nullptr && m_currentTable[i]->m_used){
        iteration ++;
        i = (index + (iteration * iteration)) % m_currentCap;
    }

    return i;
}

//Helper function for quadratic probe to remove
int VDetect::probeQuadraticRemove(int index, const int ide) const{
    //Check index given first
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && m_currentTable[index]->m_used){
        return index;
    }
    //No need to keep searching if found and is not being used
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && !(m_currentTable[index]->m_used)){
        return -1;
    }
    
    //If not found iterate and search
    int  i = (index + 1) % m_currentCap;
    int iteration = 1;
    
    while(i != index && m_currentTable[i] != nullptr){
        //If found and is currently being used
        if(m_currentTable[i]->m_id == ide && m_currentTable[i]->m_used){
            return i;
        }
        //If found and is not being used no need to keep searching
        if(m_currentTable[i]->m_id == ide && !(m_currentTable[i]->m_used)){
            return -1;
        }
        iteration ++;
        i = (index + (iteration * iteration)) % m_currentCap;
    }
    
    return -1;
}

//Helper function for quadratic remove in old table
int VDetect::probeQuadraticRemoveOld(int index, const int ide) const{
    //If old table doesn't exist, it ain't there
    if(m_oldTable == nullptr){return -1;}
    
    //Check the index given first
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && m_oldTable[index]->m_used){
        return index;
    }
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && !(m_oldTable[index]->m_used)){
        return -1;
    }
    
    //If not found iterate and search
    int  i = (index + 1) % m_oldCap;
    int iteration = 1;
    
    while(i != index && m_oldTable[i] != nullptr){
        //If found and is currently being used
        if(m_oldTable[i]->m_id == ide && m_oldTable[i]->m_used){
            return i;
        }
        //If found and is not being used no need to keep searching
        if(m_oldTable[i]->m_id == ide && !(m_oldTable[i]->m_used)){
            return -1;
        }
        iteration ++;
        i = (index + (iteration * iteration)) % m_oldCap;
    }
    
    return -1;
    
}

//Probing function for double hash
int VDetect::probeDouble(int index, bool insert){
    
    int iteration = 1;
    int i = ((index % m_currentCap) + iteration * (11 - (index % 11))) % m_currentCap;
    
    while(i != index && m_currentTable[i] != nullptr && m_currentTable[i]->m_used){
        iteration++;
        i = ((index % m_currentCap) + iteration * (11 - (index % 11))) % m_currentCap;
    }
    
    return i;
}

//Overloaded for remove
int VDetect::probeDoubleRemove(int index, const int ide) const{
    //Check index given first
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && m_currentTable[index]->m_used){
        return index;
    }
    //No need to keep searching if found and is not being used
    if(m_currentTable[index] != nullptr && m_currentTable[index]->m_id == ide && !(m_currentTable[index]->m_used)){
        return -1;
    }
    
    //If not found, iterate and search
    int iteration = 1;
    int i = ((index % m_currentCap) + iteration * (11 - (index % 11))) % m_currentCap;
    
    while(i != index && m_currentTable[i] != nullptr){
        //If found and is currently being used
        if(m_currentTable[i]->m_id == ide && m_currentTable[i]->m_used){
            return i;
        }
        //If found and is not being used no need to keep searching
        if(m_currentTable[i]->m_id == ide && !(m_currentTable[i]->m_used)){
            return -1;
        }
        
        iteration++;
        i = ((index % m_currentCap) + iteration * (11 - (index % 11))) % m_currentCap;
    }
    
    return -1;
}

int VDetect::probeDoubleRemoveOld(int index, const int ide) const{
    //If old table doesn't exist, it ain't there
    if(m_oldTable == nullptr){return -1;}
    
    //Check the index given first
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && m_oldTable[index]->m_used){
        return index;
    }
    if(m_oldTable[index] != nullptr && m_oldTable[index]->m_id == ide && !(m_oldTable[index]->m_used)){
        return -1;
    }
    
    //If not found, iterate and search
    int iteration = 1;
    int i = ((index % m_oldCap) + iteration * (11 - (index % 11))) % m_oldCap;
    
    while(i != index && m_oldTable[i] != nullptr){
        //If found and is currently being used
        if(m_oldTable[i]->m_id == ide && m_oldTable[i]->m_used){
            return i;
        }
        //If found and is not being used no need to keep searching
        if(m_oldTable[i]->m_id == ide && !(m_oldTable[i]->m_used)){
            return -1;
        }
        iteration++;
        i = ((index % m_oldCap) + iteration * (11 - (index % 11))) % m_oldCap;
    }
    
    return -1;
}

bool VDetect::remove(Virus virus){
    
    bool removed = false;
    
    //Confirm valid ID
    if(virus.m_id > MAXID || virus.m_id < MINID){return false;}
    
    //Now we have a valid virus
    int insertionIndex = m_hash(virus.m_key) % m_currentCap; //Calculate insertion index
    
    //Find new index using probing function for new table
    if (m_currProbing == LINEAR) {
        insertionIndex = probeLinearRemove(insertionIndex, virus.m_id);
    }
    else if (m_currProbing == QUADRATIC) {
        insertionIndex = probeQuadraticRemove(insertionIndex, virus.m_id);
    }
    else if (m_currProbing == DOUBLEHASH) {
        insertionIndex = probeDoubleRemove(insertionIndex, virus.m_id);
    }
    
    //Check if found index
    if(insertionIndex != -1){
        //set the flag
        m_currentTable[insertionIndex]->m_used = false;
        m_currNumDeleted ++;
        removed = true;
    }else{
        //Check the old table
        //Recalculate insertion index
        if(m_oldTable != nullptr){
            insertionIndex = m_hash(virus.m_key) % m_oldCap; //Calculate insertion index
            //Find new index using probing function for new table
            if (m_oldProbing == LINEAR) {
                insertionIndex = probeLinearRemoveOld(insertionIndex, virus.m_id);
            }
            else if (m_oldProbing == QUADRATIC) {
                insertionIndex = probeQuadraticRemoveOld(insertionIndex, virus.m_id);
            }
            else if (m_oldProbing == DOUBLEHASH) {
                insertionIndex = probeDoubleRemoveOld(insertionIndex, virus.m_id);
            }
            
            if(insertionIndex != -1){
                //set the flag
                m_oldTable[insertionIndex]->m_used = false;
                m_oldNumDeleted ++;
                removed = true;
            }
        }
    }
    
    //Check if load factor becomes greater than 0.5 (make sure old table is drained)
    if(lambda() > 0.5 && m_oldTable == nullptr){
        reHash();
    }
    if(deletedRatio() > 0.8 && m_oldTable == nullptr){
        reHash();
    }
    
    //Check if transfer is necessary
    if(m_oldTable != nullptr){
        transferData();
    }
    
    return removed;
}

const Virus VDetect::getVirus(string sequence, int id) const{
    //Empty virus object to return if not found
    Virus v;
    
    //Confirm valid ID
    if(id > MAXID || id < MINID){return v;}

    //Now we have a valid virus
    int insertionIndex = m_hash(sequence) % m_currentCap; //Calculate insertion index
    
    //Find new index using probing function for new table
    if (m_currProbing == LINEAR) {
        insertionIndex = probeLinearRemove(insertionIndex, id);
    }
    else if (m_currProbing == QUADRATIC) {
        insertionIndex = probeQuadraticRemove(insertionIndex, id);
    }
    else if (m_currProbing == DOUBLEHASH) {
        insertionIndex = probeDoubleRemove(insertionIndex, id);
    }
    
    //Check if found index
    if(insertionIndex != -1){
        return *m_currentTable[insertionIndex];
    }else{
        //Check the old table
        //Recalculate insertion index
        if(m_oldTable != nullptr){
            insertionIndex = m_hash(sequence) % m_oldCap; //Calculate insertion index
            //Find new index using probing function for new table
            if (m_oldProbing == LINEAR) {
                insertionIndex = probeLinearRemoveOld(insertionIndex, id);
            }
            else if (m_oldProbing == QUADRATIC) {
                insertionIndex = probeQuadraticRemoveOld(insertionIndex, id);
            }
            else if (m_oldProbing == DOUBLEHASH) {
                insertionIndex = probeDoubleRemoveOld(insertionIndex, id);
            }
            
            if(insertionIndex != -1){
                return *m_oldTable[insertionIndex];
            }
        }
    }
    
    return v;
}

bool VDetect::updateID(Virus virus, int id){
    //Check for valid ID
    if(id < MINID || id > MAXID){
            return false;
        }

    //Get the virus
    Virus found = getVirus(virus.m_key, virus.m_id);
    if(found.m_id == 0){
        return false;
    }
    
    //Make sure we aren't making a duplicate virus
    Virus dupe = getVirus(found.m_key, id);
    if(dupe.m_id != 0){
        return false;
    }

    //Remove old one
    remove(found);

    found.m_id = id;
    
    //Add back in
    return insert(found);
  
}

//I love easy functions
float VDetect::lambda() const {
    return (float)m_currentSize / m_currentCap;
}

//More of these please
float VDetect::deletedRatio() const {
    if(m_currentSize == 0){
        return 0;
    }
    return (float)m_currNumDeleted / m_currentSize;
}

void VDetect::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool VDetect::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int VDetect::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) {
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0)
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
