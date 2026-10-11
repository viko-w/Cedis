#include "clock.hpp"
#include <ctime>
#include <string>

namespace cedis {

    Clock::Clock(){

    }
    Clock::~Clock(){
        
    }

    std::string Clock::getTimestamp(){
        time_t timestamp = time(&timestamp);
        struct tm datetime = *localtime(&timestamp);

        std::string hour = (datetime.tm_hour >= 10) ?
            std::to_string(datetime.tm_hour) : "0"+std::to_string(datetime.tm_hour);

        std::string min = (datetime.tm_min >= 10) ?
            std::to_string(datetime.tm_min) : "0"+std::to_string(datetime.tm_min);

        return hour+":"+min;

    }
}