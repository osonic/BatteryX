clear;

buffer_size = 44100 * 60 * 3;
AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jade.wav', 'OutputDataType', 'single')

counter_array = 0;
counter_offset = 1;
counter = 0;
counter_prev = 0;
total_counter = 0;

audioIn = step(AR);


buffer = audioIn(:,1);
after_sort = sort(buffer, 'descend');
crop_size = 44100 * 60 * 3 * 100 / 44100
buffer = after_sort(1:crop_size);
%a= kmeans(after_sort, 5);
min_value = min(buffer)
bar = min_value

for i = 2:buffer_size;
    
    counter = counter + 1;
    total_counter = total_counter + 1;
    
    if audioIn(i, 1) > bar && audioIn(i-1, 1) < bar && counter > 100;
        
        if counter_offset == 1
            counter_array(1, counter_offset) = counter;
            counter_array(2, counter_offset) = total_counter / 44100;
            counter_offset = counter_offset + 1;
            counter_prev = counter;
        else
            if abs(counter - counter_prev) < 100
                counter_array(1, counter_offset) = counter;
                counter_array(2, counter_offset) = total_counter / 44100;
                counter_offset = counter_offset + 1;
                counter_prev = counter;
                counter = 0;
            end
        end
        counter = 0;
    end
end

mean_value = mean(counter_array(1,:));

disp(mean_value);

plot(counter_array(2,:), counter_array(1,:))

release(AR)