#ifndef BOWVECTOR_H_
#define BOWVECTOR_H_

#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "share_headers.hpp"

using namespace std;

/**
 * This class represents a bag-of-words vector "id-freq",
 * where id is termID and freq is absolute term frequency.
 * */
class BowVector : public map<long, long> {
public:
	BowVector();
	bool increment_word(long id);
	bool increment_word(long id, long increment);
	void multiply(long scalar);
	string to_string(bool is_unit_length);
	void add(BowVector* vector);
	virtual ~BowVector();
private:
	double get_norm(void);
};

#endif /* BOWVECTOR_H_ */
