//
//  cxTexture.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <libpng/png.h>
#include <libjpeg/jpeglib.h>
#include <core/cxUtil.h>
#include <core/cxJson.h>
#include <core/cxLQT.h>
#include "cxOpenGL.h"
#include "OpenGL.h"
#include "cxTexture.h"
#include "cxEngine.h"
#include "cxLabel.h"

#pragma pack(1)

typedef struct{
    cxUInt32 headerLength;
    cxUInt32 height;
    cxUInt32 width;
    cxUInt32 numMipmaps;
    cxUInt32 flags;
    cxUInt32 dataLength;
    cxUInt32 bpp;
    cxUInt32 bitmaskRed;
    cxUInt32 bitmaskGreen;
    cxUInt32 bitmaskBlue;
    cxUInt32 bitmaskAlpha;
    cxUInt32 pvrTag;
    cxUInt32 numSurfs;
} cxPVRHeader;

// v2
enum{
    cxFormatPVR2BPP = 24,
    cxFormatPVR4BPP,
};

#define CX_PVR_TAG (*(cxUInt32 *)("PVR!"))

#define CX_PVR_TEXTURE_FLAG_TYPE_MASK   0xff

#define CX_PVR_MAX_TABLE_ELEMENTS       16

typedef struct{
    cxUInt32 pkmTag;
    cxUInt16 version;
    cxUInt16 type;
    cxUInt16 extWidth;
    cxUInt16 extHeight;
    cxUInt16 orgWidth;
    cxUInt16 orgHeight;
} cxPKMHeader;

#define CX_PKM_TAG  (*(cxUInt32 *)("PKM\x20"))
#define CX_PKM_V1   (*(cxUInt16 *)("10"))
#define CX_PKM_V2   (*(cxUInt16 *)("20"))

#pragma pack()

CX_CPP_BEGIN

CX_IMPLEMENT(cxTexCoord);

cxTexCoord::cxTexCoord()
{
    texture = nullptr;
    coord = cxBoxCoord2F::Default;
    rotated = false;
    trimmed = false;
}

cxTexCoord::~cxTexCoord()
{
    
}

void cxTexCoord::SetTexture(cxTexture *v)
{
    texture = v;
}

cxTexture *cxTexCoord::Texture()
{
    return texture;
}

const cxSize2F &cxTexCoord::Size() const
{
    return sourceSize;
}

const cxSize2F cxTexCoord::FrameSize() const
{
    return cxSize2F(frame.w, frame.h);
}

const cxBool cxTexCoord::IsEmpty() const
{
    cxBool bx = cxFloatIsZero(spriteSourceSize.x);
    cxBool by = cxFloatIsZero(spriteSourceSize.y);
    cxBool bw = cxFloatIsEqual(spriteSourceSize.w, 1.0f);
    cxBool bh = cxFloatIsEqual(spriteSourceSize.h, 1.0f);
    return bx && by && bw && bh;
}

cxBox4F cxTexCoord::Trimmed(const cxBoxPoint3F &box,const cxSize2F &size,cxBool flipx,cxBool flipy) const
{
    cxBox4F b = box.ToBox4F();
    return Trimmed(b, size,flipx, flipy);
}

cxBox4F &cxTexCoord::Trimmed(cxBox4F &vbox,const cxSize2F &size,cxBool flipx,cxBool flipy) const
{
    cxFloat xs = 1.0f;
    cxFloat ys = 1.0f;
    if(trimmed){
        vbox.l = spriteSourceSize.x - sourceSize.w/2.0f;
        if(flipx){
            vbox.l = -vbox.l - spriteSourceSize.w;
        }
        vbox.r = vbox.l + spriteSourceSize.w;
        vbox.t = sourceSize.h/2.0f - spriteSourceSize.y;
        if(flipy){
            vbox.t = -vbox.t + spriteSourceSize.h;
        }
        vbox.b = vbox.t - spriteSourceSize.h;
        xs = size.w / sourceSize.w;
        ys = size.h / sourceSize.h;
    }
    vbox.l = vbox.l * xs;
    vbox.r = vbox.r * xs;
    vbox.t = vbox.t * ys;
    vbox.b = vbox.b * ys;
    return vbox;
}

const cxBoxCoord2F &cxTexCoord::BoxCoord(const cxBox4F &pixel,cxBool flipx,cxBool flipy)
{
    if(texture == nullptr){
        return cxBoxCoord2F::Default;
    }
    cxSize2F size = texture->Size();
    if(rotated){
        cxFloat l = (frame.x + pixel.t) / size.w;
        cxFloat r = (frame.x + frame.h - pixel.t - pixel.b)/size.w;
        cxFloat t = (frame.y + pixel.l)/size.h;
        cxFloat b = (frame.y + frame.w - pixel.l - pixel.r)/size.h;
        if(flipx)CX_SWAP_VAR(t,b);
        if(flipy)CX_SWAP_VAR(l,r);
        coord.lb = cxCoord2F(l,t);
        coord.rb = cxCoord2F(l,b);
        coord.rt = cxCoord2F(r,b);
        coord.lt = cxCoord2F(r,t);
    }else{
        cxFloat l = (frame.x + pixel.l) / size.w;
        cxFloat r = (frame.x + frame.w - pixel.l - pixel.r)/size.w;
        cxFloat t = (frame.y + pixel.t)/size.h;
        cxFloat b = (frame.y + frame.h - pixel.t - pixel.b)/size.h;
        if(flipx)CX_SWAP_VAR(l,r);
        if(flipy)CX_SWAP_VAR(t,b);
        coord.lb = cxCoord2F(l,b);
        coord.rb = cxCoord2F(r,b);
        coord.rt = cxCoord2F(r,t);
        coord.lt = cxCoord2F(l,t);
    }
    return coord;
}

CX_IMPLEMENT(cxTexture);

const cxTextureParams cxTextureParams::Default  = {GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
const cxTextureParams cxTextureParams::Repeat   = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};

cxTexture::cxTexture()
{
    type = RAW;
    success = true;
    texId = 0;
    size = cxSize2F(0, 0);
    coords = cxHash::Alloc();
}

cxTexture::~cxTexture()
{
    coords->Release();
    cxOpenGL::Instance()->DeleteTexture(texId);
}

const cxSize2F &cxTexture::Size() const
{
    return size;
}

cxTexture *cxTexture::Bind()
{
    cxOpenGL::Instance()->BindTexture(texId);
    return this;
}

cxTexture *cxTexture::GenTexture()
{
    texId = cxOpenGL::Instance()->GenTexture();
    return this;
}

cxTexture *cxTexture::SetParams(const cxTextureParams &params)
{
    cxOpenGL::Instance()->BindTexture(texId);
    cxOpenGL::Instance()->SetTextureParams(params);
    return this;
}

cxBool cxTexture::IsSuccess() const
{
    return success && texId > 0;
}

const cxTextureId cxTexture::ID() const
{
    return texId;
}

cxTexture *cxTexture::Atlas(cchars file)
{
    CX_ASSERT(IsSuccess(), "not load texture,first use FromPNG...");
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)){
        return this;
    }
    return Atlas(data);
}

cxTexCoord *cxTexture::At(cchars key) const
{
    return static_cast<cxTexCoord *>(coords->Get(key));
}

cxTexCoord *cxTexture::At(const cxStr *key) const
{
    return At(key->ToString());
}

cxTexture *cxTexture::From(cchars file,const cxStr *data)
{
    cchars ext = strrchr(file, '.');
    if(!cxStr::IsOK(ext)){
        CX_ASSERT(false, "file not ext name");
        return nullptr;
    }
    if(cxStr::IsEqu(ext, ".png")){
        return FromPNG(data);
    }
    if(cxStr::IsEqu(ext, ".pvr")){
        return FromPVR(data);
    }
    if(cxStr::IsEqu(ext, ".pkm")){
        return FromPKM(data);
    }
    if(cxStr::IsEqu(ext, ".jpg")){
        return FromJPG(data);
    }
    if(cxStr::IsEqu(ext, ".lqt")){
        return FromLQT(data);
    }
    return nullptr;
}

cxTexture *cxTexture::From(cchars file)
{
    cchars ext = strrchr(file, '.');
    if(!cxStr::IsOK(ext)){
        CX_ASSERT(false, "file not ext name");
        return nullptr;
    }
    if(cxStr::IsEqu(ext, ".png")){
        return FromPNG(file);
    }
    if(cxStr::IsEqu(ext, ".pvr")){
        return FromPVR(file);
    }
    if(cxStr::IsEqu(ext, ".pkm")){
        return FromPKM(file);
    }
    if(cxStr::IsEqu(ext, ".jpg")){
        return FromJPG(file);
    }
    if(cxStr::IsEqu(ext, ".lqt")){
        return FromLQT(file);
    }
    return nullptr;
}

cxInt cxTexture::Type() const
{
    return type;
}

cxTexture *cxTexture::From(cxInt type,const cxStr *data)
{
    if(type == PNG){
        return FromPNG(data);
    }
    if(type == PVR){
        return FromPVR(data);
    }
    if(type == PKM){
        return FromPKM(data);
    }
    return this;
}

cxTexture *cxTexture::Atlas(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "data error");
    cxJson *json = cxJson::Create()->From(data);
    if(json == nullptr){
        CX_ERROR("load atlas json file failed");
        success = false;
        return this;
    }
    const cxJson *meta = json->At("meta");
    if(meta == nullptr){
        CX_ERROR("atlas json file miss meta node");
        success = false;
        return this;
    }
    const cxJson *frames = json->At("frames");
    if(frames == nullptr){
        CX_ERROR("atlas json file miss frams node");
        success = false;
        return this;
    }
    for(cxJson::Iter it=frames->Begin();it != frames->End();it++){
        cxJson *item = cxJson::Alloc();
        it.Value(item);
        cxTexCoord *coord = cxTexCoord::Alloc();
        cchars key = item->Get("filename", nullptr);
        CX_ASSERT(key != nullptr, "filename node miss");
        coord->SetTexture(this);
        coord->rotated = item->Get("rotated", false);
        coord->trimmed = item->Get("trimmed", false);
        coord->frame = cxRect4F(item->At("frame"));
        coord->sourceSize = cxSize2F(item->At("sourceSize"));
        coord->spriteSourceSize = cxRect4F(item->At("spriteSourceSize"));
        coords->Set(key, coord);
        coord->Release();
        item->Release();
    }
    return this;
}

typedef struct {
    struct jpeg_error_mgr pub;
    cxBool error;
}cxJPEGError;

static void cxJPGErrorExit(j_common_ptr cinfo)
{
    cxJPEGError *error = (cxJPEGError *)cinfo->err;
    error->error = true;
}

cxTexture *cxTexture::FromJPG(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "data error");
    struct jpeg_decompress_struct cinfo={0};
    cxJPEGError error;
    cinfo.err = jpeg_std_error(&error.pub);
    error.error = false;
    error.pub.error_exit = cxJPGErrorExit;
    jpeg_create_decompress(&cinfo);
    do{
        if(error.error){
            CX_ERROR("jpg create decompress failed");
            success = false;
            break;
        }
        jpeg_mem_src(&cinfo, (cxByte *)data->Buffer(), data->Size());
        jpeg_read_header(&cinfo, (boolean)true);
        if(error.error){
            CX_ERROR("jpg read head failed");
            success = false;
            break;
        }
        jpeg_start_decompress(&cinfo);
        if(error.error){
            CX_ERROR("jpg start decompress failed");
            success = false;
            break;
        }
        size = cxSize2F(cinfo.image_width, cinfo.image_height);
        int row_stride = cinfo.output_width * cinfo.output_components;
        char *dataptr = new char[row_stride * cinfo.output_height];
        JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
        int line = 0;
        while(cinfo.output_scanline < cinfo.output_height){
            jpeg_read_scanlines(&cinfo, buffer, 1);
            memcpy((char *)dataptr + line * row_stride, buffer[0], row_stride);
            line++;
        }
        GLint unpack = 0;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GenTexture()->Bind()->SetParams(cxTextureParams::Default);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cinfo.image_width, cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataptr);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
        delete []dataptr;
        jpeg_finish_decompress(&cinfo);
    }while(0);
    jpeg_destroy_decompress(&cinfo);
    return this;
}

cxTexture *cxTexture::UpdateRGBA(cchars data)
{
    Bind()->SetParams(cxTextureParams::Default);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return this;
}

cxTexture *cxTexture::FromRGBA(cchars data,cxInt width,cxInt height)
{
    if(data == nullptr || width == 0 || height == 0){
        success = false;
        return this;
    }
    size = cxSize2F(width, height);
    type = RAW;
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return this;
}

cxTexture *cxTexture::UpdateRGB(cchars data)
{
    GLint unpack = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    Bind()->SetParams(cxTextureParams::Default);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.w, size.h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    return this;
}

cxTexture *cxTexture::FromRGB(cchars data,cxInt width,cxInt height)
{
    if(data == nullptr || width == 0 || height == 0){
        success = false;
        return this;
    }
    size = cxSize2F(width, height);
    type = RAW;
    GLint unpack = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    return this;
}

//*.lqt
cxTexture *cxTexture::FromLQT(const cxStr *data)
{
    cxInt hl = sizeof(LQT);
    LQT *lqt=(LQT *)data->Buffer();
    if(lqt->flag != CX_LQT_TAG){
        CX_ERROR("lqt file tag error");
        success = false;
        return this;
    }
    if(lqt->filebytes != lqt->atlasbytes + lqt->databytes + hl){
        CX_ERROR("lqt file length error");
        success = false;
        return this;
    }
    if(lqt->format == 0){
        CX_ERROR("lqt file format error");
        success = false;
        return this;
    }
    if(lqt->databytes <= 0){
        CX_ERROR("lqt file data miss");
        success = false;
        return this;
    }
    cxStr *tmp = cxStr::Create()->Init(data->Buffer() + hl + lqt->atlasbytes, lqt->databytes);
    if(!cxStr::IsOK(tmp)){
        CX_ERROR("lqt image data miss");
        success = false;
        return this;
    }
    const cxStr *imagedata = tmp->LzmaUncompress();
    if(!cxStr::IsOK(imagedata)){
        CX_ERROR("lqt image data unzip error");
        success = false;
        return this;
    }
    size = cxSize2F(lqt->width,lqt->height);
    GLenum type = GL_UNSIGNED_BYTE;
    if(lqt->format == LQT::FormatRGBA4444){
        CX_ASSERT(imagedata->Size() == lqt->width * lqt->height * 2, "rgba4444 data error");
        type = GL_UNSIGNED_SHORT_4_4_4_4;
    }else if(lqt->format == LQT::FormatRGBA5551){
        CX_ASSERT(imagedata->Size() == lqt->width * lqt->height * 2, "rgba5551 data error");
        type = GL_UNSIGNED_SHORT_5_5_5_1;
    }else if(lqt->format == LQT::FormatRGB565){
        CX_ASSERT(imagedata->Size() == lqt->width * lqt->height * 2, "rgba565 data error");
        type = GL_UNSIGNED_SHORT_5_6_5;
    }else{
        CX_ASSERT(imagedata->Size() == lqt->width * lqt->height * 4, "rgba8888 data error");
        type = GL_UNSIGNED_BYTE;
    }
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    if(lqt->format == LQT::FormatRGB565){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lqt->width, lqt->height, 0, GL_RGB, type, imagedata->Buffer());
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lqt->width, lqt->height, 0, GL_RGBA, type, imagedata->Buffer());
    }
    if(lqt->atlasbytes == 0){
        return this;
    }
    tmp = cxStr::Create()->Init(data->Buffer() + hl,lqt->atlasbytes);
    if(!cxStr::IsOK(tmp)){
        CX_WARN("lqt atlas data miss");
        success = false;
        return this;
    }
    const cxStr *atlasdata = tmp->LzmaUncompress();
    if(!cxStr::IsOK(atlasdata)){
        CX_WARN("atlas data unzip error");
        return this;
    }
    return Atlas(atlasdata);
}

cxTexture *cxTexture::From(cxTextureId name,const cxSize2F &siz)
{
    texId = name;
    size = siz;
    success = true;
    return this;
}

cxTexture *cxTexture::FromPNG(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "data error");
    png_image image={0};
    image.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_memory(&image, data->Data(), data->Size())){
        success = false;
        return this;
    }
    image.format = PNG_FORMAT_RGBA;
    size = cxSize2F(image.width,image.height);
    cxInt bufsiz = PNG_IMAGE_SIZE(image);
    cxAny buffer = malloc(bufsiz);
    if(png_image_finish_read(&image, NULL, buffer, 0, NULL)){
        GenTexture()->Bind()->SetParams(cxTextureParams::Default);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    }
    free(buffer);
    png_image_free(&image);
    type = PNG;
    return this;
}

cxTexture *cxTexture::FromPVR(const cxStr *data)
{
    CX_ASSERT(cxStr::IsOK(data), "data error");
    GLenum format;
    GLenum glFormat = 0;
    cxPVRHeader *header = (cxPVRHeader *)data->Buffer();
    if(header->headerLength != sizeof(cxPVRHeader) || header->pvrTag != CX_PVR_TAG){
        CX_ERROR("pvr file head error");
        success = false;
        return this;
    }
    cxUInt32 flags = header->flags;
    format = flags & CX_PVR_TEXTURE_FLAG_TYPE_MASK;
    if(format != cxFormatPVR2BPP && format != cxFormatPVR4BPP){
        CX_ERROR("only support 2bpp and 4bpp format");
        success = false;
        return this;
    }
    if(format == cxFormatPVR2BPP){
        glFormat = header->bitmaskAlpha?GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
    }else {
        glFormat = header->bitmaskAlpha?GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    }
    size = cxSize2F(header->width, header->height);
    cxUInt blockSize = 0;
    cxUInt bpp = 4;
    cxUInt widthBlocks = 0;
    cxUInt heightBlocks = 0;
    cxAny buffer = data->Buffer() + sizeof(cxPVRHeader);
    if (format == cxFormatPVR4BPP){
        blockSize = 4 * 4;
        widthBlocks = header->width / 4;
        heightBlocks = header->height / 4;
        bpp = 4;
    }else{
        blockSize = 8 * 4;
        widthBlocks = header->width / 8;
        heightBlocks = header->height / 4;
        bpp = 2;
    }
    if (widthBlocks < 2){
        widthBlocks = 2;
    }
    if (heightBlocks < 2){
        heightBlocks = 2;
    }
    cxUInt bufSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormat, header->width, header->height, 0, bufSize, buffer);
    type = PVR;
    return this;
}

cxTexture *cxTexture::FromPKM(const cxStr *data)
{
    cxPKMHeader *header = (cxPKMHeader *)data->Data();
    if(header->pkmTag != CX_PKM_TAG){
        CX_ERROR("read pkm header failed");
        success = false;
        return this;
    }
    if(header->version != CX_PKM_V1){
        CX_ERROR("PKM only use V1 version");
        success = false;
        return this;
    }
    header->extWidth    = CX_SWAP16(header->extWidth);
    header->extHeight   = CX_SWAP16(header->extHeight);
    header->orgWidth    = CX_SWAP16(header->orgWidth);
    header->orgHeight   = CX_SWAP16(header->orgHeight);
    size = cxSize2F(header->extWidth, header->extHeight);
#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    cxInt bufsiz = data->Size() - sizeof(cxPKMHeader);
    cxAny buffer = data->Buffer() + sizeof(cxPKMHeader);
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, header->extWidth, header->extHeight, 0, bufsiz, buffer);
#else
    success = false;
#endif
    type = PKM;
    return this;
}

cxTexture *cxTexture::FromTXT(const cxStr *txt,const cxTextAttr &attr,cxUInt64 *key)
{
    const cxStr *data = cxEngine::Instance()->TextImage(txt, attr, size);
    if(!cxStr::IsOK(data)){
        success = false;
        return this;
    }
    type = TXT;
    GenTexture()->Bind()->SetParams(cxTextureParams::Default);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->Buffer());
    return this;
}

cxTexture *cxTexture::FromPKM(cchars file)
{
    if(!cxOpenGL::Instance()->support_GL_OES_compressed_ETC1_RGB8_texture){
        CX_ERROR("platform not support pkm etc1 texture format");
        success = false;
        return this;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        CX_ERROR("texture file data miss:%s",file);
        success = false;
        return this;
    }
    return FromPKM(data);
}

cxTexture *cxTexture::FromPVR(cchars file)
{
    if(!cxOpenGL::Instance()->support_GL_IMG_texture_compression_pvrtc){
        CX_ERROR("platform not support pvrtc texture format");
        success = false;
        return this;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        CX_ERROR("texture file data miss:%s",file);
        success = false;
        return this;
    }
    return FromPVR(data);
}

cxTexture *cxTexture::FromJPG(cchars file)
{
    if(!cxOpenGL::Instance()->support_GL_OES_rgb8_rgba8){
        CX_ERROR("platform not support rgb8 rgba8 format");
        success = false;
        return this;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        CX_ERROR("texture file data miss:%s",file);
        success = false;
        return this;
    }
    return FromJPG(data);
}

cxTexture *cxTexture::FromLQT(cchars file)
{
    if(!cxOpenGL::Instance()->support_GL_OES_rgb8_rgba8){
        CX_ERROR("platform not support rgb8 rgba8 format");
        success = false;
        return this;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        CX_ERROR("texture file data miss:%s",file);
        success = false;
        return this;
    }
    return FromLQT(data);
}

cxTexture *cxTexture::FromPNG(cchars file)
{
    if(!cxOpenGL::Instance()->support_GL_OES_rgb8_rgba8){
        CX_ERROR("platform not support rgb8 rgba8 format");
        success = false;
        return this;
    }
    const cxStr *data = cxUtil::Assets(file);
    if(!cxStr::IsOK(data)) {
        CX_ERROR("texture file data miss:%s",file);
        success = false;
        return this;
    }
    return FromPNG(data);
}

CX_CPP_END

