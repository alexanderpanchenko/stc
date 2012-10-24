function [w,X,y] = lms_classifier(path, dimensions, rho)
% Least Mean Squares (LMS) classifier
% path          Path to the training data file in LIBSVM format
% dimensions    Number of the dimensions to keep

% Load the data
[y, X] = libsvmread(path);

% Prepare the data
%n=10000; % maximum number of data
%m=10000; % maximum number of features
%X=X(1:n,1:m);
X=X(:,1:dimensions)';
%y=y(1:n,:);
y=y';

% Find the weights vector
tic;
[l,N] = size(X);
w = zeros(dimensions,1);
%w = random_range(+1,-1,dimensions);
%r = randn(dimensions,1);
%w = (r-min(r))/(max(r)-min(r));
for i=1:N
    w= w + (rho/i)*(y(i)-X(:,i)'*w)*X(:,i);
end
    
toc;

end


