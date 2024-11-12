#include "class.h" // include the header file 

using namespace std; // you may include this in .cpp file for easier code writing 
// you should NOT include this in .h file

Class::Class() { 
    // constructor 
    // initialize any variable here if needed 
    std::cout << "Constructor of Class\n"; 
}

Class::~Class() {
    // destructor
    // free any memory left here 
    // clean up anything else if needed 
}

// implement methods here 
int Class::method1(int arg1, char arg2){
    std::cout << "Inside method1 of Class\n"; 
    return 0; 
}

int Class::method2(int arg1, char arg2){
    std::cout << "Inside method1 of Class\n"; 
    return 0; 
}
