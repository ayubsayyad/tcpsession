#pragma once
#include <cstdlib>

template<size_t BUFFER_SIZE = 4096>
struct Basic_Buffer
{
    constexpr static size_t Buffer_Size = BUFFER_SIZE;
    Basic_Buffer()
    {
        bufferSize_ = 0;
    }

    void moveToStart(size_t idx)
    {
        std::memcpy(buffer_, buffer_ + idx, bufferSize_ - idx);
        bufferSize_ = bufferSize_ - idx;
    }

    void append(char* data, size_t len)
    {
        std::memcpy(buffer_ + bufferSize_, data, len);
    }


    char buffer_[BUFFER_SIZE];
    size_t bufferSize_;
};

typedef Basic_Buffer <4096> Buffer;
