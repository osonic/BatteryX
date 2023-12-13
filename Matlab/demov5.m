clear;
buffer_size = 16384;
AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jeep.wav', 'OutputDataType', 'double')
%AR = dsp.AudioRecorder('SamplesPerFrame', buffer_size, 'OutputDataType', 'double')
%AR.DeviceName='USB Audio CODEC'

total_delta = 0;
total_abs = 0;
total_counter = 0;
sample_counter = 0;

%for i=1:500
while 1   
    audioIn = step(AR);
    raw = audioIn(:,1);
    [found, time_delta, time_abs] = stream_process2(raw);
    
    if found ~= 0
    
        output_size = length(time_abs);

        for n=1:output_size
            %total_delta(n + total_counter) = time_delta(n);
            total_delta(n + total_counter) = (1 / (time_delta(n) / 44100)) * 60 / 2;
            total_abs(n + total_counter) = time_abs(n) + sample_counter;
        end
        total_counter = total_counter + output_size;
        
        if total_counter > 500
            handle = plot(total_abs(total_counter - 500:total_counter), total_delta(total_counter - 500:total_counter));
            %ylim([0,5000])
            drawnow
        end
    else
        disp('notfound')
    end
    sample_counter = sample_counter + buffer_size;
     
end

%plot(total_abs, total_delta)
