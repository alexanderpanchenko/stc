#!/usr/bin/python 
#######################################################
# A command line lemmatizer and a lemmatization library
# based on TreeTagger
#######################################################

import os
import sys
import re
import treetaggerwrapper
import unicodedata

TREETAGGER_ROOT = '/home/sasha/work/icop/treetagger/'

# CSV file
SEPARATOR = ";"

# XML file
ROOT_TAG = "texts"
TEXT_TAG = "text"
ID_ATT = " id="
POSITIVE_ATT = " class='positive'"
NEGATIVE_ATT = " class='negative'"
ORIGINAL_TAG = "original"
LEMMAS_TAG = "lemmas"
    
def clean_text(text):
    # This function cleans up the text of an element (filename, tags, or category)  
    
    text = re.sub("http:[^\s]*", " ", text)
    text = re.sub(r"\b(www|com|ru|be|nl|no|fr|gr|us|gov|uk|org|ch|cn|kr|eu)\b", " ", text)
    text = re.sub("_+", " ", text, flags=re.IGNORECASE)    
    text = re.sub("pichunter", " ", text, flags=re.IGNORECASE)
    text = re.sub("pornhub", " ", text, flags=re.IGNORECASE)
    text = re.sub("\s+-+\s+", " ", text, flags=re.IGNORECASE)
        
    text = re.sub("\(\s*disambiguation\s*\)"," ", text)     
    text = re.sub(";"," ", text)
    text = re.sub("@en .", "", text)
    
    #text = text.decode('raw_unicode_escape').encode('utf-8','replace') 
    text = re.sub("\?+", " ", text, flags=re.IGNORECASE)
    text = re.sub(r"\\u....", " ", text)
    
    text = re.sub("\s+-", " ", text, flags=re.IGNORECASE)
    text = re.sub("-\s+", " ", text, flags=re.IGNORECASE)        
    text = re.sub("\.\.+", " ", text, flags=re.IGNORECASE)
    text = re.sub("!+", " ", text, flags=re.IGNORECASE)
    text = re.sub("\?+", " ", text, flags=re.IGNORECASE)
    text = re.sub("\++", " ", text, flags=re.IGNORECASE)    
    text = re.sub("&(emsp|thinsp|emsp|ndash|gt|lt|apos)", " ", text, flags=re.IGNORECASE)    
    text = re.sub(r"[%|{}$@><~&^+=*\/;]", " ", text, flags=re.IGNORECASE)
    text = re.sub("\"", "", text)    
    text = re.sub("%\d\d", "", text)
    text = re.sub("%[a-f][a-f]", " ", text)
    text = re.sub("&[a-z]+", " ", text)
    

    text = strip_accents(text)
                        
    text = re.sub("\s+", " ", text, flags=re.IGNORECASE).strip()
        
    return text

# Related to the scenes and numbers:
# resembles a normal text - strange separators are replaced by whitespaces
# \n is \d: scene\s+\n=' ';  #\n=' '; part\s+\n+of\n+=' '; 
# part\s+\n+; vol[.][\n+]=' '; volume\n+ \n=' '; \n+\s+of\s+\n+=' '; ii+=' '   

def strip_accents(input_string):
# Removes accents from the input string and returns the 
# deaccentized verison of the string
# Used by clean_text 
    try:
        output_string = ''
        for char in unicodedata.normalize('NFKD', unicode(input_string)):
            unicode_cat = unicodedata.category(char)
            is_char_to_remove = (
                                 unicode_cat == "Mn" or # NonSpacingMark
                                 unicode_cat == "Mc" or # SpacingCombiningMark
                                 unicode_cat == "Me" or # EnclosingMark
                                 unicode_cat == "Lm" or # ModifierLetter
                                 unicode_cat == "Sk" or # ModifierSymbol
                                 unicode_cat == "Cf" or # Format
                                 unicode_cat == "Cs" or # Surrogate
                                 unicode_cat in set(("Lm","Cc","Co","So","Cn"))
                                 )
            if (not is_char_to_remove): 
                output_string = output_string + char
                                
        return output_string.encode('utf8')
    except (UnicodeDecodeError):
        print "UnicodeDecodeError"
        return input_string

def escape_xml_file(xml_fpath, output_fpath="tmp"):
# This function performs escaping of an input XML file:
# "=&quot; '=&apos;  <=&lt; >=&gt; &=&amp; </caterogy>=</category>

    # Open the files
    xml_file = open(xml_fpath, "r")
    if(output_fpath == "tmp"):
        output_fpath = xml_fpath + ".tmp"
    output_file = open(output_fpath, "w")
    
    for line in xml_file:        
        # Skip the void lines
        if line.strip() == "": continue
        line = re.sub("caterogy>", "category>", line)
        line = re.sub("&", "&amp;", line)
        line = re.sub("\"", "&quot;", line)
        line = re.sub("'", "&apos;", line)
        #line = re.sub("<", "&lt;", line)
        #line = re.sub(">", "&gt;", line)
       
        # Write normalized line to output
        output_file.write(line)

    # Save the changes
    xml_file.close()    
    output_file.close()
    
    if(output_fpath == xml_fpath + ".tmp"):
        os.remove(xml_fpath)
        os.rename(xml_fpath + ".tmp", xml_fpath)


def lemmatize_file(input_fpath, output_fpath, is_xml, is_positive_class, add_texts_tag=False):
# Creates a lemmatized version of an input text file
# input_fpath         A CSV file with one text per line
# output_fpath        An XML file with lemmatized texts
# is_xml              If true then output is saved to XML format, otherwise in CSV (one lemmatized text per line)
# is_positive_class   If true then class is positive, else class is negative
# add_texts_tag       If true then <texts> opening and closing tag is added 
               
    # Initialization
    print "Lemmatization of the", input_fpath, "file..."
    tagger = treetaggerwrapper.TreeTagger(TAGLANG='en',TAGDIR=TREETAGGER_ROOT)
    definitions_file = open(input_fpath) 
    output_file = open(output_fpath,"w")
    id = 1
    if(add_texts_tag): output_file.write("<" + ROOT_TAG + ">\n")
     
    # Write the POS tagged text into the output file
    tokens_count = 0
    for original_line in definitions_file:
        # POS tag the original_line
        tags = tagger.TagText(original_line.lower())
        
        lemmas_line = ""   
        for index, tag in enumerate(tags):
            # Get the token (surface, pos, lemma)
            token = tag.split("\t")
            if len(token) != 3: 
                print token
                continue
            surface = token[0]
            pos = token[1]
            if re.search("<unknown>", tag, flags=re.IGNORECASE): 
                lemma = surface
            else:
                lemma = token[2] 
        
            if(surface == SEPARATOR):
                if(not is_xml):
                    lemmas_line = lemmas_line + "\n"
                #output_file.write("\n")
                continue
                        
            # Write other tags in the 'surface#POS#lemma' format
            lemmas_line = lemmas_line + " " + surface + "#" + pos + "#" + lemma
            
            
            tokens_count = tokens_count + 1
        
        # Clean string with lemmas from the trivial parsing errors
        #lemmas_line = re.sub(r"\s+(\d+)\.\s+", r" \1 ", lemmas_line, flags=re.IGNORECASE)    
        lemmas_line = re.sub(r"([^# ]+)#([^# ]+)#@[^# ]+", r"\1#\2#\1", lemmas_line, flags=re.IGNORECASE)
        lemmas_line = re.sub(r"replaced-url#[^# ]+#replaced-url", " ", lemmas_line, flags=re.IGNORECASE)
            
        # Write line to output
        if(is_xml):
            output_file.write("<" + TEXT_TAG + ID_ATT + '\'' + str(id) + '\'' +
                                (POSITIVE_ATT if is_positive_class else NEGATIVE_ATT) + ">\n")                        
            output_file.write("<" + ORIGINAL_TAG + ">")
            original_line = re.sub(SEPARATOR, " ", original_line)
            original_line = original_line.strip()            
            output_file.write(original_line)
            output_file.write("</" + ORIGINAL_TAG + ">\n")            
            output_file.write("<" + LEMMAS_TAG + ">")
            output_file.write(lemmas_line.strip())
            output_file.write("</" + LEMMAS_TAG + ">\n")            
            output_file.write("</" + TEXT_TAG + ">\n")
            id = id + 1
        else:
            output_file.write(lemmas_line)

    if(add_texts_tag): output_file.write("</" + ROOT_TAG + ">\n")            
    output_file.close()
    print "Lemmatized version of the file", input_fpath, "was saved at", output_fpath
    print tokens_count, "tokens were lemmatized"

def str2bool(v):
# Returns true if input string denotes true, otherwise returns false
    return v.lower() in ("yes", "true", "true", "1", "t", "y")

def main(args):
    #Process parameters
    PARAM_NUM = 5
    if len(args) < PARAM_NUM + 1:
        print "Expected", PARAM_NUM, "parameters but was", str(len(args)-1)
        print "Usage:", args[0], "<input-file> <output-file> <is-xml> <is-positive-class> <add-texts-tag>"
        print "<input-file>\tFile with one text per line."
        print "<output-file>\tFile with lemmatized texts."
        print "<is-xml>\tIf true then output is saved to XML format, otherwise in CSV (one lemmatized text per line)."
        print "<is-positive-class>\tIf true and <is-xml>==true then texts has positive class, else class is negative."
        print "<add-texts-tag>\tIf true then <texts> opening and closing tag is added."
        sys.exit()

    # Read the command line arguments
    input_fpath = args[1]
    output_fpath = args[2]
    is_xml = str2bool(args[3])
    is_positive_class = str2bool(args[4])
    is_texts_tag = str2bool(args[5])

    # Clean the text
    input_file = open(input_fpath, "r")
    tmp_file = open(input_fpath + ".tmp", "w")
    for line in input_file: 
        tmp_file.write(clean_text(line) + "\n")

    input_file.close()
    tmp_file.close()
    os.remove(input_fpath)
    os.rename(input_fpath + ".tmp", input_fpath)
                    
    lemmatize_file(input_fpath, output_fpath, is_xml, is_positive_class, is_texts_tag)
    print "Script has finished successfully."
 
if __name__ == '__main__':
    sys.exit(main(sys.argv))
