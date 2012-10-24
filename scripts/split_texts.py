#!/usr/bin/python

import sys
import linecache
import random
import os
                
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

def main(args):
    #Process parameters
    PARAM_NUM = 3
    if len(args) < PARAM_NUM + 1:
        print "This script splits input file with text into two files -- a train and a test (10/90 by default)"
        print "Expected", PARAM_NUM, "parameters but was", str(len(args)-1)
        print "Usage:", args[0], "<input-file> <output-file-1> <output-file-2> [<split-fraction>] [input-file-2]"
        print "<input-file>\t\tAn input CSV file with one text per line"
        print "<output-file-1>\t\tAn output CSV file with <split-fraction>% of texts -- test"
        print "<output-file-2>\t\tAn output CSV file with (1-<split-fraction>)% of texts -- train"
        print "<split-fraction>\tPercent of texts in the <output-file-1> (in (0;1), default 0.1 => 10/90)"
        print "<input-file-2>\t\tA second input CSV file with one text per line. It will be splitted in the"
        print "              \t\tway as the first one and added to the <output-file-1> and <output-file-2>."
        sys.exit()

    # Read the command line arguments
    input_fpath = args[1]
    output1_fpath = args[2]
    output2_fpath = args[3]
    if len(args) > 4:
        SPLIT_PERCENT = float(args[4])
    else:
        SPLIT_PERCENT = 0.1
    
    if len(args) <= 5:
        # Only one input file specified
        split_csv_texts(input_fpath, output1_fpath, output2_fpath, SPLIT_PERCENT)
    elif len(args) > 5:
        # Two input files were specified
        split_csv_texts(input_fpath, output1_fpath + ".tmp1", output2_fpath + ".tmp1", SPLIT_PERCENT)
        input2_fpath = args[5]
        split_csv_texts(input2_fpath, output1_fpath + ".tmp2", output2_fpath + ".tmp2", SPLIT_PERCENT)
    
        # Cat <output-file-1> files 
        output1_file = open(output1_fpath, "w")
        tmp1 = open(output1_fpath + ".tmp1", "r")
        output1_file.write(tmp1.read())    
        tmp1.close()
        tmp2 = open(output1_fpath + ".tmp2", "r")
        output1_file.write(tmp2.read())    
        tmp2.close()    
        output1_file.close()
    
        # Cat <output-file-2> files 
        output2_file = open(output2_fpath, "w")
        tmp1 = open(output2_fpath + ".tmp1", "r")
        output2_file.write(tmp1.read())    
        tmp1.close()
        tmp2 = open(output2_fpath + ".tmp2", "r")
        output2_file.write(tmp2.read())    
        tmp2.close()    
        output2_file.close()
    
        # Remove temporary files
        os.remove(output1_fpath + ".tmp1")
        os.remove(output2_fpath + ".tmp1")
        os.remove(output1_fpath + ".tmp2")
        os.remove(output2_fpath + ".tmp2")
    else:
        # Default
        split_csv_texts(input_fpath, output1_fpath, output2_fpath, SPLIT_PERCENT)
    
    print "Script has finished successfully."

if __name__ == '__main__':
    sys.exit(main(sys.argv))
