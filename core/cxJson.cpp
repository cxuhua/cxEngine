//
//  cxJson.cpp
//  cxEngineCore
//
//  Created by xuhua on 5/24/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxJson.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxJson);

cxJson::Iter &cxJson::Iter::operator++(int)
{
    return Next();
}

cxJson::Iter &cxJson::Iter::operator++()
{
    return Next();
}

cxBool cxJson::Iter::operator==(const Iter &v)
{
    return json == v.json;
}

cxBool cxJson::Iter::operator!=(const Iter &v)
{
    return json != v.json;
}

cxJson::Iter &cxJson::Iter::operator+=(cxInt n)
{
    while(n-- > 0 && IsOK())Next();
    return *this;
}

cxBool cxJson::Iter::IsOK()
{
    return json != nullptr;
}

cxJson::Iter &cxJson::Iter::Next()
{
    CX_ASSERT(IsOK(), "iter error");
    if(json_is_object(json)){
        objIt = json_object_iter_next(json, objIt);
        if(objIt == nullptr)json = nullptr;
    }else if(json_is_array(json)){
        step ++;
        if(step >= number)json = nullptr;
    }else{
        json = nullptr;
    }
    return *this;
}

cxBool cxJson::Iter::Value(cxJson *value)
{
    json_t *j = nullptr;
    if(json_is_array(json)){
        j = json_array_get(json, step);
    }else if(json_is_object(json)){
        j = json_object_iter_value(objIt);
    }else{
        j = json;
    }
    value->From(j);
    return true;
}

cxJson *cxJson::Iter::Alloc()
{
    cxJson *ret = cxJson::Alloc();
    Value(ret);
    return ret;
}

cxJson *cxJson::Iter::Value()
{
    cxJson *ret = cxJson::Create();
    Value(ret);
    return ret;
}

cxInt cxJson::Iter::Index()
{
    return step;
}

cchars cxJson::Iter::Key()
{
    if(json_is_object(json)){
        return json_object_iter_key(objIt);
    }
    return nullptr;
}

cxJson::Iter::Iter(json_t *j)
{
    number = 0;
    step = 0;
    json = j;
}

const cxJson::Iter cxJson::Begin() const
{
    if(Size() == 0){
        return Iter(nullptr);
    }
    Iter iter(json);
    if(IsArray()){
        iter.number = (cxInt)json_array_size(json);
    }else if(IsObject()){
        iter.number = (cxInt)json_object_size(json);
        iter.objIt = json_object_iter(json);
    }
    return iter;
}
const cxJson::Iter &cxJson::End() const
{
    static Iter end = Iter(nullptr);
    return end;
}

cxJson::cxJson() : json(nullptr)
{
    
}

cxJson::~cxJson()
{
    json_decref(json);
}

cxJson *cxJson::Create(cchars fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    cxStr *str = cxStr::Create()->AppFmt(fmt, ap);
    va_end(ap);
    return cxJson::Create()->From(str);
}

cxBool cxJson::IsOK(const cxJson *json)
{
    if(json == nullptr){
        return false;
    }
    if(json->IsNull()){
        return false;
    }
    return true;
}

cxInt cxJson::Size() const
{
    if(IsObject()){
        return (cxInt)json_object_size(json);
    }
    if(IsArray()){
        return (cxInt)json_array_size(json);
    }
    return 0;
}

cxBool cxJson::Get(cchars key,cxBool value) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return value;
    }
    return j->ToBool();
}

cxInt cxJson::Get(cchars key,cxInt value) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return value;
    }
    return j->ToInt();
}

cxInt64 cxJson::Get(cchars key,cxInt64 value) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return value;
    }
    return j->ToInt64();
}

cxFloat cxJson::Get(cchars key,cxFloat value) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return value;
    }
    return j->ToFloat();
}

cchars cxJson::Get(cchars key,cchars value) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return value;
    }
    return j->ToChars();
}

const cxStr *cxJson::Get(cchars key) const
{
    const cxJson *j = At(key);
    if(j == nullptr){
        return nullptr;
    }
    return j->ToStr();
}

cxBool cxJson::Get(cxInt idx,cxBool value) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return value;
    }
    return j->ToBool();
}

cxInt cxJson::Get(cxInt idx,cxInt value) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return value;
    }
    return j->ToInt();
}

cxInt64 cxJson::Get(cxInt idx,cxInt64 value) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return value;
    }
    return j->ToInt64();
}

cxFloat cxJson::Get(cxInt idx,cxFloat value) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return value;
    }
    return j->ToFloat();
}

cchars cxJson::Get(cxInt idx,cchars value) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return value;
    }
    return j->ToChars();
}

const cxStr *cxJson::Get(cxInt idx) const
{
    const cxJson *j = At(idx);
    if(j == nullptr){
        return nullptr;
    }
    return j->ToStr();
}

cxInt cxJson::jsonDumpFunc(cchars buffer, size_t size, void *data)
{
    cxStr *str = static_cast<cxStr *>(data);
    str->Append(buffer, (cxInt)size);
    return 0;
}

const cxStr *cxJson::Dumps(cxInt flags) const
{
    cxStr *str = cxStr::Create();
    cxInt x = json_dump_callback(json, cxJson::jsonDumpFunc, str, JSON_ENCODE_ANY|flags);
    if(x != 0){
        CX_ERROR("dump json error");
        return nullptr;
    }
    return str;
}

cxJson *cxJson::Set(cchars key,cxBool value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    json_object_set_new(json, key, json_boolean(value));
    return this;
}

cxJson *cxJson::Set(cchars key,cxInt value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    json_object_set_new(json, key, json_integer(value));
    return this;
}

cxJson *cxJson::Set(cchars key,cxInt64 value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    json_object_set_new(json, key, json_integer(value));
    return this;
}

cxJson *cxJson::Set(cchars key,cxFloat value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    json_object_set_new(json, key, json_real(value));
    return this;
}

cxJson *cxJson::Set(cchars key,cchars value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    if(value == nullptr){
        json_object_set_new(json, key, json_null());
    }else{
        json_object_set_new(json, key, json_string(value));
    }
    return this;
}

cxJson *cxJson::Set(cchars key,const cxStr *value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    if(!cxStr::IsOK(value)){
        json_object_set_new(json, key, json_null());
    }else{
        json_object_set_new(json, key,json_string(value->Data()));
    }
    return this;
}

cxJson *cxJson::Set(cchars key,const cxJson *value)
{
    CX_ASSERT(IsObject() && key != nullptr, "json not object,set error");
    if(value == nullptr){
        json_object_set_new(json, key, json_null());
    }else{
        json_object_set_new(json, key, json_incref(value->ToJson()));
    }
    return this;
}

const cxJson *cxJson::At(cxInt idx) const
{
    if(!IsArray()){
        CX_WARN("Use At for array not is object");
        return nullptr;
    }
    if(idx < 0 || idx >= json_array_size(json)){
        return nullptr;
    }
    json_t *rv = json_array_get(json, idx);
    return (rv != nullptr)?cxJson::Create()->From(rv):nullptr;
}

const cxJson *cxJson::At(const cxStr *key) const
{
    return At(key->Data());
}

const cxJson *cxJson::At(cchars key) const
{
    if(!IsObject()){
        CX_WARN("Use At for object not is object");
        return nullptr;
    }
    json_t *rv = json_object_get(json, key);
    return (rv != nullptr)?cxJson::Create()->From(rv):nullptr;
}

//if field is number
static cxBool jsonKeyIsNumber(cchars ckey,cxInt *num)
{
    if(ckey == nullptr){
        return false;
    }
    cxInt len = (cxInt)strlen(ckey);
    for(cxInt i=0; i < len; i++){
        if(!isdigit(ckey[i])){
            return false;
        }
    }
    if(num != NULL){
        *num = atoi(ckey);
    }
    return true;
}

//obj.0.id.x
const cxJson *cxJson::Select(cchars sel) const
{
    cxInt index = 0;
    json_t *rv = nullptr;
    if(jsonKeyIsNumber(sel, &index)){
        rv = json_array_get(json, index);
    }else{
        rv = json_object_get(json, sel);
    }
    if(rv != NULL){
        return cxJson::Create()->From(rv);
    }
    cxInt len = (cxInt)strlen(sel);
    char ckey[len];
    memcpy(ckey, sel, len);
    cxInt num = 0;
    chars ckeys[16];
    chars src = ckey;
    ckeys[num++] = src;
    while(*src++ != '\0'){
        if(*src != '.'){
            continue;
        }
        CX_ASSERT(num < 16, " . opt too more");
        ckeys[num++] = src+1;
        *src++ = '\0';
    }
    json_t *pv = json;
    for(cxInt i=0; i < num;i ++){
        if(pv == nullptr){
            return nullptr;
        }
        cchars ckey = ckeys[i];
        if(json_is_array(pv)){
            cxBool isint = jsonKeyIsNumber(ckey, &index);
            CX_ASSERT(isint, "key type error");(void)isint;
            rv = json_array_get(pv, index);
        }else if(json_is_object(pv)){
            rv = json_object_get(pv, ckey);
        }else {
            return nullptr;
        }
        pv = rv;
    }
    return cxJson::Create()->From(rv);
}

cxJson *cxJson::Append(cxBool value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    json_array_append_new(json, json_boolean(value));
    return this;
}

cxJson *cxJson::Append(cxInt value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    json_array_append_new(json, json_integer(value));
    return this;
}

cxJson *cxJson::Append(cxInt64 value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    json_array_append_new(json, json_integer(value));
    return this;
}

cxJson *cxJson::Append(cxFloat value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    json_array_append_new(json, json_real(value));
    return this;
}

cxJson *cxJson::Append(cchars value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    if(value == nullptr){
        json_array_append_new(json, json_null());
    }else{
        json_array_append_new(json, json_string(value));
    }
    return this;
}

cxJson *cxJson::Append(const cxStr *value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    if(!cxStr::IsOK(value)){
        json_array_append_new(json, json_null());
    }else{
        json_array_append_new(json, json_string(value->Data()));
    }
    return this;
}

cxJson *cxJson::Append(const cxJson *value)
{
    CX_ASSERT(IsArray(), "json not array,append error");
    if(value == nullptr){
        json_array_append_new(json, json_null());
    }else{
        json_array_append_new(json, json_incref(value->ToJson()));
    }
    return this;
}

cxBool cxJson::IsObject() const
{
    return json_is_object(json);
}
cxBool cxJson::IsArray() const
{
    return json_is_array(json);
}

cxBool cxJson::IsBool() const
{
    return json_is_boolean(json);
}

cxBool cxJson::IsString() const
{
    return json_is_string(json);
}

cxBool cxJson::IsFloat() const
{
    return json_is_real(json);
}

cxBool cxJson::IsNumber() const
{
    return json_is_integer(json) || json_is_real(json);
}

cxBool cxJson::IsInt() const
{
    return json_is_integer(json);
}

cxBool cxJson::IsNull() const
{
    return json_is_null(json);
}

cxJson *cxJson::Array()
{
    json_decref(json);
    json = json_array();
    return this;
}

cxJson *cxJson::Object()
{
    json_decref(json);
    json = json_object();
    return this;
}

cxJson *cxJson::CreateObject()
{
    return cxJson::Create()->Object();
}

cxJson *cxJson::CreateArray()
{
    return cxJson::Create()->Array();
}

cxJson *cxJson::ToAny() const
{
    return cxJson::Create()->From(json);
}

cxJson *cxJson::ToArray() const
{
    CX_ASSERT(IsArray(), "json not array");
    return cxJson::Create()->From(json);
}

cxJson *cxJson::ToObject() const
{
    CX_ASSERT(IsObject(), "json not object");
    return cxJson::Create()->From(json);
}

json_t *cxJson::ToJson() const
{
    return json;
}

cxBool cxJson::ToBool() const
{
    if(json_is_boolean(json)){
        return json_is_true(json);
    }
    if(json_is_real(json)){
        return json_real_value(json) != 0;
    }
    if(json_is_integer(json)){
        return json_integer_value(json) != 0;
    }
    if(json_is_string(json)){
        return strcasecmp(json_string_value(json), "true") == 0;
    }
    return false;
}

cxInt cxJson::ToInt() const
{
    if(json_is_integer(json)){
        return json_integer_value(json);
    }
    if(json_is_real(json)){
        return json_real_value(json);
    }
    if(json_is_boolean(json)){
        return json_is_true(json)?1:0;
    }
    if(json_is_string(json)){
        return atoll(json_string_value(json));
    }
    return 0;
}

cxInt64 cxJson::ToInt64() const
{
    if(json_is_integer(json)){
        return json_integer_value(json);
    }
    if(json_is_real(json)){
        return json_real_value(json);
    }
    if(json_is_boolean(json)){
        return json_is_true(json)?1:0;
    }
    if(json_is_string(json)){
        return atoll(json_string_value(json));
    }
    return 0;
}

cxFloat cxJson::ToFloat() const
{
    if(json_is_real(json)){
        return json_real_value(json);
    }
    if(json_is_integer(json)){
        return json_integer_value(json);
    }
    if(json_is_boolean(json)){
        return json_is_true(json)?1.0f:0.0f;
    }
    if(json_is_string(json)){
        return atof(json_string_value(json));
    }
    return 0;
}

cchars cxJson::ToChars() const
{
    static char buf[32]={0};
    if(json_is_string(json)){
        return json_string_value(json);
    }
    if(json_is_real(json)){
        snprintf(buf, 32, "%f",json_real_value(json));
        return buf;
    }
    if(json_is_integer(json)){
        snprintf(buf, 32, "%lld",json_integer_value(json));
        return buf;
    }
    if(json_is_boolean(json)){
        snprintf(buf, 32, "%d",json_is_true(json)?1:0);
        return buf;
    }
    return buf;
}

const cxStr *cxJson::ToStr() const
{
    cchars str = ToChars();
    return cxStr::Create()->Init(str);
}

cxJson *cxJson::From(cxInt value)
{
    json_decref(json);
    json = json_integer(value);
    return this;
}

cxJson *cxJson::From(cxFloat value)
{
    json_decref(json);
    json = json_real(value);
    return this;
}

cxJson *cxJson::From(cxBool value)
{
    json_decref(json);
    json = json_boolean(value);
    return this;
}

cxJson *cxJson::From(json_t *js)
{
    json_decref(json);
    if(js == nullptr){
        json = json_null();
    }else{
        json = js;
        json_incref(json);
    }
    return this;
}

cxJson *cxJson::From(const cxStr *str)
{
    json_decref(json);
    if(!cxStr::IsOK(str)){
        json = json_null();
        return this;
    }
    return From(str->Data());
}

cxJson *cxJson::From(const cxJson *json)
{
    CX_ASSERT(json->IsString(), "type error");
    return cxJson::Create()->From(json->ToStr());
}

cxJson *cxJson::From(cchars buf,cxInt size)
{
    if(buf == NULL || size == 0){
        return nullptr;
    }
    json_decref(json);
    json_error_t error = {0};
    json = json_loadb(buf,size,JSON_DECODE_ANY, &error);
    if(json == nullptr){
        CX_ERROR("cxJson load error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
        return nullptr;
    }
    return this;
}

cxJson *cxJson::From(cchars str)
{
    if(!cxStr::IsOK(str)){
        return nullptr;
    }
    json_decref(json);
    json_error_t error = {0};
    json = json_loads(str,JSON_DECODE_ANY, &error);
    if(json == nullptr){
        CX_ERROR("cxJson load error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
        return nullptr;
    }
    return this;
}


CX_CPP_END
