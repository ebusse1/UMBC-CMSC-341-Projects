// CMSC 341 - Spring 2026 - Project 4
#include "vdetect.h"
#include <random>
#include <vector>
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


unsigned int hashCode(const string str);
string sequencer(int size, int seedNum);

int findNextPrime(int current);//ToRemove
int tempArray[101] = {0};//ToRemove, used to count number of collisions

class Tester{
public:
    //Make a virus array with non-colliding keys
    vector<Virus> makeNonCollidingViruses(int count, int tableCap) {
        //vector of viruses and used indexes
        vector<Virus> viruses;
        vector<bool> usedIndexes(tableCap, false);

        //Random IDs
        Random RndID(MINID, MAXID);
        int seed = 0;

        
        while (viruses.size() < count) {
            string key = sequencer(5, seed);
            int id = RndID.getRandNum();

            int index = hashCode(key) % tableCap;

            if (!usedIndexes[index]) {
                viruses.push_back(Virus(key, id, true));
                usedIndexes[index] = true;
            }

            seed++;
        }

        return viruses;
    }
    
    //Make a virus array with half-colliding keys
    vector<Virus> makeHalfCollidingViruses(int count, int tableCap) {
        vector<Virus> viruses;
        vector<bool> usedIndexes(tableCap, false);

        Random RndID(MINID, MAXID);

        int seed = 0;

        int nonCollidingTarget = count / 2;
        int collidingTarget = count - nonCollidingTarget;

        //make noncolliding viruses
        while (viruses.size() < nonCollidingTarget) {
            string key = sequencer(5, seed);
            int id = RndID.getRandNum();

            int index = hashCode(key) % tableCap;

            if (!usedIndexes[index]) {
                viruses.push_back(Virus(key, id, true));
                usedIndexes[index] = true;
            }

            seed++;
        }

        //make Colliding viruses
        int collisionsAdded = 0;

        while (collisionsAdded < collidingTarget) {
            string key = sequencer(5, seed);
            int id = RndID.getRandNum();

            int index = hashCode(key) % tableCap;

            if (usedIndexes[index]) {
                viruses.push_back(Virus(key, id, true));
                collisionsAdded++;
            }

            seed++;
        }

        return viruses;
    }

    bool InsertNormal(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
            
            //Check that it is in the correct location in the hash table
            int hashed = hashCode(v[i].m_key) % vdetect.m_currentCap;
            if(!(*vdetect.m_currentTable[hashed] == v[i])){
                return false;
            }
        }
        
        //Check that size is now correct
        if(vdetect.m_currentSize == 50){
            return true;
        }
        else{
            return false;
        }
    }
    
    bool InsertCollisionLinear(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeHalfCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
            
            //Check that it is in the correct location in the hash table
            int hashed = hashCode(v[i].m_key) % vdetect.m_currentCap;
            
            //If we hash and find nullptr, bad
            if(vdetect.m_currentTable[hashed] == nullptr){
                return false;
            }
        }
        
        //Check that size is now correct
        if(vdetect.m_currentSize == 50){
            return true;
        }
        else{
            return false;
        }
    }
    
    bool InsertCollisionQuad(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, QUADRATIC);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeHalfCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
            
            //Check that it is in the correct location in the hash table
            int hashed = hashCode(v[i].m_key) % vdetect.m_currentCap;
            
            //If we hash and find nullptr, bad
            if(vdetect.m_currentTable[hashed] == nullptr){
                return false;
            }
        }
        
        //Check that size is now correct
        if(vdetect.m_currentSize == 50){
            return true;
        }
        else{
            return false;
        }
    }
    
    bool InsertCollisionDouble(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, DOUBLEHASH);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeHalfCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
            
            //Check that it is in the correct location in the hash table
            int hashed = hashCode(v[i].m_key) % vdetect.m_currentCap;
            
            //If we hash and find nullptr, bad
            if(vdetect.m_currentTable[hashed] == nullptr){
                return false;
            }
        }
        
        //Check that size is now correct
        if(vdetect.m_currentSize == 50){
            return true;
        }
        else{
            return false;
        }
    }
    
    bool GetError(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the viruses
            vdetect.insert(v[i]);
        }
        
        //Find a virus ID that doesn't exist
        Random RndID(MINID, MAXID);
        int seed = 0;
        string key = "TTCGA";
        int id = RndID.getRandNum();
        
        //Check variable for searching
        bool dupe = false;
        
        //Find a virus that isn't already in the table
        do {
            dupe = false;

            //New seed and ID if seed or ID already existsd in the table
            key = sequencer(5, seed);
            id = RndID.getRandNum();

            for (int i = 0; i < v.size(); i++) {
                if (v[i].m_key == key || v[i].m_id == id) {
                    dupe = true;
                    break;
                }
            }
            seed++;
        } while (dupe);
        
        //Search for nonexistent virus
        Virus vGet = vdetect.getVirus(key, id);
        
        //Make sure virus is empty
        if(vGet.m_id != 0 || vGet.m_key != ""){
            return false;
        }
        
        return true;
    }
    
    bool GetNormal(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //Search for half the viruses
        for(int i = 0; i < v.size()/2; i++){
            //Search for the viruses
            Virus vSearch = vdetect.getVirus(v[i].m_key, v[i].m_id);
            
            if(vSearch.m_key != v[i].m_key || vSearch.m_id != v[i].m_id){
                return false;
            }
        }
        
        return true;
    }
    
    bool GetCollision(){
        
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 25 colliding keys
        vector<Virus> v = makeHalfCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //Search for the 2nd half of the viruses which are the collisions
        for(size_t i = (v.size()/2); i < v.size(); i++){
            //Search for the viruses
            Virus vSearch = vdetect.getVirus(v[i].m_key, v[i].m_id);
            
            if(vSearch.m_key != v[i].m_key || vSearch.m_id != v[i].m_id){
                return false;
            }
        }
        
        return true;
    }
    
    bool RemoveNormal(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non-colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //remove half the viruses
        for(int i = 0; i < v.size()/2; i++){
            //remove the virus
            vdetect.remove(v[i]);
            
            //Get the virus and make sure it is now unused
            if(vdetect.getVirus(v[i].m_key, v[i].m_id).m_used){
                return false;
            }
        }
        
        return true;
    }
    
    bool RemoveCollision(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 25 colliding keys
        vector<Virus> v = makeHalfCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //remove half the viruses
        for(size_t i = v.size()/2; i < v.size(); i++){
            //remove the virus
            vdetect.remove(v[i]);
            
            //Get the virus and make sure it is now unused
            if(vdetect.getVirus(v[i].m_key, v[i].m_id).m_used){
                return false;
            }
        }
        
        return true;
    }
    
    bool RehashTrigger(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 51 non colliding keys
        vector<Virus> v = makeHalfCollidingViruses(51, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //Check that rehash has been triggered
        //Old table should exist
        if(vdetect.m_oldTable == nullptr){
            return false;
        }
        
        //New table should have less than 51 elements
        if(vdetect.m_currentSize >= 51){
            return false;
        }
        
        //New table should be larger than MINPRIME
        if(vdetect.m_currentCap <= MINPRIME){
            return false;
        }
        
        return true;
    }
    
    bool RehashCompletion(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 51 non colliding keys
        vector<Virus> v = makeHalfCollidingViruses(54, MINPRIME);
        
        
        //Insert them into table
        for (int i = 0; i < v.size() - 3; i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        //Complete the rehash by inserting 3 more viruses
        for (size_t i = v.size() - 3; i < v.size(); i++) {
            vdetect.insert(v[i]);
        }
        
        //Old table should NOT exist
        if(vdetect.m_oldTable != nullptr){
            return false;
        }
        
        //New table should have all 54 elements
        if(vdetect.m_currentSize != 54){
            return false;
        }
        
        //New table should be larger than MINPRIME
        if(vdetect.m_currentCap <= MINPRIME){
            return false;
        }
        
        return true;
    }
    
    bool RehashTriggerRemoval(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        for(int i = 0; i < v.size() * 0.8; i++){
           vdetect.remove(v[i]);
        }
        
        //Check that rehash has been triggered
        //Old table should exist
        if(vdetect.m_oldTable == nullptr){
            return false;
        }
        
        return true;
    }
    
    bool RehashCompletionRemoval(){
        //Make a vdetect object
        VDetect vdetect(MINPRIME, hashCode, LINEAR);
        
        //Array of vectors with 50 non colliding keys
        vector<Virus> v = makeNonCollidingViruses(50, MINPRIME);
        
        //Insert them into table
        for (int i = 0; i < v.size(); i++) {
            //Insert the virus
            vdetect.insert(v[i]);
        }
        
        for(int i = 0; i < v.size() * 0.85; i++){
           vdetect.remove(v[i]);
        }
        
        //Check that rehash has been triggered
        //Old table should not exist
        if(vdetect.m_oldTable != nullptr){
            return false;
        }
        
        return true;
    }
    
};

int main(){
    Tester test;
    
    if(test.InsertNormal()){
        cout << "Test Insert Normal Linear has passed" << endl;
    }else{
        cout << "Test Insert Normal Linear has failed" << endl;
    }
    
    if(test.InsertCollisionLinear()){
        cout << "Test Insert Collision Linear has passed" << endl;
    }else{
        cout << "Test Insert Collision Linear has failed" << endl;
    }
    
    if(test.InsertCollisionQuad()){
        cout << "Test Insert Collision Quad has passed" << endl;
    }else{
        cout << "Test Insert Collision Quad has failed" << endl;
    }
    
    if(test.InsertCollisionDouble()){
        cout << "Test Insert Collision Double has passed" << endl;
    }else{
        cout << "Test Insert Collision Double has failed" << endl;
    }
    
    if(test.GetError()){
        cout << "Test Get Error has passed" << endl;
    }else{
        cout << "Test Get Error has failed" << endl;
    }
    
    if(test.GetNormal()){
        cout << "Test Get Normal has passed" << endl;
    }else{
        cout << "Test Get Normal has failed" << endl;
    }
    
    if(test.GetCollision()){
        cout << "Test Get Collision has passed" << endl;
    }else{
        cout << "Test Get Collision has failed" << endl;
    }
    
    if(test.RemoveNormal()){
        cout << "Test Remove Normal has passed" << endl;
    }else{
        cout << "Test Remove Normal has failed" << endl;
    }
    
    if(test.RemoveCollision()){
        cout << "Test Remove Collision has passed" << endl;
    }else{
        cout << "Test Remove Collision has failed" << endl;
    }
    
    if(test.RehashTrigger()){
        cout << "Test Rehash Trigger has passed" << endl;
    }else{
        cout << "Test Rehash Trigger has failed" << endl;
    }
   
    
    if(test.RehashCompletion()){
        cout << "Test Rehash Completion has passed" << endl;
    }else{
        cout << "Test Rehash Completion has failed" << endl;
    }
    
    if(test.RehashTriggerRemoval()){
        cout << "Test Rehash Trigger Removal has passed" << endl;
    }else{
        cout << "Test Rehash Trigger Removal has failed" << endl;
    }
    
    if(test.RehashCompletionRemoval()){
        cout << "Test Rehash Completion Removal has passed" << endl;
    }else{
        cout << "Test Rehash Completion Removal has failed" << endl;
    }
    
    
    return 0;
}
unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for ( int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}
string sequencer(int size, int seedNum){
    //this function returns a random DNA sequence
    string sequence = "";
    Random rndObject(0,3);
    rndObject.setSeed(seedNum);
    for (int i=0;i<size;i++){
        sequence = sequence + ALPHA[rndObject.getRandNum()];
    }
    return sequence;
}
