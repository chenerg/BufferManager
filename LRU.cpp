//
// Created by 陈智 on 2017/11/21.
//
#include "LRU.h"

LRU::LRU(int size_):fullSize(size_), size(0) {
    head = nullptr;
    tail = nullptr;
};

/**
 * 插入新的frame_id到队列头
 * @param frame_id
 */
void LRU::Insert(int frame_id) {
    if (IsFull()) {
        std::cerr << "LRU队列已满" << std::endl;
    } else {
        Frame* frame = new Frame(frame_id);
        if (head == nullptr) {
            head = frame;
            tail = frame;
            frame->next = nullptr;
            frame->prev = nullptr;
        } else {
            frame->next = head;
            head->prev = frame;
            head = frame;
        }

        size++;
    }
};

/**
 * 访问frame_id,将对应的frame提到队列最开始
 * @param frame_id
 */
void LRU::Visit(int frame_id) {
    Frame * cur = head;
    while (cur != nullptr) {
        if (cur->frame_id == frame_id) {
            if (cur == head) {
                return;
            } else if (cur == tail) {
                cur->prev->next = nullptr;
                tail = cur->prev;
                cur->prev = nullptr;
                cur->next = head;
                head->prev = cur;
                head = cur;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;

                cur->prev = nullptr;
                cur->next = head;
                head->prev = cur;
                head = cur;
            }
            return;
        }
        cur = cur->next;
    }
    std::cerr << "LRU无法找到 " << frame_id << std::endl;
};


/**
 * 访问frame_id,将对应的frame提到队列最开始
 * @param frame_id
 */
void LRU::Remove(int frame_id) {
    Frame * cur = head;
    while (cur != nullptr) {
        if (cur->frame_id == frame_id) {
            if (cur == head) {
                head = cur->next;
                free(cur);
            } else if (cur == tail) {
                cur->prev->next = nullptr;
                tail = cur->prev;
                free(cur);
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                free(cur);
            }
            size--;
            return;
        }
        cur = cur->next;
    }
    std::cerr << "LRU无法找到 " << frame_id << std::endl;
};


/**
 * 获取替换的frame_id, 如果队列为空，则返回-1报错
 * @return
 */
int LRU::GetFrame() {
    if (IsEmpty()) {
        std::cerr << "LRU队列为空" << std::endl;
        return -1;
    } else {
        Frame * cur = tail;
        if (cur == head){
            head = nullptr;
            tail = nullptr;
        } else {
            cur->prev->next = nullptr;
            tail = cur->prev;
        }
        int frame_id = cur->frame_id;
        free(cur);
        size--;
        return frame_id;
    }
};


bool LRU::IsFull() {
    return size == fullSize;
};

bool LRU::IsEmpty() {
    return size == 0;
}

void LRU::Print() {
    Frame * cur = head;
    while (cur != nullptr) {
        std::cout << cur->frame_id << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}