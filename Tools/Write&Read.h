#pragma once

#include <unistd.h>
#include <string>
#include <vector>

static int Write(int socket, const std::string &str)
{
    unsigned long length = str.length();
    if(write(socket, &length, sizeof(length)) == -1)
    {
        return -1;
    }

    if(write(socket, str.c_str(), length) == -1)
    {
        return -1;
    }

    return 0;
}


static void Close(int socket)
{
    close(socket);
}
