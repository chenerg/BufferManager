//
// Created by 陈智 on 2017/11/21.
//

#ifndef BUFFERMANAGER_TYPES_H
#define BUFFERMANAGER_TYPES_H

#define FRAMESIZE 4096
#define PAGESIZE 4096
#define MAXPAGES 500000
#define DEFBUFSIZE 1024*2

struct bFrame {
    char field[FRAMESIZE];
};

#endif //BUFFERMANAGER_TYPES_H
