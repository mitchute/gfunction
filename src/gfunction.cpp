// C++ headers
# include <iostream>
# include <stdio.h>
# include <vector>

// project headers
# include "gfunction.hpp"

// for convenience
using json = nlohmann::json;

using namespace std;

namespace gfunction {

    void GHEField::buildField (const json& name, const json& arr) {
        this->name = name;

        for (auto data : arr) {
            // build borehole instance
            double x = static_cast<double>(data["x"]);
            double y = static_cast<double>(data["y"]);
            double z = static_cast<double>(data["z"]);
            double d = static_cast<double>(data["d"]);
            double h = static_cast<double>(data["h"]);
            this->boreholes.push_back(SingleBorehole(x, y, z, h, d));
        }
    }

    void UHFgFunctions::calcGFunctions() {
        cout << "Making g-functions.";
    }
}
