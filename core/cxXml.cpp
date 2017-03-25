//
//  cxXml.cpp
//  cxEngineCore
//
//  Created by xuhua on 13/01/2017.
//  Copyright © 2017 xuhua. All rights reserved.
//


#include "cxXml.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxXml);

cxXml::cxXml()
{
    parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, cxXml::xmlStart, cxXml::xmlEnd);
    XML_SetCharacterDataHandler(parser, cxXml::xmlCharacter);
    XML_SetCdataSectionHandler(parser, cxXml::xmlStartCdataSection, cxXml::xmlEndCdataSection);
}

cxXml::~cxXml()
{
    XML_ParserFree(parser);
}

void cxXml::OnElementStart(cchars name,cchars *attr)
{
    
}

void cxXml::OnElementEnd(cchars name)
{
    
}

void cxXml::OnCharacter(cchars data,cxInt len)
{
    
}

void cxXml::OnStartCdataSection()
{
    
}

void cxXml::OnEndCdataSection()
{
    
}

void XMLCALL cxXml::xmlStartCdataSection(void *data)
{
    cxXml *pxml = static_cast<cxXml *>(data);
    pxml->OnStartCdataSection();
}

void XMLCALL cxXml::xmlEndCdataSection(void *data)
{
    cxXml *pxml = static_cast<cxXml *>(data);
    pxml->OnStartCdataSection();
}

void XMLCALL cxXml::xmlCharacter(void *data,const XML_Char *s,int len)
{
    cxXml *pxml = static_cast<cxXml *>(data);
    pxml->OnCharacter(s, len);
}

void XMLCALL cxXml::xmlStart(void *data, const char *el, const char **attr)
{
    cxXml *pxml = static_cast<cxXml *>(data);
    pxml->OnElementStart(el, attr);
}

void XMLCALL cxXml::xmlEnd(void *data, const char *el)
{
    cxXml *pxml = static_cast<cxXml *>(data);
    pxml->OnElementEnd(el);
}

cxBool cxXml::Parse(const cxStr *data)
{
    XML_Status ret = XML_Parse(parser, data->Buffer(), data->Size(), true);
    if(ret == XML_STATUS_ERROR){
        XML_Size line = XML_GetCurrentLineNumber(parser);
        CX_ERROR("parse xml error: %ld %s", line, XML_ErrorString(XML_GetErrorCode(parser)));
        return false;
    }
    return true;
}

cxXml *cxXml::Create(const cxStr *data)
{
    cxXml *ret = cxXml::Create();
    if(!ret->Parse(data)){
        return nullptr;
    }
    return ret;
}

CX_CPP_END

