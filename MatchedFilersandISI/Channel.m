function [ output_args ] = Channel( input_args ,noise, multipath)
%CHANNEL Summary of this function goes here
%   Detailed explanation goes here


%multipath noise
if strcmp( multipath, 'm') == 0
    delay = 900;
    scalek = .5;
    path_noise = scalek.*[zeros(1, delay) input_args(1:end-delay)];

    input_args = input_args + path_noise;
end

%all white gaussian noise
if strcmp( noise , 'w') == 0
    noise = length(input_args);
    a_noise = wgn(noise,1,-.5)';
    a_noise = 0;
    input_args = input_args + a_noise;
end


output_args = input_args;

end

