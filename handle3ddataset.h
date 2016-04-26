/* Class to handle datasets
	by: Jeronimo G. Grandi
	Jun,2013
*/  

#ifndef HANDLE_3D_DATASET
#define HANDLE_3D_DATASET

#include <cstdlib>
#include <fstream>

#include "handle3ddataset_utils.h"
#include <vector3f.h>

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

	// from and to can be 0(char) or 1(unsigned int)
	template<class K>
	K**  changeBits(int to)
	{

		K** newBitAlloc = (K**)malloc(HPP_MOD.resDepth * sizeof(K*));
		for (int i=0; i < HPP_MOD.resDepth; i++)
			newBitAlloc[i] = (K*)malloc(sizeof(K) * (HPP_MOD.resHeight*HPP_MOD.resWidth));

		for( int d = 0; d < HPP_MOD.resDepth; d++ )
			for( int w = 0; w < HPP_MOD.resWidth; w++ )
				for( int h = 0; h < HPP_MOD.resHeight; h++ )
				{
					unsigned int value = datasetRaw[d][ijn(w,h,HPP_MOD.resWidth)];
					printf("%d\n",value );
					if(to == 0)
					{
						char newVal = char(value);//*255)/65535;
						
						newBitAlloc[d][ijn(w,h,HPP_MOD.resWidth)] = newVal;
					}
					else if (to == 1)
					{
						unsigned int newVal = (value*65535)/255;
						newBitAlloc[d][ijn(w,h,HPP_MOD.resWidth)] = newVal;
					}
				}		
		return newBitAlloc;
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

	bool saveModifiedDataset(T **datasetToSave, DATAINFO INFO)
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

	template <class D>
	bool saveModifiedDataset(int to)
	{
		// open output dataset file
		if(!(outFile = fopen(HPP_MOD.fileName, "wb+")))
			return false;

		D** datasetBitsChanged = changeBits<D>(to);

		//save the new view plane into a new raw file
		for(int i = 0; i < HPP_MOD.resDepth; i++)
			fwrite(datasetBitsChanged[i], 1, sizeof(D)*HPP_MOD.resWidth*HPP_MOD.resHeight, outFile);

		fclose(outFile);
		return true;
	}


	bool saveModifiedImage(T *imageToSave, DATAINFO INFO)
	{
		// open output dataset file
		if(!(outFile = fopen(INFO.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		fwrite(imageToSave, 1, sizeof(T)*INFO.resWidth*INFO.resHeight, outFile);

		fclose(outFile);
		return true;
	}

	template <class D>
	bool saveModifiedImage(D *imageToSave, DATAINFO INFO)
	{
		// open output dataset file
		if(!(outFile = fopen(INFO.fileName, "wb+")))
			return false;

		//save the new view plane into a new raw file
		fwrite(imageToSave, 1, sizeof(D)*INFO.resWidth*INFO.resHeight, outFile);

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




// Direction vector from point (-1, 8, 0) to (2, 4, -3) = <3, -4, -3> 
// Direction vector from point (-1, 8, 0) to (-1, 9, 2) = <0, 1, 2> 

// Vector normal to plane is 
// <3, -4, -3> x <0, 1, 2> = <-5, -6, 3> 
// Vector -1 * <-5, -6, 3> = <5, 6, -3> is also normal to plane 

// Using normal vector <5, 6, -3> and point (2, 4, -3), we get equation of plane: 
// 5 (x-2) + 6 (y-4) - 3 (z+3) = 0 
// 5x - 10 + 6y - 24 - 3z - 9 = 0 
// 5x + 6y - 3z = 43

	void arbitraryPlane(T *&img, int vp1, int vp2,float dxx1,float dxx2, vector3f &vec_normal, float &plane_d)
	{

		int rW = HPP_RAW.resWidth;
		int rH = HPP_RAW.resHeight;
		//int rD = HPP_RAW.resDepth;

		//T  *img = (T*)calloc(rD,sizeof(T*)* rW*rH); 

		// int var1 = rW/2; //8
		// int var2 = rW; //16
		// int var3 = rW/4; //8

		vector3f p1(0,0,(float)vp1);  // para inclinação lateral
		vector3f p2(0,rH-1,(float)vp2);
		vector3f p3(rW-1,0,(float)vp1);

		// vector3f p1(0,0,10); //para inclinação frontal
		// vector3f p2(rW,0,10);
		// vector3f p3(0,rH,31);

		// vector3f p1(0,10,0); //para inclinação vertical
		// vector3f p2(0,10,rD-1);
		// vector3f p3(rW-1,10,10);


		vector3f v1 = sub(p2,p1);
		vector3f v2 = sub(p3,p1);

		vector3f dx1 =v1/(float)dxx1; //qnto mais inclinado, diminuir o valor de dx
		vector3f dx2 =v2/(float)dxx2;

		v1.normalize();
		v2.normalize();

		vector3f interp1 = p1;// + dx1 + v1;
		vector3f interp2 = p2;//+ dx2 + v2;

		for (int w = 0; w < rW; w++)
		{
			
			//printf("INTERP1:%d,%d,%d\n", (int)interp1.x,(int)interp1.y, (int)interp1.z );

			for (int h = 0; h < rH; h++)
			{
				img[ijn(w,h,rW)] = datasetRaw[(int)interp1.z][ijn((int)interp1.y,(int)interp2.x,rW)]; //para inclinação lateral
				// img[ijn(h,w,rW)] = datasetRaw[(int)interp2.z][ijn((int)interp2.y,(int)interp1.x,rW)]; // para inclinação frontal
//				img[ijn(h,w,rW)] = datasetRaw[(int)interp1.y][ijn((int)interp2.x,(int)interp1.z,rW)]; // para inclinação vertical
				interp2 = interp2 + dx2 + v2;
				//h++;
			}
			//vector3f aux = interp1;
			interp1 = interp1+ dx1 + v1;
			interp2 = p2;			
		}


		// while(interp1.y<=rH-1)
		// {
		// 	interp1 = interp1+ dx1 + v1;
		// 	printf("INTERP1:%d,%d,%d\n", (int)interp1.x,(int)interp1.y, (int)interp1.z );
			
		// 	while(interp2.x<=rW-1)
		// 	{
		// 		interp2 = interp2 + dx2 + v2;
		// 		printf("INTERP2:%d,%d,%d\n", (int)interp2.x,(int)interp2.y, (int)interp2.z );
		// 		img[ijn(w,h,rW)] = datasetRaw[(int)interp1.z][ijn((int)interp1.y,(int)interp2.x,rW)];
		// 		h++;				
		// 	}
		// 	w++;
		// 	interp2 = p2 + dx2 + v2;
		// }
		 

//		v1.x = abs(v1.x); v1.y = abs(v1.y); v1.z = abs(v1.z);
//		v2.x = abs(v2.x); v2.y = abs(v2.y); v2.z = abs(v2.z);


		// do
		// {
		// 	do
		// 	{
		// 		aux2 = v1+dx1+v1n;

		// 	}
		// 	while(aux2<=p1.x)

		// }
		// while(aux<=p2.y)




		vector3f n = -crossProduct(v1,v2);
		//p1.normalize();
		
		vector3f cn(n.z,n.y,n.x);


		//vector3f other = p3/2;
		vector3f p3n(p3.x/HPP_RAW.resWidth*2-1, p3.y/HPP_RAW.resWidth*2-1,p3.z/HPP_RAW.resWidth*2-1);

        float  d = -dotProduct(p3n,n);
		

		//d = d/HPP_RAW.resWidth*2-1;

		// printf("V1:%f,%f,%f\n",v1.x,v1.y,v1.z );
		// printf("V2:%f,%f,%f\n",v2.x,v2.y,v2.z );
		// printf("VNormal:%f,%f,%f\n",n.z,n.y,n.x );
		// printf("%f\n",d );

		vec_normal = cn;
		plane_d = d;
		//allocate memory for the 2d image

		//T **newVolume;
		
		// if((viewOrientation == 'c') || (viewOrientation == 's'))
		// {
		// 	// get the volume with view orientation changed
		// 	newVolume = changePlane(viewOrientation);
		// 	// change the the values of height to depth if 'coronal' or 'sagittal'
		// 	std::swap(rH,rD);	
		// }	

		// tranfer the data to a new dataset with new view plane
	//	for( int d = 0; d < rD; d++ )


		// int d = 0;
		// for( int w = 0; w < rW; w++, d++ )
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

		//return img;				
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
		else
			return HPP_RAW;
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