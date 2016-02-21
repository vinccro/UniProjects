function [ output_args ] = ISIReceiver( input_args )
%RECEIVER Summary of this function goes here
%   Detailed explanation goes here

len = length(input_args);
m = len/2000
ff = [];
for jj = 0:m-1;
    a = input_args( ((jj*1000)+1):((jj+1)*1000) );
  %  figure(jj); plot(a);
    if max(a) > 0.6 
        ff = [ff 1]
    else 
        ff = [ff 0]
    end
    %figure(4); plot(s);

end





output_args = ff;
length(output_args)
end

