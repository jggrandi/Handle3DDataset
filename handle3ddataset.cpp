#include "handle3ddataset.h"

Handle3DDataset::Handle3DDataset(){}

Handle3DDataset::Handle3DDataset(DATAINFO h_pp)
{
	HPP = h_pp;
}

Handle3DDataset::~Handle3DDataset()
{
//	free(datasetRaw);
//	free(datasetModified);
}

bool Handle3DDataset::loadFile()
{
	//open input dataset file
	if(!(inFile = fopen( HPP.inputFileName, "rb")))
		return false;

	// allocate memory for the 3d dataset
	datasetRaw = (unsigned short**)malloc(HPP.resDepth * sizeof(unsigned short*));
	for (int i=0; i < HPP.resDepth; i++)
		datasetRaw[i] = (unsigned short*)malloc(sizeof(unsigned short) * (HPP.resHeight*HPP.resWidth));

	// read file into dataset matrix
	for( int i = 0; i < HPP.resDepth; i++ )
		for( int j = 0; j < HPP.resWidth; j++ )
			for( int l = 0; l < HPP.resHeight; l++ )
			{
				unsigned short value;
				fread( &value, 1, sizeof(unsigned short), inFile );
				datasetRaw[i][l*HPP.resWidth+j] = value;
			}

	fclose(inFile);
	return true;
}

bool Handle3DDataset::changePlane()
{
	// change the the values of height to depth
	std::swap(HPP.resHeight,HPP.resDepth);
	
	// allocate memory for the 3d dataset
	datasetModified = (unsigned short**)malloc(HPP.resDepth * sizeof(unsigned short*));
	for (int i=0; i < HPP.resDepth; i++)
		datasetModified[i] = (unsigned short*)malloc(sizeof(unsigned short) * (HPP.resWidth*HPP.resHeight));
	
	// tranfer the data to a new dataset with new view plane
	for( int i = 0; i < HPP.resDepth; i++ )
		for( int j = 0; j < HPP.resWidth; j++ )
			for(int l = 0; l < HPP.resHeight; l++)
			{
				if(HPP.viewOrientation == 'c')
					datasetModified[i][l*HPP.resWidth+j] = datasetRaw[l][j*HPP.resWidth+i];
				else if(HPP.viewOrientation == 's')
					datasetModified[i][l*HPP.resWidth+j] = datasetRaw[l][i*HPP.resWidth+j];
			}
	return true;	
}

bool Handle3DDataset::saveModifiedDataset()
{
	// open output dataset file
	if(!(outFile = fopen(HPP.outputFileName, "wb+")))
		return false;

	//save the new view plane into a new raw file
	for(int i = 0; i < HPP.resDepth; i++)
		fwrite(datasetModified[i], 1, sizeof(unsigned short)*HPP.resWidth*HPP.resHeight, outFile);

	fclose(outFile);
	return true;
}

void Handle3DDataset::setDatasetInfo(DATAINFO h_pp)
{
	HPP = h_pp;
}

unsigned short** Handle3DDataset::getDataset()
{
	return datasetRaw;
}

DATAINFO Handle3DDataset::getDatasetInfo()
{
	return HPP;
}
