//
// Created by Sonic on 5/8/15.
//
#include<String>

#ifndef WAVEANALYZER_WAVE_LOADER_H
#define WAVEANALYZER_WAVE_LOADER_H

struct wave_file_info
{
    unsigned int 	number_of_channels;
    unsigned int 	number_of_samples;
    unsigned int 	bytes_per_sample;
    unsigned int 	sample_rate;
    unsigned int 	sample_depth;
    unsigned int 	data_length;
    double** 	    channels;
};


class wave {


public:
    bool load(std::string str);
    struct wave_file_info m_record_file;

private:



    unsigned int get_4_size(const unsigned char* buffer);

    unsigned int get_3_size(const unsigned char* buffer);

    unsigned int get_2_size(const unsigned char* buffer);

    bool seek_to_subchunk(const char* subchunk_name, const unsigned char* raw_buffer,
                          const unsigned int raw_buffer_length, unsigned int* offset_ret);

    bool get_format(const unsigned char* raw_buffer,
                    const unsigned int raw_buffer_length);

    bool get_data(const unsigned char* raw_buffer,
                  const unsigned int raw_buffer_length);


};


#endif //WAVEANALYZER_WAVE_LOADER_H
