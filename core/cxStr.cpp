//
//  cxStr.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <ext/md5.h>
#include <ext/xxtea.h>
#include <ext/lzma.h>
#include <ext/aes.h>
#include <ext/utf8.h>
#include "cxStr.h"
#include "cxUtil.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxStr);

cxBool cxStr::IsOK(const cxStr *str)
{
    return str != nullptr && str->Size() > 0;
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

cxStr *cxStr::UTF8(cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    cxStr *rv = cxStr::Create()->AppFmt(fmt, ap);
    va_end(ap);
    return rv;
}

cxStr::cxStr()
{
    
}

cxStr::~cxStr()
{
    
}

cxStr *cxStr::Init(cxInt size,char c)
{
    assign(size,c);
    return this;
}

cxStr *cxStr::Init(cxAny data,cxInt size)
{
    assign((char *)data,size);
    return this;
}

cxStr *cxStr::Init(cchars str)
{
    assign(str);
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
    cxByte *rv = xxtea_encrypt((cxByte *)Data(), (xxtea_long)Size(), (cxByte *)key->Data(),(xxtea_long) key->Size(), &length);
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
    cxByte *rv = xxtea_decrypt((cxByte *)Data(), (xxtea_long)Size(), (cxByte *)key->Data(),(xxtea_long) key->Size(), &length);
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
    if(desLen < Size()){
        rv->Erase(desLen);
    }
    return rv;
}

const cxStr *cxStr::LzmaUncompress() const
{
    cxInt desLen = cxLzmaGetUncompressLen(Buffer());
    cxStr *rv = cxStr::Create()->Init(desLen);
    char *dst = rv->Buffer();
    if(cxLzmaUncompress(Data(), Size(), dst, &desLen) == nullptr){
        return nullptr;
    }
    rv->Erase(desLen);
    return rv;
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
    erase(begin() + p, begin() + p + n);
    return this;
}

cxStr *cxStr::Erase(cxInt p)
{
    erase(begin() + p);
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
    int len = (int)size();
    if(Size() != len){
        return false;
    }
    return memcmp(Data(), str, len) == 0;
}

cxBool cxStr::IsCaseEqu(cchars str) const
{
    if(str == nullptr || size() == 0){
        return false;
    }
    return strcasecmp(str, c_str()) == 0;
}

cxStr *cxStr::Clear()
{
    clear();
    return this;
}

cxStr *cxStr::Append(const cxStr *str)
{
    Append(str);
    return this;
}

cxStr *cxStr::Append(cchars data,cxInt n)
{
    append(data, n);
    return this;
}

cxStr *cxStr::Append(cchars data)
{
    append(data);
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
    insert(pos, buffer,size);
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
    insert(pos,data);
    return this;
}

cxStr *cxStr::Insert(cxInt pos,cchars data,cxInt n)
{
    insert(pos,data, n);
    return this;
}

cxStr *cxStr::Insert(cxInt pos,const cxStr *str)
{
    insert(pos, str->Data(),str->Size());
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
    append(buffer,size);
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

const cxStr *cxStr::MD5()
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
    return (chars)data();
}

cchars cxStr::Data() const
{
    return data();
}

char cxStr::At(int n) const
{
    return at(n);
}

cxBool cxStr::IsEmpty() const
{
    return empty();
}

cxInt cxStr::UTF8Size() const
{
    return getUTF8StrLength((unsigned char *)Buffer());
}

cxInt cxStr::Size() const
{
    return (cxInt)size();
}

CX_CPP_END