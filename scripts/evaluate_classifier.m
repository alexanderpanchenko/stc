function evaluate_classifier(w, valid_file, dimensions)
% Evaluates quality of a linear classifier
% valid_file        Path to a svmlib file for validation
% dimensions        Number of dimensions

[Yv, Xv] = libsvmread(valid_file);
Xv=Xv(:,1:dimensions);
P=w'*Xv';
Yv = (Yv==+1);
Yp = (P>0)';

% Calculate the statistics 
%R=[Yv Yp Yv==Yp];
tp = sum(Yv==1 & Yp==1 & Yv==Yp);
tn = sum(Yv==0 & Yp==0 & Yv==Yp);
n = sum(Yv==0);
p = sum(Yv==1);
t = sum(Yv==Yp);
l = length(Yv);

fprintf('tp=%d/%d, tn=%d/%d, t=%d\n',...
    tp, p, tn, n, t);

fprintf('Correct %d/%d, Accuracy=%1.4f\n',...
    sum(Yv==Yp), length(Yv), t/l);

end

