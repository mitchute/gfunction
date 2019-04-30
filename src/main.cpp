// C++ headers
# include <iostream>
# include <fstream>
# include <sstream>

// project headers
# include "../third_party/nlohmann/json.hpp"

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
	
    cout << j["answer"]["everything"];
    return 0;
}
