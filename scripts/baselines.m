%% Least Squares (sum of error squares) classifier
clc
dimensions = 41732;% 72259;
w = mse_classifier('/home/sasha/Desktop/icop/50K/filenames.xml.train.csv',dimensions);

%% Least Mean Squares (Widrow-Hoff) algorithm 
clc
dimensions = 72259; %41732;
%rho=1:-0.1:0.1;
%rho=0.1:-0.01:0;
rho=10:-1:1;
%rho=100:-5:10;
for i=1:length(rho)
%for i=1:10
    %rho(i) = 10;
    fprintf('\nrho=%1.2f\n',rho(i));
    w = lms_classifier('/home/sasha/Desktop/icop/150K/filenames.xml.train.csv',dimensions,rho(i));
    evaluate_classifier(w,'/home/sasha/Desktop/icop/150K/filenames.xml.valid.csv',dimensions)
end

%% Test the classifier accuracy

evaluate_classifier(w,'/home/sasha/Desktop/icop/50K/filenames.xml.valid.csv',dimensions)

%% Perceptron
clc

dimensions = 72259;
for p=0.2:-0.02:0.02
    w = perceptron_classifier('/home/sasha/Desktop/icop/150K/filenames.xml.train.csv', dimensions,p);
    evaluate_classifier(w,'/home/sasha/Desktop/icop/150K/filenames.xml.valid.csv',dimensions)
end

%%
dimensions = 41732;
for p=[0]
%for p=[0.02 0.01 0]
%for p=0.2:-0.02:0
    w = perceptron_classifier('/home/sasha/Desktop/icop/50K/filenames.xml.train.csv', dimensions,p);
    evaluate_classifier(w,'/home/sasha/Desktop/icop/50K/filenames.xml.valid.csv',dimensions)
end

dimensions = 72259;
for p=[0]
    w = perceptron_classifier('/home/sasha/Desktop/icop/150K/filenames.xml.train.csv', dimensions,p);
    evaluate_classifier(w,'/home/sasha/Desktop/icop/150K/filenames.xml.valid.csv',dimensions)
end


%% Naive Bayes
clc
load fisheriris
y = [zeros(50,1); ones(50,1)];
model = NaiveBayes.fit(meas(1:100,:),y);
yhat = model.predict(meas(1:100,:));
cMat1 = confusionmat(y,yhat)

%%
clc
dimensions = 41732;% 72259;

% Load the data
[y, X] = libsvmread('/home/sasha/Desktop/icop/50K/filenames.xml.train.csv');

% Prepare the data
%n=10000; % maximum number of data
%m=10000; % maximum number of features
%X=X(1:n,1:m);
X=X(:,1:dimensions);
%L = (X>0);
%X(L)=1;
y=(y~=1);
null_features_pos=(sum(X(y,:))==0);
d1= full([(sum(X(y,:))==0)' (sum(X(~y,:))==0)']);
null_features_neg=(sum(X(~y,:))==0);
null_features = null_features_pos | null_features_neg;
X(:,null_features) = [];
%X(:,sum(X(~y,:))==0) = [];

%%
tic;
class  = classify(X,X,y, 'diaglinear');
model = ClassificationDiscriminant.fit(X,y);
nbmodel = NaiveBayes.fit(X,y);
toc;

%%
%model = NaiveBayes.fit(X,y);
%yhat = model.predict(meas(1:100,:));
%cmat = confusionmat(y,yhat)




