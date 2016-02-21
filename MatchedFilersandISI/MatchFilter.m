function [ output_args ] = MatchFilter( input_args )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
len = length(input_args);
m = len/1000;

size_array = 1000;
t = linspace(0, .5,size_array);
g = zeros(1,size_array);
f = sin(2*pi*t);
f = [ zeros(1, 2000) f];
f = fliplr(f);
%figure(5); plot(input_args);
ff = [];

for jj = 0:m-1;
    a = input_args( ((jj*1000)+1):((jj+1)*1000) );
  %  figure(jj); plot(a);
    s = xcorr(a, f);
    %figure(4); plot(s);
   ff = [ff s];
end
max_num = max(ff);
ff = [ff 0];
output_args = ff./max_num;