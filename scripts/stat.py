#!/usr/bin/python

#############################################################
# Count statistics on a training dataset 
# Input: training/test XML file with texts (train.xml, test.xml, etc.)
# Output: statistics about the input dataset (the standard output)
############################################################

import sys
import csv
#import linecache
#import random
import os
import re
import string
from numpy import array

def calc_stat(xml_fpath):
	xml_file = open(xml_fpath, "r")
	lemmas_freq = {}
	tokens_freq = {}
	pos_freq = {}
	lemmas_sent_freq = []
	lemmas_sentnon_freq = []

	for line in xml_file:        
        	# Skip the void lines
	        if line.strip() == "": continue
		
		# Get the parsed text
		match = re.search("<lemmas>(.*)</lemmas>", line)
		if match:
	        	raw = match.group(1)

			# Split into words "lemma#POS#token"
			words = raw.split()

			# Save sentence length
			if len(words) > 0: 
				lemmas_sent_freq.append(len(words))
				lemmas_sentnon_freq.append(nonstop_freq(words))

			# Build frequency dictionary
			for w in words:
				elements = w.split('#')
				if len(elements) == 3:
					token = elements[0]
					pos = elements[1]
					lemma = elements[2]

					if not token in tokens_freq:
						tokens_freq[token] = 1
					else:
						tokens_freq[token] = tokens_freq[token] + 1

					if not pos in pos_freq:
						pos_freq[pos] = 1
					else:
						pos_freq[pos] = pos_freq[pos] + 1

					if not lemma in lemmas_freq:
						lemmas_freq[lemma] = 1
					else:
						lemmas_freq[lemma] = lemmas_freq[lemma] + 1
	
	xml_file.close()

	# Save the results
	save_freq_dict(tokens_freq, xml_fpath + ".tokens-freq")
	save_freq_dict(pos_freq, xml_fpath + ".pos-freq")
	save_freq_dict(lemmas_freq, xml_fpath + ".lemmas-freq")
	list_stat(lemmas_sent_freq, "Sentence")
	list_stat(lemmas_sentnon_freq, "Sentence w/o stopwords")


def list_stat(input_list, message):
	nums = array(input_list)
	print message, ": mean =",  nums.mean(), ", std =", nums.std()

def nonstop_freq(words):
	nonstop_count = 0
	for w in words:
		e = w.split("#")
		if len(e) != 3: continue
		if not e[2] in _stoplist:
			nonstop_count = nonstop_count + 1
		#else:
		#	print e[2]
	return nonstop_count

def save_freq_dict(freq_dict, fpath):
	tokens_file = open(fpath, "w")
	for key in sorted(freq_dict, key=freq_dict.get, reverse=True):
		tokens_file.write(str(key) + ";" + str(freq_dict[key]) + "\n")
	tokens_file.close()
	print "Saved ", len(freq_dict), " keys to: ", fpath

def load_stoplist(fname):
	"""Reads a stoplist from a utf-8 CSV file 'fname' in the follwing format: 'lemma'
	"""
	lemmas_file = open(fname, "r")
	lemmas = {}
	for line in lemmas_file: lemmas[line.strip()] = 1 
	print len(lemmas), "stopwords loaded"

	#for i, l in enumerate(lemmas):
	#	if i < 30:
	#		print "==%s==%s==" % (l, lemmas[l])
	#lemmas_file.close()
	return lemmas


#####################################
# Entry point
#####################################
#Process parameters
PARAM_NUM = 1
if len(sys.argv) < PARAM_NUM + 1:
    print "Expected", PARAM_NUM, "parameters but was", str(len(sys.argv)-1)
    print "Usage:", sys.argv[0], "<xml-file>"
    sys.exit()

# Read the command line arguments
xml_fpath = sys.argv[1]
_stoplist = load_stoplist("./../data/gallery/stopwords.csv")
calc_stat(xml_fpath)

print "Script has finished successfully."
