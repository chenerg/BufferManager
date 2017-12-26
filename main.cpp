#include <fstream>

#include "LRU.h"
#include "BMgr.h"

int main() {
    std::ifstream inFile;

    inFile.open("/Users/chenzhi/CLionProjects/BufferManager/data/data-5w-50w-zipf.txt");

    if (!inFile) {
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }

    std::string typePage;
    int type, page_id;

    BMgr* bmgr = new BMgr();


    while (inFile >> typePage) {
        int pos = typePage.find(',');
        type = stoi(typePage.substr(0, pos));
        page_id = stoi(typePage.substr(pos+1, typePage.length()));

        if (type == 0) {    // 读page,将page先占用再释放
            bmgr->FixPage(page_id);
            bmgr->UnfixPage(page_id);
        } else {
            int frame_id = bmgr->FixPage(page_id);
            bmgr->SetDirty(frame_id);
            bmgr->UnfixPage(page_id);
        }

    }
    bmgr->PrintIOCount();
    bmgr->WriteDirtys();
    bmgr->PrintIOCount();

//    LRU lru = LRU(DEFBUFSIZE);
//    lru.Print();
//    lru.Insert(1);
//    lru.Print();
//    lru.Insert(4);
//    lru.Print();
//    lru.Insert(3);
//    lru.Print();
//    lru.Remove(4);
//    lru.Insert(4);
//    lru.Print();
//    lru.GetFrame();
//    lru.Print();
    return 0;
}