//
// Created by Sonic on 5/8/15.
//

#include "wave.h"
#include "stdlib.h"

bool wave::load(std::string str)
{
    FILE *fp = fopen(str.c_str(),"rb");

    unsigned char buffer[4];
    unsigned int i = 0;
    unsigned char* raw_buffer;
    unsigned int raw_buffer_length;

    // check the header
    fread(buffer,sizeof(unsigned char), 4,fp);
    if (strncmp((char*)buffer, "RIFF", 4) != 0)
    {
        return false;
    }

    fread(buffer,sizeof(unsigned char), 4,fp);
    raw_buffer_length = get_4_size(buffer);
    raw_buffer = (unsigned char*)malloc(raw_buffer_length * sizeof(unsigned char));

    // check the header
    fread(buffer,sizeof(unsigned char), 4,fp);
    if (strncmp((char*)buffer, "WAVE", 4) != 0)
    {
        return false;
    }

    raw_buffer_length -= 4;

    // put everything into raw buffer
    if(fread(raw_buffer,sizeof(unsigned char), raw_buffer_length,fp) != raw_buffer_length)
    {
        return false;
    }

    if (get_format(raw_buffer, raw_buffer_length) != true)
    {
        return false;
    }

    if (get_data(raw_buffer, raw_buffer_length) != true)
    {
        return false;
    }
    //print_data(record_file);
    fclose(fp);
    return true;
}

unsigned int wave::get_4_size(const unsigned char *buffer)
{
    return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
}

unsigned int wave::get_3_size(const unsigned char *buffer)
{
    return (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
}

unsigned int wave::get_2_size(const unsigned char *buffer)
{
    return (buffer[1] << 8) + buffer[0];
}

bool wave::seek_to_subchunk(const char* subchunk_name, const unsigned char *raw_buffer,
                                    const unsigned int raw_buffer_length, unsigned int *offset_ret)
{
    unsigned int i = 0;
    unsigned int offset = 0;
    unsigned int subchunk_size = 0;

    while(1)
    {
        if(strncmp((char*)raw_buffer + offset, subchunk_name, 4) != 0)
        {
            subchunk_size = get_4_size(raw_buffer + offset + 4);
            //printf("%u|", subchunk_size);
            offset += (4 + 4 + subchunk_size);
            if(offset >= raw_buffer_length)
            {
                return false;
            }
        }
        else
        {
            break;
        }
    }
    *offset_ret = offset;
    return true;
}

bool wave::get_format(const unsigned char *raw_buffer,
                              const unsigned int raw_buffer_length)
{
    unsigned int offset = 0;
    if(seek_to_subchunk("fmt ", raw_buffer, raw_buffer_length, &offset) == true)
    {
        // AudioFormat = 1(PCM)
        if(get_2_size(raw_buffer + offset + 4 + 4) != 1)
        {
            return false;
        }

        m_record_file.number_of_channels = get_2_size(raw_buffer + offset + 4 + 4 + 2);
        m_record_file.sample_rate = get_4_size(raw_buffer + offset + 4 + 4 + 2 + 2);
        m_record_file.bytes_per_sample = get_2_size(raw_buffer + offset + 4 + 4 + 2 + 2 + 4 + 4);
        m_record_file.sample_depth = get_2_size(raw_buffer + offset + 4 + 4 + 2 + 2 + 4 + 4 + 2);

        // let's only support 8-bit / 16-bit / 24-bit / 32-bit

        return true;
    }
    else
    {
        return false;
    }
}

bool wave::get_data(const unsigned char *raw_buffer,
                            const unsigned int raw_buffer_length)
{
    unsigned int offset = 0;
    unsigned int i = 0;
    unsigned int n = 0;
    if(seek_to_subchunk("data", raw_buffer, raw_buffer_length, &offset) == true)
    {

        m_record_file.data_length = get_4_size(raw_buffer + offset + 4);

        m_record_file.channels = (double**)malloc(m_record_file.number_of_channels * sizeof(double*));

        m_record_file.number_of_samples = (unsigned int)(m_record_file.data_length / m_record_file.bytes_per_sample);

        for(i = 0; i < m_record_file.number_of_channels; i++)
        {
            m_record_file.channels[i] = (double*)malloc(m_record_file.number_of_samples * sizeof(double) * 2);
        }

        for(n = 0; n < m_record_file.number_of_samples; n++)
        {
            for(i = 0; i < m_record_file.number_of_channels; i++)
            {
                if(m_record_file.sample_depth == 8)
                {
                    m_record_file.channels[i][n] = (raw_buffer + offset + 4 + 4 + n * m_record_file.bytes_per_sample + i)[0];
                }
                if(m_record_file.sample_depth == 16)
                {
                    m_record_file.channels[i][n * 2] = get_2_size(raw_buffer + offset + 4 + 4 + n * m_record_file.bytes_per_sample + i * 2);
                    m_record_file.channels[i][n * 2 + 1] = 0;

                }
                if(m_record_file.sample_depth == 24)
                {
                    m_record_file.channels[i][n] = get_3_size(raw_buffer + offset + 4 + 4 + n * m_record_file.bytes_per_sample + i * 3);
                }
                if(m_record_file.sample_depth == 32)
                {
                    m_record_file.channels[i][n] = get_4_size(raw_buffer + offset + 4 + 4 + n * m_record_file.bytes_per_sample + i * 4);
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
