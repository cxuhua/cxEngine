//
//  cxStr.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <sys/time.h>
#include <ext/md5.h>
#include <ext/xxtea.h>
#include <ext/lzma.h>
#include <ext/aes.h>
#include <ext/utf8.h>
#include <math/cxPoint2F.h>
#include <math/cxPoint3F.h>
#include <math/cxColor4F.h>
#include "cxStr.h"
#include "cxUtil.h"
#include "cxArray.h"
#include "cxLocalized.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxStr);

cxBool cxStr::IsOK(const cxStr *str)
{
    return str!=nullptr && str->Size() > 0;
}

cxBool cxStr::IsOK(cchars str)
{
    return str != nullptr && strlen(str) > 0;
}

cxBool cxStr::IsEqu(cchars s1,cchars s2)
{
    if(s1 == nullptr || s2 == nullptr){
        return false;
    }
    return strcmp(s1, s2) == 0;
}

cxBool cxStr::IsCaseEqu(cchars s1,cchars s2)
{
    if(s1 == nullptr || s2 == nullptr){
        return false;
    }
    return strcasecmp(s1, s2) == 0;
}

cxStr *cxStr::UTF8(cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    cxStr *rv = cxStr::Create()->AppFmt(fmt, ap);
    va_end(ap);
    return rv;
}

cxStr *cxStr::Localized(cchars key,...)
{
    const cxStr *fmt = cxLocalized::Text(key);
    va_list ap;
    va_start(ap, key);
    cxStr *ret = cxStr::Create()->AppFmt(fmt->ToString(), ap);
    va_end(ap);
    return ret;
}

cxStr::cxStr()
{
    s.clear();
}

cxStr::~cxStr()
{
    s.clear();
}

const cxBool cxStr::ToBool() const
{
    return IsCaseEqu("true")?true:false;
}

const cxInt cxStr::ToInt() const
{
    return atoi(ToString());
}

static cxBool ccharsHasChar(cchars cs,cxByte c)
{
    cxInt len = (cxInt)strlen(cs);
    for(cxInt i=0;i<len;i++){
        if((cxByte)cs[i] == c){
            return true;
        }
    }
    return false;
}

const cxArray *cxStr::Split(cchars cs) const
{
    cxArray *rv = cxArray::Create();
    cchars data = Data();
    cxInt len = Size();
    cxInt b = 0;
    cxInt i = 0;
    for(;i<len;i++){
        if(!ccharsHasChar(cs,(cxByte)data[i])){
            continue;
        }
        rv->Append(cxStr::Create()->Append(data + b, i-b));
        b = i+1;
    }
    if(len > b){
        rv->Append(cxStr::Create()->Append(data + b, len - b));
    }
    return rv;
}

const cxArray *cxStr::Split(cxInt c) const
{
    cxArray *rv = cxArray::Create();
    cchars data = Data();
    cxInt len = Size();
    cxInt b = 0;
    cxInt i = 0;
    for(;i<len;i++){
        if(data[i] != c){
            continue;
        }
        rv->Append(cxStr::Create()->Append(data + b, i-b));
        b = i+1;
    }
    if(len > b){
        rv->Append(cxStr::Create()->Append(data + b, len - b));
    }
    return rv;
}

const cxBool cxStr::IsNumber() const
{
    for(cxInt i=0;i<Size();i++){
        char c = At(i);
        if(c >= '0' && c <= '9'){
            continue;
        }
        if(c == '.' || c == 'e'){
            continue;
        }
        if(c == '+' || c == '-'){
            continue;
        }
        return false;
    }
    return true;
}

const cxPoint2F cxStr::ToPoint2F() const
{
    const cxArray *ps = Split(',');
    if(ps->IsEmpty()){
        return cxPoint2F(0, 0);
    }
    if(ps->Size() == 1){
        cxFloat v = ps->At(0)->To<cxStr>()->ToFloat();
        return cxPoint2F(v, v);
    }
    if(ps->Size() == 2){
        cxFloat x = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat y = ps->At(1)->To<cxStr>()->ToFloat();
        return cxPoint2F(x, y);
    }
    return cxPoint2F(0, 0);
}

const cxPoint3F cxStr::ToPoint3F() const
{
    const cxArray *ps = Split(',');
    if(ps->IsEmpty()){
        return cxPoint3F(0, 0, 0);
    }
    if(ps->Size() == 1){
        cxFloat v = ps->At(0)->To<cxStr>()->ToFloat();
        return cxPoint3F(v, v, v);
    }
    if(ps->Size() == 2){
        cxFloat x = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat y = ps->At(1)->To<cxStr>()->ToFloat();
        return cxPoint3F(x, y, 0);
    }
    if(ps->Size() == 3){
        cxFloat x = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat y = ps->At(1)->To<cxStr>()->ToFloat();
        cxFloat z = ps->At(2)->To<cxStr>()->ToFloat();
        return cxPoint3F(x, y,z);
    }
    return cxPoint3F(0, 0, 0);
}

const cxColor4F cxStr::ToColor4F() const
{
    const cxArray *ps = Split(',');
    if(ps->IsEmpty()){
        return cxColor4F::WHITE;
    }
    if(ps->Size() == 1){
        cxFloat v = ps->At(0)->To<cxStr>()->ToFloat();
        return cxColor4F(v, v, v, 1.0f);
    }
    if(ps->Size() == 2){
        cxFloat r = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat g = ps->At(1)->To<cxStr>()->ToFloat();
        return cxColor4F(r, g, 1, 1);
    }
    if(ps->Size() == 3){
        cxFloat r = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat g = ps->At(1)->To<cxStr>()->ToFloat();
        cxFloat b = ps->At(2)->To<cxStr>()->ToFloat();
        return cxColor4F(r, g, b, 1);
    }
    if(ps->Size() == 4){
        cxFloat r = ps->At(0)->To<cxStr>()->ToFloat();
        cxFloat g = ps->At(1)->To<cxStr>()->ToFloat();
        cxFloat b = ps->At(2)->To<cxStr>()->ToFloat();
        cxFloat a = ps->At(3)->To<cxStr>()->ToFloat();
        return cxColor4F(r, g, b, a);
    }
    return cxColor4F::WHITE;
}

const cxFloat cxStr::ToFloat() const
{
    return atof(ToString());
}

cchars cxStr::ToString() const
{
    return Data();
}

cxObject *cxStr::ToObject()
{
    return cxObject::create(ToString());
}

cxStr *cxStr::Init(cxInt size,char c)
{
    s = s.assign(size,c);
    return this;
}

cxStr *cxStr::Init(cxAny data,cxInt size)
{
    s = s.assign((char *)data,size);
    return this;
}

cxStr *cxStr::Init(cchars str)
{
    s = s.assign(str);
    return this;
}

const cxStr *cxStr::AESEncode(const cxStr *key) const
{
    cxUtil::SetRandSeed();
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    for(cxInt i=0; i < AES_KEY_LENGTH; i++){
        iv[i] = cxUtil::Rand(0, 255);
    }
    cxInt keylen = key->Size();
    if(keylen > AES_KEY_LENGTH){
        memcpy(ik, key->Data(), AES_KEY_LENGTH);
    }else {
        memcpy(ik, key->Data(), keylen);
    }
    AES_KEY eKey;
    if(AES_set_encrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH * 8, &eKey) != 0){
        return NULL;
    }
    cxInt length = Size();
    cchars dataptr = Data();
    cxInt newsize = 0;
    if(length % AES_KEY_LENGTH == 0){
        newsize = length;
    }else{
        newsize = length + AES_KEY_LENGTH - (length % AES_KEY_LENGTH);
    }
    cxInt bc = newsize - length;
    cxInt bytessize = newsize + AES_KEY_LENGTH;
    cxStr *rv = cxStr::Create()->Init(bytessize);
    chars bytes = rv->Buffer();
    memcpy(bytes, iv, AES_KEY_LENGTH);
    chars ptr = bytes + AES_KEY_LENGTH;
    memcpy(ptr, dataptr, length);
    if(bc > 0){
        memset(ptr + length, bc, bc);
    }
    AES_cbc_encrypt((const cxUInt8 *)ptr, (cxUInt8 *)ptr, newsize, &eKey, iv, AES_ENCRYPT);
    return rv;
}

const cxStr *cxStr::AESDecode(const cxStr *key) const
{
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    if(key->Size() > AES_KEY_LENGTH){
        memcpy(ik, key->Data(), AES_KEY_LENGTH);
    }else {
        memcpy(ik, key->Data(), key->Size());
    }
    AES_KEY dKey;
    if(AES_set_decrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH*8, &dKey) != 0){
        return NULL;
    }
    cchars dataptr = Data();
    cxInt length = Size();
    memcpy(iv, dataptr, AES_KEY_LENGTH);
    cxInt bytessize = length - AES_KEY_LENGTH;
    cxStr *rv = cxStr::Create()->Init(bytessize);
    chars bytes = rv->Buffer();
    AES_cbc_encrypt((const cxUInt8 *)(dataptr + AES_KEY_LENGTH), (cxUInt8 *)bytes, bytessize, &dKey, iv, AES_DECRYPT);
    char bc = bytes[bytessize - 1];
    if(bc > 0 && bc < AES_KEY_LENGTH){
        char bv[bc];memset(bv, bc, bc);
        if(memcmp(bv,bytes+bytessize-bc,bc)==0){
            rv->Erase(bytessize - bc, bc);
        }
    }
    return rv;
}

const cxStr *cxStr::TeaEncode(const cxStr *key) const
{
    xxtea_long length = 0;
    cxByte *rv = xxtea_encrypt((cxByte *)Data(),(xxtea_long)Size(),(cxByte *)key->Data(),(xxtea_long)key->Size(),&length);
    if(rv == nullptr){
        return nullptr;
    }
    cxStr *ret = cxStr::Create()->Init(rv, length);
    free(rv);
    return ret;
}

const cxStr *cxStr::TeaDecode(const cxStr *key) const
{
    xxtea_long length = 0;
    cxByte *rv = xxtea_decrypt((cxByte *)Data(),(xxtea_long)Size(),(cxByte *)key->Data(),(xxtea_long)key->Size(),&length);
    if(rv == nullptr){
        return nullptr;
    }
    cxStr *ret = cxStr::Create()->Init(rv, length);
    free(rv);
    return ret;
}

const cxStr *cxStr::LzmaCompress() const
{
    cxInt desLen = cxLzmaGetCompressLen(Size());
    cxStr *rv = cxStr::Create()->Init(desLen);
    char *dst = rv->Buffer();
    if(cxLzmaCompress(Data(), Size(), dst, &desLen) == nullptr){
        return nullptr;
    }
    return rv->KeepBytes(desLen);
}

const cxStr *cxStr::LzmaUncompress() const
{
    cxInt desLen = cxLzmaGetUncompressLen(Buffer());
    cxStr *rv = cxStr::Create()->Init(desLen);
    char *dst = rv->Buffer();
    if(cxLzmaUncompress(Data(), Size(), dst, &desLen) == nullptr){
        return nullptr;
    }
    return rv->KeepBytes(desLen);
}

void cxStr::Print() const
{
    CX_LOGGER("%s",Data());
}

cxULong cxStr::Hash() const
{
    return XXH32(Data(), Size(), 0);
}

cxBool cxStr::IsEqu(const cxStr *str) const
{
    if(str == NULL){
        return false;
    }
    if(Size() != str->Size()){
        return false;
    }
    return memcmp(Data(), str->Data(), str->Size()) == 0;
}

cxBool cxStr::IsCaseEqu(const cxStr *str) const
{
    if(str == NULL){
        return false;
    }
    if(Size() != str->Size()){
        return false;
    }
    return strcasecmp(Data(), str->Data()) == 0;
}

cxStr *cxStr::Erase(cxInt p,cxInt n)
{
    if(n == 0)return this;
    s = s.erase(p, n);
    return this;
}

cxStr *cxStr::KeepBytes(cxInt bytes)
{
    if(bytes < 0){
        s.erase(s.end() + bytes , s.end());
        return this;
    }
    if(bytes == 0){
        Clear();
        return this;
    }
    s.erase(s.begin() + bytes, s.end());
    return this;
}

cxBool cxStr::IsEqu(cchars d,cxInt l) const
{
    if(d == NULL || l == 0){
        return false;
    }
    if(Size() != l){
        return false;
    }
    return memcmp(Data(), d, l) == 0;
}

cxBool cxStr::IsEqu(cchars str) const
{
    if(str == nullptr){
        return false;
    }
    int len = (int)s.size();
    if(Size() != len){
        return false;
    }
    return memcmp(Data(), str, len) == 0;
}

cxBool cxStr::IsCaseEqu(cchars str) const
{
    if(str == nullptr || s.size() == 0){
        return false;
    }
    return strcasecmp(str, s.c_str()) == 0;
}

cxStr *cxStr::Clear()
{
    s.clear();
    return this;
}

cxStr *cxStr::Append(const cxStr *str)
{
    s = s.append(str->Data(), str->Size());
    return this;
}

cxStr *cxStr::Append(cchars data,cxInt n)
{
    s = s.append(data, n);
    return this;
}

cxStr *cxStr::Append(cchars data)
{
    s = s.append(data);
    return this;
}


cxStr *cxStr::InsFmt(cxInt pos,cchars fmt,va_list ap)
{
    CX_ASSERT(fmt != nullptr, "fmt error");
    char *buffer = nullptr;
    int size = vasprintf(&buffer, fmt, ap);
    if(buffer == nullptr){
        return this;
    }
    s = s.insert(pos, buffer,size);
    free(buffer);
    return this;
}

cxStr *cxStr::InsFmt(cxInt pos,cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    InsFmt(pos,fmt, ap);
    va_end(ap);
    return this;
}

cxStr *cxStr::Insert(cxInt pos,cchars data)
{
    s = s.insert(pos,data);
    return this;
}

cxStr *cxStr::Insert(cxInt pos,cchars data,cxInt n)
{
    s = s.insert(pos,data, n);
    return this;
}

cxStr *cxStr::Insert(cxInt pos,const cxStr *str)
{
    s = s.insert(pos, str->Data(),str->Size());
    return this;
}

cxStr *cxStr::AppFmt(cchars fmt,va_list ap)
{
    CX_ASSERT(fmt != nullptr, "fmt error");
    char *buffer = nullptr;
    int size = vasprintf(&buffer, fmt, ap);
    if(buffer == nullptr){
        return this;
    }
    s = s.append(buffer,size);
    free(buffer);
    return this;
}

cxStr *cxStr::AppFmt(cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    AppFmt(fmt, ap);
    va_end(ap);
    return this;
}

cxBool cxStr::HasPrefix(cchars str) const
{
    if(str == nullptr){
        return false;
    }
    int len = (int)strlen(str);
    if(Size() < len){
        return false;
    }
    return memcmp(Data(), str, len) == 0;
}

cxBool cxStr::HasSuffix(cchars str) const
{
    if(str == nullptr){
        return false;
    }
    int len = (int)strlen(str);
    if(Size() < len){
        return false;
    }
    return memcmp(Data() + Size() - len, str, len) == 0;
}

cxInt32 cxStr::ReadInt32()
{
    CX_ASSERT(Size() >= sizeof(cxInt32), "bytes not enough");
    cxInt32 v = *(cxInt32 *)Buffer();
    Erase(0, sizeof(cxInt32));
    return v;
}

void cxStr::WriteInt32(cxInt32 v)
{
    Append((cchars)&v, sizeof(cxInt32));
}

const cxStr *cxStr::ReadBytes(cxInt bytes)
{
    CX_ASSERT(Size() >= bytes, "bytes not enough");
    cxStr *v = cxStr::Create();
    v->Append(Data(), bytes);
    Erase(0, bytes);
    return v;
}

void cxStr::WriteBytes(const cxStr *bytes)
{
    s = s.append(bytes->Data(), bytes->Size());
}

cxByte cxStr::ReadByte()
{
    cxByte v = *(cxByte *)Buffer();
    Erase(0, sizeof(cxByte));
    return v;
}

void cxStr::WriteByte(cxByte v)
{
    Append((cchars)&v,sizeof(cxByte));
}

const cxStr *cxStr::MD5() const
{
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    mongo_md5_state_t state={0};
    mongo_md5_byte_t digest[MD5_DIGEST_LENGTH+1]={0};
    mongo_md5_init(&state);
    mongo_md5_append(&state, (const mongo_md5_byte_t *)Data(), Size());
    mongo_md5_finish(&state, digest);
    char md5[MD5_DIGEST_LENGTH*2 + 1]={0};
    for(cxInt i = 0; i < MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
    return cxStr::UTF8(md5);
}

const cxStr *cxStr::NewObjectId()
{
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    srand((unsigned)val.tv_sec);
    cxUInt32 vtime = (cxUInt32)val.tv_sec;
    cxUInt32 vrand = (cxUInt32)rand();
    cxUInt32 vlast = (cxUInt)val.tv_usec;
    char buf[12]={0};
    memcpy(buf + 0, &vtime, sizeof(cxUInt32));
    memcpy(buf + 4, &vrand, sizeof(cxUInt32));
    memcpy(buf + 8, &vlast, sizeof(cxUInt32));
    char txt[25]={0};
    for(cxInt i = 0; i < 12; i++){
        txt[2*i] = hex[(buf[i] & 0xf0)>> 4];
        txt[2*i + 1] = hex[buf[i] & 0x0f];
    }
    return cxStr::Create()->Init(txt);
}

cxBool cxStr::HasPrefix(const cxStr *str) const
{
    return HasPrefix(str->Data());
}

cxBool cxStr::HasSuffix(const cxStr *str) const
{
    return HasSuffix(str->Data());
}

cxBool cxStr::WriteToFile(cchars file,cxBool replace)
{
    return cxUtil::Instance()->WriteDocument(file, this, replace);
}

const cxStr *cxStr::ReadFromFile(cchars file)
{
    return cxUtil::Instance()->DocumentData(file);
}

chars cxStr::Buffer() const
{
    return (chars)s.data();
}

cchars cxStr::Data() const
{
    return s.data();
}

char cxStr::At(cxInt n) const
{
    return s.at(n);
}

cxBool cxStr::IsEmpty() const
{
    return s.empty();
}

cxInt cxStr::EachUTF8(std::function<void(cchars cptr)> func) const
{
    cxInt ret = 0;
    char buf[8]={0};
    cchars ptr = Data();
    cxInt size = Size();
    for(cxInt i=0;i<size;){
        cxInt c = getNumBytesForUTF8(ptr[i]);
        memcpy(buf, ptr + i, c);
        buf[c] = 0;
        func(buf);
        i+=c;
        ret++;
    }
    return ret;
}

cxInt cxStr::UTF8Size() const
{
    return getUTF8StrLength((unsigned char *)Buffer());
}

cxInt cxStr::Size() const
{
    return (cxInt)s.size();
}

CX_CPP_END