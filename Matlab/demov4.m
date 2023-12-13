clear;
buffer_size = 44100 * 60 * 3;
AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jade.wav', 'OutputDataType', 'double')

audioIn = step(AR);
raw = audioIn(:,1);
%plot(raw);

for i=1:length(raw)
    if raw(i) < 0
        raw(i) = 0;
    end
end

buffer = mapminmax(raw', 0, 1);

[pks,locs] = findpeaks(buffer,'MINPEAKHEIGHT', 0.5, 'MINPEAKDISTANCE', 200);

rpm = 0;
rpm_count = 0;
for n=2:length(pks)
    %if buffer(locs(n) - 3) == 0 && buffer(locs(n) - 4) == 0 && buffer(locs(n) - 5) == 0
        rpm_count = rpm_count + 1;
        rpm(rpm_count) = locs(n) - locs(n-1);
    %end
end

plot(rpm)
