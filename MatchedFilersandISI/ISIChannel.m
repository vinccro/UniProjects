function [ output_args ] = ISIChannel( input_args, noise, multipath , delay, scalek, wDelays, e, factor)
%CHANNEL Summary of this function goes here
%   Detailed explanation goes here

%whole delay of the signal in channel
input_args = [zeros(1, wDelays) input_args];
ccc =[];

%multipath noise before awgn
if strcmp( multipath, 'm')
    for k = 1:factor;
        %delay = 100;
        %scalek = .5;
        %path_noise = scalek.*input_args;
        a = scalek.*[zeros(1, delay) input_args];
        b = [input_args zeros(1, delay)];
        %figure(k+10); 
        %plot(input_args)
        %ccc = [ccc b'];
        input_args = a + b;
        scalek = scalek *0.1;
    end
    if e == 1
        figure(k+4);plot(a);title('Multipath')
    end
end

%all white gaussian noise
if strcmp( noise, 'w')
    noise = length(input_args);
    a_noise = wgn(noise,1,2,'linear')';
    input_args = input_args + a_noise;
end


output_args = input_args;


end

