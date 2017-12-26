//
// Created by 陈智 on 2017/11/21.
//

#ifndef BUFFERMANAGER_LRU_H
#define BUFFERMANAGER_LRU_H

#include <iostream>

#include "types.h"


struct Frame{
    explicit Frame(int id){
        frame_id = id;
        next = nullptr;
        prev =nullptr;
    }
    int frame_id;
    Frame * next;
    Frame * prev;
};

class LRU{
public:
    explicit LRU(int size_);
    void Insert(int frame_id);
    void Visit(int fram_id);
    void Remove(int fram_id);
    int GetFrame();
    bool IsFull();
    bool IsEmpty();
    void Print();
private:
    int size;
    int fullSize;
    Frame * head;
    Frame * tail;
};

#endif //BUFFERMANAGER_LRU_H
