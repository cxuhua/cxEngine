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
#include <math/cxBoxColor4F.h>

#include "cxOpenGL.h"
#include "cxAction.h"
#include "cxShader.h"
#include "cxRender.h"
#include "cxDraw.h"
#include "cxTouch.h"

CX_CPP_BEGIN

class cxEngine;
class cxViewExt;
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
    static const DirtyMode DirtyModeAll         = -1;
    static const DirtyMode DirtyModeSize        = 1 << 0;
    static const DirtyMode DirtyModeScale       = 1 << 1;
    static const DirtyMode DirtyModePosition    = 1 << 2;
    static const DirtyMode DirtyModeColor       = 1 << 3;
    static const DirtyMode DirtyModeAngle       = 1 << 4;
    static const DirtyMode DirtyModeTexture     = 1 << 5;
    static const DirtyMode DirtyModeAnchor      = 1 << 6;
    static const DirtyMode DirtyModeLayout      = 1 << 7;
    static const DirtyMode DirtyModeForce       = 1 << 8;
    static const DirtyMode DirtyModeNormal      = DirtyModePosition|DirtyModeAngle|DirtyModeScale;
    
    static const cxUInt FlagsNone               = 0;
    static const cxUInt FlagsDirty              = 1 << 0;
    static const cxUInt FlagsClip               = 1 << 1;
protected:
    explicit cxView();
    virtual ~cxView();
protected:
    cxRenderState state;
private:
    cxULong idx;            //update idx
    cxFloat direction;
    Resize resizeflags;
    cxBox4F resizebox;
    
    cxArray *actions;
    
    void runAppends(cxFloat dt);
    void runUpdates(cxFloat dt);

    cxView *parent;
    DirtyMode dirtymode;
    cxBool isnewadd;
    cxArray *views;
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
    
    cxFloat maxz;
    cxFloat z;
    
    cxBool isvisible;
    cxBool issleep;
    cxBool hideTop;
    cxBool isremoved;
    cxBool isclip;
    cxBool isclear;
    void clearViews();
    
    cxShader *shader;
    cxBool istouch;
    
    cxBoxPoint3F wb;    //transform after
    cxBoxPoint3F bp;    //transform before
    cxBoxColor4F cc;    //box color
    
    void runActions(cxFloat dt);
    
    static cxInt defaultSortFunc(const void *lp,const void *rp);
    cxCmpFunc sortFunc;
    cxBool issort;
    
    void transform();
    cxBox4F clipbox;
    
    cxUInt flags;           //update and render flags
    cxBool isdir;
    Relative relative;
public:
    cxView *SetSortFunc(cxCmpFunc func);
    
    cxView *SetRelative(Relative v);
    
    const cxBool EnableDir() const;
    cxView *SetEnableDir(cxBool v);
    
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
    
    const cxBox4F ToMaxRect4F();
    cxView *SetClip(cxBool v);
    
    //max z front
    cxFloat Z() const;
    cxView *SetZ(cxFloat v);
    
    cxFloat Distance(cxView *target);
    
    cxView *SetBlend(const BlendFunc &b);
    const BlendFunc &Blend() const;

    const cxMatrixF &ModelView() const;
    
    cxShader *Shader();
    cxView *SetShader(cchars key,cxBool deep=false);
    cxView *SetShader(cxShader *ps,cxBool deep=false);
    
    cxView *SetFrame(cxFloat x,cxFloat y,cxFloat w,cxFloat h);
   
    cxUInt Flags() const;
    cxView *SetFlags(cxUInt f);
    
    cxBoxColor4F &BoxColor();
    cxBoxPoint3F &BoxPoint();
    cxBox4F Box();
    cxBox4F ToBox4F();
    
    cxView *SetParent(cxView *v);
    cxView *Parent() const;
    
    cxBool EnableHideTop() const;
    cxView *SetEnableHideTop(cxBool v);
    
    cxBool EnableVisible() const;
    cxView *SetEnableVisible(cxBool v);
    
    cxBool IsRemoved() const;
    
    cxBool EnableSleep() const;
    cxView *SetEnableSleep(cxBool v);
    
    cxView *SetColor(const cxColor4F &v);
    cxView *SetColor(const cxBoxColor4F &v);
    cxView *SetAlpha(const cxFloat a);
    const cxFloat Alpha() const;
    const cxColor4F &Color() const;
    
    cxBool IsDirtyMode(DirtyMode v) const;
    cxView *SetDirty(DirtyMode mode);
    cxView *DelDirty(DirtyMode mode);
    void ClearDirty();
    
    const cxPoint3F &Axis() const;
    cxView *SetAxis(const cxPoint2F &v);
    cxView *SetAxis(const cxPoint3F &v);
    
    const cxFloat Angle() const;
    cxView *SetAngle(const cxFloat &v,cxBool isext=true);
    
    const cxPoint2F &Offset() const;
    cxView *SetOffset(const cxPoint2F &v);
    
    const cxPoint2F &Position() const;
    cxView *AddPosition(const cxPoint2F &v);
    cxView *SetPosition(const cxPoint2F &v,cxBool isext=true);
    cxBool SetPosition(const cxPoint2F &np,const cxPoint2F &wp,const cxFloat min=cxEqualFloat);
    cxView *SetX(cxFloat x);
    cxView *SetY(cxFloat y);
    
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
    cxView *SetEnableTouch(cxBool v);
    
    const cxArray *Subviews() const;
    cxView *At(cxInt idx);
protected:
    virtual void OnRender(cxRender *render,const cxMatrixF &model);
    virtual void OnIndex(cxULong idx);
    virtual void OnUpdate(cxFloat dt);
    virtual void OnDirty();
    virtual void OnLayout();
    virtual void OnSort();
    virtual void OnAngle();
    virtual void OnAppend(cxView *nview);
    virtual void OnRemove(cxView *oview);
    virtual cxBool OnDispatch(const cxTouchable *e);
    virtual cxBool OnDispatch(const cxKey &key);
    virtual void OnEnter();
    virtual void OnLeave();
public:
    //when add subview
    cxEvent<cxView, cxView *> onAppend;
    //when remove subview
    cxEvent<cxView, cxView *> onRemove;
public:
    void ResetIdx();
    const cxBox4F ParentBox() const;
    cxView *Select(cchars path);
    
    cxView *AttachTo(cxView *pview);
    
    void Elements(std::function<void(cxView *pview)> func);
    
    cxView *Sort();
    cxBool IsEmpty() const;
    
    cxBool HasAction(cxActionId aid) const;
    cxAction *GetAction(cxActionId aid);
    cxInt ActionSize() const;
    cxView *StopAction(cxActionId aid=0);
    cxView *ExitAction(cxActionId aid=0);

    void RenderSubviews(cxRender *render,const cxMatrixF &mv);
    virtual void Render(cxRender *render,const cxMatrixF &mv);
    
    void Update(cxFloat dt);
    
    const cxHitInfo HitTest(const cxPoint2F &wp);
    cxBool Dispatch(const cxTouchable *e);
    cxBool Dispatch(const cxKey &key);

    //when add to view
    cxEvent<cxView> onEnter;
    //when remove from view
    cxEvent<cxView> onLeave;

    //ratio height
    cxView *SetW(cxFloat w);
    //ratio width
    cxView *SetH(cxFloat h);
    
    void Layout();
    
    cxView *BringFront();
    
    cxView *Append(cxView *view);
    cxView *Append(cxAction *action);

    cxView *RemoveSubviews();
    cxView *Remove();
public:
    typedef std::function<void(cxAction *pav)> ICB;
    cxAction *Invoke(cxActionId id,ICB cb);
    cxAction *Invoke(cxActionId id,cxFloat delay,ICB cb);
    cxAction *Invoke(cxActionId id,cxFloat delay,cxInt repeat,ICB cb);
public:
    cxEvent<cxView, cxFloat> onUpdate;
private:
    cxViewExt *extvp;
public:
    cxBool HasExt();
    void SetExt(cxViewExt *ext);
    cxViewExt *GetExt();
};

CX_CPP_END

#endif


