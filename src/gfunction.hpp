# include <vector>
# include <stdio.h>

namespace gfunction {

    class SingleBorehole {

    public:
        float x;
        float y;
        float z;
        float h;
        float d;

        // default constructor
        SingleBorehole() :
                x(0), y(0), z(0), h(0), d(0) {}

        // destructor
        ~SingleBorehole() = default;
    };

    class GHEField {

    public:
        std::vector<SingleBorehole> boreholes;

        void buildField();

        // destructor
        ~GHEField() = default;
    };

    class UHFgFunctions {

    public:
        std::vector<GHEField> Fields;

        void calcGFunctions();

        // destructor
        ~UHFgFunctions() = default;
    };
}