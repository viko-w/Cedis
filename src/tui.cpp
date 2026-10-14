#include "cpptui.hpp"
#include "tui.hpp"
#include "clock.hpp"
#include <chrono>
#include <ctime>
#include <memory>
#include <thread>

namespace cedis {
    Tui::Tui(cpptui::Theme theme){

        fps = 24;
        refresh_rate = 1000 / fps;

        cpptui::Theme::set_theme(theme);
        cpptui::App app;
        auto root = std::make_shared<cpptui::Vertical>();

        Clock *clock = new Clock();

        auto time = std::make_shared<cpptui::Label>(clock->getTimestamp());
        
        root->add(time);
        root->add(std::make_shared<cpptui::VerticalSpacer>(1));

        //time = std::make_shared<cpptui::Label>(clock->getTimestamp());
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));

        app.add_timer(refresh_rate, [&]() {
            time->set_text(clock->getTimestamp());
        });

        app.register_exit_key('q');
        app.run(root);
    }

    Tui::~Tui(){

    }
}