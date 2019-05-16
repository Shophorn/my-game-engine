#include <string>
#include <fstream>
#include <sstream>

#include "file.hpp"

std::string file::readText(std::string path)
{
	using namespace std;

	ifstream file (path, ios::in | ios::binary);
	stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}