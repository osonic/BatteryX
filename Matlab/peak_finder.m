clear;
buffer_size = 44100;
AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jade.wav', 'OutputDataType', 'double')

for i=1:1000
    audioIn = step(AR);
    raw = audioIn(:,1);
    %plot(raw);

    for i=1:length(raw)
        if raw(i) < 0
            raw(i) = 0;
        end
    end

    buffer = mapminmax(raw', 0, 1);

    [pks,locs] = findpeaks(buffer,'MINPEAKHEIGHT', 0.4, 'MINPEAKDISTANCE', 100)
    
    refine_pks = 0;
    refine_locs = 0;
    refine_count = 0;
    for n=1:length(pks)
        %if buffer(locs(n) - 3) == 0 && buffer(locs(n) - 4) == 0 && buffer(locs(n) - 5) == 0
            refine_count = refine_count + 1;
            refine_pks(refine_count) = pks(n);
            refine_locs(refine_count) = locs(n);
        %end
    end

    hold on;
    plot(buffer)
    plot(refine_locs,refine_pks,'--rs')
    hold off;
    pause()
    clf
end
