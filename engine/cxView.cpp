    //
//  cxView.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <core/cxNotice.h>
#include <core/cxAutoPool.h>
#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxView.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxView);

cxInt cxView::defaultSortFunc(const void *lp,const void *rp)
{
    cxView *l = *(cxView **)lp;
    cxView *r = *(cxView **)rp;
    if(l->Z() > r->Z()){
        return 1;
    }
    if(l->Z() < r->Z()){
        return -1;
    }
    return 0;
}

cxView::cxView()
{
    idx = 0;
    sortFunc = defaultSortFunc;
    relative = RelativeNone;
    maxz = 0;
    direction = INFINITY;
    isdir = false;
    resizeflags = ResizeNone;
    resizebox = 0.0f;
    z = 0;
    flags = FlagsNone;
    isclip = false;
    issort = true;
    istouch = true;
    isclear = false;
    shader = nullptr;
    isremoved = false;
    hideTop = true;
    issleep = false;
    isvisible = true;
    parent = nullptr;
    position = 0.0f;
    scale = 1.0f;
    fixscale = 1.0f;
    anchor = 0.0f;
    size = 0.0f;
    offset = 0.0f;
    angle = 0.0f;
    axis = cxPoint3F::AxisZ;
    dirtymode = DirtyModeAll;
    cc = cxColor4F::WHITE;
    views = cxArray::Alloc();
    actions  = cxArray::Alloc();
    isnewadd = false;
}

cxView::~cxView()
{
    cxObject::release(&shader);
    actions->Release();
    views->Release();
}

cxView *cxView::SetSortFunc(cxCmpFunc func)
{
    sortFunc = func;
    return this;
}

cxView *cxView::BringFront()
{
    cxView *p = Parent();
    if(p != nullptr){
        SetZ(p->maxz + 1);
    }
    return this;
}

cxView *cxView::SetResizeFlags(Resize flags)
{
    if(resizeflags != flags){
        resizeflags = flags;
    }
    return this;
}

cxView *cxView::SetResizeBox(const cxBox4F &box)
{
    if(resizebox != box){
        resizebox = box;
    }
    return this;
}

cxPoint2F cxView::ToGLPoint(const cxPoint2F &vp)
{
    cxSize2F size = cxEngine::Instance()->WinSize() * 0.5f;
    cxPoint2F wp = ToPoint(vp);
    wp.x += size.w;
    wp.y = size.h - wp.y;
    return wp;
}

cxPoint2F cxView::FromGLPoint(const cxPoint2F &gp)
{
    cxSize2F size = cxEngine::Instance()->WinSize() * 0.5f;
    cxPoint2F wp = gp;
    wp.x -= size.w;
    wp.y = size.h - wp.y;
    return FromPoint(wp);
}

cxPoint2F cxView::ToPoint(const cxPoint2F &vp)
{
    cxPoint3F out(vp.x, vp.y, 0);
    out *= modelview;
    return out.ToPoint2F();
}

cxPoint2F cxView::FromPoint(const cxPoint2F &wp)
{
    cxPoint3F out(wp.x, wp.y, 0);
    out *= modelview.Inverse();
    return out.ToPoint2F();
}

cxShader *cxView::Shader()
{
    return shader;
}

cxView *cxView::SetShader(cchars key,cxBool deep)
{
    cxShader *s = cxObject::gcGet<cxShader>(key);
    SetShader(s,deep);
    return this;
}

cxView *cxView::SetShader(cxShader *ps,cxBool deep)
{
    CX_ASSERT(ps != nullptr, "shader null");
    cxObject::swap(&shader, ps);
    Elements([ps](cxView *pview){
        pview->SetShader(ps);
    });
    return this;
}

cxView *cxView::AttachTo(cxView *pview)
{
    pview->Append(this);
    return this;
}

cxView *cxView::SetParent(cxView *v)
{
    parent = v;
    return this;
}

cxView *cxView::At(cxInt i)
{
    return  views->At(i)->To<cxView>();
}

const cxArray *cxView::Subviews() const
{
    return views;
}

cxView *cxView::Parent() const
{
    return parent;
}

cxView *cxView::SetDirty(DirtyMode mode)
{
    dirtymode |= mode;
    return this;
}

cxView *cxView::DelDirty(DirtyMode mode)
{
    dirtymode &= ~mode;
    return this;
}

cxBool cxView::IsDirtyMode(DirtyMode v) const
{
    return dirtymode & v;
}

void cxView::ClearDirty()
{
    dirtymode = DirtyModeNone;
}

const cxPoint2F &cxView::Offset() const
{
    return offset;
}

cxView *cxView::SetOffset(const cxPoint2F &v)
{
    if(offset != v){
        offset = v;
        SetDirty(DirtyModePosition);
    }
    return this;
}

cxView *cxView::AddPosition(const cxPoint2F &v)
{
    SetPosition(position + v);
    return this;
}

const cxPoint2F &cxView::Position() const
{
    return position;
}

const cxBool cxView::EnableDir() const
{
    return isdir;
}

cxView *cxView::SetEnableDir(cxBool v)
{
    isdir = v;
    return this;
}

cxView *cxView::SetDirection(cxFloat v)
{
    if(!cxFloatIsOK(v)){
        return this;
    }
    if(!cxFloatIsEqual(direction, v)){
        direction = v;
        OnAngle();
    }
    return this;
}

const cxFloat cxView::Direction() const
{
    return direction;
}

void cxView::OnAngle()
{
    
}

cxView *cxView::SetRelative(Relative v)
{
    relative = v;
    return this;
}

// will set wp postion,if view arrive to np -> true
// equ distance value
cxBool cxView::SetPosition(const cxPoint2F &np,const cxPoint2F &wp,const cxFloat min)
{
    cxPoint2F cp = Position();
    if(cp == np){
        return true;
    }
    cxFloat cdis = cp.Distance(np);
    if(cdis <= min){
        return true;
    }
    cxFloat a1 = cp.Angle(np);
    cxFloat a2 = wp.Angle(np);
    if(!cxRadiansEqu(a1, a2)){
        SetPosition(np);
        return true;
    }
    SetPosition(wp);
    return false;
}

cxView *cxView::SetPosition(const cxPoint2F &v)
{
    if(position == v){
        return this;
    }
    cxFloat tmp = position.Angle(v);
    if(!cxFloatIsEqual(direction,tmp) && cxFloatIsOK(tmp)){
        direction = tmp;
        OnAngle();
    }
    position = v;
    SetDirty(DirtyModePosition);
    return this;
}

cxView *cxView::SetX(cxFloat x)
{
    cxPoint2F pos = position;
    pos.x = x;
    return SetPosition(pos);
}

cxView *cxView::SetY(cxFloat y)
{
    cxPoint2F pos = position;
    pos.y = y;
    return SetPosition(pos);
}

const cxPoint2F cxView::TransScale() const
{
    return scale * fixscale;
}

const cxPoint2F &cxView::FixScale() const
{
    return fixscale;
}

const cxPoint2F &cxView::Scale() const
{
    return scale;
}

cxView *cxView::SetScale(const cxPoint2F &v)
{
    if(scale != v){
        scale = v;
        SetDirty(DirtyModeScale);
    }
    return this;
}

cxView *cxView::SetFixScale(const cxPoint2F &v)
{
    if(fixscale != v){
        fixscale = v;
        SetDirty(DirtyModeScale);
    }
    return this;
}

const cxPoint2F &cxView::Anchor() const
{
    return anchor;
}

const cxPoint2F cxView::ToAnchor(const cxPoint2F &pos)
{
    return pos / (size/2.0f);
}

cxView *cxView::SetAnchor(const cxPoint2F &v,cxBool keep)
{
    cxPoint2F op = Anchor();
    cxPoint2F ap = v;
    cxBool cv = false;
    if(anchor != v){
        anchor = v;
        SetDirty(DirtyModeAnchor);
        cv = true;
    }
    if(keep && cv){
        SetPosition((ap - op) * ContentSize() + Position());
    }
    return this;
}

const cxSize2F &cxView::Size() const
{
    return size;
}

const cxSize2F cxView::ContentSize() const
{
    return size * TransScale();
}

cxView *cxView::SetSize(const cxSize2F &v)
{
    if(size != v){
        size = v;
        SetDirty(DirtyModeSize);
    }
    return this;
}

const cxPoint3F &cxView::Axis() const
{
    return axis;
}

cxView *cxView::SetAxis(const cxPoint2F &v)
{
    return SetAxis(cxPoint3F(v.x, v.y, 0));
}

cxView *cxView::SetAxis(const cxPoint3F &v)
{
    if(axis != v){
        axis = v;
        SetDirty(DirtyModeAngle);
    }
    return this;
}

const cxFloat cxView::Angle() const
{
    return angle;
}

cxView *cxView::SetAngle(const cxFloat &v)
{
    if(!cxFloatIsOK(v)){
        return this;
    }
    if(!cxFloatIsEqual(angle, v)){
        angle = v;
        SetDirty(DirtyModeAngle);
    }
    return this;
}

cxView *cxView::SetColor(const cxColor4F &v)
{
    if(cc != v){
        cc.SetColor(v);
        SetDirty(DirtyModeColor);
    }
    return this;
}

cxView *cxView::SetColor(const cxBoxColor4F &v)
{
    if(cc != v){
        cc = v;
        SetDirty(DirtyModeColor);
    }
    return this;
}

cxView *cxView::SetAlpha(const cxFloat a)
{
    if(!cxFloatIsEqual(cc.Alpha(), a)){
        cc.SetAlpha(a);
        SetDirty(DirtyModeColor);
    }
    return this;
}

const cxFloat cxView::Alpha() const
{
    return cc.Alpha();
}

const cxColor4F &cxView::Color() const
{
    return cc.Color();
}

cxBool cxView::EnableSleep() const
{
    return issleep;
}

cxBool cxView::IsRemoved() const
{
    return isremoved;
}

cxView *cxView::SetEnableSleep(cxBool v)
{
    issleep = v;
    return this;
}

cxBool cxView::EnableHideTop() const
{
    return hideTop;
}

cxView *cxView::SetEnableHideTop(cxBool v)
{
    hideTop = v;
    return this;
}

cxBool cxView::EnableVisible() const
{
    return isvisible;
}

cxView *cxView::SetEnableVisible(cxBool v)
{
    isvisible = v;
    return this;
}

cxView *cxView::RemoveSubviews()
{
    isclear = true;
    return this;
}

void cxView::OnAppend(cxView *nview)
{
    onAppend.Fire(this, nview);
}

void cxView::OnRemove(cxView *oview)
{
    onRemove.Fire(this, oview);
}

cxView *cxView::Append(cxView *view)
{
    CX_ASSERT(view != nullptr, "args error");
    CX_ASSERT(view->parent == nullptr, "view repeat append");
    view->SetParent(this);
    view->isremoved = false;
    view->z = views->Size();
    view->isnewadd = true;
    views->Append(view);
    return this;
}

cxView *cxView::Append(cxAction *action)
{
    CX_ASSERT(action != nullptr, "args error");
    CX_ASSERT(action->View() == nullptr, "action repeat append");
    ExitAction(action->ID());
    action->SetView(this);
    actions->Append(action);
    action->OnAppend(this);
    return this;
}

cxView *cxView::Remove()
{
    isremoved = true;
    SetParent(nullptr);
    return this;
}

void cxView::OnDirty()
{
    
}

cxView *cxView::Sort()
{
    issort = true;
    return this;
}

cxView *cxView::SetW(cxFloat w)
{
    CX_ASSERT(!size.IsZero(), "size not set");
    return SetSize(cxSize2F(w, w * size.h/size.w));
}

cxView *cxView::SetH(cxFloat h)
{
    CX_ASSERT(!size.IsZero(), "size not set");
    return SetSize(cxSize2F(h * size.w/size.h, h));
}

const cxMatrixF &cxView::ModelView() const
{
    return modelview;
}

cxAction *cxView::GetAction(cxActionId aid)
{
    cxArray::FIter it = actions->FBegin();
    while(it != actions->FEnd()){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid){
            return action;
        }
        it++;
    }
    return nullptr;
}

cxBool cxView::HasAction(cxActionId aid) const
{
    cxArray::FIter it = actions->FBegin();
    while(it != actions->FEnd()){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid && !action->IsExit()){
            return true;
        }
        it++;
    }
    return false;
}

cxInt cxView::ActionSize() const
{
    return actions->Size();
}

cxView *cxView::StopAction(cxActionId aid)
{
    cxArray::FIter it = actions->FBegin();
    while(it != actions->FEnd()){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid || aid == 0){
            action->Stop();
        }
        it++;
    }
    return this;
}

cxView *cxView::ExitAction(cxActionId aid)
{
    cxArray::FIter it = actions->FBegin();
    while(it != actions->FEnd()){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid || aid == 0){
            action->Exit(true);
        }
        it++;
    }
    return this;
}

cxBoxColor4F &cxView::BoxColor()
{
    return cc;
}

cxBox4F cxView::ToBox4F()
{
    cxFloat l = -(anchor.x + 0.5f) * size.w;;
    cxFloat r = l + size.w;
    cxFloat b = -(anchor.y + 0.5f) * size.h;
    cxFloat t = b + size.h;
    return cxBox4F(l, r, t, b);
}

cxBox4F cxView::Box()
{
    cxFloat l = -size.w/2.0f;
    cxFloat r = +size.w/2.0f;
    cxFloat b = -size.h/2.0f;
    cxFloat t = +size.h/2.0f;
    return cxBox4F(l, r, t, b);
}

cxBoxPoint3F &cxView::BoxPoint()
{
    cxFloat l = -size.w/2.0f;
    cxFloat r = +size.w/2.0f;
    cxFloat b = -size.h/2.0f;
    cxFloat t = +size.h/2.0f;
    bp.Set(l, r, t, b);
    return bp;
}

cxView *cxView::SetFrame(cxFloat x,cxFloat y,cxFloat w,cxFloat h)
{
    SetPosition(cxPoint2F(x,y));
    SetSize(cxSize2F(w, h));
    return this;
}

void cxView::OnEnter()
{
    onEnter.Fire(this);
}

void cxView::OnLeave()
{
    onLeave.Fire(this);
}

void cxView::runAppends(cxFloat dt)
{
    cxInt cnt = views->Size();
    for(cxInt i = 0; i < cnt; i++){
        cxView *view = views->At(i)->To<cxView>();
        if(!view->isnewadd){
            continue;
        }
        view->isnewadd = false;
        OnAppend(view);
        view->OnEnter();
    }
}

void cxView::runActions(cxFloat dt)
{
    cxInt mvc = 0;
    cxInt cnt = actions->Size();
    cxObject *mvs[64]={0};
    for(cxInt i = 0; i < cnt; i++){
        cxAction *action = actions->At(i)->To<cxAction>();
        if(!action->Update(dt)){
            continue;
        }
        if(mvc < 64){
            mvs[mvc++] = action;
        }
    }
    for(cxInt i = 0;i < mvc; i++){
        actions->Remove(mvs[i]);
    }
}

void cxView::runUpadtes(cxFloat dt)
{
    cxInt mvc = 0;
    cxInt cnt = views->Size();
    cxObject *mvs[128]={0};
    for(cxInt i = 0; i < cnt; i++){
        cxView *view = views->At(i)->To<cxView>();
        if(!view->IsRemoved()){
            view->Update(dt);
        }else if(mvc < 128){
            view->OnLeave();
            OnRemove(view);
            mvs[mvc++] = view;
        }
    }
    for(cxInt i = 0;i < mvc; i++){
        views->Remove(mvs[i]);
    }
}

cxBool cxView::EnableTouch() const
{
    return istouch;
}

cxView *cxView::SetEnableTouch(cxBool v)
{
    istouch = v;
    return this;
}

const cxHitInfo cxView::HitTest(const cxPoint2F &wp)
{
    cxHitInfo info;
    cxBox4F box = BoxPoint().ToBox4F();
    info.point = FromPoint(wp);
    info.hited = box.Contain(info.point);
    return info;
}

cxBool cxView::OnDispatch(const cxTouchable *e)
{
    return false;
}

cxBool cxView::OnDispatch(const cxKey &key)
{
    return false;
}

cxBool cxView::Dispatch(const cxTouchable *e)
{
    if(IsRemoved()){
        return false;
    }
    if(!EnableTouch()){
        return false;
    }
    if(!EnableVisible()){
        return false;
    }
    cxInt cnt = views->Size();
    for(cxInt i = cnt - 1;i >= 0; i--){
        cxView *view = views->At(i)->To<cxView>();
        if(view->IsRemoved()){
            continue;
        }
        if(view->Dispatch(e))return true;
    }
    return OnDispatch(e);
}

cxBool cxView::Dispatch(const cxKey &key)
{
    if(IsRemoved()){
        return false;
    }
    if(!EnableTouch()){
        return false;
    }
    if(!EnableVisible()){
        return false;
    }
    cxInt cnt = views->Size();
    for(cxInt i = cnt - 1;i >= 0; i--){
        cxView *view = views->At(i)->To<cxView>();
        if(view->IsRemoved()){
            continue;
        }
        if(view->Dispatch(key))return true;
    }
    return OnDispatch(key);
}

void cxView::transform()
{
    if(IsDirtyMode(DirtyModeLayout)){
        OnLayout();
        DelDirty(DirtyModeLayout);
    }
    OnDirty();
    if(IsDirtyMode(DirtyModeLayout)){
        OnLayout();
        DelDirty(DirtyModeLayout);
    }
    if(EnableDir() && cxFloatIsOK(direction)){
        SetAngle(Direction());
    }
    if(IsDirtyMode(DirtyModeNormal)){
        normalMatrix.InitTrans(Position() + Offset());
        normalMatrix.Rotation(axis.x, axis.y, axis.z, angle);
        normalMatrix.Scaling(TransScale());
    }
    if(IsDirtyMode(DirtyModeAnchor)){
        anchorMatrix.InitTrans(-size.w * anchor.x, -size.h * anchor.y, 0);
    }
    if(isclip){
        SetFlags(FlagsClip);
    }
    ClearDirty();
    SetFlags(FlagsDirty);
}

void cxView::ResetIdx()
{
    idx = 0;
}

void cxView::Update(cxFloat dt)
{
    flags = 0;
    if(IsRemoved() || EnableSleep()){
        return;
    }
    OnUpdate(dt);
    if(!actions->IsEmpty()){
        runActions(dt);
    }
    if(!views->IsEmpty()){
        runAppends(dt);
    }
    if(dirtymode != DirtyModeNone){
        transform();
    }
    if(!views->IsEmpty()){
        runUpadtes(dt);
    }
    OnIndex(idx++);
}

cxFloat cxView::Z() const
{
    return z;
}

cxView *cxView::SetZ(cxFloat v)
{
    if(cxFloatIsEqual(z, v)){
        return this;
    }
    z = v;
    if(parent == nullptr){
        return this;
    }
    if(z > parent->maxz){
        parent->maxz = z;
    }
    parent->Sort();
    return this;
}

cxView *cxView::SetBlend(const BlendFunc &b)
{
    state.blend = b;
    return this;
}

cxFloat cxView::Distance(cxView *target)
{
    return cxPoint2F::Distance(Position(), target->Position());
}

const BlendFunc &cxView::Blend() const
{
    return state.blend;
}

cxView *cxView::SetClip(cxBool v)
{
    isclip = v;
    return this;
}

void cxView::Layout()
{
    SetDirty(DirtyModeLayout);
    Elements([](cxView *pview){
        pview->SetDirty(DirtyModeLayout);
    });
}

cxBool cxView::IsEmpty() const
{
    return views->IsEmpty();
}

cxRenderState &cxView::State()
{
    state.Set(Shader());
    state.Set(this);
    return state;
}
//this.0.0
cxView *cxView::Select(cchars path)
{
    chars ckey = strdup(path);
    cxInt num = 0;
    chars ckeys[16];
    chars src = ckey;
    ckeys[num++] = src;
    while (*src++ != '\0') {
        if(*src != '.')continue;
        CX_ASSERT(num < 16, ". opt too more");
        ckeys[num++] = src + 1;
        *src++ = '\0';
    }
    cxView *pv = this;
    cxView *rv = NULL;
    for(cxInt i=0; i < num;i ++){
        cchars ckey = ckeys[i];
        if(cxStr::IsEqu(ckey, "parent")){
            rv = pv->Parent();
        }else if(cxStr::IsEqu(ckey, "this")){
            rv = pv;
        }else{
            rv = pv->At(atoi(ckey));
        }
        if(rv == NULL){
            break;
        }
        pv = rv;
    }
    free(ckey);
    return rv;
}

void cxView::OnIndex(cxULong idx)
{
    
}

void cxView::OnRender(cxRender *render,const cxMatrixF &model)
{
    
}

const cxBox4F cxView::ToMaxRect4F()
{
    cxBoxPoint3F bp = BoxPoint() * modelview;
    return bp.ToMaxRect4F();
}

cxUInt cxView::Flags() const
{
    return flags;
}

cxView *cxView::SetFlags(cxUInt f)
{
    flags |= f;
    return this;
}

const cxBox4F cxView::ParentBox() const
{
    if(parent != nullptr && parent->isclip){
        return parent->ToMaxRect4F();
    }
    return cxEngine::Instance()->WinBox();
}

void cxView::RenderSubviews(cxRender *render,const cxMatrixF &mv)
{
    for(cxArray::FIter it=views->FBegin();it!=views->FEnd();it++){
        cxView *view = (*it)->To<cxView>();
        view->Render(render,mv);
    }
}

void cxView::clearViews()
{
    issort = false;
    cxInt cnt = views->Size();
    for(cxInt i=0;i<cnt;i++){
        cxView *view = views->At(i)->To<cxView>();
        if(view->IsRemoved()){
            continue;
        }
        view->OnLeave();
        OnRemove(view);
    }
    views->Clear();
}

void cxView::Render(cxRender *render,const cxMatrixF &mv)
{
    if(!EnableVisible() || EnableSleep() || IsRemoved()){
        return;
    }
    gl->Push();
    gl->MultMatrix(normalMatrix);
    gl->MultMatrix(anchorMatrix);
    //selector modelview
    if(relative == RelativeParent && Parent() != nullptr){
        modelview = Parent()->ModelView();
    }else if(relative == RelativeGlobal){
        modelview = cxEngine::Instance()->Window()->ModelView();
    }else{
        modelview = gl->ModelView();
    }
    //clip view test
    if(isclip){
        if(flags & FlagsClip)clipbox = ToMaxRect4F();
        render->Clip(cxRenderState::ClipOn,clipbox);
    }
    //start render
    OnRender(render, modelview);
    if(isclear){
        clearViews();
        isclear = false;
    }
    if(issort){
        //subviews->Sort(cxView::sortFunc);
        issort = false;
    }
    if(!views->IsEmpty()){
        RenderSubviews(render, modelview);
    }
    if(isclip){
        render->Clip(cxRenderState::ClipOff,clipbox);
    }
finished:
    gl->Pop();
    flags = 0;
}

void cxView::OnSort()
{
    
}

//autoalign process
void cxView::OnLayout()
{
    cxView *parent = Parent();
    if(parent == nullptr || resizeflags == ResizeNone){
        return;
    }
    
    CX_ASSERT(!cxEngine::Instance()->PlanScale().IsZero(), "not set plansize");
    
    cxBox4F pbox = Parent()->BoxPoint().ToBox4F();
    cxBox4F vbox = BoxPoint().ToBox4F();
    
    cxSize2F    psiz = Parent()->Size();
    cxPoint2F   panchor = Parent()->Anchor();

    cxSize2F    vsiz = ContentSize();
    cxPoint2F   vanchor = Anchor();
    cxSize2F    nsiz = Size();
    
    cxPoint2F   vpos = Position();
    cxPoint2F   vscale = Scale();
    cxPoint2F   vfixscale = FixScale();
    
    cxBox4F     vresizebox = resizebox * fixscale;

    //auto mask
    if(resizeflags & ResizeLeftSide){
        resizeflags |= ResizeLeft;
    }
    if(resizeflags & ResizeRightSide){
        resizeflags |= ResizeRight;
    }
    //left right
    if((resizeflags & ResizeLeft) && (resizeflags & ResizeRight)){
        vscale.x = 1.0f;
        vfixscale.x = 1.0f;
        vbox.l = pbox.l + vresizebox.l;
        vbox.r = pbox.r - vresizebox.r;
        vsiz.w = vbox.r - vbox.l;
        vpos.x = (vresizebox.l - vresizebox.r)/2.0f;
        vpos.x -= panchor.x * psiz.w;
        vpos.x += vanchor.x * vsiz.w;
        nsiz.w = vsiz.w;
    }else if(resizeflags & ResizeLeft){
        vbox.l = pbox.l + vresizebox.l;
        vbox.r = vbox.l + vsiz.w;
        vpos.x = vbox.l + vsiz.w/2.0f;
        vpos.x += vanchor.x * vsiz.w;
        if(resizeflags & ResizeLeftSide){
            vpos.x -= (vresizebox.l * 2 * vfixscale.x + vsiz.w);
        }
    }else if(resizeflags & ResizeRight){
        vbox.r = pbox.r - vresizebox.r;
        vbox.l = vbox.r - vsiz.w;
        vpos.x = vbox.l + vsiz.w/2.0f;
        vpos.x += vanchor.x * vsiz.w;
        if(resizeflags & ResizeRightSide){
            vpos.x += (vresizebox.r * 2 + vsiz.w);
        }
    }
    //auto mask
    if(resizeflags & ResizeTopSide){
        resizeflags |= ResizeTop;
    }
    if(resizeflags & ResizeBottomSide){
        resizeflags |= ResizeBottom;
    }
    //top bottom
    if((resizeflags & ResizeTop) && (resizeflags & ResizeBottom)){
        vscale.y = 1.0f;
        vfixscale.y = 1.0f;
        vbox.b = pbox.b + vresizebox.b;
        vbox.t = pbox.t - vresizebox.t;
        vsiz.h = vbox.t - vbox.b;
        vpos.y = (vresizebox.b - vresizebox.t)/2.0f;
        vpos.y -= panchor.y * psiz.h;
        vpos.y += vanchor.y * vsiz.h;
        nsiz.h = vsiz.h;
    }else if(resizeflags & ResizeBottom){
        vbox.b = pbox.b + vresizebox.b;
        vbox.t = vbox.b + vsiz.h;
        vpos.y = vbox.b + vsiz.h/2.0f;
        vpos.y += vanchor.y * vsiz.h;
        if(resizeflags & ResizeBottomSide){
            vpos.y -= (vresizebox.b * 2 + vsiz.h);
        }
    }else if(resizeflags & ResizeTop){
        vbox.t = pbox.t - vresizebox.t;
        vbox.b = vbox.t - vsiz.h;
        vpos.y = vbox.b + vsiz.h/2.0f;
        vpos.y += vanchor.y * vsiz.h;
        if(resizeflags & ResizeTopSide){
            vpos.y += (vresizebox.t * 2 + vsiz.h);
        }
    }
    SetFixScale(vfixscale);
    SetScale(vscale);
    SetSize(nsiz);
    SetPosition(vpos);
}

void cxView::Elements(std::function<void(cxView *pview)> func)
{
    cxInt cnt = views->Size();
    for(cxInt i=0;i<cnt;i++){
        cxView *pv = views->At(i)->To<cxView>();
        if(pv->IsRemoved()){
            continue;
        }
        func(pv);
    }
}

void cxView::OnUpdate(cxFloat dt)
{
    onUpdate.Fire(this, dt);
}

void cxView::Invoke(std::function<void(cxView *pview)>func)
{
    Invoke(0.001f, 1, func);
}

void cxView::Invoke(cxFloat delay,std::function<void(cxView *pview)>func)
{
    Invoke(delay, 1, func);
}

void cxView::Invoke(cxFloat delay,cxInt repeat,std::function<void(cxView *pview)>func)
{
    CX_ASSERT(delay > 0 , "delay args error");
    cxAction *timer = cxAction::Create();
    timer->SetTime(delay);
    timer->SetRepeat(repeat);
    timer->onStop+=[this,func](cxAction *pav){func(this);};
    Append(timer);
}

CX_CPP_END











