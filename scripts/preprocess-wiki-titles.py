#!/usr/bin/python

#############################################################
# Preprocessing of the titles of Wikipedia articles. 
# Input: a file with Wikipedia titles in DBPedia format e.g. http://downloads.dbpedia.org/3.7/en/labels_en.nq.bz2
# Output: a CSV file with a subset of input Wikipedia titles (one title per line)
# The script:
# 1) removes the RDF markup leaving only the text;
# 2) removes one-word titles;
# 3) randomly selects a specified number of titles out of all Wikipedia titles;
############################################################

import sys
import linecache
import random
import os
import re
import string
from lemmatizer import lemmatize_file, clean_text
import lemmatizer
from split_texts import split_csv_texts

V = False # Verbose output
SKIP_CRITERION = 0 # 0 - keep all, 1 - keep with at least one space, 2 - keep with at least two spaces, 3 - ...

def cleanup_dbpedia_titles(dbpedia_file, output_file):
# Performs preprocessing of the DBPedia abstracts file
# and writes it to the output file in the format "title\n"

    # Open the files
    dbpedia_file = open(dbpedia_file, "r")
    output_file = open(output_file,"w")
    
    # Read DBPedia definitions line by line
    titles_written_count = 0
    titles_total_count = 0
    
    for line in dbpedia_file:        
        # Skip the void lines
        if line.strip() == "": continue
        
        titles_total_count = titles_total_count + 1
            
        match = re.search(">\s+\"([^\"]+)\"@en\s+<http://", line)
        if match:
            # Get the title text from the line
            title = match.group(1)
            title = clean_text(title)
            if (title == "" or skip_title(title)): 
                if V: print "DELETED", title
                continue
            
            # Write the title to the output file
            output_file.write(title + " " + lemmatizer.SEPARATOR + "\n")
            titles_written_count = titles_written_count + 1
            if V: print title            
        else: 
            continue
       	
    print titles_written_count, "definitions were written out of", titles_total_count, " definitions in the input file."
            	
    # Save the changes
    dbpedia_file.close()    
    output_file.close()
   
def skip_title(title):
# Returns true if the title should be skipped according to some criterion
# (too short, too specific, etc.)
    if(SKIP_CRITERION == 0): return False
    if(SKIP_CRITERION == 1): return not re.search("\s", title, flags=re.IGNORECASE) 
    if(SKIP_CRITERION == 2): return not re.search("\s.+\s", title, flags=re.IGNORECASE)
    if(SKIP_CRITERION == 3): return not re.search("\s.+\s.+\s", title, flags=re.IGNORECASE)
    
def get_random_sample(input_fpath, output_fpath, number):
# Creates output_fpath file containing number of lines from input_fpath. The lines are randomly selected.
# input_fpath     Path to an input CSV file  
# output_fpath    Path to a CSV file with number of lines randomly selected from input_fpath
# number          Number of lines in the output file

	# Open the files
	input_file = open(input_fpath, "r")
	output_file = open(output_fpath,"w")
 
 	# Get number of lines in the file
	number_of_lines = len(input_file.readlines())
	#print number_of_lines
	if number > number_of_lines:
		print "Required number of lines is", number, "but input file contains only", number_of_lines
	
	# Choose a random number from the file
	for i in random.sample(range(1, number_of_lines + 1), number):
		#print i
		output_file.write(linecache.getline(input_fpath, i))

	linecache.clearcache()	
	input_file.close()
	output_file.close()

#####################################
# Entry point
#####################################
#Process parameters
PARAM_NUM = 3
if len(sys.argv) < PARAM_NUM + 1:
    print "Expected", PARAM_NUM, "parameters but was", str(len(sys.argv)-1)
    print "Usage:", sys.argv[0], "<dbpedia-titles-file> <csv-titles-file> <titles-number> [<skip-criterion>]"
    print "<dbpedia-titles-file>\tInput: DBPedia file with Wikipedia titles"
    print "<xml-titles-file>\tOuput: XML filenames file with 0.9*<titles-number> of entries. Two files will be written -- <xml-titles-file>.train and <xml-title-file>.valid (0.1*<titles-number>)"
    print "<titles-number>\t\tNumber of titles out of all input titles"
    print "<skip-criterion>\tIf 0 all titles are keps, 1 with at least one space, 2 with at least two space, 3 ..."

    sys.exit()

# Read the command line arguments
dbpedia_fpath = sys.argv[1]
output_fpath = sys.argv[2]
titles_number = int(sys.argv[3])
if len(sys.argv) > 4:
	SKIP_CRITERION = int(sys.argv[4])

# Temporary files
all_titles_fpath = output_fpath + ".all.csv"
titles_fpath = output_fpath + ".csv"

cleanup_dbpedia_titles(dbpedia_fpath, all_titles_fpath)
get_random_sample(all_titles_fpath, titles_fpath, titles_number)
split_csv_texts(titles_fpath, titles_fpath + ".valid", titles_fpath + ".train", 0.1)
lemmatize_file(titles_fpath + ".valid", output_fpath + ".valid", True, False)
lemmatize_file(titles_fpath + ".train", output_fpath + ".train", True, False)

print "Script has finished successfully."
