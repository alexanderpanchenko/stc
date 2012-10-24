function [class, odds, pr_p_labels, pr_n_labels, labels_p, labels_n] = classify_nb(labels)
% This function converts labeled vector of tokens
% into a vector of probabilities
% labels 	A labeled vector of tokens (labels are 1=Porn, 2=Ambigous, 3=Non-porn)
%
% labels_p 	A vector of probabilities for porn
% labels_n 	A vector of probabilities for non-porn

% Probabilities
pr_p_p = 0.34;	% P(Wp|Cp)
pr_a_p = 0.33;	% P(Wa|Cp)
pr_n_p = 0.33;	% P(Wn|Cp)
pr_p_n = 0.0000005;	% P(Wp|Cn)
pr_a_n = 0.0099995;	% P(Wa|Cn)
pr_n_n = 0.99;	% P(Wn|Cn)

pr_p = 0.5; %pr_p = 0.10;	% P(Cp)
pr_n = 0.5; %pr_n = 0.90;	% P(Cn)

% Convert labels to probabilities
labels_p = labels;
labels_p(labels_p==1) = pr_p_p; 
labels_p(labels_p==2) = pr_a_p;
labels_p(labels_p==3) = pr_n_p;  		

labels_n = labels;
labels_n(labels_n==1) = pr_p_n; 
labels_n(labels_n==2) = pr_a_n;
labels_n(labels_n==3) = pr_n_n; 

% Decide what class is the most likely
pr_p_labels = pr_p * prod(labels_p); % P(Cp|labels)
pr_n_labels = pr_n * prod(labels_n); % P(Cn|labels)

odds = pr_p_labels / pr_n_labels;
if (odds > 1)
	class = 'p';
else
	class = 'n'; 
end


end
