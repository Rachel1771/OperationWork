#ifndef BUDDY_H
#define BUDDY_H

struct Buddy
{
    int size;
    int address;
    Buddy *parent;
    Buddy *right;
    Buddy *left;
    Buddy *next;
    Buddy *previous;
    bool allocated;
    int freeSpace;
    int level;
    bool deleteThis;
};

#endif // BUDDY_H
