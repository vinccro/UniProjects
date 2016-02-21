function [ output_args ] = ISITransmitter( input_args, wave, delay )
%TRANSMITTER Summary of this function goes here
%   Detailed explanation goes here

size_array = 1000;
if strcmp(wave, 'isi') 
    g = zeros(1,size_array);
    %delay = 122;
    T = 1;
    b = .2;
    t = linspace(-10, 10, 1000);
    f = (sinc(t./T)) .* ( (cos((pi*b*t)./T))./((1-(4*b*b*t.*t))/(T*T)) );  

else
    %delay = 1000;
    size_array = 1000;
    t = linspace(0, .5,size_array);
    g = zeros(1,size_array);
    f = sin(2*pi*t);

end




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
end

output_args = a;

end


