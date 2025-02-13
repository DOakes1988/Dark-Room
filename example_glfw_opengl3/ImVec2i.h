#pragma once
#ifndef IMVEC2I_H
#define IMVEC2I_H

struct ImVec2i
{
    int x, y;

    ImVec2i() : x(0), y(0) {}
    ImVec2i(int _x, int _y) : x(_x), y(_y) {}
};

struct ImVec2u
{
    unsigned int x, y;

    ImVec2u() : x(0), y(0) {}
    ImVec2u(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
};

#endif 
