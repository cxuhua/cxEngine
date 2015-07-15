    //
//  cxView.cpp
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxView.h"

CX_CPP_BEGIN

CX_IMPLEMENT(cxView);

cxView::cxView()
{
    maxz = 0;
    direction = INFINITY;
    isdir = false;
    islayout = false;
    alignflags = ResizeNone;
    alignbox = 0.0f;
    z = 0;
    flags = 0;
    isclip = false;
    issort = true;
    istouch = true;
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
    angle = 0;
    axis = cxPoint3F::AxisZ;
    dirtymode = 0xFFFFFFFF;
    cc = cxColor4F::WHITE;
    subviews = cxArray::Alloc();
    viewapps = cxArray::Alloc();
    actions  = cxArray::Alloc();
    actapps  = cxArray::Alloc();
}

cxView::~cxView()
{
    onFree.Fire(this);
    cxObject::release(&shader);
    actapps->Release();
    viewapps->Release();
    actions->Release();
    subviews->Release();
}

cxView *cxView::BringFront()
{
    cxView *p = Parent();
    if(p == nullptr){
        return this;
    }
    SetZ(p->maxz + 1);
    return this;
}

cxView *cxView::SetResizeFlags(Resize flags)
{
    if(alignflags != flags){
        alignflags = flags;
        SetDirty(DirtyModeLayout);
    }
    return this;
}

cxView *cxView::SetResizeBox(const cxBox4F &box)
{
    if(alignbox != box){
        alignbox = box;
        SetDirty(DirtyModeLayout);
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

cxView *cxView::SetShader(cchars key)
{
    cxShader *s = cxObject::gcpull<cxShader>(key);
    SetShader(s);
    return this;
}

cxView *cxView::SetShader(cxShader *ps)
{
    CX_ASSERT(ps != nullptr, "shader null");
    cxObject::swap(&shader, ps);
    return this;
}

cxView *cxView::SetParent(cxView *v)
{
    parent = v;
    return this;
}

cxArray *cxView::Subviews() const
{
    return subviews;
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

cxBool cxView::IsDirtyMode(DirtyMode v) const
{
    return dirtymode & v;
}

void cxView::ClearDirty()
{
    dirtymode = 0;
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

cxView *cxView::EnableDir(cxBool v)
{
    isdir = v;
    return this;
}

cxView *cxView::SetDirection(cxFloat v)
{
    if(!cxFloatIsEqual(direction, v) && cxFloatIsOK(v)){
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

cxView *cxView::EnableSleep(cxBool v)
{
    issleep = v;
    return this;
}

cxBool cxView::EnableHideTop() const
{
    return hideTop;
}

cxView *cxView::EnableHideTop(cxBool v)
{
    hideTop = v;
    return this;
}

cxBool cxView::EnableVisible() const
{
    return isvisible;
}

cxView *cxView::EnableVisible(cxBool v)
{
    isvisible = v;
    return this;
}

cxView *cxView::RemoveSubviews()
{
    for(cxArray::FIter it = subviews->FBegin();it != subviews->FEnd();it++){
        cxView *pview = (*it)->To<cxView>();
        OnRemove(pview);
    }
    subviews->Clear();
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
    viewapps->Append(view);
    return this;
}

cxView *cxView::Append(cxAction *action)
{
    CX_ASSERT(action != nullptr, "args error");
    CX_ASSERT(action->View() == nullptr, "action repeat append");
    action->SetView(this);
    actapps->Append(action);
    return this;
}

cxView *cxView::Clone()
{
    cxView *rv = cxView::Create();
    rv->SetSize(size);
    rv->SetPosition(position);
    rv->SetScale(scale);
    rv->SetFixScale(fixscale);
    rv->SetColor(Color());
    return rv;
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

const cxMatrixF &cxView::ModelView() const
{
    return modelview;
}

cxBool cxView::HasAction(cxULong aid) const
{
    for(cxArray::FIter it=actions->FBegin();it!=actions->FEnd();it++){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid){
            return true;
        }
    }
    return false;
}

cxView *cxView::StopAction(cxULong aid)
{
    for(cxArray::FIter it=actions->FBegin();it!=actions->FEnd();it++){
        cxAction *action = (*it)->To<cxAction>();
        if(action->ID() == aid || aid == 0){
            action->Stop();
        }
    }
    return this;
}

void cxView::updateActions(cxFloat dt)
{
    for(cxArray::FIter it=actapps->FBegin();it!=actapps->FEnd();it++){
        actions->Append(*it);
    }
    actapps->Clear();
    for(cxArray::FIter it=actions->FBegin();it!=actions->FEnd();){
        cxAction *action = (*it)->To<cxAction>();
        if(action->Update(dt)){
            it = actions->Remove(it);
        }else{
            it++;
        }
    }
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

void cxView::runAppends(cxFloat dt)
{
    for(cxArray::FIter it=viewapps->FBegin();it!=viewapps->FEnd();it++){
        cxView *view = (*it)->To<cxView>();
        subviews->Append(view);
        view->onEnter.Fire(view);
        OnAppend(view);
    }
    viewapps->Clear();
}

void cxView::runRemoves(cxFloat dt)
{
    for(cxArray::FIter it=subviews->FBegin();it!=subviews->FEnd();){
        cxView *view = (*it)->To<cxView>();
        if(!view->IsRemoved()){
            view->Update(dt);
        }
        if(!view->IsRemoved()){
            it++;
            continue;
        }
        view->onLeave.Fire(view);
        OnRemove(view);
        it = subviews->Remove(it);
    }
}

cxBool cxView::EnableTouch() const
{
    return istouch;
}

cxView *cxView::EnableTouch(cxBool v)
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

cxBool cxView::Dispatch(const cxTouchable *e)
{
    if(!EnableTouch()){
        return OnDispatch(e);
    }
    for(cxArray::RIter it=subviews->RBegin();it!=subviews->REnd();it++){
        cxView *view = (*it)->To<cxView>();
        if(view->Dispatch(e))return true;
    }
    return OnDispatch(e);
}

void cxView::transform()
{
    OnDirty();
    if(IsDirtyMode(DirtyModeLayout)){
        Layout();
    }
    if(EnableDir() && cxFloatIsOK(direction)){
        SetAngle(Direction());
    }
    if(IsDirtyMode(DirtyModeNormal)){
        normalMatrix.InitTrans(Position() + Offset());
        normalMatrix.Rotation(axis.x,axis.y,axis.z, angle);
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

void cxView::Update(cxFloat dt)
{
    flags = 0;
    if(IsRemoved() || EnableSleep()){
        return;
    }
    if(!islayout){
        OnLayout();
        islayout = true;
    }
    OnUpdate(dt);
    if(!actions->IsEmpty() || !actapps->IsEmpty()){
        updateActions(dt);
    }
    if(!viewapps->IsEmpty()){
        runAppends(dt);
    }
    if(dirtymode != DirtyModeNone){
        transform();
    }
    if(!subviews->IsEmpty()){
        runRemoves(dt);
    }
}

cxInt cxView::Z() const
{
    return z;
}

cxView *cxView::SetZ(cxInt v)
{
    if(z == v){
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
    islayout = false;
    for(cxArray::FIter it=subviews->FBegin();it!=subviews->FEnd();it++){
        cxView *view = (*it)->To<cxView>();
        view->Layout();
    }
}

cxBool cxView::IsEmpty() const
{
    return subviews->IsEmpty();
}

cxInt cxView::sortFunc(const void *lp,const void *rp)
{
    cxView *l = *(cxView **)lp;
    cxView *r = *(cxView **)rp;
    return l->Z() - r->Z();
}

cxRenderState &cxView::State()
{
    state.shader = Shader();
    state.Set(cxRenderState::BoxRender);
    return state;
}

void cxView::OnRender(cxRender *render,const cxMatrixF &model)
{
    
}

const cxBox4F cxView::ToMaxBox()
{
    cxBoxPoint3F bp = BoxPoint() * modelview;
    return bp.ToMaxBox();
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

void cxView::Render(cxRender *render,const cxMatrixF &mv)
{
    CX_ASSERT(!size.IsZero(), "view size not set");
    if(!EnableVisible() || EnableSleep() || IsRemoved()){
        return;
    }
    gl->Push();
    gl->MultMatrix(normalMatrix);
    gl->MultMatrix(anchorMatrix);
    modelview = gl->ModelView();
    if(isclip){
        if(flags & FlagsClip)clipbox = ToMaxBox();
        render->Clip(cxRenderState::ClipOn,clipbox);
    }
    OnRender(render, modelview);
    if(issort){
        subviews->Sort(cxView::sortFunc);
        issort = false;
    }
    for(cxArray::FIter it=subviews->FBegin();it!=subviews->FEnd();it++){
        cxView *view = (*it)->To<cxView>();
        view->Render(render,modelview);
    }
    if(isclip){
        render->Clip(cxRenderState::ClipOff,clipbox);
    }
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
    if(parent == nullptr || alignflags == ResizeNone){
        return;
    }
    
    cxBox4F pbox = Parent()->BoxPoint().ToBox4F();
    cxBox4F vbox = BoxPoint().ToBox4F();
    
    cxEngine *engine = cxEngine::Instance();
    cxBox4F box = alignbox * engine->ScaleFactor();
    
    cxSize2F    psiz = Parent()->Size();
    cxPoint2F   panchor = Parent()->Anchor();
    
    cxSize2F    vsiz = ContentSize();
    cxPoint2F   vanchor = Anchor();
    
    cxPoint2F   vpos = Position();
    cxPoint2F   vscale = Scale();
    cxPoint2F   vfixscale = FixScale();
    
    //auto mask
    if(alignflags & ResizeLeftSide){
        alignflags |= ResizeLeft;
    }
    if(alignflags & ResizeRightSide){
        alignflags |= ResizeRight;
    }
    //left right
    if((alignflags & ResizeLeft) && (alignflags & ResizeRight)){
        vscale.x = 1.0f;
        vfixscale.x = 1.0f;
        vbox.l = pbox.l + box.l;
        vbox.r = pbox.r - box.r;
        vsiz.w = vbox.r - vbox.l;
        vpos.x -= panchor.x * psiz.w;
        vpos.x += vanchor.x * vsiz.w;
    }else if(alignflags & ResizeLeft){
        vbox.l = pbox.l + box.l;
        vbox.r = vbox.l + vsiz.w;
        vpos.x = vbox.l + vsiz.w/2.0f;
        vpos.x += vanchor.x * vsiz.w;
        if(alignflags & ResizeLeftSide){
            vpos.x -= (box.l * 2 + vsiz.w);
        }
        vsiz.w = size.w;
    }else if(alignflags & ResizeRight){
        vbox.r = pbox.r - box.r;
        vbox.l = vbox.r - vsiz.w;
        vpos.x = vbox.l + vsiz.w/2.0f;
        vpos.x += vanchor.x * vsiz.w;
        if(alignflags & ResizeRightSide){
            vpos.x += (box.r * 2 + vsiz.w);
        }
        vsiz.w = size.w;
    }
    //auto mask
    if(alignflags & ResizeTopSide){
        alignflags |= ResizeTop;
    }
    if(alignflags & ResizeBottomSide){
        alignflags |= ResizeBottom;
    }
    //top bottom
    if((alignflags & ResizeTop) && (alignflags & ResizeBottom)){
        vscale.y = 1.0f;
        vfixscale.y = 1.0f;
        vbox.b = pbox.b + box.b;
        vbox.t = pbox.t - box.t;
        vsiz.h = vbox.t - vbox.b;
        vpos.y -= panchor.y * psiz.h;
        vpos.y += vanchor.y * vsiz.h;
    }else if(alignflags & ResizeBottom){
        vbox.b = pbox.b + box.b;
        vbox.t = vbox.b + vsiz.h;
        vpos.y = vbox.b + vsiz.h/2.0f;
        vpos.y += vanchor.y * vsiz.h;
        if(alignflags & ResizeBottomSide){
            vpos.y -= (box.b * 2 + vsiz.h);
        }
        vsiz.h = size.h;
    }else if(alignflags & ResizeTop){
        vbox.t = pbox.t - box.t;
        vbox.b = vbox.t - vsiz.h;
        vpos.y = vbox.b + vsiz.h/2.0f;
        vpos.y += vanchor.y * vsiz.h;
        if(alignflags & ResizeTop){
            vpos.y += (box.t * 2 + vsiz.h);
        }
        vsiz.h = size.h;
    }
    SetFixScale(vfixscale);
    SetScale(vscale);
    SetSize(vsiz);
    SetPosition(vpos);
}

void cxView::OnUpdate(cxFloat dt)
{
    
}

const cxInt cxView::BindesSize() const
{
    return (cxInt)bindes.size();
}

const cxInt cxView::BindedSize() const
{
    return (cxInt)binded.size();
}

void cxView::Bind(cxView *obj)
{
    CX_ASSERT(obj != nullptr, "obj error");
    CX_ASSERT(obj != this, "self can't bind self");
    cxLong i = (cxLong)this;
    cxLong m = (cxLong)obj;
    bindes.insert(m);
    obj->binded.insert(i);
}

const cxBool cxView::HasBindes(cxView *pview) const
{
    return bindes.find((cxLong)pview) != bindes.end();
}

const cxBool cxView::HasBinded(cxView *pview) const
{
    return binded.find((cxLong)pview) != binded.end();
}

void cxView::EachBindes(std::function<void(cxView *pview)> func)
{
    std::set<cxLong>::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxView *pview = (cxView *)(*it);
        func(pview);
        it++;
    }
}

void cxView::EachBinded(std::function<void(cxView *pview)> func)
{
    std::set<cxLong>::iterator it = binded.begin();
    while(it != binded.end()){
        cxView *pview = (cxView *)(*it);
        func(pview);
        it++;
    }
}

void cxView::UnBind(cxView *pview)
{
    CX_ASSERT(pview != nullptr, "obj error");
    cxLong i = (cxLong)this;
    cxLong m = (cxLong)pview;
    bindes.erase(m);
    pview->binded.erase(i);
}

void cxView::UnBind()
{
    cxLong i = (cxLong)this;
    std::set<cxLong>::iterator it = bindes.begin();
    while(it != bindes.end()){
        cxView *pview = (cxView *)(*it);
        pview->binded.erase(i);
        it++;
    }
    bindes.clear();
    it = binded.begin();
    while(it != binded.end()){
        cxView *pview = (cxView *)(*it);
        pview->bindes.erase(i);
        it++;
    }
    binded.clear();
}


CX_CPP_END











