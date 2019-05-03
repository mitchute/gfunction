// C++ headers
# include <iostream>
# include <stdio.h>
# include <vector>

// project headers
# include "../third_party/nlohmann/json.hpp"
# include "gfunction.hpp"

// namespaces
using namespace std;

// for convenience
using json = nlohmann::json;

namespace gfunction {


int num_fields = 0;
int num_bh = 0;

    std::shared_ptr<GHEField> GHEField::buildField (const std::string _name, const json& _arr) {
        this->name = static_cast<std::string>(_name);
        this->field_no = num_fields;

        for (auto data : _arr) {
            // build borehole instance
            double x = static_cast<double>(data["x"]);
            double y = static_cast<double>(data["y"]);
            double z = static_cast<double>(data["z"]);
            double d = static_cast<double>(data["d"]);
            double h = static_cast<double>(data["h"]);
            auto bh = std::shared_ptr<SingleBorehole>(new SingleBorehole(num_fields, num_bh, x, y, z, h, d));
            this->boreholes.push_back(bh);
            this->h_total += h;
            ++num_bh;
        }
        ++num_fields;

        return std::shared_ptr<GHEField>(this);
    }

    void UHFgFunctions::buildUHF(const json& _j_self, const json& _j_cross) {
        
        // build the borehole fields
        GHEField self;
        this->selfField = self.buildField("self", _j_self);
        
        GHEField cross;
        this->crossField = cross.buildField("cross", _j_cross);    

    };

    void UHFgFunctions::calc_gFunctions() {
        cout << "Computing g-functions.";
    }
}
