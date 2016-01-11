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
#include "cxObject.h"

CX_CPP_BEGIN

#define AES_KEY_LENGTH  16

struct cxPoint2F;
struct cxPoint3F;
struct cxColor4F;
class cxArray;
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
    const cxFloat ToFloat() const;
    const cxPoint2F ToPoint2F() const;
    const cxPoint3F ToPoint3F() const;
    const cxColor4F ToColor4F() const;
    cchars ToString() const;
    
    cxObject *ToObject();
    
    cxStr *ToLower();
    cxStr *ToUpper();
    
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
    
    cxInt EachUTF8(std::function<void(cchars cptr)> func) const;
    
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
};

CX_CPP_END

#endif /* defined(__cxEngineCore__cxStr__) */
