#pragma once

#include <string>

namespace cedis{

    class Clock{
        public:
        Clock();
        ~Clock();
        
        std::string getTimestamp();
    };
}