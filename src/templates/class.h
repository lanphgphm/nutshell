#ifndef CLASS_H // these are include "guards"
#define CLASS_H

#include<iostream> 
#include<unistd.h> 
// include all library you need here

class Class { 
public: 
    Class();  // constructor
    ~Class(); // destructor

    int method1(int arg1, char arg2); 

private:
    int variable1; 
    char variable2; 

    int method2(int arg1, char arg2); 
}; 

#endif // CLASS_H