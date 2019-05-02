#ifndef gfunction_hh_INCLUDED
#define gfunction_hh_INCLUDED

// C++ headers
# include <vector>
# include <stdio.h>

// project headers
# include "../third_party/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

namespace gfunction {

    struct SingleBorehole {

        double x;
        double y;
        double z;
        double h;
        double d;

        // constructor
        SingleBorehole(double _x, double _y, double _z, double _h, double _d) {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
            d = _d;
        }

        // default constructor
        SingleBorehole() :
            x(0), y(0), z(0), h(0), d(0) {}

        // destructor
        ~SingleBorehole() = default;
    };

    struct GHEField {
        std::string name;
        std::vector<SingleBorehole> boreholes;

        void buildField(const json& name, const json &arr);

        // destructor
        ~GHEField() = default;
    };

    struct UHFgFunctions {
        std::vector<GHEField> fields;

        void calcGFunctions();

        // destructor
        ~UHFgFunctions() = default;
    };
}
#endif // !gfunction
