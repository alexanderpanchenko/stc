function [w,X,y] = perceptron_classifier(path, dimensions, err_percent)
% Least Mean Squares (LMS) classifier
% path          Path to the training data file in LIBSVM format
% dimensions    Number of the dimensions to keep
% err_percent   Acceptable percent of errors (0 in the original
% algorithm, but if data are not linearly separable, then algorithm will
% never converge (stop after max_iter=10000).

% Load the data
[y, X] = libsvmread(path);

% Prepare the data
X=X(:,1:dimensions)';
y=y';

% Find the weights vector
tic;
[l,N]=size(X);
rho = 0.05;
w = random_range(+1,-1,dimensions);
iter=0;
max_iter=2000;
mis_clas=N;

max_mis_clas = N*err_percent;
fprintf('\n\nerr_percent=%.2f, %.0f/%d\n',...
    err_percent,max_mis_clas,N);
while ((mis_clas>max_mis_clas) && (iter<max_iter))
    iter=iter+1;
    mis_clas=0;
    gradi=zeros(l,1);
    for i=1:N
        if((X(:,i)'*w)*y(i)<0)
            mis_clas=mis_clas+1;
            gradi=gradi+rho*(-y(i)*X(:,i));
        end
    end
    w=w-rho*gradi;  
    %fprintf('%d, %d\n',iter, mis_clas);
end
fprintf('%d iterations\n',iter);
toc;

end


