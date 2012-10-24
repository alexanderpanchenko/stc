#!/usr/bin/python

import sys
import linecache
import random
import os
from lemmatizer import lemmatize_file

def cat_files(input1_fpath, input2_fpath, output_fpath, prefix_line = "", postfix_line = ""):
    output_file = open(output_fpath, "w")
    if(prefix_line != ""):
        output_file.write(prefix_line + "\n")
    
    input1_file = open(input1_fpath, "r")
    output_file.write(input1_file.read())
    input1_file.close()
    
    input2_file = open(input2_fpath, "r")
    output_file.write(input2_file.read())
    input2_file.close()
    
    if(postfix_line != ""):
        output_file.write(postfix_line)    
    output_file.close()
                
def split_csv_texts(input_fpath, output1_fpath, output2_fpath, split_percent):
    # Check the parameters 
    if (not os.path.exists(input_fpath)) or (split_percent < 0) or (split_percent > 1):
        print "Error: wrong input arguments."
        return
    	
    # Open the files
    input_file = open(input_fpath, "r")
    output1_file = open(output1_fpath,"w")
    output2_file = open(output2_fpath,"w")
    
    # Get number of lines 
    input_number = len(input_file.readlines())
    output1_number = int(input_number * split_percent)
    print input_fpath, ":", input_number, "texts"
    print output1_fpath, ":", output1_number, "texts"
    print output2_fpath, ":", input_number - output1_number, "texts"    

    # Get a random sample of line numbers
    input_lines = range(1, input_number + 1)
    output1_lines = random.sample(input_lines, output1_number)
    
    # Save the lines in two separate files
    for line in input_lines:
        if(line in output1_lines):
            output1_file.write(linecache.getline(input_fpath, line))
        else:
            output2_file.write(linecache.getline(input_fpath, line))
    
    linecache.clearcache()    
    input_file.close()
    output1_file.close()
    output2_file.close()

#####################################
# Entry point
#####################################

#Process parameters
PARAM_NUM = 4
if len(sys.argv) < PARAM_NUM + 1:
    print "This script creates train and test datasets from a set of positive and negative texts samples"
    print "Expected", PARAM_NUM, "parameters but was", str(len(sys.argv)-1)
    print "Usage:", sys.argv[0], "<input-positive-texts> <input-negative-texts> <output-test> <output-train> [<split-fraction>]"
    print "<input-positive>\t\tAn input CSV file with one text per line (positive training examples)"
    print "<input-negative>\t\tAn output CSV file with one text per line (negative training examples)"
    print "<output-test>\t\tAn output XML file with texts of both classes"
    print "<output-train>\t\tAn output XML file with texts of both classes"
    print "<split-fraction>\tPercent of texts in the <output-test> (in (0;1), default 0.1 => 10/90)"
    sys.exit()

# Read the command line arguments
positive_fpath = sys.argv[1]
negative_fpath = sys.argv[2]
test_fpath = sys.argv[3]
train_fpath = sys.argv[4]
if len(sys.argv) > 5:
    SPLIT_PERCENT = float(sys.argv[5])
else:
    SPLIT_PERCENT = 0.1

# Split positive and negative texts
split_csv_texts(positive_fpath, positive_fpath + ".test", positive_fpath + ".train", SPLIT_PERCENT)
split_csv_texts(negative_fpath, negative_fpath + ".test", negative_fpath + ".train", SPLIT_PERCENT)

# Lemmatize positive and negative texts
lemmatize_file(positive_fpath + ".test", positive_fpath + ".test.xml", True, True)
lemmatize_file(positive_fpath + ".train", positive_fpath + ".train.xml", True, True)
lemmatize_file(negative_fpath + ".test", negative_fpath + ".test.xml", True, False)
lemmatize_file(negative_fpath + ".train", negative_fpath + ".train.xml", True, False)

# Cat test files 
cat_files(positive_fpath + ".test.xml", negative_fpath + ".test.xml", test_fpath, "<texts>", "</texts>")
cat_files(positive_fpath + ".train.xml", negative_fpath + ".train.xml", train_fpath, "<texts>", "</texts>")    

# Remove temporary files
os.remove(positive_fpath + ".test")
os.remove(positive_fpath + ".train")
os.remove(negative_fpath + ".test")
os.remove(negative_fpath + ".train")
os.remove(positive_fpath + ".test.xml")
os.remove(positive_fpath + ".train.xml")
os.remove(negative_fpath + ".test.xml")
os.remove(negative_fpath + ".train.xml")
    
print "Script has finished successfully."
