realtime_mode = 1;
playback_mode = 0;

%dsp.AudioFileWriter
%dsp.AudioFileReader




echo off
clear;
buffer_size = 8192;
%jade=3
%pajero=1.5
number_of_cyl = 1.5;
%'DeviceName', 'USB Audio CODEC'
%AR = dsp.AudioRecorder('SamplesPerFrame', buffer_size, 'DeviceName', 'USB Audio CODEC')
AR = dsp.AudioFileReader('SamplesPerFrame', buffer_size, 'Filename', 'jade.wav', 'OutputDataType', 'single')
%???????????
counter = 0;
single peak;
single peak_filtered;
fft_bg = 0;



tic;
while toc < 3
    counter = counter+1;
    audioIn = step(AR);
    fft_ret=abs(fft(custom_filter(audioIn(:,1))));
    fft_bg = fft_bg + fft_ret;
end
fft_bg = fft_bg / counter;

counter = 0;
tic;
while toc < 10
    counter = counter+1;
    audioIn = step(AR);
    fft_ret = abs(fft(custom_filter(audioIn(:,1)))) - fft_bg;
    %cut the return vector in half
    fft_ret = fft_ret(1:buffer_size/2,1);
    %jade=400
    %pajero=200
    for i = 1:100
        fft_ret(i) = 0;
    end
    
    for i = 1:10
        
        [value,index] = max(fft_ret);
        fft_ret(index) = 0;
        peak(i, counter) = index;
    end
    
    %freq = floor((i-1)*(44100/buffer_size)) / number_of_cyl;
    %xlabel(num2str(freq));
    
    
    figure(1)
    %hold on;
    
    plot(fft_ret)

    %plot(i,c,'--rs')
    %std(peak)
    %len = length(peak);
    %if len > 100
    %    delta = abs(peak(len-100) - peak(len)) + 1;
    %    disp(std(peak(1,len-100:len)) / delta);
    %end
    %plot(peak)
    
    %hold off;
    drawnow
end

windowSize = 5;
b = (1/windowSize)*ones(1,windowSize);
a = 1;

    pt0_offset = 1;

    for i = 1:counter-1
        
        pt0 = peak(pt0_offset,i);

        peak_filtered(i) = pt0;
        
        dis_array = 0;
        
        for n = 1:10
            
            pt1 = peak(n,i+1);
            
            dis_array(n) = abs(pt0 - pt1);
        end
        
        [value, index] = min(dis_array);
        
        pt0_offset = index
    end
    
figure(2)
plot(peak')
figure(3)
plot(peak_filtered)

disp(counter)
release(AR);
