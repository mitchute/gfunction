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

        // member variables
        int field_no = 0;
        int bh_no = 0;
        double x = 0;
        double y = 0;
        double z = 0;
        double h = 0;
        double d = 0;

        // constructor
        SingleBorehole(int _field_no, int _bh_no, double _x, double _y, double _z, double _h, double _d) {
            field_no = _field_no;
            bh_no = _bh_no;
            x = _x;
            y = _y;
            z = _z;
            h = _h;
            d = _d;
        }

        // default constructor
        SingleBorehole() = default;

        // destructor
        ~SingleBorehole() = default;
    };

    struct GHEField {
        std::string name;
        int field_no = 0;
        double h_total = 0;
        std::vector<std::shared_ptr<gfunction::SingleBorehole>> boreholes;

        // default constructor
        GHEField() = default; 

        // destructor
        ~GHEField() = default;
        
        // member functions
        std::shared_ptr<GHEField> buildField(const std::string _name, const json &_arr);
    };

    struct UHFgFunctions {
        std::shared_ptr<GHEField> selfField;
        std::shared_ptr<GHEField> crossField;

        // default constructor
        UHFgFunctions() = default;

        // destructor
        ~UHFgFunctions() = default;

        // member functions
        void buildUHF(const json& _j_self, const json& _j_cross);
        void calc_gFunctions();
    };
}
#endif // !gfunction
