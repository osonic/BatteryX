clear;

total = 44100 * 60 * 3;

buffer_size = 44100;

AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jade.wav', 'OutputDataType', 'single')

%audioIn = step(AR);
%buffer = audioIn(:,1);

init_counter = 750;
tail_size = 0;
tail_sum = 0;
total_counter = 0;
total_array = 0;

for i = 1:floor(total / buffer_size)
    audioIn = step(AR);
    buffer = audioIn(:,1);
    ret_array = 0;
    
    [tail, sum, ret_array, peak_found] = stream_process(buffer, tail_sum, tail_size);
    
    if peak_found == 1
        
        total_size = length(total_array(:,1));
        
        for n = 1:length(ret_array(:,1))
            total_counter = total_counter + 1;
            counter_value = ret_array(n, 1);
            %total_array(total_size + n, 1) = (1 / ((1 / 44100) * counter_value)) * 60 / 3;
            total_array(total_size + n, 1) = counter_value;
            total_array(total_size + n, 2) = total_counter;
            total_array(total_size + n, 3) = ret_array(n, 3);
            total_array(total_size + n, 4) = ret_array(n, 4);
            total_array(total_size + n, 5) = ret_array(n, 5);

        end
        
        disp('found')

    else
        
        tail = 0;
        tail_sum = 0;
        
        disp('not found')
        
    end
    
    tail_size = tail;
    tail_sum = sum;
end
plot(total_array(:,2), total_array(:,1))

disp('done')
