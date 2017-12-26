//
// Created by 陈智 on 2017/12/25.
//

#ifndef BUFFERMANAGER_BMGR_H
#define BUFFERMANAGER_BMGR_H

#include <stdio.h>

//#include "DSMgr.h"
#include "types.h"
#include "LRU.h"

struct BCB {
    BCB(int frame_id, int page_id){
        this->frame_id = frame_id;
        this->page_id = page_id;
        count = 1;
        dirty = 0;
        next = nullptr;
    }
    int page_id;
    int frame_id;
    int latch;
    int count;
    int dirty;
    BCB * next;
};

class BMgr {
public:
    BMgr();
    int FixPage(int page_id);
    void FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();
    void PrintFrame(int frame_id);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintIOCount();

private:
    int LookUp(int page_id);
    int SelectVictim();
    int Hash(int page_id);
    void RemoveBCB(BCB * ptr, int page_id);
    void RemoveLRUEle(int frid);

private:
    // Hash Table
    int ftop[DEFBUFSIZE];
    BCB* ptof[DEFBUFSIZE];
    bFrame buffer[DEFBUFSIZE];
    LRU lru = LRU(DEFBUFSIZE);  //管理替换
    int freeFrames = DEFBUFSIZE; // 用来记录缓存中空闲frame的数量


    int InCount = 0;
    int OutCount = 0;
//    DSMgr dsMgr;
};

#endif //BUFFERMANAGER_BMGR_H

