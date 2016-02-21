clear all;
% 1st argument is array of binary [ 1 1 0 1 1]
% 2nd argument is wave type
% isi is raised cosine filter
% anything else is cosine
% 3rd argument is delay of wave
% recommended values isi = 122 or cosine = 1000
wavetype = 'isih';
dddd = [1];
aaaa = [1 1 1 1 1 1 1 1 1 1 1 1 1 1];
bbbb = [1 1 1 0 1 0 1 1 1 0 0 0 1 1 1 1 1 0 1 0 1 0 0 0 1 0 0 0 1 1 0 1 0 1 0 0 1 0 1 0 1 0 0 0 1 0 1 1 1];
cccc = [0 0 1 1 1 1 1 0 1 0 0 0 1 0 0 0];
a = ISITransmitter(cccc,wavetype, 123 );
figure(1); plot(a);title('ISITransmitter');
%pause;
% 2nd argument w enables All White Gaussian Noise
% 3rd argument m enables multipath
% 4th argument is delay value for multipath
% 5th argument is scale value for multipath
% 6th argument is Whole delay of Signal
% 7th argument plots multipath if e = 1
% 8th argument this repeats the delay and scale in channel default is 1 
multipath_factor = 6;
e = 0;
a = ISIChannel( a, 'w', 'm', 100, .500, 4000, e, multipath_factor);
figure(2); plot(a);title('ISIChannel');
%pause;
% 1st argument is input array
% 2nd argument is raised cosine filter or cosine
a = ISIMatchFilter( a , wavetype);
figure(3); plot(a); title('ISIMatchFilter');
%pause;
%figure(17)
%plot(a)