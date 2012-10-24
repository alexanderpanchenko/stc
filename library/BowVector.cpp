#include "BowVector.hpp"

BowVector::BowVector() {}

BowVector::~BowVector() { }


bool BowVector::increment_word(long id, long increment) {
	// Try to find existing entry
	map<long,long>::iterator it = this->find(id);
	if (it != this->end()){
		(*it).second = (*it).second + increment;
	}
	// Create new entry
	else {
		this->insert(make_pair(id, increment));
	}
	return true;
}

bool BowVector::increment_word(long id) {
	return increment_word(id, 1);
}

/**
 * Creates a string representing current BoW vector.
 * The vector representation can be either raw or
 * unit normalized if is_unit_length = true;
 * */
string BowVector::to_string(bool is_unit_length) {
	const char FEATURE_LENGTH = 50; // chars
	string feature_string; //libsvm feature vector
	double norm = (is_unit_length ? get_norm() : 1);

	map<long, long>::iterator it;
	for(it = this->begin(); it != this->end(); it++) {
	    char feature[FEATURE_LENGTH];
	    double value =  it->second / norm;
	    sprintf(feature, "%ld: %f ", it->first, value);
	    feature_string.append(feature);
	}

	return feature_string;
}


/**
 * Calculates L1 norm of the vector (euclidian norm).
 * norm = sqrt(sum_i(f_i^2))
 * */
double BowVector::get_norm(void) {
	double norm = 0;

	map<long, long>::iterator it;
	for(it = this->begin(); it != this->end(); it++) {
		norm += it->second * it->second;
	}
	norm = sqrt(norm);

	return norm;
}

/**
 * Multiply all values of the vector by a scalar
 * */
void BowVector::multiply(long scalar) {
	map<long, long>::iterator it;
	for(it = this->begin(); it != this->end(); it++) {
		it->second = it->second * scalar;
	}
}

void test_bowvector(void){
	BowVector* b = new BowVector();
	b->increment_word(10);
	b->increment_word(10);
	b->increment_word(10);
	b->increment_word(10);
	b->increment_word(20);
	b->increment_word(30);
	b->increment_word(20);
	b->increment_word(30);
	b->increment_word(50);
	b->increment_word(60);
	string s = b->to_string(false);
	cout << endl << "===" << s << "===" << endl;
	s = b->to_string(true);
	cout << endl << "===" << s << "===" << endl;

	b->multiply(4);
	s = b->to_string(false);
	cout << endl << "===" << s << "===" << endl;
	s = b->to_string(true);
	cout << endl << "===" << s << "===" << endl;

	b->increment_word(11);
	b->increment_word(22);
	b->increment_word(22);
	b->increment_word(33);
	b->increment_word(55);
	cout << endl << "===" << b->to_string(false) << "===" << endl;
	cout << endl << "===" << b->to_string(true) << "===" << endl;

	delete b;
}

void BowVector::add(BowVector* vector) {
	for(map<long,long>::iterator it = vector->begin(); it != vector->end(); it++) {
		this->increment_word(it->first, it->second);
	}
}






