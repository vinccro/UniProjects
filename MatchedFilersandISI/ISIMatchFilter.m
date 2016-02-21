function [ output_args ] = ISIMatchFilter( input_args, wavetype )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
len = length(input_args)
m = len/1000

if strcmp(wavetype, 'isi')
    T = 1;
    b = .2;
    t = linspace(-10, 10, 1000);
    f = (sinc(t./T)) .* ( (cos((pi*b*t)./T))./((1-(4*b*b*t.*t))/(T*T)) );
else
    size_array = 1000;
    t = linspace(0, .5,size_array);
    g = zeros(1,size_array);
    f = sin(2*pi*t);
end



temp =  xcorr(input_args, f);
output_args = temp./max(temp);

% output_args = ff./max_num;
% 
% 
% length(output_args)
% 

end