function [class, words_num, percent, porn_num, ambigous_num] = classify_rb(labels)
% This function classify a filename with help of a rule-based keywords classifier. 
% If at least 1 "porn" or 2-3 "ambigous" words present in the file 
% then it is considered as a porn. 
% labels 	A vector of tokens/words labels (0=Non-porn, 1=Ambigous, 2=Porn)
%
% class 	"p" for Porn, "n" for Non-porn
% words_num	Number of porn words in the filename
% percent	Percent of porn words in the filename
% porn_num	Number of unambigous porn words
% ambigous_num	Number of ambigous porn words

% Parameters
PORN_WEIGHT = 1;
AMBIGOUS_WEIGHT = 1/3;
MIN_WORDS_NUM = 1;

% Calculate the variables
porn_num = sum(labels==1);
ambigous_num = sum(labels==2);
words_num = PORN_WEIGHT*porn_num + AMBIGOUS_WEIGHT*ambigous_num;
percent = words_num/length(labels); 

% Decide 
if (words_num >= MIN_WORDS_NUM)
	class = 'p';
else
	class = 'n'; 
end

end
