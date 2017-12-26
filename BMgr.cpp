//
// Created by 陈智 on 2017/12/25.
//

#include "BMgr.h"

#include <iostream>

BMgr::BMgr(){
    for (int i = 0; i < DEFBUFSIZE; ++i) {
        ftop[i] = -1;   // 空闲frame初始化为-1
        ptof[i] = nullptr;
    }
    return;
};

/**
 * 将page加载到buffer中，如果buffer中存在该page,增加count数；否则如果
 * buffer有free frame则读入page，写在该frame上；如果buffer无空闲，则选择
 * 合适的frame进行替换
 * @param page_id
 * @return
 */
int BMgr::FixPage(int page_id){
    int frame_id = LookUp(page_id);
    if (frame_id != -1) { //page在buffer中，增加count
        int hash_id = Hash(page_id);

        BCB * cur = ptof[hash_id];

        while (cur != nullptr){
            if (cur->page_id == page_id) {
                cur->count++;
                if (cur->count == 1) { //frame在lru中，count>0取出
                    lru.Remove(cur->frame_id);
                }
                return frame_id;
            } else {
                cur = cur->next;
            }
        }

        std::cerr << "page_id 并不在BCB中，与page在buffer中矛盾" << std::endl;
        return -1;
    } else {

        //选择一个frame进行替换
        frame_id = SelectVictim();

        // 将相应信息不全
        ftop[frame_id] = page_id;
        BCB* ptr = ptof[Hash(page_id)];
        if (ptr == nullptr) {
            ptof[Hash(page_id)] = new BCB(frame_id, page_id);
        } else {
            BCB* temp = new BCB(frame_id, page_id);
            temp->next = ptr;
            ptof[Hash(page_id)] = temp;
        }
        InCount++;
//    buffer[frame_id] = dsMgr.ReadPage(page_id)
        return frame_id;
    }

};

void BMgr::FixNewPage(){
    return;
};

/**
 * 释放对frame的使用，如果释放后count为0则加入lru队列
 * @param page_id
 * @return 对应的frame_id
 */
int BMgr::UnfixPage(int page_id){
    int hash_id = Hash(page_id);

    BCB * cur = ptof[hash_id];

    while (cur != nullptr){
        if (cur->page_id == page_id) {
            cur->count--;
            if (0 == cur->count){
                lru.Insert(cur->frame_id);
            }
            return cur->frame_id;
        } else {
            cur = cur->next;
        }
    }

    std::cerr << "UnfixPage未能找到所需page_id" << std::endl;
    return -1;
};


/**
 * 返回空闲的frame_id
 * @return
 */
int BMgr::NumFreeFrames(){
    if (freeFrames == 0) {
        std::cerr << "buffer中不含空闲frame" << std::endl;
    }
    int frame_id = DEFBUFSIZE - freeFrames;
    freeFrames -= 1;
    return frame_id;
};

/**
 * 选取victim frame, 如果该置换frame为dirty，写回磁盘
 * @return vic_frame_id
 */
int BMgr::SelectVictim(){
    if (freeFrames > 0) { //仍有空闲frame的情况，在空闲buffer中选择frame
        return NumFreeFrames();
    }

    int vic_frame_id = lru.GetFrame(); //获取要被替换的frame_id
    if (vic_frame_id == -1) { //lru为空,说明所有frame的count均大于零,都在使用中，
        std::cerr << "未能获得替换的frame_id" << std::endl;
    }

    int page_id = ftop[vic_frame_id];
    RemoveBCB(nullptr, page_id);    //该函数的第一个指针意义不明

    return vic_frame_id;
};

/**
 * 寻找ptof表中是否有page_id, 有的话返回frame_id， 否则返回-1
 * @param page_id
 * @return
 */
int BMgr::LookUp(int page_id){
    for (int i = 0; i < DEFBUFSIZE; ++i) {
        BCB * cur = ptof[i];
        while (cur != nullptr){
            if (cur->page_id == page_id) {
                return cur->frame_id;
            } else {
                cur = cur->next;
            }
        }
    }
    return -1;
};

/**
 * 返回对应的hash_id(对应的hash值）
 * @param page_id
 * @return
 */
int BMgr::Hash(int page_id){
    return page_id % DEFBUFSIZE;
};

/**
 * 删除对应的BCB
 * @param ptr
 * @param page_id
 */
void BMgr::RemoveBCB(BCB * ptr, int page_id){
    int hash_id = Hash(page_id);
    BCB * cur = ptof[hash_id];

    if (cur == nullptr) {
        std::cerr << "对应ptof为nullptr, buf 中不存在page_id "
                "对应的frame, 删除失败" << std::endl;
        return;
    }

    if (cur->page_id == page_id) {
        ptof[hash_id] = cur->next;
    } else {
        BCB * pre = cur;
        cur = cur->next;
        while (cur != nullptr){
            if (page_id == cur->page_id) {
                pre->next = cur->next;
                break;
            } else {
                pre = cur;
                cur = cur->next;
            }
        }
    }

    if (cur == nullptr) {
        std::cerr << "buf 中不存在page_id 对应的frame, 删除失败";
    }

    if (cur && cur->dirty == 1) {
        bFrame frame = buffer[cur->frame_id];
        OutCount++;
//      dsMgr.WritePage(page_id, frame);
    }
    free(cur);
};

void BMgr::RemoveLRUEle(int frid){

};

/**
 * 将frame_id 设置为dirty
 * @param frame_id
 */
void BMgr::SetDirty(int frame_id){
    int page_id = ftop[frame_id];
    int hash_id = Hash(page_id);

    BCB * cur = ptof[hash_id];

    while (cur != nullptr){
        if (cur->page_id == page_id) {
            cur->dirty = 1;
            return;
        } else {
            cur = cur->next;
        }
    }
    std::cerr << "buf 中不存在frame_id 对应的frame, set dirty失败";
};

/**
 * 将frame_id 设置为undirty
 * @param frame_id
 */
void BMgr::UnsetDirty(int frame_id){
    int page_id = ftop[frame_id];
    int hash_id = Hash(page_id);

    BCB * cur = ptof[hash_id];

    while (cur != nullptr){
        if (cur->page_id == page_id) {
            cur->dirty = 1;
            return;
        } else {
            cur = cur->next;
        }
    }
    std::cerr << "buf 中不存在frame_id 对应的frame, unset dirty失败";
};

void BMgr::WriteDirtys(){
    for (int i = 0; i < DEFBUFSIZE; ++i) {
        BCB * cur = ptof[i];
        while (cur != nullptr){
            if (cur->dirty == 1) {
                int page_id = cur->page_id;
                int frame_id = cur->frame_id;
                bFrame frame = buffer[frame_id];
                OutCount++;
//                dsMgr.WritePage(page_id, frame);
            }
            cur = cur->next;
        }
    }
};

/**
 * 输出frame_id 对应的数据
 * @param frame_id
 */
void BMgr::PrintFrame(int frame_id) {
    std::cout << buffer[frame_id].field << std::endl;
};


void BMgr::PrintIOCount() {
    std::cout << InCount << std::endl;
    std::cout << OutCount << std::endl;

}
