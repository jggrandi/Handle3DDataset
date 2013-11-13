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
	Handle3DDataset(DATAINFO h_pp_raw, DATAINFO h_pp_mod)
	{
		HPP_RAW = h_pp_raw;
		HPP_MOD = h_pp_mod;
	}
	
	Handle3DDataset(DATAINFO h_pp_raw)
	{
		HPP_RAW = h_pp_raw;
	}
	
	~Handle3DDataset(){}
	
	bool loadFile()
	{
		//open input dataset file
		if(!(inFile = fopen( HPP_RAW.fileName, "rb")))
			return false;

		// allocate memory for the 3d dataset
		datasetRaw = (T**)malloc(HPP_RAW.resDepth * sizeof(T*));
		for (int i=0; i < HPP_RAW.resDepth; i++)
			datasetRaw[i] = (T*)malloc(sizeof(T) * (HPP_RAW.resHeight*HPP_RAW.resWidth));

		// read file into dataset matrix
		// int size = m_datasetInfo.resWidth*m_datasetInfo.resHeight*m_datasetInfo.resDepth;
		// unsigned short *pVolume = new unsigned short[size];
		// bool ok = (size == fread(pVolume,sizeof(unsigned short), size,pFile));

		for( int d = 0; d < HPP_RAW.resDepth; d++ )
			for( int w = 0; w < HPP_RAW.resWidth; w++ )
				for( int h = 0; h < HPP_RAW.resHeight; h++ )
				{
					T value;
					fread( &value, 1, sizeof(T), inFile );
					datasetRaw[d][ijn(w,h,HPP_RAW.resWidth)] = value;
				}

		fclose(inFile);
		return true;
	}

	bool saveModifiedDataset()
	{
		// open output dataset file
		if(!(outFile = fopen(HPP_MOD.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		for(int i = 0; i < HPP_MOD.resDepth; i++)
			fwrite(datasetModified[i], 1, sizeof(T)*HPP_MOD.resWidth*HPP_MOD.resHeight, outFile);

		fclose(outFile);
		return true;
	}

	bool saveModifiedDataset(char **datasetToSave, DATAINFO INFO)
	{
		// open output dataset file
		if(!(outFile = fopen(INFO.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		for(int i = 0; i < INFO.resDepth; i++)
			fwrite(datasetToSave[i], 1, sizeof(T)*INFO.resWidth*INFO.resHeight, outFile);

		fclose(outFile);
		return true;
	}
	template <class D>
	bool saveModifiedDataset(D **datasetToSave, DATAINFO INFO)
	{
		// open output dataset file
		if(!(outFile = fopen(INFO.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		for(int i = 0; i < INFO.resDepth; i++)
			fwrite(datasetToSave[i], 1, sizeof(D)*INFO.resWidth*INFO.resHeight, outFile);

		fclose(outFile);
		return true;
	}

	bool saveModifiedImage(char *imageToSave, DATAINFO INFO)
	{
		// open output dataset file
		if(!(outFile = fopen(INFO.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		fwrite(imageToSave, 1, sizeof(T)*INFO.resWidth*INFO.resHeight, outFile);

		fclose(outFile);
		return true;
	}

	bool changePlane()
	{
		if(!clearDataset(1)){printf("Failed to clear struct\n"); return false; }

		HPP_MOD.resWidth = HPP_RAW.resWidth;
		HPP_MOD.resDepth = HPP_RAW.resHeight;
		HPP_MOD.resHeight = HPP_RAW.resDepth;

		// change the the values of height to depth
		//std::swap(HPP_MOD.resHeight,HPP_MOD.resDepth);
		
		// allocate memory for the 3d dataset
		datasetModified = (T**)malloc(HPP_MOD.resDepth * sizeof(T*));
		for (int i=0; i < HPP_MOD.resDepth; i++)
			datasetModified[i] = (T*)malloc(sizeof(T) * (HPP_MOD.resWidth*HPP_MOD.resHeight));
		
		// tranfer the data to a new dataset with new view plane
		for( int d = 0; d < HPP_MOD.resDepth; d++ )
			for( int w = 0; w < HPP_MOD.resWidth; w++ )
				for(int h = 0; h < HPP_MOD.resHeight; h++)
				{
					if(HPP_MOD.viewOrientation == 's')
						datasetModified[d][ijn(w,h,HPP_MOD.resWidth)] = datasetRaw[h][ijn(w,d,HPP_MOD.resWidth)];
					else if(HPP_MOD.viewOrientation == 'c')
						datasetModified[d][ijn(w,h,HPP_MOD.resWidth)] = datasetRaw[h][ijn(d,w,HPP_MOD.resWidth)];
				}
		return true;		
	}

	T** changePlane( char viewOrientation)
	{
		// change the the values of height to depth
		int rW = HPP_RAW.resWidth;
		int rD = HPP_RAW.resHeight;
		int rH = HPP_RAW.resDepth;

		//std::swap(rH,rD);
		
		// allocate memory for the 3d dataset
		T **newOrientation = (T**)malloc(rD * sizeof(T*));
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


	T* arbitraryPlane(int planeNumber, char viewOrientation, int slope )
	{
		int rW = HPP_RAW.resWidth;
		int rH = HPP_RAW.resHeight;
		int rD = HPP_RAW.resDepth;

		//allocate memory for the 2d image
		T  *img = (char*)malloc(sizeof(char*)* rW*rH); 
		T **newVolume;
		
		if((viewOrientation == 'c') || (viewOrientation == 's'))
		{
			// get the volume with view orientation changed
			newVolume = changePlane(viewOrientation);
			// change the the values of height to depth if 'coronal' or 'sagittal'
			std::swap(rH,rD);	
		}	

		// tranfer the data to a new dataset with new view plane
	//	for( int d = 0; d < rD; d++ )


		int d = 0;
		for( int w = 0; w < rW; w++, d++ )
			for(int h = 0; h < rH; h++)
			{
				if((viewOrientation == 'c' || viewOrientation == 's') && slope == 0)
					img[ijn(w,h,rW)] = newVolume[planeNumber][ijn(w,h,rW)];
				else if((viewOrientation == 'c' || viewOrientation == 's') && slope == 1)
					img[ijn(w,h,rW)] = newVolume[d][ijn(h,w,rW)];
				else if(viewOrientation == 'a' && slope == 0)
					img[ijn(w,h,rW)] = datasetRaw[planeNumber][ijn(w,h,rW)];
				else if(viewOrientation == 'a' && slope == 1 )
					img[ijn(w,h,rW)] = datasetRaw[d][ijn(h,w,rW)];
			}

		// int d = 0;
		// int w2 = 0;
		// for( int w = rW; w > 0; w--, d++, w2++ )
		// 	for(int h = 0; h < rH; h++)
		// 	{
		// 		if((viewOrientation == 'c' || viewOrientation == 's') && slope == 0)
		// 			img[ijn(w,h,rW)] = newVolume[planeNumber][ijn(w,h,rW)];
		// 		else if((viewOrientation == 'c' || viewOrientation == 's') && slope == 1)
		// 			img[ijn(w,h,rW)] = newVolume[d][ijn(h,w,rW)];
		// 		else if(viewOrientation == 'a' && slope == 0)
		// 			img[ijn(w,h,rW)] = datasetRaw[planeNumber][ijn(w,h,rW)];
		// 		else if(viewOrientation == 'a' && slope == 1 )
		// 			img[ijn(w,h,rW)] = datasetRaw[d][ijn(h,w,rW)];
		// 	}
		return img;				
	}

	bool scale3dDataset(int slicesInit, int slicesEnd, int scaleDownFactor)
	{
		if(!clearDataset(1)){printf("Failed to clear struct\n"); return false; }

		HPP_MOD.resWidth  = HPP_RAW.resWidth/scaleDownFactor;
		HPP_MOD.resHeight = HPP_RAW.resHeight/scaleDownFactor;
		HPP_MOD.resDepth  = slicesEnd-slicesInit;

		int xLimite = HPP_MOD.resWidth;
		int yLimite = HPP_MOD.resHeight;

		datasetModified = (T**)malloc(slicesEnd * sizeof(T*));
		for (int i=0; i < slicesEnd; i++)
			datasetModified[i] = (T*)malloc(sizeof(T) * (HPP_MOD.resWidth*HPP_MOD.resHeight));

		for (int d = slicesInit ; d < slicesEnd ; d++)
			for( int w = 0; w < xLimite ; w++ )
					for( int h = 0; h < yLimite ; h++)
						datasetModified[d-slicesInit][ijn(w,h,xLimite)] = datasetRaw[d][ijn(w*HPP_MOD.resWidth*scaleDownFactor,h*scaleDownFactor,scaleDownFactor)];
		return true;					
	}

	void setDatasetInfo(DATAINFO h_pp, int option)
	{
		if(option == 0)
			HPP_RAW = h_pp;
		else if (option == 1)
			HPP_MOD = h_pp;
	}

	T** getDataset(int plane)
	{
		if(plane==0)
			return datasetRaw;
		else
			return datasetModified;
	}

	DATAINFO getDatasetInfo(int option)
	{
		if(option == 0)
			return HPP_RAW;
		else if (option == 1)
			return HPP_MOD;
	}

	bool clearDatasetInfo(int option)
	{
		if(option == 0)
		{
		    HPP_RAW.resWidth = 0;
		    HPP_RAW.resHeight = 0;
		    HPP_RAW.resDepth = 0;
		    HPP_RAW.initStack = 0;
		    HPP_RAW.endStack = 0;
		}
		else if(option == 1)
		{
		    HPP_MOD.resWidth = 0;
		    HPP_MOD.resHeight = 0;
		    HPP_MOD.resDepth = 0;
		    HPP_MOD.initStack = 0;
		    HPP_MOD.endStack = 0;
		}
		return true;
	}

	bool clearDataset(int option)
	{
		if(!clearDatasetInfo(1)) 
			return false;
		datasetModified = NULL;
		return true;
	}

private:
	DATAINFO HPP_RAW, HPP_MOD;
	T **datasetRaw, **datasetModified;
	FILE *inFile, *outFile;		
};



#endif // CHANGEVIEWORIENTATION