#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include "../src/gfunction.hpp"
#include "../third_party/nlohmann/json.hpp"


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

    // bh 3
    j["cross"][0]["x"] = 5;
    j["cross"][0]["y"] = 0;
    j["cross"][0]["z"] = 1;
    j["cross"][0]["d"] = 0.1;
    j["cross"][0]["h"] = 100;

    // bh 4
    j["cross"][1]["x"] = 5;
    j["cross"][1]["y"] = 5;
    j["cross"][1]["z"] = 1;
    j["cross"][1]["d"] = 0.1;
    j["cross"][1]["h"] = 100;

    // soil
    j["soil"]["diffusivity"] = 1E-6;
    
    UHFgFunctions tst;
    tst.buildUHF(j);

    EXPECT_EQ(tst.selfField.boreholes.size(), 2);
    EXPECT_EQ(tst.crossField.boreholes.size(), 2);
}

TEST(gfunction, test_calc_gFunctions) {

    // init json object
    json j;

    // bh 1
    j["self"][0]["x"] = 0;
    j["self"][0]["y"] = 0;
    j["self"][0]["z"] = 1;
    j["self"][0]["d"] = 0.1;
    j["self"][0]["h"] = 100;

    // bh 2
    j["cross"][0]["x"] = 0;
    j["cross"][0]["y"] = 5;
    j["cross"][0]["z"] = 1;
    j["cross"][0]["d"] = 0.1;
    j["cross"][0]["h"] = 100;

    // bh 3
    j["cross"][1]["x"] = 5;
    j["cross"][1]["y"] = 0;
    j["cross"][1]["z"] = 1;
    j["cross"][1]["d"] = 0.1;
    j["cross"][1]["h"] = 100;

    // bh 4
    j["cross"][2]["x"] = 5;
    j["cross"][2]["y"] = 5;
    j["cross"][2]["z"] = 1;
    j["cross"][2]["d"] = 0.1;
    j["cross"][2]["h"] = 100;

    // soil
    j["soil"]["diffusivity"] = 1E-6;

    // build field
    UHFgFunctions tst;
    tst.buildUHF(j);

    // calc g-functions
    tst.calc_gFunctions();

    // test lntts
    ASSERT_THAT(tst.lntts, testing::ElementsAre(-8.5, -8.0, -7.5, -7.0, -6.5, -6.0, -5.5, -5.0,
                                                -4.5, -4.0, -3.5, -3.0, -2.5, -2.0, -1.5, -1.0,
                                                -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0));

    // test self g-functions
    EXPECT_NEAR(tst.gfcnSelf[0], 2.58793, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[1], 2.83225, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[2], 3.07665, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[3], 3.32087, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[4], 3.56463, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[5], 3.80758, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[6], 4.04925, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[7], 4.28905, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[8], 4.52626, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[9], 4.76001, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[10], 4.98916, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[11], 5.21232, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[12], 5.42771, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[13], 5.63303, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[14], 5.8254, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[15], 6.00108, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[16], 6.15543, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[17], 6.28333, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[18], 6.38118, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[19], 6.44927, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[20], 6.49229, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[21], 6.51724, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[22], 6.53076, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[23], 6.53773, 0.00001);

    // test cross g-functions
    EXPECT_NEAR(tst.gfcnCross[0], 3.37372e-14, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[1], 2.88617e-09, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[2], 3.37823e-06, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[3], 0.000290268, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[4], 0.00508234, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[5], 0.0338889, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[6], 0.125265, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[7], 0.318335, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[8], 0.630175, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[9], 1.05045, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[10], 1.55211, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[11], 2.10415, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[12], 2.67854, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[13], 3.25191, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[14], 3.80438, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[15], 4.31765, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[16], 4.77329, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[17], 5.15324, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[18], 5.44503, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[19], 5.64857, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[20], 5.77735, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[21], 5.85208, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[22], 5.8926, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[23], 5.91353, 0.00001);
}