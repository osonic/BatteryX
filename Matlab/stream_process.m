function [tail, sum, ret_array, peak_found] = stream_process(raw, tail_sum, tail_size)


    for i=1:length(raw)
        if raw(i) < 0
            raw(i) = 0;
        end
    end
    buffer          = mapminmax(raw', 0, 1);
    buffer_size     = length(buffer);
    after_sort      = sort(buffer, 'descend');
    crop_size       = ceil(buffer_size * 100 / 44100);
    tmp             = after_sort(1:crop_size);
    bar             = min(tmp);
    bar = 0.4;
    counter         = tail_size;
    counter_sum     = tail_sum;
    local_counter   = 0;
    counter_offset  = 0;
    local_array     = 0;
    
    %return
    peak_found      = 0;
    ret_array       = 0;

    
    counter     = counter + 2;
    counter_sum   = counter_sum + buffer(2) + buffer(1);

    % to-do : may miss a peak here
    for i = 3:buffer_size

        counter     = counter + 1;
        %if buffer(i) > 0
            counter_sum   = counter_sum + buffer(i);
        %end
        local_counter = local_counter + 1;
                        
        if buffer(i) > bar
            
            if (buffer(i) - buffer(i-2)) > 0.7
            
                if counter > 400
            
                %if (counter_offset > 0 && abs(local_array(counter_offset, 1) - counter) < 100) || counter_offset == 0
                    counter_offset = counter_offset + 1;
                    local_array(counter_offset, 1) = counter;
                    local_array(counter_offset, 2) = local_counter;
                    local_array(counter_offset, 3) = buffer(i);
                    local_array(counter_offset, 4) = counter_sum;
                    local_array(counter_offset, 5) = counter_sum / counter;
                    counter = 0;
                    counter_sum = 0;
                    peak_found = 1;
                %end
                end
            end

        end
    end
    
    %filter after process
    ret_array = local_array;
    %ret_array = medfilt1(local_array, 3);
    %plot(local_array(:,2), local_array(:,1), local_array(:,2), local_array(:,5) * 40000)

    tail    = counter;
    sum     = counter_sum;
    % skip 1 @ for i = 1:(buffer_size - 1)
end