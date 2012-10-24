############################################################################################
# This script detects sentence boundaries with the standard Unitex FST graphs
############################################################################################

import os
import sys
import shutil
import utils

ROOT = "D:\\Work\\CENTAL\\Thesauri\\Utils\\semantic_miner\\" # to be loaded from a config file

# Unitex configuration
LANGUAGE = "French"
U = ROOT + "Unitex\\App\\"
UNITEX_RES = ROOT + "Unitex\\" + LANGUAGE + "\\"
UNITEX_W_RES = ROOT+ "Unitex\\Work\\" + LANGUAGE + "\\"
ALPHABET = UNITEX_W_RES + "Alphabet.txt"

def tokenize_sentences(input_dir, input_enc, output_enc):
	# Precompile graphs
	os.system(U + "Grf2Fst2 %1s y %2s" % (UNITEX_W_RES + "Graphs\\Preprocessing\\Sentence\\Sentence.grf", ALPHABET))
	os.system(U + "Flatten %1s RTN 5" % (UNITEX_W_RES + "\\Graphs\\Preprocessing\\Sentence\\Sentence.fst2"))
	os.system(U + "Grf2Fst2 %1s y %2s" % (UNITEX_W_RES + "Graphs\\Preprocessing\\Replace\\Replace.grf", ALPHABET))
	
	for input_file in os.listdir(input_dir):
		tokenize_file_sentences(input_dir + "\\" + input_file, input_enc, output_enc)

# Normalize files and detect sentence boundaries with Unitex tools
# input_file 	UTF8 text file in French
def tokenize_file_sentences(input_file, input_enc, output_enc):
	utils.print_action(input_file)
	TEXT_SNT = utils.cut_ext(input_file) + ".snt"

	utils.print_action("Converting input file from UTF8 to Little-Endian")
	os.system(U + "Convert %1s LITTLE-ENDIAN -r %2s" % (input_enc, input_file))
	
	utils.print_action("Normalizing")
	os.system(U + "Normalize %1s -f=%2s" % (input_file, UNITEX_RES + "Norm.txt"))
	
	utils.print_action("Detecting sentence boundaries")
	os.system(U + "Fst2Txt %1s %2s %3s -merge" % (TEXT_SNT, UNITEX_W_RES + "Graphs\\Preprocessing\\Sentence\\Sentence.fst2", ALPHABET))

	utils.print_action("Normalization of non-ambigous forms")
	os.system(U + "Fst2Txt %1s %2s %3s -replace" % (TEXT_SNT, UNITEX_W_RES + "Graphs\\Preprocessing\\Replace\\Replace.fst2", ALPHABET))
	
	utils.print_action("Converting input file from Little-Endian to UTF8")
	os.system(U + "Convert LITTLE-ENDIAN %1s -r %2s" % (output_enc, TEXT_SNT))

	os.remove(input_file)
	os.rename(TEXT_SNT, input_file)

############################################################################################
# Entry point
############################################################################################
MAIN_PAR_COUNT = 3 
def main(input_dir, input_enc, output_enc):
	input_dir = utils.get_py_filepath(input_dir)
	#utils.clear()
	tokenize_sentences(input_dir, input_enc, output_enc)
	utils.print_h("Done!")

###########################################################################################
print __name__
print sys.argv
if (__name__ == "__main__") and (len(sys.argv) >= MAIN_PAR_COUNT + 1):        	
	# Handle call from command line
	main(sys.argv[1],sys.argv[2],sys.argv[3])
elif (__name__ == "<module>") and (len(sys.argv) >= MAIN_PAR_COUNT):
	# Handle call from .NET application
	main(sys.argv[0],sys.argv[1],sys.argv[2])
else:
	print "Required %1s arguments, but were given %2s" % (MAIN_PAR_COUNT + 1, len(sys.argv))
	print "Parameters: <input_text> <output_dir>"
