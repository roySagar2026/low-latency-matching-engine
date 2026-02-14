#pragma once
#include <string>
#include "position_manager.h"

class ReplayEngine {
    public:
        explicit ReplayEngine(const std::string& file);
        void replay(PositionManager& pm);

    private:
        std:: string filename;
};