// C++ headers
# include <iostream>
# include <fstream>
# include <sstream>

// project headers
# include "../third_party/nlohmann/json.hpp"
# include "gfunction.hpp"

// namespaces
using namespace std;

// shortcuts
using json = nlohmann::json;

int main(int argc, char *argv[])
{
	// read input file
	std::string ifPath = argv[1];
	std::ifstream t(ifPath);
	std::stringstream buffer;
	buffer << t.rdbuf();

	// read json input data
	auto j = json::parse(buffer.str());

	// init the UHF g-function class
	auto gFuncs = new gfunction::UHFgFunctions();

	// build the borehole fields
	int numFields = j.size();
	for (int idx = 0; idx < numFields; ++idx) {
        auto a = new gfunction::GHEField();
        gFuncs->Fields.push_back(a.buildField());
    }
    return 0;
}
