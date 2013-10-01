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

	bool saveModifiedDataset(char **datasetModified)
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

	char** changePlane( char viewOrientation)
	{
		// change the the values of height to depth
		int rW = HPP.resWidth;
		int rH = HPP.resHeight;
		int rD = HPP.resDepth;

		std::swap(rH,rD);
		
		// allocate memory for the 3d dataset
		char **newOrientation = (T**)malloc(rD * sizeof(T*));
		for (int i=0; i < rD; i++)
			newOrientation[i] = (T*)malloc(sizeof(T) * (rW*rH));
		
		// tranfer the data to a new dataset with new view plane
		for( int d = 0; d < rD; d++ )
			for( int w = 0; w < rW; w++ )
				for(int h = 0; h < rH; h++)
				{
					if(viewOrientation == 's')
						newOrientation[d][ijn(w,h,rW)] = datasetRaw[h][ijn(w,d,rW)];
					else if(viewOrientation == 'c')
						newOrientation[d][ijn(w,h,rW)] = datasetRaw[h][ijn(d,w,rW)];
				}
		return newOrientation;		
	}


	char* arbitraryPlane(int planeNumber, char viewOrientation)
	{
		int rW = HPP.resWidth;
		int rH = HPP.resHeight;
		int rD = HPP.resDepth;

		//allocate memory for the 2d image
		char  *img = (char*)malloc(sizeof(char*)* rW*rH); 
		char **newVolume;

		
		if(viewOrientation == 'c' || viewOrientation == 's')
		{
			// get the volume with view orientation changed
			newVolume = changePlane(viewOrientation);
			// change the the values of height to depth if 'coronal' or 'sagittal'
			std::swap(rH,rD);	
		}	
	
		// tranfer the data to a new dataset with new view plane
		for( int d = 0; d < rD; d++ )
			for( int w = 0; w < rW; w++ )
				for(int h = 0; h < rH; h++)
				{
					if     (viewOrientation == 's' || viewOrientation == 'c')
						img[ijn(w,h,rW)] = newVolume[planeNumber][ijn(w,h,rW)];
					else if(viewOrientation == 'a')				
						img[ijn(w,h,rW)] = datasetRaw[planeNumber][ijn(w,h,rW)];
				}
		return img;				
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