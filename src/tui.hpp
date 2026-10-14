#include "cpptui.hpp"
#include <memory>

namespace cedis {
    class Tui{
        public:
        Tui(cpptui::Theme theme = cpptui::Theme::Nord());
        ~Tui();

        int fps;

        private:
        int refresh_rate;

    };
}