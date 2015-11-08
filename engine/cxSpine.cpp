//
//  cxSpine.cpp
//  cxEngineCore
//
//  Created by xuhua on 11/6/15.
//  Copyright © 2015 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include <spine/extension.h>
#include "cxSpine.h"

static const int quadTriangles[6] = {0, 1, 2, 2, 3, 0};

//spine ext method
void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
    cxengine::cxTexture *ptex = cxengine::cxTexture::Alloc()->From(path);
    self->rendererObject = ptex;
    cxengine::cxSize2F size = ptex->Size();
    self->width = size.w;
    self->height = size.h;
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
    cxengine::cxTexture *ptex = static_cast<cxengine::cxTexture *>(self->rendererObject);
    ptex->Release();
}

char* _spUtil_readFile (const char* path, int* length) {
    const cxengine::cxStr *data = cxengine::cxUtil::Instance()->AssetsData(path);
    if(data == nullptr){
        return nullptr;
    }
    *length = data->Size();
    char *ret = MALLOC(char, *length);
    memcpy(ret, data->Data(), *length);
    return ret;
}

void _spineAnimationCallback(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
    cxengine::cxSpine *sp = static_cast<cxengine::cxSpine *>(state->rendererObject);
    sp->AnimationListen(state, trackIndex, type, event, loopCount);
}

CX_CPP_BEGIN

CX_IMPLEMENT(cxSpine);

cxSpine::cxSpine()
{
    vertices = new cxFloat[1024];
    timeScale = 1.0f;
    atlas = nullptr;
    skeletonData = nullptr;
    skeleton = nullptr;
    spState = nullptr;
}

cxSpine::~cxSpine()
{
    if(spState != nullptr){
        spAnimationState_dispose(spState);
        spState = nullptr;
    }
    if(skeleton != nullptr){
        spSkeleton_dispose(skeleton);
        skeleton = nullptr;
    }
    if(skeletonData != nullptr){
        spSkeletonData_dispose(skeletonData);
        skeletonData = nullptr;
    }
    if(atlas != nullptr){
        spAtlas_dispose(atlas);
        atlas = nullptr;
    }
    delete []vertices;
}

void cxSpine::SetTimeScale(cxFloat v)
{
    timeScale = v;
}

void cxSpine::AnimationListen(spAnimationState* state, cxInt trackIndex, spEventType type, spEvent* event, cxInt loopCount)
{
    
}

void cxSpine::OnUpdate(cxFloat dt)
{
    cxTriangles::OnUpdate(dt);
    dt = dt * timeScale;
    if(skeleton != nullptr){
        spSkeleton_update(skeleton, dt);
    }
    if(spState != nullptr){
        spAnimationState_update(spState, dt);
    }
    if(spState != nullptr && skeleton != nullptr){
        spAnimationState_apply(spState, skeleton);
    }
    if(skeleton != nullptr){
        spSkeleton_updateWorldTransform(skeleton);
    }
}

void cxSpine::OnRender(cxRender *render, const cxMatrixF &model)
{
    const cxColor4F &color = Color();
    skeleton->r = color.r;
    skeleton->g = color.g;
    skeleton->b = color.b;
    skeleton->a = color.a;
    verticesNum = 0;
    cxColor4F cv;
    const cxFloat *vs = nullptr;
    const cxFloat *us = nullptr;
    cxInt vnum = 0;
    const cxInt *is = nullptr;
    cxInt inum = 0;
    cxTexture *texture = nullptr;
    for (cxInt i = 0; i < skeleton->slotsCount; i++) {
        spSlot* slot = skeleton->drawOrder[i];
        if (slot->attachment == nullptr){
            continue;
        }
        if(slot->attachment->type == SP_ATTACHMENT_REGION){
            spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
            spAtlasRegion *region = (spAtlasRegion *)attachment->rendererObject;
            texture = (cxTexture *)region->page->rendererObject;
            spRegionAttachment_computeWorldVertices(attachment, slot->bone, vertices);
            cv = cxColor4F(attachment->r, attachment->g, attachment->b, attachment->a) * color;
            vs = vertices;
            us = attachment->uvs;
            vnum = 8;
            is = quadTriangles;
            inum = 6;
        }else if(slot->attachment->type == SP_ATTACHMENT_MESH){
            spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
            spAtlasRegion *region = (spAtlasRegion *)attachment->rendererObject;
            texture = (cxTexture *)region->page->rendererObject;
            spMeshAttachment_computeWorldVertices(attachment, slot, vertices);
            cv = cxColor4F(attachment->r, attachment->g, attachment->b, attachment->a) * color;
            vs = vertices;
            us = attachment->uvs;
            vnum = attachment->verticesCount;
            is = attachment->triangles;
            inum = attachment->trianglesCount;
        }else if(slot->attachment->type == SP_ATTACHMENT_SKINNED_MESH){
            spSkinnedMeshAttachment* attachment = (spSkinnedMeshAttachment*)slot->attachment;
            spAtlasRegion *region = (spAtlasRegion *)attachment->rendererObject;
            texture = (cxTexture *)region->page->rendererObject;
            spSkinnedMeshAttachment_computeWorldVertices(attachment, slot, vertices);
            cv = cxColor4F(attachment->r, attachment->g, attachment->b, attachment->a) * color;
            vs = vertices;
            us = attachment->uvs;
            vnum = attachment->uvsCount;
            is = attachment->triangles;
            inum = attachment->trianglesCount;
        }else{
            continue;
        }
        if(texture != nullptr){
            Renders().Clear();
            Indices().Clear();
            SetTexture(texture);
            //append is
            for(cxInt j=0; j < inum; j++) {
                Indices().Append(is[j] + verticesNum);
            }
            //append vs
            for(cxInt i=0; i < vnum; i+=2){
                cxRenderF render;
                render.colors = cv;
                render.vertices = cxPoint3F(vs[i], vs[i+1], 0);
                render.coords = cxCoord2F(us[i], us[i+1]);
                Renders().Append(render);
                verticesNum++;
            }
            state.Set(cxRenderState::TrianglesVBO);
            state.Set(Texture());
            state.Set(Blend());
            render->Render(Renders(), Indices(), model, State(), Flags());
        }
    }
}

spSkin *cxSpine::FindSkin(cchars name)
{
    return spSkeletonData_findSkin(skeleton->data, name);
}

void cxSpine::SetSkin(cchars name)
{
    spSkin *skin = FindSkin(name);
    if(skin == nullptr){
        return;
    }
    spSkeleton_setSkin(skeleton, skin);
}

spTrackEntry *cxSpine::SetAnimation(cxInt idx,cchars name,cxBool loop)
{
    spTrackEntry *track = spAnimationState_setAnimationByName(spState, idx, name, loop);
    if(track != nullptr){
        track->rendererObject = this;
    }
    return track;
}

spTrackEntry *cxSpine::AddAnimation(cxInt idx,cchars name,cxBool loop,cxFloat delay)
{
    spTrackEntry *track = spAnimationState_addAnimationByName(spState, idx, name, loop, delay);
    if(track != nullptr){
        track->rendererObject = this;
    }
    return track;
}

spAnimation *cxSpine::FindAnimation(cchars name)
{
    return spSkeletonData_findAnimation(skeleton->data, name);
}

cxFloat cxSpine::GetMix(cchars from,cchars to)
{
    return spAnimationStateData_getMix(spState->data, FindAnimation(from), FindAnimation(to));
}

void cxSpine::SetMix(cchars from,cchars to,cxFloat duration)
{
    spAnimationStateData_setMixByName(spState->data, from, to, duration);
}

void cxSpine::Init(cchars path,cxFloat scale)
{
    char dataFile[PATH_MAX]={0};
    snprintf(dataFile, PATH_MAX, "%s.json",path);
    
    char atlasFile[PATH_MAX]={0};
    snprintf(atlasFile, PATH_MAX, "%s.atlas",path);
    
    atlas = spAtlas_createFromFile(atlasFile, this);
    CX_ASSERT(atlas != nullptr, "create atlas file error");
    
    spSkeletonJson* json = spSkeletonJson_create(atlas);
    json->scale = scale;
    skeletonData = spSkeletonJson_readSkeletonDataFile(json, dataFile);
    CX_ASSERT(skeletonData != nullptr, "create skeletion data error");
    spSkeletonJson_dispose(json);
    
    skeleton = spSkeleton_create(skeletonData);
    CX_ASSERT(skeleton != nullptr, "skeleton create error");
    rootBone = skeleton->bones[0];
    
    spState = spAnimationState_create(spAnimationStateData_create(skeleton->data));
    CX_ASSERT(spState != nullptr, "create animation state error");
    spState->rendererObject = this;
    spState->listener = _spineAnimationCallback;
}

cxSpine *cxSpine::Create(cchars path,cxFloat scale)
{
    cxSpine *ret = cxSpine::Create();
    ret->Init(path,scale);
    return ret;
}

CX_CPP_END

