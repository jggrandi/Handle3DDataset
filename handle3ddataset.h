/* Class to handle datasets
	by: Jeronimo G. Grandi
	Jun,2013
*/  

#ifndef HANDLE_3D_DATASET
#define HANDLE_3D_DATASET

#include <cstdlib>
#include <fstream>

#include "handle3ddataset_utils.h"

class Handle3DDataset
{
public:
	Handle3DDataset();
	Handle3DDataset(DATAINFO h_pp);
	~Handle3DDataset();
	bool loadFile();
	bool saveModifiedDataset();
	bool changePlane();
	void setDatasetInfo(DATAINFO h_pp);
	unsigned short** getDataset();
	DATAINFO getDatasetInfo();
private:
	DATAINFO HPP;
	unsigned short **datasetRaw, **datasetModified;
	FILE *inFile, *outFile;	
};


#endif // CHANGEVIEWORIENTATION