function [w,X,Y] = mse_classifier(path, dimensions)
% Least Squares (sum of error squares) classifier
% path          Path to the training data file in LIBSVM format
% dimensions    Number of the dimensions to keep

% Load the data
[Y, X] = libsvmread(path);

% Prepare the data
%n=10000; % maximum number of data
%m=10000; % maximum number of features
%X=X(1:n,1:m);
X=X(:,1:dimensions)';
%Y=Y(1:n,:);
Y=Y';

% Find the weights vector
tic;
w=(X*X')\(X*Y'); 
toc;

end

