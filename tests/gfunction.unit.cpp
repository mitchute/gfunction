#include <gtest/gtest.h>

# include "../src/gfunction.hpp"
# include "../third_party/nlohmann/json.hpp"


using namespace gfunction;

// for convenience
using json = nlohmann::json;

TEST(gfunction, test_buildField) {
    
    // init json object
    json j;

    // bh 1
    j["self"][0]["x"] = 0;
    j["self"][0]["y"] = 0;
    j["self"][0]["z"] = 1;
    j["self"][0]["d"] = 0.1;
    j["self"][0]["h"] = 100;

    // bh 2
    j["self"][1]["x"] = 0;
    j["self"][1]["y"] = 5;
    j["self"][1]["z"] = 1;
    j["self"][1]["d"] = 0.1;
    j["self"][1]["h"] = 100;

    // build field
    Field tst;
    tst.buildField(j["self"]);

    EXPECT_EQ(tst.boreholes.size(), 2);
    EXPECT_EQ(tst.totBHLength, 200);
    EXPECT_EQ(tst.aveBHLength, 100);
}

TEST(gfunction, test_calcDistances) {

    CartPoint pt_i(0, 0, 0);
    CartPoint pt_j(5, 0, 0);

    auto dists = calcDistances(pt_i, pt_j);

    EXPECT_EQ(dists.size(), 2);
    EXPECT_EQ(dists[0], 5);
    EXPECT_EQ(dists[1], 5);
}

TEST(gfunction, test_buildUHF) {
    // init json object
    json j;

    // bh 1
    j["self"][0]["x"] = 0;
    j["self"][0]["y"] = 0;
    j["self"][0]["z"] = 1;
    j["self"][0]["d"] = 0.1;
    j["self"][0]["h"] = 100;

    // bh 2
    j["self"][1]["x"] = 0;
    j["self"][1]["y"] = 5;
    j["self"][1]["z"] = 1;
    j["self"][1]["d"] = 0.1;
    j["self"][1]["h"] = 100;

    // soil
    j["soil"]["diffusivity"] = 1E-6;
    
    UHFgFunctions tst;
    tst.buildUHF(j);

    EXPECT_EQ(tst.selfField.boreholes.size(), 2);
    EXPECT_EQ(tst.crossField.boreholes.size(), 2);
}