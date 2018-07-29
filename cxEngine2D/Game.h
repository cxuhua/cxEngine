//
//  IOSEngine.h
//  cxEngine2D
//
//  Created by xuhua on 6/17/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine2D_IOSEngine_h
#define cxEngine2D_IOSEngine_h

#include <engine/cxEngine.h>
#include <engine/cxTcp.h>
#include <gmunpacket.h>
#include <gmpacket.h>

CX_CPP_BEGIN

class cxButton;
class Game : public cxEngine,public CGmUnPacketEx
{
public:
    CX_DECLARE(Game);
protected:
    explicit Game();
    virtual ~Game();
private:
    cxTcp *tcp;
public:
    void OnDispatch(const cxTouchable *e);
    void OnMain();
    int32 OnProcUserPkt(GMPKT_DATA *ptagPktData);
};

CX_CPP_END

#endif


