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

    EXPECT_EQ(tst.boreholes.size(), 2u);
    EXPECT_EQ(tst.totBHLength, 200);
    EXPECT_EQ(tst.aveBHLength, 100);
}

TEST(gfunction, test_calcDistances) {

    CartPoint pt_i(0, 0, 0);
    CartPoint pt_j(5, 0, 0);

    auto dists = calcDistances(pt_i, pt_j);

    EXPECT_EQ(dists.size(), 2u);
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

    EXPECT_EQ(tst.selfField.boreholes.size(), 2u);
    EXPECT_EQ(tst.crossField.boreholes.size(), 2u);
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
    EXPECT_NEAR(tst.gfcnSelf[0], 2.63599, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[1], 2.88371, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[2], 3.13139, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[3], 3.37867, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[4], 3.62517, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[5], 3.87052, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[6], 4.11429, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[7], 4.35596, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[8], 4.59485, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[9], 4.8301, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[10], 5.06063, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[11], 5.28507, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[12], 5.50164, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[13], 5.7081, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[14], 5.90155, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[15], 6.07828, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[16], 6.23363, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[17], 6.36249, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[18], 6.46118, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[19], 6.52996, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[20], 6.57347, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[21], 6.59872, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[22], 6.61242, 0.00001);
    EXPECT_NEAR(tst.gfcnSelf[23], 6.61949, 0.00001);

    // test cross g-functions
    EXPECT_NEAR(tst.gfcnCross[0], 3.42122e-14, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[1], 2.92603e-09, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[2], 3.42285e-06, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[3], 0.000293813, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[4], 0.00513842, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[5], 0.0342254, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[6], 0.126392, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[7], 0.320959, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[8], 0.634985, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[9], 1.05797, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[10], 1.56264, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[11], 2.11782, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[12], 2.6954, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[13], 3.27193, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[14], 3.82752, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[15], 4.34383, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[16], 4.80245, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[17], 5.18524, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[18], 5.47955, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[19], 5.68513, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[20], 5.81537, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[21], 5.89103, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[22], 5.93209, 0.00001);
    EXPECT_NEAR(tst.gfcnCross[23], 5.9533, 0.00001);
}

TEST(gfunction, test_isEven) {
    EXPECT_FALSE(isEven(1));
    EXPECT_TRUE(isEven(2));
}