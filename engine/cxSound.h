//
//  cxSound.h
//  cxEngineCore
//
//  Created by xuhua on 7/29/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxSound_h
#define cxEngineCore_cxSound_h

#include "cxOpenAL.h"
#include "cxAction.h"

CX_CPP_BEGIN

class cxSound : public cxAction
{
public:
    CX_DECLARE(cxSound);
protected:
    explicit cxSound();
    virtual ~cxSound();
protected:
    void OnInit();
    void OnExit();
private:
    cxALSource *source;
    static cxFloat Load(cchars file);
    static void Play(cxFloat volume=1.0f,cxBool loop=true);
    static void StopMusic();
public:
    static cxSound *Create(cchars file);
    static void SetVolume(cxFloat volume);
    static void Pause();
    static void Resume();
};

CX_CPP_END

#endif


