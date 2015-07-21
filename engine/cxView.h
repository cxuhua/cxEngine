//
//  cxView.h
//  cxEngineCore
//
//  Created by xuhua on 6/14/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxView_h
#define cxEngineCore_cxView_h

#include <map>
#include <vector>
#include <core/cxArray.h>
#include <core/cxHash.h>
#include <core/cxLocalized.h>

#include <math/cxPoint2F.h>
#include <math/cxPoint3F.h>
#include <math/cxSize2F.h>
#include <math/cxColor4F.h>
#include <math/cxMatrixF.h>
#include <math/cxBox4F.h>
#include <math/cxBoxPoint3F.h>
#include <math/cxboxColor4F.h>

#include "cxOpenGL.h"
#include "cxAction.h"
#include "cxShader.h"
#include "cxRender.h"
#include "cxDraw.h"
#include "cxTouch.h"

CX_CPP_BEGIN

class cxEngine;
class cxView : public cxObject,public TDrawable
{
public:
    CX_DECLARE(cxView);
public:
    typedef cxULong Resize;
    typedef cxUInt DirtyMode;
    typedef cxUInt Relative;
    
    static const Relative RelativeNone          = 0;
    static const Relative RelativeParent        = 1;
    static const Relative RelativeGlobal        = 2;
    
    static const Resize ResizeNone              = 0;
    static const Resize ResizeLeft              = 1 << 0;
    static const Resize ResizeRight             = 1 << 1;
    static const Resize ResizeTop               = 1 << 2;
    static const Resize ResizeBottom            = 1 << 3;
    
    static const Resize ResizeWidth             = ResizeLeft | ResizeRight;
    static const Resize ResizeHeight            = ResizeTop | ResizeBottom;
    
    static const Resize ResizeLeftTop           = ResizeLeft | ResizeTop;
    static const Resize ResizeLeftBottom        = ResizeBottom | ResizeLeft;
    static const Resize ResizeRightTop          = ResizeTop | ResizeRight;
    static const Resize ResizeRightBottom       = ResizeBottom | ResizeRight;
    
    static const Resize ResizeFill              = ResizeWidth | ResizeHeight;
    
    static const Resize ResizeTopSide           = 1 << 4;
    static const Resize ResizeBottomSide        = 1 << 5;
    static const Resize ResizeLeftSide          = 1 << 6;
    static const Resize ResizeRightSide         = 1 << 7;
    static const Resize ResizeAllSide           = ResizeTopSide|ResizeBottomSide|ResizeLeftSide|ResizeRightSide;
    
    static const DirtyMode DirtyModeNone        = 0;
    static const DirtyMode DirtyModeSize        = 1 << 0;
    static const DirtyMode DirtyModeScale       = 1 << 1;
    static const DirtyMode DirtyModePosition    = 1 << 2;
    static const DirtyMode DirtyModeColor       = 1 << 3;
    static const DirtyMode DirtyModeAngle       = 1 << 4;
    static const DirtyMode DirtyModeTexture     = 1 << 5;
    static const DirtyMode DirtyModeLayout      = 1 << 6;
    static const DirtyMode DirtyModeAnchor      = 1 << 7;
    static const DirtyMode DirtyModeForce       = 1 << 8;
    static const DirtyMode DirtyModeNormal      = DirtyModePosition|DirtyModeAngle|DirtyModeScale;
    
    static const cxUInt FlagsDirty              = 1 << 0;
    static const cxUInt FlagsClip               = 1 << 1;
protected:
    explicit cxView();
    virtual ~cxView();
protected:
    cxRenderState state;
private:
    typedef std::map<cxLong,cxLong> BindMap;
    cxFloat direction;
    BindMap bindes;
    BindMap binded;
    
    Resize resizeflags;
    cxBox4F resizebox;
    
    cxArray *actapps;
    cxArray *actions;
    
    void runAppends(cxFloat dt);
    void runRemoves(cxFloat dt);

    cxView *parent;
    DirtyMode dirtymode;
    cxArray *viewapps;
    cxArray *subviews;
    cxMatrixF modelview;
    cxMatrixF normalMatrix;
    cxMatrixF anchorMatrix;

    cxPoint2F offset;
    cxPoint2F position;
    cxSize2F size;
    cxPoint2F scale;
    cxPoint2F fixscale;
    cxPoint2F anchor;
    cxPoint3F axis;
    cxFloat angle;
    
    cxInt maxz;
    cxInt z;
    
    cxBool isvisible;
    cxBool issleep;
    cxBool hideTop;
    cxBool isremoved;
    cxBool isclip;
    
    cxShader *shader;
    cxBool istouch;
    
    cxBoxPoint3F wb;    //transform after
    cxBoxPoint3F bp;    //transform before
    cxBoxColor4F cc;    //box color
    
    void updateActions(cxFloat dt);
    
    static cxInt sortFunc(const void *lp,const void *rp);
    cxBool issort;
    
    void transform();
    cxBox4F clipbox;
    
    cxUInt flags;           //update and render flags
    cxBool islayout;
    cxBool isdir;
    
    Relative relative;
public:
    cxView *SetRelative(Relative v);
    
    const cxBool EnableDir() const;
    cxView *EnableDir(cxBool v);
    
    cxView *SetDirection(cxFloat v);
    const cxFloat Direction() const;
    
    cxView *SetResizeFlags(Resize flags);
    cxView *SetResizeBox(const cxBox4F &box);
    
    cxRenderState &State();
    //local point -> gl screen point
    cxPoint2F ToGLPoint(const cxPoint2F &wp);
    //gl screen point -> local point
    cxPoint2F FromGLPoint(const cxPoint2F &gp);
    //local point -> window point
    cxPoint2F ToPoint(const cxPoint2F &vp);
    //window point -> local point
    cxPoint2F FromPoint(const cxPoint2F &wp);
    
    const cxBox4F ToMaxBox();
    cxView *SetClip(cxBool v);
    
    //max z front
    cxInt Z() const;
    cxView *SetZ(cxInt v);
    
    cxView *SetBlend(const BlendFunc &b);
    const BlendFunc &Blend() const;

    const cxMatrixF &ModelView() const;
    
    cxShader *Shader();
    cxView *SetShader(cchars key);
    cxView *SetShader(cxShader *ps);
    
    cxView *SetFrame(cxFloat x,cxFloat y,cxFloat w,cxFloat h);
   
    cxUInt Flags() const;
    cxView *SetFlags(cxUInt f);
    
    cxBoxColor4F &BoxColor();
    cxBoxPoint3F &BoxPoint();
    cxBox4F ToBox4F();
    
    cxView *SetParent(cxView *v);
    cxView *Parent() const;
    
    cxBool EnableHideTop() const;
    cxView *EnableHideTop(cxBool v);
    
    cxBool EnableVisible() const;
    cxView *EnableVisible(cxBool v);
    
    cxBool IsRemoved() const;
    
    cxBool EnableSleep() const;
    cxView *EnableSleep(cxBool v);
    
    cxView *SetColor(const cxColor4F &v);
    cxView *SetAlpha(const cxFloat a);
    const cxFloat Alpha() const;
    const cxColor4F &Color() const;
    
    cxBool IsDirtyMode(DirtyMode v) const;
    cxView *SetDirty(DirtyMode mode);
    void ClearDirty();
    
    const cxPoint3F &Axis() const;
    cxView *SetAxis(const cxPoint2F &v);
    cxView *SetAxis(const cxPoint3F &v);
    
    const cxFloat Angle() const;
    cxView *SetAngle(const cxFloat &v);
    
    const cxPoint2F &Offset() const;
    cxView *SetOffset(const cxPoint2F &v);
    
    const cxPoint2F &Position() const;
    cxView *AddPosition(const cxPoint2F &v);
    cxView *SetPosition(const cxPoint2F &v);
    
    const cxPoint2F &FixScale() const;
    const cxPoint2F TransScale() const;
    const cxPoint2F &Scale() const;
    cxView *SetScale(const cxPoint2F &v);
    cxView *SetFixScale(const cxPoint2F &v);
    
    const cxPoint2F ToAnchor(const cxPoint2F &pos);
    const cxPoint2F &Anchor() const;
    cxView *SetAnchor(const cxPoint2F &v,cxBool keep=false);
    
    const cxSize2F ContentSize() const;
    const cxSize2F &Size() const;
    cxView *SetSize(const cxSize2F &v);
    
    cxBool EnableTouch() const;
    cxView *EnableTouch(cxBool v);
    
    cxArray *Subviews() const;
protected:
    virtual void OnRender(cxRender *render,const cxMatrixF &model);
    virtual void OnUpdate(cxFloat dt);
    virtual void OnDirty();
    virtual void OnLayout();
    virtual void OnSort();
    virtual void OnAngle();
    virtual void OnAppend(cxView *nview);
    virtual void OnRemove(cxView *oview);
    virtual cxBool OnDispatch(const cxTouchable *e);
public:
    cxEvent<cxView, cxView *> onAppend;
    cxEvent<cxView, cxView *> onRemove;
    cxEvent<cxView> onFree;
public:
    cxView *Sort();
    
    cxBool IsEmpty() const;
    
    cxBool HasAction(cxActionId aid) const;
    cxAction *GetAction(cxActionId aid);
    cxView *StopAction(cxActionId aid=0);
    cxView *ExitAction(cxActionId aid=0);

    virtual void Render(cxRender *render,const cxMatrixF &mv);
    
    void Update(cxFloat dt);
    
    const cxHitInfo HitTest(const cxPoint2F &wp);
    cxBool Dispatch(const cxTouchable *e);

    cxEvent<cxView> onEnter;
    cxEvent<cxView> onLeave;
    
    void Layout();
    
    cxView *BringFront();
    
    virtual cxView *Clone();
    
    cxView *Append(cxView *view);
    cxView *Append(cxAction *action);

    cxView *RemoveSubviews();
    cxView *Remove();
    
    //bind support
    const cxInt BindesSize() const;
    const cxInt BindedSize() const;
    //
    const cxArray *GetBindes();
    const cxArray *GetBinded();
    //if this bind obj
    const cxBool HasBindes(cxView *pview) const ;
    //if this binded obj
    const cxBool HasBinded(cxView *pview) const ;
    //
    void Bind(cxView *pview,cxLong tag=0);
    void UnBind(cxView *pview);
    void UnBind();
};

CX_CPP_END

#endif


