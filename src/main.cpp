#include "clock.hpp"
#include "tui.hpp"
#include <iostream>

void showtime(){
    cedis::Clock* clock = new cedis::Clock();
    std::cout << clock->getTimestamp() << '\n';
}

int main(){
    cedis::Tui *tui = new cedis::Tui();
    return 0;
}