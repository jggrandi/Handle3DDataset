#ifndef HANDLE3DDATASET_UTILS
#define HANDLE3DDATASET_UTILS

typedef struct data
{
	char *fileName;
	char viewOrientation;
    int  resWidth;
    int  resHeight;
    int  resDepth;
    int  initStack;
    int  endStack;
    int  resampleFactorImg;
    int  resampleFactor;
    int  resampleFactorZ;
}DATAINFO;


#endif // HANDLE3DDATASET_UTILS

