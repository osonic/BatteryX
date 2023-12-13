function [found, time_delta, time_abs] = stream_process2(raw)

    % make all below zero, zero
    buffer_size = length(raw);
    for i=1:buffer_size
        if raw(i) < 0
            raw(i) = 0;
        end
    end
    
    % normalize 0~1
    buffer = mapminmax(raw', 0, 1);
    
    [pks,locs] = findpeaks(buffer,'MINPEAKHEIGHT', 0.3, 'MINPEAKDISTANCE', 100);
    
    
%     time_delta = 0;
%     time_abs = 0;
%     counter = 1;
%     peak_distance_prev = locs(2) - locs(1);
%     time_delta(counter) = peak_distance_prev;
%     time_abs(counter) = locs(2);
%     for i=3:length(locs)
%         peak_distance = locs(i) - locs(i-1);
%         if abs(peak_distance - peak_distance_prev) < 50
%             counter = counter + 1;
%             time_delta(counter) = peak_distance;
%             time_abs(counter) = locs(i);
%             peak_distance_prev = peak_distance;
%         end
%     end


    %??????
    pks_count = length(locs);
    dist_array = 0;
    locs_array = 0;
    dist_array_counter = 0;
    for i=1:(pks_count - 1)
        
        for n=(i+1):pks_count
            
            tmp_dis = locs(n) - locs(i);
            dist_array_counter = dist_array_counter + 1;
            dist_array(dist_array_counter) = tmp_dis;
            locs_array(dist_array_counter) = locs(n);
            
        end
    end
    
    [dist_array_sort, sort_index] = sort(dist_array);
    %disp(sort_index)
    
    %???
    diff = 20;
    same_value_count = 0;
    for i=1:(length(dist_array_sort) - 1)
        
        same_value_count(i) = 0;
        
        for n=(i+1):length(dist_array_sort)
            
            if (dist_array_sort(n) - dist_array_sort(i)) < diff
                same_value_count(i) = same_value_count(i) + 1;
            else
                break;
            end
            
        end
        
    end
    
    [value, index] = max(same_value_count);
    
    %plot(sort(dist_array),'--rs')
    %pause()
    if value > 3
        found = 1;
        %time_delta = dist_array_sort(i);
        %time_abs = buffer_size / 2;
        for i=0:value
            time_delta(i+1) = dist_array_sort(index + i);
            time_abs(i+1) = locs_array(sort_index(index + i));
        end
        time_delta_tmp = 0;
        [sort_tmp, index_tmp] = sort(time_abs);
        for m=1:length(index_tmp)
            time_delta_tmp(m) = time_delta(index_tmp(m));
        end
        time_delta = time_delta_tmp;
        time_abs = sort_tmp;
    else
        found = 0;
        time_delta = 0;
        time_abs = buffer_size / 2;
    end
%     %?distance???
%     dist_array = 0;
%     for i=2:length(locs)
%         peak_distance = locs(i) - locs(i-1);
%         dist_array(i-1) = peak_distance;
%     end
%     
%     dist_array_sort = sort(dist_array);
%     plot(dist_array_sort)
%     pause()
%     %time_delta = max(dist_array);
%     time_delta = dist_array_sort(ceil(length(dist_array) * 4 / 5));
%     time_abs = buffer_size / 2;
end