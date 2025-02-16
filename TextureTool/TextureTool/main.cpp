//
//  main.cpp
//  TextureTool
//
//  Created by xuhua on 7/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxAutoPool.h>
#include <libpng/png.h>
#include <iostream>
#include <core/cxUtil.h>
#include <core/cxLQT.h>

using namespace cxengine;

int ToLQT(cchars png,cchars json,cchars out,cchars fmt)
{
    LQT lqt={0};
    lqt.flag = CX_LQT_TAG;
    if(strcasecmp(fmt, "rgba4444") == 0){
        lqt.format = LQT::FormatRGBA4444;
    }else if(strcasecmp(fmt, "rgba5551") == 0){
        lqt.format = LQT::FormatRGBA5551;
    }else if(strcasecmp(fmt, "rgba565") == 0){
        lqt.format = LQT::FormatRGB565;
    }else{
        lqt.format = LQT::FormatRGBA8888;
    }
    //json 数据
    const cxStr *jd = nullptr;
    //图片数据压缩
    const cxStr *dd = nullptr;
    //压缩数据头
    const cxStr *jsondata = cxUtil::Assets(json);
    if(cxStr::IsOK(jsondata)) {
        jd = jsondata->LzmaCompress();
        CX_ASSERT(cxStr::IsOK(jd), "json data compress failed");
        lqt.atlasbytes = jd->Size();
        CX_LOGGER("Json data %d -> %d",jsondata->Size(),jd->Size());
    }
    //获取图片数据
    const cxStr *pngdata = cxUtil::Assets(png);
    if(!cxStr::IsOK(pngdata)) {
        return 1;
    }
    png_image image={0};
    image.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_memory(&image, pngdata->Data(), pngdata->Size())){
        return 2;
    }
    image.format = PNG_FORMAT_RGBA;
    lqt.width = image.width;
    lqt.height = image.height;
    cxStr *imagedata = cxStr::Create()->Init(PNG_IMAGE_SIZE(image));
    if(!png_image_finish_read(&image, NULL, imagedata->Buffer(), 0, NULL)){
        return 3;
    }
    png_image_free(&image);
    //转换结构
    if(lqt.format == LQT::FormatRGBA4444){
        pixelRGBA8888ToRGBA4444(imagedata->Buffer(), imagedata->Size(), imagedata->Buffer());
        imagedata->KeepBytes(imagedata->Size()/2);
    }else if(lqt.format == LQT::FormatRGBA5551){
        pixelRGBA8888ToRGBA5551(imagedata->Buffer(), imagedata->Size(), imagedata->Buffer());
        imagedata->KeepBytes(imagedata->Size()/2);
    }else if(lqt.format == LQT::FormatRGB565){
        pixelRGBA8888ToRGB565(imagedata->Buffer(), imagedata->Size(), imagedata->Buffer());
        imagedata->KeepBytes(imagedata->Size()/2);
    }
    //压缩数据
    dd = imagedata->LzmaCompress();
    CX_ASSERT(cxStr::IsOK(dd), "image compress error");
    CX_LOGGER("Image data %d -> %d",imagedata->Size(),dd->Size());
    
    lqt.databytes = dd->Size();
    lqt.filebytes = lqt.databytes + lqt.atlasbytes + sizeof(LQT);
    
    CX_LOGGER("Write File:%s",out);
    FILE *fd = fopen(out, "wb");
    CX_LOGGER("Write LQT Header %ld",sizeof(LQT));
    size_t l = fwrite(&lqt, sizeof(LQT), 1, fd);
    CX_ASSERT(l == 1, "write file error");
    
    CX_LOGGER("Write Json data %d",jd->Size());
    l = fwrite(jd->Data(), jd->Size(), 1, fd);
    CX_ASSERT(l == 1, "write file error");
    
    CX_LOGGER("Write Image data %d",dd->Size());
    l = fwrite(dd->Data(), dd->Size(), 1, fd);
    CX_ASSERT(l == 1, "write file error");
    
    CX_LOGGER("Write file total bytes %d",lqt.filebytes);
    fclose(fd);
    
    return 0;
}

int main(int argc, const char * argv[]) {
    int ret = 0;
    cxAutoPool::Start();
    if(argc == 3){
        char png[PATH_MAX]={0};
        snprintf(png, PATH_MAX, "%s.png",argv[1]);
        char json[PATH_MAX]={0};
        snprintf(json, PATH_MAX, "%s.json",argv[1]);
        char out[PATH_MAX]={0};
        snprintf(out, PATH_MAX, "%s.lqt",argv[1]);
        ret = ToLQT(png, json, out, argv[2]);
    }else{
        std::cerr << "Useage:TextureTool name(use name.png name.json -> name.lqt) fmt(rgba4444,rgba5551,rgba8888)\n";
    }
    cxAutoPool::Update();
    return 0;
}








