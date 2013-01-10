#include "share_headers.hpp"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;


bool file_exists(const char* fileName, bool silent) {
	path p(fileName);
    if(exists(p)) {
    	return true;
    }
    else{
	  if(!silent) printf("Error:  file '%s' does not not exist!\n", fileName);
	  return false;
    }
}

bool file_exists(string fileName, bool silent) {
	path p(fileName);
	if(exists(p)) {
		return true;
	}
	else{
		if(!silent) printf("Error:  file '%s' does not not exist!\n", fileName.c_str());
			return false;
	}
}

/**
 * Return XML attribute of a label by a integer value of a label.
 * */
string get_label_name(int label) {
	switch (label) {
	case POSITIVE_CLASS_I:
		return POSITIVE_CLASS_ATT;
	case NEGATIVE_CLASS_I:
		return NEGATIVE_CLASS_ATT;
	default:
	    return NEGATIVE_CLASS_ATT;
	}
}


