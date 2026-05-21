// UMBC - CMSC 341 - Spring 2026 - Proj1
#include "fuel.h"

FuelSys::FuelSys(){
    
    m_current = nullptr;
    
}

FuelSys::~FuelSys(){
    
    //Empty list
    if(m_current == nullptr){
        return;
    }
    
    Tank * head = m_current;
    
    Tank * curr = m_current->m_next;
    
    while(curr != head){
        
        //Delete pumps
        while(curr->m_pumps != nullptr){
            
            Pump * deletePump = curr->m_pumps;
            curr->m_pumps = curr->m_pumps->m_next;
            delete deletePump;
            
        }
        
        //Delete Tank
        Tank * removeTank = curr;
        curr = curr->m_next;
        delete removeTank;
        
    }
    //Delete last pumps
    //Delete pumps
    while(head->m_pumps != nullptr){
        
        Pump * deletePump = curr->m_pumps;
        head->m_pumps = head->m_pumps->m_next;
        delete deletePump;
        
    }
    
    //Delete last tank
    delete head;
    m_current = nullptr;
    
}

bool FuelSys::addTank(int tankID, int tankCap, int tankFuel) {
    
    //Check tank capacity and check fuel is less than capacity
    if(tankCap < MINCAP || tankFuel > tankCap){
        return false;
    }
    
    //Logic if empty
    if(m_current == nullptr){
        
        //Create new tank
        m_current = new Tank(tankID, tankCap, tankFuel);
        
        //Have tank m_next pointer point to the tank created
        m_current->m_next = m_current;
        
        return true;
        
    }
    
    //Logic if not empty
    //Check for duplicated tank IDs
    else{
        
        //Temp pointer for iterating
        Tank * temp = m_current;
        
        //Iterate through list and check for duplicate ID
        do {
            //Iterate
            temp = temp->m_next;
            
            //Check
            if(temp->m_tankID == tankID){
                
                
                return false;
            }
            
        } while (temp != m_current);
        
        //Store location that new node will need to point to
        temp = m_current->m_next;
        
        //Create new node
        m_current->m_next = new Tank(tankID, tankCap, tankFuel);
        
        //Point node to next element in the list
        m_current->m_next->m_next = temp;
        
        //Set m_current to new node
        m_current = m_current->m_next;
        
        //Clear temp node to prevent memory leaks
        temp = nullptr;
        
        return true;
    }
    
    return false;
    
}

bool FuelSys::removeTank(int tankID){
    
    //Check if list is empty
    if(m_current == nullptr){
        return false;
    }
    
    //One node scenario
    if(m_current->m_next == m_current){
        
        //Check that the ID matches the one node
        if(m_current->m_tankID != tankID){
            
            return false;
            
        }
        
        //Remove pumps
        while(m_current->m_pumps != nullptr){
            
            Pump * deletePump = m_current->m_pumps;
            m_current->m_pumps = m_current->m_pumps->m_next;
            delete deletePump;
            
        }
        
        //Remove tank
        delete m_current;
        m_current = nullptr;
        
        return true;
        
    }
    
    //Multi node scenario
    Tank * curr = m_current;
    Tank * prev = m_current;
    
    //If deleting m_current node
    if(m_current->m_tankID == tankID){
        
        while(prev->m_next != m_current){
            prev = prev->m_next;
        }
        
        prev->m_next = m_current -> m_next;
        
        //Remove pumps
        while(m_current->m_pumps != nullptr){
            
            Pump * deletePump = m_current->m_pumps;
            m_current->m_pumps = m_current->m_pumps->m_next;
            delete deletePump;
            
        }
        
        //Remove tank
        delete m_current;
        m_current = prev;
        prev = nullptr;
        
        
        
        
        //Check all the other pumps to make sure none of them target removed tank and remove them
        
        //Just one node left
        if(m_current == m_current->m_next){
            
            //No pumps
            if(m_current->m_pumps == nullptr){
                return true;
            }
            
            for(Pump * p = m_current->m_pumps; p != nullptr; p = p->m_next){
                
                if(p->m_target == tankID){
                    removePump(m_current->m_tankID, p->m_pumpID);
                }
            }
            
            return true;
        }
        
        Tank * t = m_current;
        
        do {
            for(Pump * p = t->m_pumps; p != nullptr; p = p->m_next){
                
                if(p->m_target == tankID){
                    removePump(t->m_tankID, p->m_pumpID);
                }
            }
            
            t = t->m_next;
            
        } while (t != m_current);
        
        return true;
        
    }
    
    //If deleting another node
    while(curr->m_next != m_current){
        
        curr = curr->m_next;
        if(curr->m_tankID == tankID){
            
            //Remove pumps
            while(curr->m_pumps != nullptr){
                
                Pump * deletePump = curr->m_pumps;
                curr->m_pumps = curr->m_pumps->m_next;
                delete deletePump;
                
            }
            
            //Remove tank
            //Find previous tank
            while(prev->m_next != curr){
                prev = prev->m_next;
            }
            
            //Set previous tank to point to tank after tank being removed
            prev->m_next = curr->m_next;
            
            //Remove tank
            delete curr;
            curr = nullptr;
            prev = nullptr;
            
        
            return true;
            
        }
        
    }
    
    return false;
}

bool FuelSys::findTank(int tankID){
    
    //If empty list
    if(m_current == nullptr){
        
        return false;
        
    }
    
    //If searching for m_current
    if(m_current->m_tankID == tankID){
        
        //Just one node?
        if(m_current->m_next == m_current){
            
            return true;
            
        }
        
        //Multiple nodes
        Tank * newCurr = m_current;
        
        while(newCurr->m_next != m_current){
            newCurr = newCurr->m_next;
        }
        
        m_current = newCurr;
        
        return true;
    }
    
    //If searching for other node
    Tank * curr = m_current;
    
    while(curr->m_next !=m_current){
        
        curr = curr->m_next;
        
        if(curr->m_tankID == tankID){
            
            //Find node behind curr
            
            Tank * behind = m_current;
            
            while(behind->m_next != curr){
                
                behind = behind->m_next;
                
            }
            
            m_current = behind;
            
            return true;
            
        }
        
    }
    
    return false;
}

bool FuelSys::addPump(int tankID, int pumpID, int targetTank){
    
    //Search to ensure that tankId and targetTank exist in the list and that pumpID is unique
    if(findTank(targetTank)){
        
        //cout << "found target" << endl;
        
        if(findTank(tankID)){
            
            //cout << "found tankID" << endl;
            //Check pumpID is unique
            if(!findPump(pumpID, tankID)){
                //Create pointer to tank receiving pump
                Tank * toAddTank = m_current->m_next;
                
                //cout << endl << "adding to tank " << toAddTank->m_tankID << endl;
                
                //Create new pump pointing to head of tanks list
                Pump * toAddPump = new Pump(pumpID, targetTank, toAddTank->m_pumps);
                
                //Point head at newly added pump
                toAddTank->m_pumps = toAddPump;
                
                return true;
                
            }
        }
    }
    
    //cout << "add failed " << endl;
    
    return false;
    
}


bool FuelSys::removePump(int tankID, int pumpID){
    
    //Check that tankID and pumpID exist in system
    if(findPump(pumpID, tankID)){
        if(findTank(tankID)){
            
            //Create tank and pump pointers
            Tank * t = m_current->m_next;
            Pump * p = t->m_pumps;
            
            //Logic if deleting head of pump list
            if(p->m_pumpID == pumpID){
                
                t->m_pumps = p->m_next;
                
                delete p;
                
                return true;
                
            }
            
            //Logic if not
            Pump * tail = p;
            p = p->m_next;
            
            while(p != nullptr){
                
                if(p->m_pumpID == pumpID){
                    
                    tail->m_next = p->m_next;
                    
                    delete p;
                    
                    return true;
                    
                }
                
                tail = tail->m_next;
                p = p->m_next;
                
            }
            
        }
    }
    
    return false;
    
}


int FuelSys::totalFuel() const{
    
    //If empty list
    if(m_current == nullptr){
        return 0;
    }
    
    
    //Sum them all up
    Tank * curr = m_current;
    
    int totalFuel = 0;
    
    do {
        
        totalFuel += curr->m_tankFuel;
        
        curr = curr->m_next;
        
    } while (curr != m_current);
    
    
    return totalFuel;
    
    
}


bool FuelSys::drain(int tankID, int pumpID, int fuel){
    
    //Check that tank and pump exist
    if(findPump(pumpID, tankID) && findTank(tankID)){
        
        //Locate the pump
        Tank * t = m_current->m_next;
        Pump * p = t->m_pumps;
        
        while(p->m_pumpID != pumpID){
            p = p->m_next;
        }
        
        //Locate the target tank
        findTank(p->m_target);
        
        Tank * targetTank = m_current->m_next;
        
        int spaceInTargetTank = targetTank->m_tankCapacity - targetTank->m_tankFuel;
        
        //Logic if source tank contains as much fuel as requested
        
        if (t->m_tankFuel >= fuel){
            
            //amount requested less or equal to empty space in target
            if(fuel <= spaceInTargetTank){
                
                targetTank->m_tankFuel += fuel;
                
                t->m_tankFuel -= fuel;
                
                return true;
                
            }
            
            //amount requested more than empty space in target tank
            if(fuel > spaceInTargetTank){
                
                targetTank->m_tankFuel += spaceInTargetTank;
                
                t->m_tankFuel -= spaceInTargetTank;
                
                return true;
                
            }
            
        }
        
        //Logic if source tank does not contain enough fuel as requested
        
        if(t->m_tankFuel < fuel){
            
            targetTank->m_tankFuel += t->m_tankFuel;
            
            t->m_tankFuel = 0;
            
            return true;
            
        }
        
        
    }
    
    return false;
}


bool FuelSys::fill(int tankID, int fuel){
    
    if(findTank(tankID)){
        
        Tank * t = m_current->m_next;
        
        //More fuel being added than capacity left in tank?
        
        int spaceInTank = t->m_tankCapacity - t->m_tankFuel;
        
        if(fuel >= spaceInTank){
            
            t->m_tankFuel = t->m_tankCapacity;
            
        }
        
        if(fuel < spaceInTank){
            
            t->m_tankFuel += fuel;
            
        }
        
        return true;
    }
    
    return false;
    
}


const FuelSys & FuelSys::operator=(const FuelSys & rhs){
    
    if(this == &rhs){
        
        return *this;
        
    }
    
    //Check if assigned to empty object
    if(rhs.m_current == nullptr){
        
        //Empty list
        if(m_current == nullptr){
            return *this;
        }
        
        Tank * head = m_current;
        
        Tank * curr = m_current->m_next;
        
        while(curr != head){
            
            //Delete pumps
            while(curr->m_pumps != nullptr){
                
                Pump * deletePump = curr->m_pumps;
                curr->m_pumps = curr->m_pumps->m_next;
                delete deletePump;
                
            }
            
            //Delete Tank
            Tank * removeTank = curr;
            curr = curr->m_next;
            delete removeTank;
            
        }
        //Delete last pumps
        //Delete pumps
        while(head->m_pumps != nullptr){
            
            Pump * deletePump = curr->m_pumps;
            head->m_pumps = head->m_pumps->m_next;
            delete deletePump;
            
        }
        
        //Delete last tank
        delete head;
        m_current = nullptr;
        
        return * this;
        
    }
    
    //Assigned to non empty object
    
    //If this object is not empty
    if(m_current){
        
        //Delete everything
        Tank * head = m_current;
        
        Tank * curr = m_current->m_next;
        
        while(curr != head){
            
            //Delete pumps
            while(curr->m_pumps != nullptr){
                
                Pump * deletePump = curr->m_pumps;
                curr->m_pumps = curr->m_pumps->m_next;
                delete deletePump;
                
            }
            
            //Delete Tank
            Tank * removeTank = curr;
            curr = curr->m_next;
            delete removeTank;
            
        }
        //Delete last pumps
        //Delete pumps
        while(head->m_pumps != nullptr){
            
            Pump * deletePump = curr->m_pumps;
            head->m_pumps = head->m_pumps->m_next;
            delete deletePump;
            
        }
        
        //Delete last tank
        delete head;
        m_current = nullptr;
        
        
    }
    
    //Copy over RHS and assign new values
    
    Tank * toCopy = rhs.m_current;
    
    //Just one tank to copy over
    if(rhs.m_current->m_next == rhs.m_current){
        
        //Add the tank
        addTank(rhs.m_current->m_tankID, rhs.m_current->m_tankCapacity, rhs.m_current->m_tankFuel);
        
        //Add the pumps, first check if any pumps
        if(rhs.m_current->m_pumps == nullptr){
            return *this;
        }else{
            //There are pumps to add (facepalms, cries, locks back in)
            //Iterate through and add the pumps
            for(Pump * p = rhs.m_current->m_pumps; p == nullptr; p = p->m_next){
                
                addPump(rhs.m_current->m_tankID, p->m_pumpID, p->m_target);
                
            }
        }
        
        return *this;
    }
    
    //Multiple tanks
    do {
        
        toCopy = toCopy->m_next;
        
        //Copy tank
        addTank(toCopy->m_tankID, toCopy->m_tankCapacity, toCopy->m_tankFuel);
        
        
    } while (toCopy != rhs.m_current);
    
    //save the head tank to reassign after pump process
    
    Tank * storeHead = m_current;
    
    toCopy = rhs.m_current;
    
    do {
        //Check if any pumps
        if(toCopy->m_pumps != nullptr){
            
            //Iterate and add the pumps
            for(Pump * p = toCopy->m_pumps; p != nullptr; p = p->m_next){
                
                addPump(toCopy->m_tankID, p->m_pumpID, p->m_target);
                
            }
        }
        
        toCopy = toCopy->m_next;
        
    } while (toCopy != rhs.m_current);
    
    m_current = storeHead;
   
    
    return *this;
    
    
}
 


bool FuelSys::findPump(int pumpID, int tankID){
    
    //Logic if list is empty
    if(m_current == nullptr){
        return false;
    }
    
    //Logic if list is not
    Tank * tanks = m_current;
    
    //Find the tank we need
    do {
        //Iterate through list of pumps for each tank
        
       
        for(Pump * pumps = tanks->m_pumps; pumps != nullptr; pumps = pumps->m_next){
            if(tanks->m_tankID == tankID){
                if(pumps->m_pumpID == pumpID){
                    return true;
                }
            }
        }
        
        //Iterate tanks
        tanks = tanks->m_next;
        
    } while (tanks != m_current);
    
    return false;
};


void FuelSys::dumpSys() const{
    if (m_current != nullptr){
        Tank* tempTank = m_current->m_next;//we start at front item
        //we continue visting nodes until we reach the cursor
        while(m_current != nullptr && tempTank != m_current){
            cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel << " kg)" << endl;
            // now dump the targets for all pumps in this tank
            // we need to traverse the list of pumps
            dumpPumps(tempTank->m_pumps);
            tempTank = tempTank->m_next;
        }
        //at the end we visit the cursor (current)
        //this also covers the case that there is only one item
        cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel << " kg)" << endl;
        dumpPumps(tempTank->m_pumps);
        cout << "The current tank is " << m_current->m_tankID << endl;
    }
    else
        cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump* pumps) const{
    // this traverses the linked list to the end
    Pump* tempPump = pumps;
    while (tempPump != nullptr){
        cout << " => pump " << tempPump->m_pumpID << "(To tank " << tempPump->m_target << ")" << endl;
        tempPump = tempPump->m_next;
    }
}
