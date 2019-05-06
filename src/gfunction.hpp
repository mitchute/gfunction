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

    struct CartPoint {
        double x = 0;
        double y = 0;
        double z = 0;

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
        int fieldNo = 0;
        int bhNo = 0;
        double x = 0;
        double y = 0;
        double z = 0;
        double h = 0;
        double d = 0;
        double dl_i;
        double dl_ii;
        double dl_j;
        std::vector<CartPoint> ptLocs_i;
        std::vector<CartPoint> ptLocs_ii;
        std::vector<CartPoint> ptLocs_j;

        // constructor
        Borehole(int _fieldNo, int _bhNo, double _x, double _y, double _z, double _h, double _d) {
            fieldNo = _fieldNo;
            bhNo = _bhNo;
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

    struct UHFgFunctions {
        std::vector<Borehole> boreholes;
        Props soil;
        double len_self = 0;
        double len_cross = 0;
        std::vector<double> lntts;
        std::vector<double> gfcn_self;
        std::vector<double> gfcn_self_to_cross;
        std::vector<double> gfcn_cross;
        std::vector<double> gfcn_cross_to_self;

        // default constructor
        UHFgFunctions() = default;

        // destructor
        ~UHFgFunctions() = default;

        // member functions
        void buildUHF(const json& _j_cross);
        double calcResponse(std::vector<double> const dists, double const currTime);
        double integral(CartPoint const &point_i, Borehole const &bh_j, double const &currTime);
        double doubleIntegral(Borehole const &bh_i, Borehole const &bh_j, double const &currTime);
        void calc_gFunctions();
        void write_gFunctions(const std::string &fpath);
    };
}
#endif // !gfunction
