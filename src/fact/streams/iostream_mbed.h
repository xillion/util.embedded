#pragma once

//#include "mbed.h"

extern "C"
{
    //#include <unistd.h>
    #include <stdio.h>
}

#include "Stream.h"

//#error "MBED OS not yet supported, but coming"


namespace FactUtilEmbedded { namespace std
{
using namespace mbed;

class istream : public basic_istream<char>
{
    Stream* input;
    FileLike& getFile() { return *input; }

public:
    istream& read(char* s, streamsize n)
    {
        FileLike& in = getFile();

        in.read(s, n);

        return *this;
    }

    int get()
    {
        return input->getc();
    }

    bool eof() { return false; }
};


class ostream : public basic_ostream<char>
{
    Stream* output;
    FileLike& getFile() { return *output; }

public:
    __ostream_type& write(const char* s, streamsize n) override
    {
        FileLike& file = getFile();

        file.write(s, n);

        return *this;
    }

    __ostream_type& put(char ch) override
    {
        output->putc(ch);

        return *this;
    }
};

} }