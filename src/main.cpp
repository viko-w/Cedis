#include "clock.hpp"
#include <iostream>


int main(){
    cedis::Clock* clock = new cedis::Clock();
    std::cout << clock->getTimestamp() << '\n';

    return 0;
}