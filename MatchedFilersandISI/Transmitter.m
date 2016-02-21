function [ output_args ] = Transmitter( input_args )
%TRANSMITTER Summary of this function goes here
%   Detailed explanation goes here
delay = 1000
size_array = 1000;
t = linspace(0, .5,size_array);
g = zeros(1,size_array);
f = sin(2*pi*t);

%T = 1;
%b = .2;
%t = linspace(-10, 10, 1000);
%f = (sinc(t./T)) .* ( (cos((pi*b*t)./T))./((1-(4*b*b*t.*t))/(T*T)) );

a= zeros(1,size_array);
for jj = 1:length(input_args);
    if input_args(jj) == 1
        bb = [a zeros(1, delay)];
        a = [zeros(1, (jj)*delay) f];
        a = a + bb;
    else
        bb = [a zeros(1, delay)];
        a = [zeros(1, (jj)*delay) g];
        a = a + bb;
end






% a = [];
% for jj = 1:length(input_args)
%     if input_args(jj) ==1
%         a=[a f];
%     else
%         a = [a g];
%     end
%     
%     
% end
%all white gaussian noise
noise = length(a);
a_noise = wgn(noise,1,.01,'linear')';
a_noise = 0;
output_args = a + a_noise;
end

