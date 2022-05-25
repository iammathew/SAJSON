//
//	SAJSON.cpp
//
// Copyright (c) 2013 Natural Style Co. Ltd.
// Modified by iammathew 2022
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "SuperAnimCommon.h"

#define SAM_VERSION 1
#define TWIPS_PER_PIXEL (20.0f)
#define LONG_TO_FLOAT (65536.0f)

#define FRAMEFLAGS_REMOVES		0x01
#define FRAMEFLAGS_ADDS			0x02
#define FRAMEFLAGS_MOVES		0x04
#define FRAMEFLAGS_FRAME_NAME	0x08

#define MOVEFLAGS_ROTATE		0x4000
#define MOVEFLAGS_COLOR			0x2000
#define MOVEFLAGS_MATRIX		0x1000
#define MOVEFLAGS_LONGCOORDS	0x0800


#ifndef max
#define max(x,y) (((x) < (y)) ? (y) : (x))
#endif



namespace SuperAnim{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Animation object definition
	
	class SuperAnimObject
	{
	public:
		int mObjectNum;
		int mResNum;
		SuperAnimTransform mTransform;
		Color mColor;
	};
	
	typedef std::vector<SuperAnimObject> SuperAnimObjectVector;
	typedef std::map<int, SuperAnimObject> IntToSuperAnimObjectMap;
	
	class SuperAnimImage
	{
	public:
		SuperAnimSpriteId mSpriteId;
		std::string mImageName;
		int mWidth;
		int mHeight;
		SuperAnimTransform mTransform;
	};
	typedef std::vector<SuperAnimImage> SuperAnimImageVector;
	
	class SuperAnimFrame
	{
	public:
		SuperAnimObjectVector mObjectVector;
	};
	typedef std::vector<SuperAnimFrame> SuperAnimFrameVector;
	typedef std::map<std::string, int> StringToIntMap;
	class SuperAnimLabel{
	public:
		std::string mLabelName;
		int mStartFrameNum;
		int mEndFrameNum;
	};
	typedef std::vector<SuperAnimLabel> SuperAnimLabelArray;
	class SuperAnimMainDef
	{
	public:
		SuperAnimFrameVector mFrames;
		int mStartFrameNum;
		int mEndFrameNum;
		int mAnimRate;
		SuperAnimLabelArray mLabels;
		int mX;
		int mY;
		int mWidth;
		int mHeight;
		SuperAnimImageVector mImageVector;
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, SuperAnimMainDef> SuperAnimMainDefMap;
	class SuperAnimDefMgr
	{
	private:
		SuperAnimMainDefMap mMainDefCache;
	private:
		SuperAnimDefMgr();
		~SuperAnimDefMgr();
		
		// std::string theSuperAnimFile include the absolute path
		bool LoadSuperAnimMainDef(const std::string &theSuperAnimFile);
	public:
		static SuperAnimDefMgr *GetInstance();
		static void DestroyInstance();

		// std::string theSuperAnimFile include the absolute path
		SuperAnimMainDef *Load_GetSuperAnimMainDef(const std::string &theSuperAnimFile);
        SuperAnimMainDef *Load_GetSuperAnimMainDefEffect(const std::string &theSuperAnimFile);
		void UnloadSuperAnimMainDef(const std::string &theName);
	};


    SuperAnimSpriteId LoadSuperAnimSprite(std::string theSpriteName){
        return InvalidSuperAnimSpriteId;
    }
    
    void UnloadSuperSprite(SuperAnimSpriteId theSpriteId){
        // none
    }
}



unsigned char* GetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize){
    unsigned char * pBuffer = NULL;
    *pSize = 0;
    do
    {
        // read the file from hardware
        FILE *fp = fopen(pszFileName, pszMode);
        if(!fp) break;
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    return pBuffer;
}









int main(int argc, char* argv[]){
    if(argc < 2){
        printf("usage: SAJSON sam_path\n");
        return 1;
    }
    
    
    SuperAnim::SuperAnimMainDef* p;
    if (argc == 2) {
        p = SuperAnim::SuperAnimDefMgr::GetInstance()->Load_GetSuperAnimMainDef(argv[1]);
    } else {
        // ex: SAJON sam_path -effect
        // will run as long as there are more than 2 arguments
        p = SuperAnim::SuperAnimDefMgr::GetInstance()->Load_GetSuperAnimMainDefEffect(argv[1]);
    }
    
    printf("{");
    
    printf("\"mAnimRate\":%d,", p->mAnimRate);
    printf("\"mX\":%d,", p->mX);
    printf("\"mY\":%d,", p->mY);
    printf("\"mWidth\":%d,", p->mWidth);
    printf("\"mHeight\":%d,", p->mHeight);
    
    printf("\"mImageVector\":[");
    bool first = true;
    for(SuperAnim::SuperAnimImageVector::const_iterator i=p->mImageVector.begin(); i!=p->mImageVector.end(); ++i){
        if (first) {
            first = false;
        } else {
            printf(",");
        }
        printf("{");
        printf("\"mImageName\":\"%s\",", i->mImageName.c_str());
        printf("\"mWidth\":%d,", i->mWidth);
        printf("\"mHeight\":%d,", i->mHeight);
        
        printf("\"mTransform\":{\"mMatrix\":{\"m\":[[%f,%f,%f],[%f,%f,%f],[%f,%f,%f]]}}",
            i->mTransform.mMatrix.m[0][0],
            i->mTransform.mMatrix.m[0][1],
            i->mTransform.mMatrix.m[0][2],
            i->mTransform.mMatrix.m[1][0],
            i->mTransform.mMatrix.m[1][1],
            i->mTransform.mMatrix.m[1][2],
            i->mTransform.mMatrix.m[2][0],
            i->mTransform.mMatrix.m[2][1],
            i->mTransform.mMatrix.m[2][2]
        );
        printf("}");
    }
    printf("],");

    printf("\"mStartFrameNum\":%d,", p->mStartFrameNum);
    printf("\"mEndFrameNum\":%d,", p->mEndFrameNum);
    
    bool firstFrame = true;
    printf("\"mFrames\":[");
    for(SuperAnim::SuperAnimFrameVector::const_iterator i=p->mFrames.begin(); i!=p->mFrames.end(); ++i){
        if (firstFrame) {
            firstFrame = false;
        } else {
            printf(",");
        }
        bool firstObject = true;
        printf("{ \"mObjectVector\":[");
        for(SuperAnim::SuperAnimObjectVector::const_iterator j=i->mObjectVector.begin(); j!=i->mObjectVector.end(); ++j){
            if (firstObject) {
                firstObject = false;
            } else {
                printf(",");
            }
            printf("{\"mObjectNum\":%d,\"mResNum\":%d,\"mTransform\":{\"mMatrix\":{\"m\":[[%f,%f,%f],[%f,%f,%f],[%f,%f,%f]]}},\"mColor\":{\"mRed\":%d,\"mGreen\":%d,\"mBlue\":%d,\"mAlpha\":%d}}",
                j->mObjectNum,
                j->mResNum,
                j->mTransform.mMatrix.m[0][0],
                j->mTransform.mMatrix.m[0][1],
                j->mTransform.mMatrix.m[0][2],
                j->mTransform.mMatrix.m[1][0],
                j->mTransform.mMatrix.m[1][1],
                j->mTransform.mMatrix.m[1][2],
                j->mTransform.mMatrix.m[2][0],
                j->mTransform.mMatrix.m[2][1],
                j->mTransform.mMatrix.m[2][2],
                j->mColor.mRed,
                j->mColor.mGreen,
                j->mColor.mBlue,
                j->mColor.mAlpha
            );
        }
        printf("]}");
    }
    printf("],");
    bool firstLabel = true;
    printf("\"mLabels\":[");
    for(SuperAnim::SuperAnimLabelArray::const_iterator i=p->mLabels.begin(); i!=p->mLabels.end(); ++i){
        if (firstLabel) {
            firstLabel = false;
        } else {
            printf(",");
        }
        printf("{\"mLabelName\":\"%s\",\"mStartFrameNum\":%d,\"mEndFrameNum\":%d}",
            i->mLabelName.c_str(),
            i->mStartFrameNum,
            i->mEndFrameNum
        );
    }
    printf("]");
    
    printf("}");
    
    
    
    return 0;
}
