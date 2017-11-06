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
    static cxStr *NewObjectId();
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
    
    cxArray *Split(cxInt c) const;
    cxArray *Split(cchars cs) const;
    static cxArray *Split(cchars data,cchars cs);
    static cxArray *Split(cchars data,cxInt c);
    
    cxULong Hash() const;
    
    cxBool IsEmpty() const;
    cxInt UTF8Size() const;
    cxInt Size() const;
    char At(cxInt n) const;
    cchars Data() const;
    chars Buffer() const;
    
    cxBool IsNumber() const;
    static cxBool IsNumber(cchars cs);
    
    cxBool IsInt() const;
    static cxBool IsInt(cchars cs);
    
    cxBool ToBool() const;
    cxInt ToInt() const;
    cxInt64 ToInt64() const;
    cxFloat ToFloat() const;
    cxPoint2F ToPoint2F() const;
    cxSize2F ToSize2F() const;
    cxPoint3F ToPoint3F() const;
    cxColor4F ToColor4F() const;
    cchars ToChars() const;
    
    cxObject *ToObject();
    
    cxStr *ToLower();
    cxStr *ToUpper();
    
    cxStr *TeaEncode(const cxStr *key) const;
    cxStr *TeaDecode(const cxStr *key) const;
    
    cxStr *AESEncode(const cxStr *key) const;
    cxStr *AESDecode(const cxStr *key) const;
    
    cxStr *Base64Encode() const;
    cxStr *Base64Decode() const;
    
    cxStr *HexEncode() const;
    cxStr *HexDecode() const;
    
    cxStr *LzmaCompress() const;
    cxStr *LzmaUncompress() const;
    
    cxStr *ZlibCompress(int level=9) const;
    cxStr *ZlibUncompress() const;
    
    cxStr *MD5() const;
    cxStr *BinMD5() const;
    
    cxBool WriteToFile(cchars file,cxBool replace);
    static cxStr *ReadFromFile(cchars file);
    
    cxInt16 ReadInt16();
    void WriteInt16(cxInt16 v);
    
    cxInt32 ReadInt32();
    void WriteInt32(cxInt32 v);
    
    cxUInt32 ReadUInt32();
    void WriteUInt32(cxUInt32 v);
    
    cxInt64 ReadInt64();
    void WriteInt64(cxInt64 v);
    
    cxStr *ReadBytes(cxInt bytes);
    void WriteBytes(const cxStr *bytes);
    
    cxByte ReadByte();
    void WriteByte(cxByte v);
    
    cxStr *AppFmt(cchars fmt,va_list ap);
    cxStr *AppFmt(cchars fmt,...);
    cxStr *Append(cchars data);
    cxStr *Append(cchars data,cxInt n);
    cxStr *Append(cxAny data,cxInt n);
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
    static cxStr *Create(cxAny ptr,cxInt size);
    static cxStr *Alloc(cchars str);
    static cxStr *Alloc(cxAny ptr,cxInt size);
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxStr__) */
