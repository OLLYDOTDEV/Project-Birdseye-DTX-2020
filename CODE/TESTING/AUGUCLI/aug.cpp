// testing CLI input via cmd aug
// http://www.cplusplus.com/articles/DEN36Up4/#:~:text=11-,%23include%20int%20main(%20int%20argc%2C%20char,of%20the%20move%20function%20is
#include <iostream>

int main(int argc, char* argv[])
{
    // Check the number of parameters
    if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " NAME" << std::endl;
        /* "Usage messages" are a conventional way of telling the user
         * how to run a program if they enter the command incorrectly.
         */
        return 1;
    }
    // Print the user's name:
    std::cout << argv[0] << "says hello, " << argv[1] << "!" << std::endl;
    return 0;
}