#include "cpptui.hpp"
#include "tui.hpp"
#include "clock.hpp"
#include <memory>

namespace cedis {
    Tui::Tui(cpptui::Theme theme){
        cpptui::Theme::set_theme(theme);
        cpptui::App app;
        auto root = std::make_shared<cpptui::Vertical>();

        Clock *clock = new Clock();

        auto time = std::make_shared<cpptui::Label>(clock->getTimestamp());

        root->add(time);
        root->add(std::make_shared<cpptui::VerticalSpacer>(1));
        app.run(root);
    }

    Tui::~Tui(){

    }
}