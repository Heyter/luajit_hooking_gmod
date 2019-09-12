#include <experimental/filesystem> // C++-standard header file name
#include <fstream>
#include "library/library.hpp"

using namespace std::experimental::filesystem::v1;

bool safe_file_write(path file, const std::string& code, std::string comment) {
	path parent = file.parent_path();
	bool created = create_directories(parent);

	if (exists(parent) || created) {
		auto ofluafile = std::ofstream(file, std::ofstream::app);
		ofluafile << "-- " << comment << "\n";
		ofluafile << code << "\n\n";
		ofluafile.close();
		return true;
	}

	return false;
}