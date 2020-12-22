#ifndef GLOBAL_H
#define GLOBAL_H
#define R2 1.41421356237
#define R3 1.73205080757

int maxInt=9999;

short ***vol;
char*** spherearr;
unsigned char ***SS;
unsigned char ***GT;
unsigned char ***NML;

unsigned short ***FDT;
unsigned short ***FDTv;
unsigned short ***FDTb;
unsigned short ***tFDT;

unsigned long ***LM;

unsigned short ***FCv;
unsigned short ***FCb;

void *hdr;

 short ***MRv;
 short ***MRb;
 unsigned short ***out;

float D[26] =
        {R3,R2,R3,
         R2,1 ,R2,
         R3,R2,R3,

         R2,1 ,R2,
         1 ,   1 ,
         R2,1 ,R2,

         R3,R2,R3,
         R2,1 ,R2,
         R3,R2,R3};

short V[26][3] =
        {{-1,-1,-1},{ 0,-1,-1},{+1,-1,-1},
         {-1, 0,-1},{ 0, 0,-1},{+1, 0,-1},
         {-1,+1,-1},{ 0,+1,-1},{+1,+1,-1},

         {-1,-1, 0},{ 0,-1, 0},{+1,-1, 0},
         {-1, 0, 0},           {+1, 0, 0},
         {-1,+1, 0},{ 0,+1, 0},{+1,+1, 0},

         {-1,-1,+1},{ 0,-1,+1},{+1,-1,+1},
         {-1, 0,+1},{ 0, 0,+1},{+1, 0,+1},
         {-1,+1,+1},{ 0,+1,+1},{+1,+1,+1}};


struct points{
    unsigned short x;
    unsigned short y;
    unsigned short z;
};
typedef struct points point;

struct pointlist{
    point p;
    struct pointlist *next;
};

typedef struct pointlist vSeed;
vSeed *vShead;

#endif // GLOBAL_H
