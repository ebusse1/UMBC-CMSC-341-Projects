//
//  driver.cpp
//  CMSC341_Proj0
//
//  Created by Elijah Busse on 1/27/26.
//

// UMBC - CMSC 341 - Spring 2026 - Proj0


#include "art.h"
#include <string>


class Tester{
    public:
    // the following tests the normal case of create() function
    bool testCreateNormal(Art & art);
    
    //Tests creating an object with invalid height/width ints
    bool testCreateError(int height = 0, int width = 0);
    
    //Tests creating an object that is 1x1
    bool testCreateEdge();
    
    //Tests a copy to see if it is the same as the original and at a different location in memory
    bool testCopy(Art & art1, Art & art2);
    
    //Tests a copy to see if it is the same as the original and at a different location in memory
    bool testCopyEdge(Art & art1, Art & art2);
    
    //Tests a copy to see if it is the same as the original and at a different location in memory
    bool testAssignmentNormal(Art & art1, Art & art2);
    
    //Tests a copy to see if it is the same as the original and at a different location in memory and also empty
    bool testAssignmentEdge(Art & art1, Art & art2);
    
    //Tests a copy to see if it is the same as the original and at a different location in memory and also empty
    bool testAssignmentChain(Art & art1, Art & art2, Art & art3);
    
    //Tests the normal case of appending one obejct onto another with Left2Right function
    bool testLeft2RightNormal(Art & art1, Art & art2);
    
    //Tests the error case of appending one obejct onto another with Left2Right function with different heights
    bool testLeft2RightError(Art & art1, Art & art2);
    
    //Tests the edge case of appending one object onto an empty one
    bool testLeft2RightEdge(Art & art1, Art & art2);
    
    //Tests the edge case of appending empty object onto normal one
    bool testLeft2RightEdge2(Art & art1, Art & art2);
    
    //Tests the edge case of appending an object to itself
    bool testLeft2RightSelfAppend(Art & art1);
    
    //Tests the normal case of appending two normal objects
    bool testTop2BottomNormal(Art & art1, Art & art2);
    
    //Tests the error case of appending two objects with different widths
    bool testTop2BottomError(Art & art1, Art & art2);
    
    //Tests the error case of appending empty object to normal object
    bool testTop2BottomEdge(Art & art1, Art & art2);
    
    //Tests the error case of appending normal object to empty object
    bool testTop2BottomEdge2(Art & art1, Art & art2);
    
    //Tests the edge case of self appending
    bool testTop2BottomSelfAppend(Art & art1);
    
    //Tests the edge case of rotating an empty object
    bool testRotateEdge(Art & art1);
    
    //Tests the edge case of rotating an empty object
    bool testRotateError(Art & art1, Art & art2);
    
    //Runs a given painting through reverse and rotate, returns true if operations have been performed
    bool performOperations(Art & art);
    
    //Checks the memory addresses for two m_paintings for two art objects, outputs true if they are the same, false if they are not
    bool checkMem(Art & art1, Art & art2);
    
    //Checks the member variables for two m_paintings for two art objects, outputs true if they are the same, false if they are not
    bool checkVars(Art & art1, Art & art2);
    
    //Checks the color codess for two m_paintings for two art objects, outputs true if they are the same, false if they are not
    bool checkCodes(Art & art1, Art & art2);
    
    //Outputs whether the test succeeded or failed
    void testOutput(bool tester);

    private:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    string m_name = "function";
    
    
};


int main(){
    
    //REAL TESTS HERE
    
    Tester tester;// test object
    
    {
        
        //Test creating an art object with negative height and width
        tester.testOutput(tester.testCreateError(-1, -10));
        
    }
    
    {
        
        //Test creating an art object that is 1/1
        tester.testOutput(tester.testCreateEdge());
        
    }
    
    {
        //Test creating an art object that is 10/10
        //Create art object
        Art art(10, 10);
        tester.testOutput(tester.testCreateNormal(art));
        
    }
    
    {
        
        //Test whether the copy constructor works correctly for a normal case. Here is a sample algorithm:
        //Create a normal object,
        Art art1(10,2);
        art1.create(3);
        
        //Create a copy of previous object using the copy constructor,
        Art art2(art1);
        
        //Check whether m_painting of original object and copy object point to different memory locations,
        //Check whether the corresponding member variables of both objects carry the same values,
        //Check whether all corresponding color codes are equal in both objects.
        tester.testOutput(tester.testCopy(art1, art2));
        
    }
    
    {
        //Test whether the copy constructor works correctly for an edge case, i.e. it copies an empty object correctly.
        //Create an empty object,
        Art art1(0,0);
       
        //Create a copy of previous object using the copy constructor,
        Art art2(art1);
        
        tester.testOutput(tester.testCopyEdge(art1, art2));
    }
    
    {
        //Test whether the assignment operator works correctly for a normal case.
        //Create two normal objects,
        Art art1(10,2);
        Art art2(5,4);
        
        art1.create(3);
        art2.create(4);
        
        //Create a copy of previous object using the assignment operator,
        art2 = art1;
        
        //Check that they are the same with different memory addresses
        tester.testOutput(tester.testAssignmentNormal(art1, art2));
        
    }
    
    {
        
        //Test whether the assignment operator works correctly for an edge case, i.e. it assigns an empty object correctly.
        //Create two normal objects, art1 is empty
        Art art1(0,0);
        Art art2(5,4);
        
        art1.create(3);
        art2.create(4);
        
        //Create a copy of previous object using the assignment operator,
        art2 = art1;
        
        //Check that they are the same with different memory addresses
        tester.testOutput(tester.testAssignmentEdge(art1, art2));
        
    }
    
    {
        
        //Test whether the assignment operator works correctly for an edge case, i.e. it assigns an empty object correctly.
        //Create two normal objects, art1 is empty
        Art art1(4,3);
        Art art2(5,4);
        Art art3(6,7);
        
        art1.create(3);
        art2.create(4);
        art3.create(5);
        
        
        //Create a copy of previous object using the assignment operator, in a chain of 3
        art1 = art2 = art3;
        
        //Check that they are the same with different memory addresses
        tester.testOutput(tester.testAssignmentChain(art1, art2, art3));
        
    }
    
    {
        //Test whether left2Right(...) is working correctly for a normal case, i.e. appending two normal objects.
        //Create two normal objects
        Art art1(8,2);
        Art art2(8,4);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testLeft2RightNormal(art1,art2));
        
    }
    
    {
        //Test whether left2Right(...) is working correctly for an error case, i.e. two objects have different heights.
        Art art1(4,5);
        Art art2(5,6);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testLeft2RightError(art1, art2));
        
    }
    
    {
        //Test whether left2Right(...) is working correctly for appending a normal object to an empty object.
        Art art1(0,0);
        Art art2(5,6);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testLeft2RightEdge(art1, art2));
        
    }
    
    {
        //Test whether left2Right(...) is working correctly for appending an empty object to a normal object.
        Art art1(3,4);
        Art art2(0,0);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testLeft2RightEdge2(art1, art2));
    }
    
    {
        //Test whether left2Right(...) is working correctly for appending an object to itself.
        Art art1(3,4);
        
        art1.create(3);
 
        
        tester.testOutput(tester.testLeft2RightSelfAppend(art1));
    }
    
    {
        //Test whether top2Bottom(...) is working correctly for a normal case, i.e. appending two normal objects.
        Art art1(3 ,4);
        Art art2(4, 4);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testTop2BottomNormal(art1, art2));
    }
    
    {
        //Test whether top2Bottom(...) is working correctly for an error case, i.e. two objects have different widths.
        Art art1(3 ,2);
        Art art2(4, 4);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testTop2BottomError(art1, art2));
        
    }
    
    {
        //Test whether top2Bottom(...) is working correctly for appending a normal object to an empty object.
        Art art1(0, 0);
        Art art2(4, 4);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testTop2BottomEdge(art1, art2));
        
    }
    
    {
        //Test whether top2Bottom(...) is working correctly for appending an empty object to a normal object.
        Art art1(2, 3);
        Art art2(0, 0);
        
        art1.create(3);
        art2.create(4);
        
        tester.testOutput(tester.testTop2BottomEdge2(art1, art2));
        
    }
    
    {
        //Test whether top2Bottom(...) is working correctly for appending an object to itself.
        //Test whether top2Bottom(...) is working correctly for appending an empty object to a normal object.
        Art art1(2, 3);
        
        art1.create(3);
        
        tester.testOutput(tester.testTop2BottomSelfAppend(art1));
        
    }
    
    {
        //Test rotate for an empty object
        Art art1(0, 0);
        
        art1.create(82);
        
        tester.testOutput(tester.testRotateEdge(art1));
        
    }
    
    {
        //Test rotate for two different objects, one with odd height, one with odd width
        Art art1(2, 3);
        Art art2(3,2);
        
        art1.create(82);
        art2.create(123);
        
        tester.testOutput(tester.testRotateError(art1, art2));
        
    }
    
    return 0;
}


//This function tests that all colors in object fall within normal range
bool Tester::testCreateNormal(Art & art){
    // this function assumes the art object is not empty
    // all color codes must be within the correct range
    m_name = "testCreateNormal";
    
    bool result = true;
    
    art.create(10);
    
    if (art.m_height > 0 && art.m_width > 0 && art.m_painting != nullptr){
        for (int i=0; i < art.m_height; i++){
            for (int j=0; j < art.m_width; j++){
                if(art.m_painting[i][j] < 10 || art.m_painting[i][j] > 99)
                    result = false;
            }
        }
    }
    else{
        result = false;
    }
    return result;
}

//This function tests creating an object with invalid height/width ints
bool Tester::testCreateError(int height, int width){
    
    m_name = "testCreateError";
    // this function creates an art object with error case bounds
    // function should return an object of size 0,0
    bool result = true;
    Art art(height, width);
    art.create(10);
    if (art.m_height != 0 && art.m_width != 0 && art.m_painting != nullptr){
        result = false;
        }
    return result;
}

//Tests creating an object that is 1x1
bool Tester::testCreateEdge(){
    
    m_name = "testCreateEdge";
    // this function creates an art object with edge case size
    // function should return an object of size 1,1 and pass through both reverse and rotation operations successfully
    bool result = true;
    Art art(1,1);
    art.create(10);
    if (art.m_height != 1 && art.m_width != 1 && art.m_painting == nullptr && performOperations(art)){
        result = false;
        }
    return result;
    
}

//Tests a copy to see if it is the same as the original and at a different location in memory
bool Tester::testCopy(Art & art1, Art & art2){
    m_name = "testCopy";
    //Check whether m_painting of original object and copy object point to different memory locations,
    //Check whether the corresponding member variables of both objects carry the same values,
    //Check whether all corresponding color codes are equal in both objects.
    if(!checkMem(art1, art2) && checkVars(art1, art2) && checkCodes(art1, art2)){
        return true;
    }else{
        return false;
    }
    
}

//Tests a copy to see if it is the same as the original and at a different location in memory
bool Tester::testCopyEdge(Art & art1, Art & art2){
    m_name = "testCopyEdge";
    //Check whether m_painting of original object and copy object point to different memory locations,
    //Check whether the corresponding member variables of both objects are 0
    //cout << "art2 height == " << art2.m_height << " art2 width == " << art2.m_width << endl;
    if(art1.m_painting == nullptr && art2.m_painting == nullptr && checkVars(art1, art2) && art2.m_width == 0 && art2.m_height == 0){
        return true;
    }else{
        return false;
    }
    
}

//Tests a copy to see if it is the same as the original and at a different location in memory but with the assignment operator now WOW
bool Tester::testAssignmentNormal(Art & art1, Art & art2){
    
    m_name = "testAssignmentNormal";
    //Check whether m_painting of original object and copy object point to different memory locations,
    //Check whether the corresponding member variables of both objects carry the same values,
    //Check whether all corresponding color codes are equal in both objects.
    if(!checkMem(art1, art2) && checkVars(art1, art2) && checkCodes(art1, art2)){
        return true;
    }else{
        return false;
    }
    
}

//Tests a copy to see if it is the same as the original and at a different location in memory but with the assignment operator now WOW
bool Tester::testAssignmentEdge(Art & art1, Art & art2){
    
    m_name = "testAssignmentEdge";
    //Check whether m_painting of original object and copy object point to different memory locations,
    //Check whether the corresponding member variables of both objects carry the same values,
    //Check whether all corresponding color codes are equal in both objects.
    //cout << "art 1 and 2 height " << art1.m_height << " " << art2.m_height << " art 1 and 2 width " << art1.m_width << " " << art2.m_width << endl;
    //cout << "art 1 and 2 mem " << art1.m_painting << " " << art2.m_painting << endl;
    if(art1.m_painting == nullptr && art2.m_painting == nullptr && checkVars(art1, art2) && art2.m_width == 0 && art2.m_height == 0){
        return true;
    }else{
        return false;
    }
    
}

//Tests a copy to see if it is the same as the original and at a different location in memory but with the assignment operator now WOW
bool Tester::testAssignmentChain(Art & art1, Art & art2, Art & art3){
    
    m_name = "testAssignmentChain";
    //Check whether m_painting of original object and copy object point to different memory locations,
    //Check whether the corresponding member variables of both objects carry the same values,
    //Check whether all corresponding color codes are equal in both objects.
    
    if(!checkMem(art1, art2) && !checkMem(art1, art3) && !checkMem(art2, art3)
       && checkVars(art1, art2) && checkVars(art2, art3) && checkVars(art1, art3) &&
       checkCodes(art1, art2) && checkCodes(art1, art3) && checkCodes(art2, art3)){
        
        return true;
        
    }else{
        return false;
    }
    
}

//Tests the normal case of appending one obejct onto another with Left2Right function
bool Tester::testLeft2RightNormal(Art & art1, Art & art2){
    
    m_name = "testLeft2RightNormal";
    
    //Used to check that width is correct at the end
    int finalWidth = art1.m_width + art2.m_width;
    
    //Make copy of art1 for checking
    Art art1Copy(art1);
    
    //Perform operation
    art1.left2Right(art2);
    
    //Variable used to return result of test
    bool result = true;
    
    //Check width to confirm correct size
    if(finalWidth != art1.m_width){
        
        result = false;
        
    }
    
    //Check left half
    for(int i = 0; i < art1.m_height; i++){
        for(int j = 0; j < art1Copy.m_width; j++){
            if(art1.m_painting[i][j] != art1Copy.m_painting[i][j]){
                result = false;
            }
        }
    }
    
    //Check right half
    for(int i = 0; i < art2.m_height; i++){
        for(int j = 0; j < art2.m_width; j++){
            if(art1.m_painting[i][j + art1Copy.m_width] != art2.m_painting[i][j]){
                result = false;

            }
        }
    }
        
    
    return result;
    
}

//Tests the error case of appending one obejct onto another with Left2Right function with different heights
bool Tester::testLeft2RightError(Art & art1, Art & art2){
    
    m_name = "testLeft2RightError";
    
    //Copy both arrays pre-operation
    Art art1Copy(art1);
    Art art2Copy(art2);
    
    //Perform the opertation
    art1.left2Right(art2);
    
    //Check to make sure objects are unchanged
    if(checkCodes(art1, art1Copy) && checkCodes(art2, art2Copy) && checkVars(art1, art1Copy) && checkVars(art2, art2Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of appending one obejct onto an empty object
bool Tester::testLeft2RightEdge(Art & art1, Art & art2){
    
    m_name = "testLeft2RightEdge";
    
    //Perform the opertation
    art1.left2Right(art2);
    
    //Check to make sure objects are now equal
    if(checkVars(art1, art2) && checkCodes(art1, art2)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of appending empty obejct onto normal one
bool Tester::testLeft2RightEdge2(Art & art1, Art & art2){
    
    m_name = "testLeft2RightEdge2";
    
    //Copy the normal object
    Art art1Copy(art1);
    
    //Perform the opertation
    art1.left2Right(art2);
    
    //Check to make sure object is unchanged
    if(checkVars(art1, art1Copy) && checkCodes(art1, art1Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of appending an object to itself
bool Tester::testLeft2RightSelfAppend(Art & art1){
    
    m_name = "testLeft2RightSelfAppend";
    bool result = true;
    
    //Store width to check later
    int originalWidth = art1.m_width;
    
    //Copy object
    Art art1Copy(art1);
    
    //Perform the opertation
    art1.left2Right(art1);
    
    //Check to make sure object has correct width
    if(art1.m_width == 2 * originalWidth){
        
        //Check left half to make sure codes are correct
        for(int i = 0; i < art1Copy.m_height; i++){
            for(int j = 0; j < art1Copy.m_width; j++){
                if(art1.m_painting[i][j] != art1Copy.m_painting[i][j]){
                    result = false;
                }
            }
        }
        
        //Check right half to make sure codes are correct
        for(int i = 0; i < art1Copy.m_height; i++){
            for(int j = 0; j < art1Copy.m_width; j++){
                if(art1.m_painting[i][j + art1Copy.m_width] != art1Copy.m_painting[i][j]){
                    result = false;
                }
            }
        }
        
        if(result){
            return true;
        }
        
    }
    
    return false;
    
}

//Tests the normal case of appending one object onto another with top2Bottom function
bool Tester::testTop2BottomNormal(Art & art1, Art & art2){
    
    m_name = "testTop2BottomNormal";
    
    //Used to check that height is correct at the end
    int finalHeight = art1.m_height + art2.m_height;
    
    //Make copy of art1 for checking
    Art art1Copy(art1);
    
    //Perform Operation
    art1.top2Bottom(art2);
    
    //Variable used to return result of test
    bool result = true;
    
    //Check width to confirm correct size
    if(finalHeight != art1.m_height){
        
        result = false;
        
    }
    
    //Check top half
    for(int i = 0; i < art1Copy.m_height; i++){
        for(int j = 0; j < art1Copy.m_width; j++){
            if(art1.m_painting[i][j] != art1Copy.m_painting[i][j]){
                result = false;
            }
        }
    }
    
    //Check bottom half
    for(int i = 0; i < art2.m_height; i++){
        for(int j = 0; j < art2.m_width; j++){
            if(art1.m_painting[i + art1Copy.m_height][j] != art2.m_painting[i][j]){
                result = false;

            }
        }
    }
    
    return result;
}

//Tests the error case of appending one object onto another with top2Bottom function where the objects have different widths
bool Tester::testTop2BottomError(Art & art1, Art & art2){
    
    m_name = "testTop2BottomError";
    
    //Copy the objects
    Art art1Copy(art1);
    Art art2Copy(art2);
    
    //Perform the operation
    art1.top2Bottom(art2);
    
    //Check that both objects are unchanged
    if(checkVars(art1, art1Copy) && checkVars(art2, art2Copy) && checkCodes(art1, art1Copy) && checkCodes(art2, art2Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of appending one object onto another with top2Bottom function where one object is empty
bool Tester::testTop2BottomEdge(Art & art1, Art & art2){
    
    m_name = "testTop2BottomEdge";
    
    //Perform the operation
    art1.top2Bottom(art2);
  
    //Check that both objects are now the same
    if(checkVars(art1, art2) && checkCodes(art1, art2)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the error case of appending one object onto another with top2Bottom function where the objects have different widths
bool Tester::testTop2BottomEdge2(Art & art1, Art & art2){
    
    m_name = "testTop2BottomEdge2";
    
    //Copy the objects
    Art art1Copy(art1);
    Art art2Copy(art2);
    
    //Perform the operation
    art1.top2Bottom(art2);
    
    //Check that both objects are unchanged
    if(checkVars(art1, art1Copy) && checkVars(art2, art2Copy) && checkCodes(art1, art1Copy) && checkCodes(art2, art2Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of appending an object to itself
bool Tester::testTop2BottomSelfAppend(Art & art1){
    
    m_name = "testTop2BottomSelfAppend";
    bool result = true;
    
    //Store height to check later
    int originalHeight = art1.m_height;
    
    //Copy object
    Art art1Copy(art1);
    
    //Perform the opertation
    art1.top2Bottom(art1);
    
    //Check to make sure object has correct height
    if(art1.m_height == 2 * originalHeight){
        
        //Check top half to make sure codes are correct
        for(int i = 0; i < art1Copy.m_height; i++){
            for(int j = 0; j < art1Copy.m_width; j++){
                if(art1.m_painting[i][j] != art1Copy.m_painting[i][j]){
                    result = false;
                }
            }
        }
        
        //Check right half to make sure codes are correct
        for(int i = 0; i < art1Copy.m_height; i++){
            for(int j = 0; j < art1Copy.m_width; j++){
                if(art1.m_painting[i  + art1Copy.m_height][j] != art1Copy.m_painting[i][j]){
                    result = false;
                }
            }
        }
        
        if(result){
            return true;
        }
        
    }
    
    return false;
    
}

//Tests the edge case of rotating an empty object
bool Tester::testRotateEdge(Art & art1){
    
    m_name = "testRotateEdge";
    
    //Copy the normal object
    Art art1Copy(art1);
    
    //Perform operation
    art1.rotate();
    
    //Check to make sure object is unchanged
    if(checkVars(art1, art1Copy) && checkCodes(art1, art1Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Tests the edge case of rotating an empty object
bool Tester::testRotateError(Art &art1, Art & art2){
    
    m_name = "testRotateError";
    
    //Copy the objects
    Art art1Copy(art1);
    Art art2Copy(art2);
    
    //Perform operations
    art1.rotate();
    art2.rotate();
    
    //Check to make sure both objects are unchanged
    if(checkVars(art1, art1Copy) && checkCodes(art1, art1Copy) && checkVars(art2, art2Copy) && checkCodes(art2, art2Copy)){
        
        return true;
        
    }
    
    return false;
    
}

//Runs a given painting through reverse and rotate, returns true if operations have been performed
bool Tester::performOperations(Art & art){
    
    if(art.rotate() && art.reverse()){
        return true;
    }else{
        return false;
    }
    
}

//Checks the memory addresses for two m_paintings for two art objects, outputs true if they are the same, false if they are not
bool Tester::checkMem(Art & art1, Art & art2){
    
    if(art1.m_painting == art2.m_painting){
        //cout << "memory locations are the same " << endl;
        return true;
    }else{
        return false;
    }
}

//Checks the member variables for two m_paintings for two art objects, outputs true if they are the same, false if they are not
bool Tester::checkVars(Art & art1, Art & art2){
    
    if(art1.m_height == art2.m_height && art1.m_width == art2.m_width){
        
        return true;
    }else{
        
        return false;
    }
    
}

//Checks the color codess for two m_paintings for two art objects, outputs true if they are the same, false if they are not
bool Tester::checkCodes(Art & art1, Art & art2){
    bool result = true;
    
    //Iterate through both arrays at the same time and compare color codes
    for(int i = 0; i < art1.m_height; i++){
        for(int j = 0; j < art1.m_width; j++){
            if(art1.m_painting[i][j] != art2.m_painting[i][j]){
                result = false;
            }
        }
    }
    
    return result;
}

//This function outputs to the console whether a test has passed or failed
void Tester::testOutput(bool tester){
    
    if(tester){
        cout << m_name << " has passed " << endl;
    }
    else{
        cout << m_name << " has failed " << endl;
    }
}
