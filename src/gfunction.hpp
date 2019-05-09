#ifndef gfunction_hh_INCLUDED
#define gfunction_hh_INCLUDED

// C++ headers
#include <vector>
#include <stdio.h>

// project headers
#include "../third_party/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

namespace gfunction {

     struct CartPoint {
        double x = 0;
        double y = 0;
        double z = 0;

        // constructor
        CartPoint(int _x, int _y, int _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        // default constructor
        CartPoint() = default;

        // destructor
        ~CartPoint() = default;
    };

    struct Props {
        double diffusivity = 0;

        // default constructor
        Props() = default;

        // destructor
        ~Props() = default;
    };

    struct Borehole {

        // member variables
        int bhNo = 0;
        double x = 0;
        double y = 0;
        double z = 0;
        double h = 0;
        double d = 0;
        double dl_i = 0;
        double dl_ii = 0;
        double dl_j = 0;
        std::vector<CartPoint> ptLocs_i;
        std::vector<CartPoint> ptLocs_ii;
        std::vector<CartPoint> ptLocs_j;

        // constructor
        Borehole(double _x, double _y, double _z, double _h, double _d) {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
            d = _d;
        }

        // default constructor
        Borehole() = default;

        // destructor
        ~Borehole() = default;

    };

    struct Field {
        int numBH = 0;
        std::vector<Borehole> boreholes;
        double totBHLength = 0;
        double aveBHLength = 0;

        // default constructor
        Field() = default;

        // destructor
        ~Field() = default;

        // member functions
        void buildField(const json& _j);
    };

    struct UHFgFunctions {
        Field selfField;
        Field crossField;
        Props soil;
        std::vector<double> lntts;
        std::vector<double> gfcnSelf;
        std::vector<double> gfcnCross;

        // default constructor
        UHFgFunctions() = default;

        // destructor
        ~UHFgFunctions() = default;

        // member functions
        void buildUHF(const json &_j);
        double calcResponse(std::vector<double> const &dists, double const &currTime);
        double integral(CartPoint const &point_i, Borehole const &bh_j, double const &currTime);
        double doubleIntegral(Borehole const &bh_i, Borehole const &bh_j, double const &currTime);
        void calc_gFunctions();
        void write_gFunctions(const std::string &fpath);
    };

    std::vector<double> calcDistances(CartPoint const& point_i, CartPoint const& point_j);

}
#endif // !gfunction
