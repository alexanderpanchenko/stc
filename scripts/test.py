############################################################################################
# Test script. Writes input text to output file.
############################################################################################

import os
import sys

def run_test(input_text, output_file):
	output_file = output_file.replace("\\", "/")
	if(os.path.exists(output_file)):
		os.remove(output_file)	
	file = open(output_file, "w")
	file.write(input_text)
	file.close()

############################################################################################
# Entry point
############################################################################################
MAIN_PAR_COUNT = 2
def main(input_text, output_file):
	print input_text
	print output_file
	run_test(input_text, output_file)

###########################################################################################
print __name__
print sys.argv
if (__name__ == "__main__") and (len(sys.argv) >= MAIN_PAR_COUNT + 1):        	
	# Handle call from command line
	main(sys.argv[1],sys.argv[2])
elif (__name__ == "<module>") and (len(sys.argv) >= MAIN_PAR_COUNT):
	# Handle call from .NET application
	main(sys.argv[0],sys.argv[1])
else:
	print "Required %1s arguments, but were given %2s" % (MAIN_PAR_COUNT, len(sys.argv)-1)
	print "Parameters: input_text output_file"
