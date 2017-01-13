//
//  cxXml.h
//  cxEngineCore
//
//  Created by xuhua on 13/01/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxXml_h
#define cxEngineCore_cxXml_h

#include <expat/expat.h>
#include "cxHash.h"

CX_CPP_BEGIN

class cxXml : public cxObject
{
public:
    CX_DECLARE(cxXml);
protected:
    explicit cxXml();
    virtual ~cxXml();
private:
    XML_Parser parser;
    static void XMLCALL xmlCharacter(void *data,const XML_Char *s,int len);
    static void XMLCALL xmlStart(void *data, const char *el, const char **attr);
    static void XMLCALL xmlEnd(void *data, const char *el);
    static void XMLCALL xmlStartCdataSection(void *data);
    static void XMLCALL xmlEndCdataSection(void *data);
protected:
    virtual void OnElementStart(cchars name,cchars *attr);
    virtual void OnElementEnd(cchars name);
    virtual void OnCharacter(cchars data,cxInt len);
    virtual void OnStartCdataSection();
    virtual void OnEndCdataSection();
public:
    cxBool Parse(const cxStr *data);
public:
    static cxXml *Create(const cxStr *data);
};

CX_CPP_END

#endif


