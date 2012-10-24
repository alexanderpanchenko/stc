function r = random_range(hi,lo,nr)
% Generates random numbers in a range [hi;lo]
% hi    Highest value 
% lo    Lowest value
% nr    Number of elements in the output vector

r = lo + (hi-lo)*rand(1,nr);
r = r';
%disp([min(r) max(r) mean(r) std(r)]);
end

