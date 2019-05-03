// C++ headers
# include <iostream>
#include <exception>
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

    inline bool isEven(int const val)
    {
        if (val % 2 == 0) {
            return true;
        } else {
            return false;
        }
    }

    std::vector<double> linspace(float start, float stop, float step) {
        std::vector<double> retVect;
        double val = start;
        while (val != stop) {
            retVect.push_back(val);
            val += step;
        }
        return retVect;
    }

    std::vector<double> Borehole::calcDistances(CartPoint const& point_i, CartPoint const& point_j) {
        std::vector<double> sumVals;

        // Calculate the distance between points
        sumVals.push_back(pow(point_i.x - point_j.x, 2.0));
        sumVals.push_back(pow(point_i.y - point_j.y, 2.0));
        sumVals.push_back(pow(point_i.z - point_j.z, 2.0));

        double sumTot = 0;
        std::vector<double> retVals;
        std::for_each(sumVals.begin(), sumVals.end(), [&](double n) { sumTot += n; });
        retVals.push_back(std::sqrt(sumTot));

        // Calculate distance to mirror point
        sumVals.pop_back();
        sumVals.push_back(pow(point_i.z - (-point_j.z), 2.0));

        sumTot = 0;
        std::for_each(sumVals.begin(), sumVals.end(), [&](double n) { sumTot += n; });
        retVals.push_back(std::sqrt(sumTot));

        return retVals;
    }

    void UHFgFunctions::buildUHF(const json& _j) {
        
        int numBH = 0;
        int fieldNo = 0;

        // build self-field
        for (auto data : _j["self"]) {
            // build borehole instance
            double x = static_cast<double>(data["x"]);
            double y = static_cast<double>(data["y"]);
            double z = static_cast<double>(data["z"]);
            double d = static_cast<double>(data["d"]);
            double h = static_cast<double>(data["h"]);
            auto bh = Borehole(fieldNo, numBH, x, y, z, h, d);
            this->boreholes.push_back(bh);
            this->len_self += h;
            ++numBH;
        }

        // build cross-field
        for (auto data : _j["cross"]) {
            double x = static_cast<double>(data["x"]);
            double y = static_cast<double>(data["y"]);
            double z = static_cast<double>(data["z"]);
            double d = static_cast<double>(data["d"]);
            double h = static_cast<double>(data["h"]);
            auto bh = Borehole(fieldNo, numBH, x, y, z, h, d);
            this->boreholes.push_back(bh);
            this->len_cross += h;
            ++numBH;
        }

        // soil data
        this->soil.diffusivity = _j["soil"]["diffusivity"];

        // setup borehole integration point locations
        int numBH_i_points = 50;
        int numBH_ii_points = 50;
        int numBH_j_points = 560;
        
        // Using Simpson's rule the number of points (n+1) must be odd, therefore an even number of panels is required
        // Starting from i = 0 to i <= NumPanels produces an odd number of points
        int numPanels_i = 50;
        int numPanels_ii = 50;
        int numPanels_j = 560;

        for (auto bh = this->boreholes.begin(); bh != this->boreholes.end(); ++bh) {

            bh->dl_i = bh->h / numPanels_i;
            for (int i = 0; i <= numPanels_i; ++i) {
                CartPoint newPoint;
                newPoint.x = bh->x;
                newPoint.y = bh->y;
                newPoint.z = bh->z + (i * bh->dl_i);
                bh->ptLocs_i.push_back(newPoint);
            }

            bh->dl_ii = bh->h / numPanels_ii;
            for (int i = 0; i <= numPanels_ii; ++i) {
                CartPoint newPoint;
                // For case when bh is being compared to itself, shift points by 1 radius in the horizontal plane
                newPoint.x = bh->x + (bh->d / 2.0) / sqrt(2.0);
                newPoint.y = bh->y + (bh->d / 2.0) / (-sqrt(2.0));
                newPoint.z = bh->z + (i * bh->dl_ii);
                bh->ptLocs_ii.push_back(newPoint);
            }

            bh->dl_j = bh->h / numPanels_j;
            for (int i = 0; i <= numPanels_j; ++i) {
                CartPoint newPoint;
                newPoint.x = bh->x;
                newPoint.y = bh->y;
                newPoint.z = bh->z + (i * bh->dl_j);
                bh->ptLocs_j.push_back(newPoint);
            }
        }
    };

    std::vector<double> UHFgFunctions::distances(CartPoint const point_i, CartPoint const point_j) {

        std::vector<double> sumVals;

        // Calculate the distance between points
        sumVals.push_back(pow(point_i.x - point_j.x, 2.0));
        sumVals.push_back(pow(point_i.y - point_j.y, 2.0));
        sumVals.push_back(pow(point_i.z - point_j.z, 2.0));

        double sumTot = 0;
        std::vector<double> retVals;
        std::for_each(sumVals.begin(), sumVals.end(), [&](double n) { sumTot += n; });
        retVals.push_back(std::sqrt(sumTot));

        // Calculate distance to mirror point
        sumVals.pop_back();
        sumVals.push_back(pow(point_i.z - (-point_j.z), 2.0));

        sumTot = 0;
        std::for_each(sumVals.begin(), sumVals.end(), [&](double n) { sumTot += n; });
        retVals.push_back(std::sqrt(sumTot));

        return retVals;
    }

    double UHFgFunctions::calcResponse(std::vector<double> const dists, double const currTime)
    {
        double pointToPointResponse = 0;
        double pointToReflectedResponse = 0;

        try {
            pointToPointResponse = erfc(dists[0] / (2 * sqrt(soil.diffusivity * currTime))) / dists[0];
        }
        catch (const std::exception & e) {
            pointToPointResponse = 0;
        }

        try {
            pointToReflectedResponse = erfc(dists[1] / (2 * sqrt(soil.diffusivity * currTime))) / dists[1];
        }
        catch (const std::exception & e) {
            pointToReflectedResponse = 0;
        }

        return pointToPointResponse - pointToReflectedResponse;
    }

    double UHFgFunctions::integral(CartPoint const point_i, Borehole const bh_j, double const currTime) {

        // This code could be optimized in a number of ways.
        // The first, most simple way would be to precompute the distances from point i to point j, then store them for reuse.
        // The second, more intensive method would be to break the calcResponse calls out into four different parts.
        // The first point, last point, odd points, and even points. Then multiply the odd/even points by their respective coefficient for the
        // Simpson's method. After that, all points are summed together and divided by 3.

        double sum_f = 0;
        int index = 0;
        size_t const lastIndex_j = bh_j.ptLocs_j.size() - 1;
        for (auto& point_j : bh_j.ptLocs_j) {
            std::vector<double> dists = distances(point_i, point_j);
            double const f = calcResponse(dists, currTime);

            // Integrate using Simpson's
            if (index == 0 || index == lastIndex_j) {
                sum_f += f;
            }
            else if (isEven(index)) {
                sum_f += 2 * f;
            }
            else {
                sum_f += 4 * f;
            }

            ++index;
        }

        return (bh_j.dl_j / 3.0) * sum_f;
    }

    double UHFgFunctions::doubleIntegral(Borehole const bh_i, Borehole const bh_j, double const currTime) {

        // Similar optimizations as discussed above could happen here

        if (bh_i.bhNo == bh_j.bhNo) {

            double sum_f = 0;
            int index = 0;
            size_t const lastIndex = bh_i.ptLocs_i.size() - 1;
            for (auto& thisPoint : bh_i.ptLocs_ii) {

                double f = integral(thisPoint, bh_j, currTime);

                // Integrate using Simpson's
                if (index == 0 || index == lastIndex) {
                    sum_f += f;
                } else if (isEven(index)) {
                    sum_f += 2 * f;
                } else {
                    sum_f += 4 * f;
                }
                ++index;
            }
            return (bh_i.dl_ii / 3.0) * sum_f;
        }
        else {

            double sum_f = 0;
            int index = 0;
            size_t const lastIndex = bh_i.ptLocs_i.size() - 1;
            for (auto& thisPoint : bh_i.ptLocs_i) {

                double f = integral(thisPoint, bh_j, currTime);

                // Integrate using Simpson's
                if (index == 0 || index == lastIndex) {
                    sum_f += f;
                } else if (isEven(index)) {
                    sum_f += 2 * f;
                } else {
                    sum_f += 4 * f;
                }
                ++index;
            }
            return (bh_i.dl_i / 3.0) * sum_f;
        }
    }

    void UHFgFunctions::calc_gFunctions() {

        cout << "Computing self-g-functions" << std::endl;
        
        // time constant
        double ts = pow(this->len_self, 2.0) / (9.0 * this->soil.diffusivity);

        // log(t/ts)
        std::vector<double> lntts = linspace(-8.5, 3.5, 0.5);

        // time in seconds
        std::vector<double> time;
        for (auto it = lntts.begin(); it != lntts.end(); ++it) {
            time.push_back(std::exp(*it) * ts);
        }

        // g-function values
        this->gfcn_self = std::vector<double> (time.size(), 0.0);
        this->gfcn_self_to_cross = std::vector<double> (time.size(), 0.0);
        this->gfcn_cross = std::vector<double> (time.size(), 0.0);
        this->gfcn_cross_to_self = std::vector<double> (time.size(), 0.0);

        // compute the responses
        for (size_t idx = 1; idx <= lntts.size(); ++idx) {
            for (auto& bh_i : boreholes) {
                int i_field = bh_i.fieldNo;
                for (auto& bh_j : boreholes) {
                    int j_field = bh_j.fieldNo;
                    double sum_T_j = doubleIntegral(bh_i, bh_j, time[idx]);
                    if ((i_field == 0) && (j_field == 0)) {
                        gfcn_self[idx] += sum_T_j;
                    } else if ((i_field == 1) && (j_field == 1)) {
                        gfcn_self_to_cross[idx] += sum_T_j;
                    } else if ((i_field == 0) && (j_field == 1)) {
                        gfcn_cross[idx] += sum_T_j;
                    } else if ((i_field == 1) && (j_field == 0)) {
                        gfcn_cross_to_self[idx] += sum_T_j;
                    }
                }
            }
        }

        // convert to g-functions
        for (size_t idx = 1; idx <= lntts.size(); ++idx) {
            gfcn_self[idx] /= (2 * len_self);
            gfcn_self_to_cross[idx] /= (2 * len_cross);
            gfcn_cross[idx] /= (2 * len_self);
            gfcn_cross_to_self[idx] /= (2 * len_cross);
        }
    }
}
