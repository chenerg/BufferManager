#include "LRU.h"

int main() {
    LRU lru = LRU(DEFBUFSIZE);

    lru.Print();
    lru.Insert(1);
    lru.Print();
    lru.Insert(4);
    lru.Print();
    lru.Insert(3);
    lru.Print();
    lru.GetFrame();
    lru.Print();
    return 0;
}