//
//  cxStr.h
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef __cxEngineCore__cxStr__
#define __cxEngineCore__cxStr__

#include <string>
#include <math/cxPoint2F.h>
#include <math/cxPoint3F.h>
#include <math/cxColor4F.h>
#include <math/cxSize2F.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "cxArray.h"

CX_CPP_BEGIN

#define AES_KEY_LENGTH  16

class cxStr : public cxObject
{
public:
    CX_DECLARE(cxStr);
protected:
    explicit cxStr();
    virtual ~cxStr();
private:
    std::string s;
public:
    static const cxStr *NewObjectId();
    static cxStr *Localized(cchars key,...);
    static cxStr *UTF8(cchars fmt,...);
    static cxBool IsOK(const cxStr *str);
    static cxBool IsOK(cchars str);
    static cxBool IsEqu(cchars s1,cchars s2);
    static cxBool IsCaseEqu(cchars s1,cchars s2);
    
    cxStr *Init(cxInt size,char c=0);
    cxStr *Init(cchars str);
    cxStr *Init(cxAny data,cxInt size);
    cxStr *Init(const cxStr *v);

    void Print() const;
    
    const cxArray *Split(cxInt c) const;
    const cxArray *Split(cchars cs) const;
    
    cxULong Hash() const;
    
    cxBool IsEmpty() const;
    cxInt UTF8Size() const;
    cxInt Size() const;
    char At(cxInt n) const;
    cchars Data() const;
    chars Buffer() const;
    
    const cxBool IsNumber() const;
    const cxBool ToBool() const;
    const cxInt ToInt() const;
    const cxInt64 ToInt64() const;
    const cxFloat ToFloat() const;
    const cxPoint2F ToPoint2F() const;
    const cxSize2F ToSize2F() const;
    const cxPoint3F ToPoint3F() const;
    const cxColor4F ToColor4F() const;
    cchars ToString() const;
    
    cxObject *ToObject();
    
    cxStr *ToLower();
    cxStr *ToUpper();
    
    static const cxStr *PBEncode(const pb_field_t fields[], const void *src,cxInt max=2048);
    static cxBool PBDecode(cchars data, cxInt size, const pb_field_t fields[], void *dst);
    cxBool PBDecode(const pb_field_t fields[], void *dst) const;
    
    const cxStr *TeaEncode(const cxStr *key) const;
    const cxStr *TeaDecode(const cxStr *key) const;
    
    const cxStr *AESEncode(const cxStr *key) const;
    const cxStr *AESDecode(const cxStr *key) const;
    
    const cxStr *Base64Encode() const;
    const cxStr *Base64Decode() const;
    
    const cxStr *HexEncode() const;
    const cxStr *HexDecode() const;
    
    const cxStr *LzmaCompress() const;
    const cxStr *LzmaUncompress() const;
    
    const cxStr *MD5() const;
    
    cxBool WriteToFile(cchars file,cxBool replace);
    static const cxStr *ReadFromFile(cchars file);
    
    cxInt16 ReadInt16();
    void WriteInt16(cxInt16 v);
    
    cxInt32 ReadInt32();
    void WriteInt32(cxInt32 v);
    
    cxInt64 ReadInt64();
    void WriteInt64(cxInt64 v);
    
    const cxStr *ReadBytes(cxInt bytes);
    void WriteBytes(const cxStr *bytes);
    
    cxByte ReadByte();
    void WriteByte(cxByte v);
    
    cxStr *AppFmt(cchars fmt,va_list ap);
    cxStr *AppFmt(cchars fmt,...);
    cxStr *Append(cchars data);
    cxStr *Append(cchars data,cxInt n);
    cxStr *Append(const cxStr *str);
    
    cxStr *InsFmt(cxInt pos,cchars fmt,va_list ap);
    cxStr *InsFmt(cxInt pos,cchars fmt,...);
    cxStr *Insert(cxInt pos,cchars data);
    cxStr *Insert(cxInt pos,cchars data,cxInt n);
    cxStr *Insert(cxInt pos,const cxStr *str);
    
    cxInt EachUTF8(std::function<void(cchars cptr,cxInt len)> func) const;
    
    cxStr *Clear();
    cxStr *Erase(cxInt p,cxInt n);
    cxStr *KeepBytes(cxInt bytes);
    cxStr *Clone() const;
    
    cxBool IsEqu(const cxStr *str) const;
    cxBool IsCaseEqu(const cxStr *str) const;
    cxBool IsEqu(cchars str) const;
    cxBool IsCaseEqu(cchars str) const;
    cxBool IsEqu(cchars d,cxInt l) const;
    
    cxBool HasPrefix(const cxStr *str) const;
    cxBool HasPrefix(cchars str) const;
    cxBool HasSuffix(const cxStr *str) const;
    cxBool HasSuffix(cchars str) const;
public:
    static cxStr *Create(cchars str);
};

// for nanopb callback

bool cxStrPBEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);

bool cxStrPBDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);

#define CX_STR_PB_ENCODE(_s_)   {.funcs.encode=&cxStrPBEncode,.arg=_s_}

#define CX_STR_PB_DECODE(_s_)   {.funcs.decode=&cxStrPBDecode,.arg=_s_}

CX_CPP_END

#endif /* defined(__cxEngineCore__cxStr__) */
