#include "clock.hpp"
#include "tui.hpp"
#include <iostream>

#include "render/render.h"

void showtime(){
    cedis::Clock* clock = new cedis::Clock();
    std::cout << clock->getTimestamp() << '\n';
}

int main(){
    //cedis::Tui *tui = new cedis::Tui();

    cedis::Render *render = new cedis::Render();

    render->initWindow("shaders/default.vert", "shaders/default.frag");

    return 0;
}