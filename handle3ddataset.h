/* Class to handle datasets
	by: Jeronimo G. Grandi
	Jun,2013
*/  

#ifndef HANDLE_3D_DATASET
#define HANDLE_3D_DATASET

#include <cstdlib>
#include <fstream>

#include "handle3ddataset_utils.h"

#define ijn(a,b,n) ((a)*(n))+b

template <class T>
class Handle3DDataset
{
public:
	Handle3DDataset(){}
	Handle3DDataset(DATAINFO h_pp)
	{
		HPP = h_pp;
	}
	
	~Handle3DDataset(){}
	
	bool loadFile()
	{
		//open input dataset file
		if(!(inFile = fopen( HPP.inputFileName, "rb")))
			return false;

		// allocate memory for the 3d dataset
		datasetRaw = (T**)malloc(HPP.resDepth * sizeof(T*));
		for (int i=0; i < HPP.resDepth; i++)
			datasetRaw[i] = (T*)malloc(sizeof(T) * (HPP.resHeight*HPP.resWidth));

		// read file into dataset matrix
		// int size = m_datasetInfo.resWidth*m_datasetInfo.resHeight*m_datasetInfo.resDepth;
		// unsigned short *pVolume = new unsigned short[size];
		// bool ok = (size == fread(pVolume,sizeof(unsigned short), size,pFile));

		for( int d = 0; d < HPP.resDepth; d++ )
			for( int w = 0; w < HPP.resWidth; w++ )
				for( int h = 0; h < HPP.resHeight; h++ )
				{
					T value;
					fread( &value, 1, sizeof(T), inFile );
					datasetRaw[d][ijn(w,h,HPP.resWidth)] = value;
				}

		fclose(inFile);
		return true;
	}

	bool saveModifiedDataset()
	{
		// open output dataset file
		if(!(outFile = fopen(HPP.outputFileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		for(int i = 0; i < HPP.resDepth; i++)
			fwrite(datasetModified[i], 1, sizeof(T)*HPP.resWidth*HPP.resHeight, outFile);

		fclose(outFile);
		return true;
	}

	bool changePlane()
	{
		// change the the values of height to depth
		std::swap(HPP.resHeight,HPP.resDepth);
		
		// allocate memory for the 3d dataset
		datasetModified = (T**)malloc(HPP.resDepth * sizeof(T*));
		for (int i=0; i < HPP.resDepth; i++)
			datasetModified[i] = (T*)malloc(sizeof(T) * (HPP.resWidth*HPP.resHeight));
		
		// tranfer the data to a new dataset with new view plane
		for( int d = 0; d < HPP.resDepth; d++ )
			for( int w = 0; w < HPP.resWidth; w++ )
				for(int h = 0; h < HPP.resHeight; h++)
				{
					if(HPP.viewOrientation == 's')
						datasetModified[d][ijn(w,h,HPP.resWidth)] = datasetRaw[h][ijn(w,d,HPP.resWidth)];
					else if(HPP.viewOrientation == 'c')
						datasetModified[d][ijn(w,h,HPP.resWidth)] = datasetRaw[h][ijn(d,w,HPP.resWidth)];
				}
		return true;		
	}

	void setDatasetInfo(DATAINFO h_pp)
	{
		HPP = h_pp;
	}

	T** getDataset(int plane)
	{
		if(plane==0)
			return datasetRaw;
		else
			return datasetModified;
	}

	DATAINFO getDatasetInfo()
	{
		return HPP;
	}

private:
	DATAINFO HPP;
	T **datasetRaw, **datasetModified;
	FILE *inFile, *outFile;		
};



#endif // CHANGEVIEWORIENTATION