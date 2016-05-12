//
//  cxTouch.h
//  cxEngineCore
//
//  Created by xuhua on 6/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_cxTouch_h
#define cxEngineCore_cxTouch_h

#include <map>
#include <vector>
#include <math/cxMath.h>
#include <math/cxPoint2F.h>

CX_CPP_BEGIN

class cxView;

typedef cxUInt cxTouchType;
typedef cxULong cxTouchId;

struct cxHitInfo
{
    cxPoint2F point;
    cxBool hited;
    cxHitInfo();
};

struct cxTouchPoint
{
    cxTouchId key;
    cxTouchType type;
    cxPoint2F wp;
    cxPoint2F prev;
    cxPoint2F movement;
    cxFloat length;
    cxPoint2F delta;
    cxDouble time;
    
    static const cxTouchType None   = 0;
    static const cxTouchType Began  = 1;
    static const cxTouchType Moved  = 2;
    static const cxTouchType Ended  = 3;
    
    cxTouchPoint();
    
    cxFloat Length() const;
    cxPoint2F Speed() const;
    cxBool IsBegan() const;
    cxBool IsMoved() const;
    cxBool IsEnded() const;
    cxBool IsTap() const;
};


enum cxKeyType
{
    cxKeyTypeDown       = 0,
    cxKeyTypeUp         = 1,
    cxKeyTypeMultiple   = 2,
};

struct cxKey
{
    cxKeyType type;
    cxInt code;
};

//from android ndk keycodes.h
enum {
    CX_KEYCODE_UNKNOWN         = 0,
    CX_KEYCODE_SOFT_LEFT       = 1,
    CX_KEYCODE_SOFT_RIGHT      = 2,
    CX_KEYCODE_HOME            = 3,
    CX_KEYCODE_BACK            = 4,
    CX_KEYCODE_CALL            = 5,
    CX_KEYCODE_ENDCALL         = 6,
    CX_KEYCODE_0               = 7,
    CX_KEYCODE_1               = 8,
    CX_KEYCODE_2               = 9,
    CX_KEYCODE_3               = 10,
    CX_KEYCODE_4               = 11,
    CX_KEYCODE_5               = 12,
    CX_KEYCODE_6               = 13,
    CX_KEYCODE_7               = 14,
    CX_KEYCODE_8               = 15,
    CX_KEYCODE_9               = 16,
    CX_KEYCODE_STAR            = 17,
    CX_KEYCODE_POUND           = 18,
    CX_KEYCODE_DPAD_UP         = 19,
    CX_KEYCODE_DPAD_DOWN       = 20,
    CX_KEYCODE_DPAD_LEFT       = 21,
    CX_KEYCODE_DPAD_RIGHT      = 22,
    CX_KEYCODE_DPAD_CENTER     = 23,
    CX_KEYCODE_VOLUME_UP       = 24,
    CX_KEYCODE_VOLUME_DOWN     = 25,
    CX_KEYCODE_POWER           = 26,
    CX_KEYCODE_CAMERA          = 27,
    CX_KEYCODE_CLEAR           = 28,
    CX_KEYCODE_A               = 29,
    CX_KEYCODE_B               = 30,
    CX_KEYCODE_C               = 31,
    CX_KEYCODE_D               = 32,
    CX_KEYCODE_E               = 33,
    CX_KEYCODE_F               = 34,
    CX_KEYCODE_G               = 35,
    CX_KEYCODE_H               = 36,
    CX_KEYCODE_I               = 37,
    CX_KEYCODE_J               = 38,
    CX_KEYCODE_K               = 39,
    CX_KEYCODE_L               = 40,
    CX_KEYCODE_M               = 41,
    CX_KEYCODE_N               = 42,
    CX_KEYCODE_O               = 43,
    CX_KEYCODE_P               = 44,
    CX_KEYCODE_Q               = 45,
    CX_KEYCODE_R               = 46,
    CX_KEYCODE_S               = 47,
    CX_KEYCODE_T               = 48,
    CX_KEYCODE_U               = 49,
    CX_KEYCODE_V               = 50,
    CX_KEYCODE_W               = 51,
    CX_KEYCODE_X               = 52,
    CX_KEYCODE_Y               = 53,
    CX_KEYCODE_Z               = 54,
    CX_KEYCODE_COMMA           = 55,
    CX_KEYCODE_PERIOD          = 56,
    CX_KEYCODE_ALT_LEFT        = 57,
    CX_KEYCODE_ALT_RIGHT       = 58,
    CX_KEYCODE_SHIFT_LEFT      = 59,
    CX_KEYCODE_SHIFT_RIGHT     = 60,
    CX_KEYCODE_TAB             = 61,
    CX_KEYCODE_SPACE           = 62,
    CX_KEYCODE_SYM             = 63,
    CX_KEYCODE_EXPLORER        = 64,
    CX_KEYCODE_ENVELOPE        = 65,
    CX_KEYCODE_ENTER           = 66,
    CX_KEYCODE_DEL             = 67,
    CX_KEYCODE_GRAVE           = 68,
    CX_KEYCODE_MINUS           = 69,
    CX_KEYCODE_EQUALS          = 70,
    CX_KEYCODE_LEFT_BRACKET    = 71,
    CX_KEYCODE_RIGHT_BRACKET   = 72,
    CX_KEYCODE_BACKSLASH       = 73,
    CX_KEYCODE_SEMICOLON       = 74,
    CX_KEYCODE_APOSTROPHE      = 75,
    CX_KEYCODE_SLASH           = 76,
    CX_KEYCODE_AT              = 77,
    CX_KEYCODE_NUM             = 78,
    CX_KEYCODE_HEADSETHOOK     = 79,
    CX_KEYCODE_FOCUS           = 80,   // *Camera* focus
    CX_KEYCODE_PLUS            = 81,
    CX_KEYCODE_MENU            = 82,
    CX_KEYCODE_NOTIFICATION    = 83,
    CX_KEYCODE_SEARCH          = 84,
    CX_KEYCODE_MEDIA_PLAY_PAUSE= 85,
    CX_KEYCODE_MEDIA_STOP      = 86,
    CX_KEYCODE_MEDIA_NEXT      = 87,
    CX_KEYCODE_MEDIA_PREVIOUS  = 88,
    CX_KEYCODE_MEDIA_REWIND    = 89,
    CX_KEYCODE_MEDIA_FAST_FORWARD = 90,
    CX_KEYCODE_MUTE            = 91,
    CX_KEYCODE_PAGE_UP         = 92,
    CX_KEYCODE_PAGE_DOWN       = 93,
    CX_KEYCODE_PICTSYMBOLS     = 94,
    CX_KEYCODE_SWITCH_CHARSET  = 95,
    CX_KEYCODE_BUTTON_A        = 96,
    CX_KEYCODE_BUTTON_B        = 97,
    CX_KEYCODE_BUTTON_C        = 98,
    CX_KEYCODE_BUTTON_X        = 99,
    CX_KEYCODE_BUTTON_Y        = 100,
    CX_KEYCODE_BUTTON_Z        = 101,
    CX_KEYCODE_BUTTON_L1       = 102,
    CX_KEYCODE_BUTTON_R1       = 103,
    CX_KEYCODE_BUTTON_L2       = 104,
    CX_KEYCODE_BUTTON_R2       = 105,
    CX_KEYCODE_BUTTON_THUMBL   = 106,
    CX_KEYCODE_BUTTON_THUMBR   = 107,
    CX_KEYCODE_BUTTON_START    = 108,
    CX_KEYCODE_BUTTON_SELECT   = 109,
    CX_KEYCODE_BUTTON_MODE     = 110,
};


class cxTouchable
{
protected:
    explicit cxTouchable();
    virtual ~cxTouchable();
private:
    std::map<cxTouchId,cxTouchPoint> events;
    std::vector<cxTouchPoint *> items;
    void updateEvent(const cxTouchPoint &e);
    void removeEvent(const cxTouchPoint &e);
public:
    const cxInt TouchCount() const;
    const cxTouchPoint *TouchPoint(cxInt idx) const;
    const cxHitInfo HitTest(cxView *view,const cxPoint2F &wp) const;
    void Dispatch(cxTouchId key,cxTouchType type,cxFloat x,cxFloat y);
    cxBool Dispatch(cxKeyType type,cxInt code);
public:
    virtual void OnDispatch(const cxTouchable *e);
};

CX_CPP_END

#endif


