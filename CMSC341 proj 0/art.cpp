//
//  art.cpp
//  CMSC341_Proj0
//
//  Created by Elijah Busse on 1/27/26.
//

// UMBC - CMSC 341 - Spring 2026 - Proj0
#include "art.h"
Art::Art(){
    
    m_height = 0;
    m_width = 0;
    m_painting = nullptr;
    
}

Art::Art(int height, int width){
    
    //Check for invalid parameters
    if (height <= 0 || width <= 0){
        
        m_height = 0;
        m_width = 0;
        m_painting = nullptr;
        
    }
    else{
        
        m_height = height;
        m_width = width;
        
        //Allocate the height columb
        m_painting = new int*[m_height]();
        
        //Allocate the width rows
        for(int i = 0; i< m_height; i++){
            
            m_painting[i] = new int[m_width]();
            
        }
    }
}

Art::~Art(){

    for(int i = 0; i < m_height; i++){
        
        delete[] m_painting[i];
        
    }
    delete[] m_painting;
    m_painting = nullptr;
}

void Art::clear(){
    
    for(int i = 0; i < m_height; i++){
        
        delete[] m_painting[i];
        
    }
    delete[] m_painting;
    m_painting = nullptr;
    m_height = 0;
    m_width = 0;
}

void Art::create(int initiate){
    
    //Declare object and set the seed
    Random randGen(10, 99);
    randGen.setSeed(initiate);
    
    //Nested for loop to traverse arrays and load data
    for(int i = 0; i < m_height; i ++){
        for(int j = 0; j < m_width; j++){
            
            m_painting[i][j] = randGen.getRandNum();
            
        }
    }
}

void Art::dumpColors(string pixel) const{
    if (m_height > 0 && m_width > 0 && m_painting != nullptr){
        for (int i=1;i <= m_height;i++){
            for (int j=1;j<=m_width;j++){
                std::cout << "\x1b[38;5;" << to_string(m_painting[i-1][j-1]) << "m" << pixel << pixel << RESET;
            }
            cout << endl;
        }
        cout << endl;
    }
}

void Art::dumpValues() const{
    if (m_height > 0 && m_width > 0 && m_painting != nullptr){
        for (int i=1;i <= m_height;i++){
            for (int j=1;j<=m_width;j++){
                cout << m_painting[i-1][j-1] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

//Call constructor in initializer list to create araay
Art::Art(const Art& rhs) : Art(rhs.m_height, rhs.m_width){
    
    //Copy over the data
    for(int i = 0; i < m_height; i ++){
        for(int j = 0; j < m_width; j++){
            
            m_painting[i][j] = rhs.m_painting[i][j];
            
        }
    }
    
    
}

//Overloaded assignment operator
const Art& Art::operator=(const Art& rhs){
    
    if(this == &rhs){
        
        return *this;
        
    }
    else{
        
        //Check if assigned to empy object
        if(rhs.m_painting == nullptr){
            
            //Clear data
            for(int i = 0; i < m_height; i++){
                
                delete[] m_painting[i];
                
            }
            delete[] m_painting;
            m_painting = nullptr;
            
            m_height = 0;
            m_width = 0;
      
            return *this;
        }
        
        //Remove existing data
        if(m_painting){
            
            for(int i = 0; i < m_height; i++){
                delete[] m_painting[i];
            }
            delete[] m_painting;
        }
        
        //Assign parameters
        m_height = rhs.m_height;
        m_width = rhs.m_width;
        
        //Construct new arrays
        //Allocate the height columb
        m_painting = new int*[m_height]();
        
        //Allocate the width rows
        for(int i = 0; i< m_height; i++){
            
            m_painting[i] = new int[m_width]();
            
        }
        
        //Copy over the data
        for(int i = 0; i < m_height; i ++){
            for(int j = 0; j < m_width; j++){
                
                m_painting[i][j] = rhs.m_painting[i][j];
                
            }
        }
        
        return *this;
    }
}


bool Art::left2Right(const Art& rhs){
    
    //Logic if this object is empty
    if(m_height == 0){
        
        *this = rhs;
        
        return true;
        
    }
    
    //Logic if other object is empty
    if(rhs.m_height == 0){
        
        return true;
        
    }
    
    //Check if objects have same height
    if(m_height != rhs.m_height){
        
        return false;
        
    }
    
    //Logic if both objects contain data and have same height.
    else{
        
        //Create a new grid to move everything into before clearing and reassigning m_painting
        int newWidth = m_width + rhs.m_width;
        
        int** newGrid = new int*[m_height];
        for(int i = 0; i < m_height; i++){
            newGrid[i] = new int[newWidth];
        }
        
        //Move data into new grid
        //Load left hand side
        for(int i = 0; i < m_height; i++){
            for(int j = 0; j < m_width; j++){
                newGrid[i][j] = m_painting[i][j];
            }
        }
        
        //Load right hand side
        for(int i = 0; i < m_height; i++){
            for(int j = 0; j < rhs.m_width; j++){
                newGrid[i][m_width + j] = rhs.m_painting[i][j];
            }
        }
        
        //Delete old grid
        for (int i = 0; i < m_height; i++) {
            delete[] m_painting[i];
        }
        delete[] m_painting;
        
        //Assign pointer to new grid
        m_painting = newGrid;
        m_width = newWidth;
        
        return true;
    }
    
}


bool Art::top2Bottom(const Art& bottom){
    
    //Logic if this object is empty
    if(m_height == 0){
        
        *this = bottom;
        
        return true;
        
    }
    
    //Logic if other object is empty
    if(bottom.m_height == 0){
        
        return true;
        
    }
    
    //Check if objects have same width
    if(m_width != bottom.m_width){
        return false;
    }
    
    //Logic if both objects contain data and have same height.
    else{
        
        //Create a new grid to move everything into before clearing and reassigning m_painting
        int newHeight = m_height + bottom.m_height;
        
        int** newGrid = new int*[newHeight];
        for(int i = 0; i < newHeight; i++){
            newGrid[i] = new int[m_width];
        }
        
        //Move data into new grid
        //Load top
        for(int i = 0; i < m_height; i++){
            for(int j = 0; j < m_width; j++){
                newGrid[i][j] = m_painting[i][j];
            }
        }
        
        //Load bottom
        for(int i = 0; i < bottom.m_height; i++){
            for(int j = 0; j < bottom.m_width; j++){
                newGrid[i + m_height][j] = bottom.m_painting[i][j];
            }
        }
        
        //Delete old grid
        for (int i = 0; i < m_height; i++) {
            delete[] m_painting[i];
        }
        delete[] m_painting;
        
        //Assign pointer to new grid
        m_painting = newGrid;
        m_height = newHeight;
        
        return true;
    }
    
    
    
}
 
 
bool Art::reverse(){
    
    //Add the checks here
    //Logic if this object is empty
    if(m_height == 0){
        
        return true;
        
    }
    
    //Counters to iterate over this object while loading in new colors from tempGrid
    int l = -1, m = 0;
    
    //Make a temporary copy
    int** tempGrid = new int*[m_height];
    for(int i = 0; i< m_height; i++){
        
        tempGrid[i] = new int[m_width];
        
    }
    
    //Load data
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            tempGrid[i][j] = m_painting[i][j];
        }
    }
    
    //Traverse the copy backwards loading those values into this object
    for(int i = m_height - 1; i >= 0; i--){
        
        //iterate counter, reset the row
        l++;
        m = 0;
        
        for(int j = m_width - 1; j >= 0; j--){
            
            //set location on this object to new color
            m_painting[l][m] = tempGrid[i][j];
            
            //iterate counters
            m++;
            
        }
    }
    
    // delete the tempGrid
    for (int i = 0; i < m_height; i++) {
        delete[] tempGrid[i];
    }
    delete[] tempGrid;
    
    return true;
    
}


bool Art::rotate(){
    
    //Check for even size height and width.
    if(m_width%2 == 1 || m_height%2 == 1){
        
        return false;
        
    }
    
    //Make a temporary copy
    int** tempGrid = new int*[m_height];
    for(int i = 0; i< m_height; i++){
        
        tempGrid[i] = new int[m_width];
        
    }
    
    //Load data
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            tempGrid[i][j] = m_painting[i][j];
        }
    }
    
    //Define variables for half the height and width to be used in calculations
    int halfWidth = m_width/2;
    int halfHeight = m_height/2;
    
    //Load the top right corner
    for(int i = 0; i < halfHeight; i++){
        
        for(int j = 0; j < halfWidth; j++){
            
            m_painting[i][j + halfWidth] = tempGrid[i][j];
            
        }
        
    }
    
    //Load the bottom right corner
    for(int i = 0; i < halfHeight; i++){
        
        for(int j = halfWidth; j < m_width; j++){
            
            cout << " i = " << i << "j = " << j << endl;
            
            m_painting[i + halfHeight][j] = tempGrid[i][j];
            
        }
        
    }
    
    //Load the bottom left corner
    for(int i = halfHeight; i < m_height; i++){
        
        for(int j = halfWidth; j < m_width; j++){
            
            m_painting[i][j - halfWidth] = tempGrid[i][j];
            
        }
        
    }
    
    //Load the top left corner
    for(int i = halfHeight; i < m_height; i++){
        
        for(int j = 0; j < halfWidth; j++){
            
            m_painting[i - halfHeight][j] = tempGrid[i][j];
            
        }
        
    }
    
    // delete the tempGrid
    for (int i = 0; i < m_height; i++) {
        delete[] tempGrid[i];
    }
    delete[] tempGrid;
    
    return true;
    
    
}

