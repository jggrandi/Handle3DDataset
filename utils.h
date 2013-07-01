#ifndef UTILS
#define UTILS


typedef struct data
{
    char *inputFileName;
    char *outputFileName;
    char viewOrientation;
    int  resWidth;
    int  resHeight;
    int  resDepth;
    bool verboseMode;
}DATAINFO;

static char version[]=
"	version 1.0 \n";

static char program_name[]=
" (C) 2013 changeViewPlane\n"
"	written by Jerônimo G. Grandi \n";

static char description[]=
"\n Changes the view plane of 3D RAW datasets\n";

static char usage[] =
"\n"
" Usage: ./changePlaneOrientation [options] <resolution_width> <resolution_heigth> <resolution_depth> \n"
"\n"
" Common options:\n"
"\n"
"	-i <filename>	: Input Dataset\n"
"	-o <filename>	: Output Dataset\n"
"	-c 		: XZ view (coronal) \n"
"	-s 		: YZ view (sagittal)\n"
"	-v 		: Silent Mode\n"
"	-h 		: This instructions\n"
"\n"
" Example:\n"
" 	1:		./changePlaneView -c -i CT_Arterial.raw -o CT_Arterial_Coronal.raw 512 512 220\n"
" 	2:		./changePlaneView -v -s -i MRI_Subject1.raw -o MRI_Subject1_Sagittal.raw 128 128 128\n"
"\n"
" Report program bugs to jggrani at inf dot ufrgs dot br\n";
#endif // UTILS

