//
//  cxRaknetServer.h
//  RaknetServer
//
//  Created by xuhua on 5/4/16.
//  Copyright © 2016 xuhua. All rights reserved.
//

#ifndef RaknetServer_cxRaknetServer_h
#define RaknetServer_cxRaknetServer_h

#include <raknet/cxServer.h>

CX_CPP_BEGIN

class cxRaknetServer : public cxServer
{
public:
    CX_DECLARE(cxRaknetServer);
protected:
    explicit cxRaknetServer();
    virtual ~cxRaknetServer();
};

CX_CPP_END

#endif


