//
//  math.c
//  cxEngineCore
//
//  Created by xuhua on 6/26/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "tools.h"

static CX_ATTR_UNUSED bool isNeon32Enabled()
{
#ifdef USE_NEON32
    return true;
#elif (defined (INCLUDE_NEON32) && (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID) )
    static bool ischeck = false;
    static bool isenable = false;
    if(!ischeck){
        uint64_t cpu = android_getCpuFeatures();
        AndroidCpuFamily family = android_getCpuFamily();
        if(family == ANDROID_CPU_FAMILY_ARM && (cpu & ANDROID_CPU_ARM_FEATURE_NEON)!=0){
            isenable = true;
        }else{
            isenable = false;
        }
        ischeck = true;
    }
    return isenable;
#else
    return false;
#endif
}

static CX_ATTR_UNUSED bool isNeon64Enabled()
{
#ifdef USE_NEON64
    return true;
#else
    return false;
#endif
}

static inline CX_ATTR_UNUSED void ansic_Mat4_X_Mat4(const float* m1, const float* m2, float* dst)
{
    float product[16];
    
    product[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
    product[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
    product[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
    product[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];
    
    product[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
    product[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
    product[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
    product[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];
    
    product[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
    product[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
    product[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
    product[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];
    
    product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    
    memcpy(dst, product, sizeof(float) * 16);
}

static inline CX_ATTR_UNUSED void ansic_Vec4_X_Mat4(const float* m, float x, float y, float z, float w, float* dst)
{
    dst[0] = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst[1] = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst[2] = x * m[2] + y * m[6] + z * m[10] + w * m[14];
}

#ifdef USE_NEON32
static inline CX_ATTR_UNUSED void neon32_Mat4_X_Mat4(const float* m1, const float* m2, float* dst)
{
    asm volatile
    (
        "vld1.32     {d16 - d19}, [%1]! \n\t"         // M1[m0-m7]
        "vld1.32     {d20 - d23}, [%1]  \n\t"         // M1[m8-m15]
        "vld1.32     {d0 - d3}, [%2]!   \n\t"         // M2[m0-m7]
        "vld1.32     {d4 - d7}, [%2]    \n\t"         // M2[m8-m15]

        "vmul.f32    q12, q8, d0[0]     \n\t"         // DST->M[m0-m3] = M1[m0-m3] * M2[m0]
        "vmul.f32    q13, q8, d2[0]     \n\t"         // DST->M[m4-m7] = M1[m4-m7] * M2[m4]
        "vmul.f32    q14, q8, d4[0]     \n\t"         // DST->M[m8-m11] = M1[m8-m11] * M2[m8]
        "vmul.f32    q15, q8, d6[0]     \n\t"         // DST->M[m12-m15] = M1[m12-m15] * M2[m12]

        "vmla.f32    q12, q9, d0[1]     \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m1]
        "vmla.f32    q13, q9, d2[1]     \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m5]
        "vmla.f32    q14, q9, d4[1]     \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m9]
        "vmla.f32    q15, q9, d6[1]     \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m13]

        "vmla.f32    q12, q10, d1[0]    \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m2]
        "vmla.f32    q13, q10, d3[0]    \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m6]
        "vmla.f32    q14, q10, d5[0]    \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m10]
        "vmla.f32    q15, q10, d7[0]    \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m14]

        "vmla.f32    q12, q11, d1[1]    \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m3]
        "vmla.f32    q13, q11, d3[1]    \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m7]
        "vmla.f32    q14, q11, d5[1]    \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m11]
        "vmla.f32    q15, q11, d7[1]    \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m15]

        "vst1.32    {d24 - d27}, [%0]!  \n\t"         // DST->M[m0-m7]
        "vst1.32    {d28 - d31}, [%0]   \n\t"         // DST->M[m8-m15]

        : // output
        : "r"(dst), "r"(m1), "r"(m2) // input - note *value* of pointer doesn't change.
        : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
     );
}

static inline CX_ATTR_UNUSED void neon32_Vec4_X_Mat4(const float* m, float x, float y, float z, float w, float* dst)
{
    asm volatile
    (
        "vld1.32    {d0[0]},        [%1]    \n\t"    // V[x]
        "vld1.32    {d0[1]},        [%2]    \n\t"    // V[y]
        "vld1.32    {d1[0]},        [%3]    \n\t"    // V[z]
        "vld1.32    {d1[1]},        [%4]    \n\t"    // V[w]
        "vld1.32    {d18 - d21},    [%5]!   \n\t"    // M[m0-m7]
        "vld1.32    {d22 - d25},    [%5]    \n\t"    // M[m8-m15]

        "vmul.f32 q13,  q9, d0[0]           \n\t"    // DST->V = M[m0-m3] * V[x]
        "vmla.f32 q13, q10, d0[1]           \n\t"    // DST->V += M[m4-m7] * V[y]
        "vmla.f32 q13, q11, d1[0]           \n\t"    // DST->V += M[m8-m11] * V[z]
        "vmla.f32 q13, q12, d1[1]           \n\t"    // DST->V += M[m12-m15] * V[w]

        "vst1.32 {d26}, [%0]!               \n\t"    // DST->V[x, y]
        "vst1.32 {d27[0]}, [%0]             \n\t"    // DST->V[z]
        :
        : "r"(dst), "r"(&x), "r"(&y), "r"(&z), "r"(&w), "r"(m)
        : "q0", "q9", "q10","q11", "q12", "q13", "memory"
     );
}
#endif

#ifdef USE_NEON64
static inline CX_ATTR_UNUSED void neon64_Mat4_X_Mat4(const float* m1, const float* m2, float* dst)
{
    asm volatile
    (
        "ld1     {v8.4s, v9.4s, v10.4s, v11.4s}, [%1] \n\t"         // M1[m0-m7] M1[m8-m15] M2[m0-m7]  M2[m8-m15]
        "ld4     {v0.4s, v1.4s, v2.4s, v3.4s},  [%2]   \n\t"        // M2[m0-m15]


        "fmul    v12.4s, v8.4s, v0.s[0]     \n\t"                   // DST->M[m0-m3] = M1[m0-m3] * M2[m0]
        "fmul    v13.4s, v8.4s, v0.s[1]     \n\t"                   // DST->M[m4-m7] = M1[m4-m7] * M2[m4]
        "fmul    v14.4s, v8.4s, v0.s[2]     \n\t"                   // DST->M[m8-m11] = M1[m8-m11] * M2[m8]
        "fmul    v15.4s, v8.4s, v0.s[3]     \n\t"                   // DST->M[m12-m15] = M1[m12-m15] * M2[m12]

        "fmla    v12.4s, v9.4s, v1.s[0]     \n\t"                   // DST->M[m0-m3] += M1[m0-m3] * M2[m1]
        "fmla    v13.4s, v9.4s, v1.s[1]     \n\t"                   // DST->M[m4-m7] += M1[m4-m7] * M2[m5]
        "fmla    v14.4s, v9.4s, v1.s[2]     \n\t"                   // DST->M[m8-m11] += M1[m8-m11] * M2[m9]
        "fmla    v15.4s, v9.4s, v1.s[3]     \n\t"                   // DST->M[m12-m15] += M1[m12-m15] * M2[m13]

        "fmla    v12.4s, v10.4s, v2.s[0]    \n\t"                   // DST->M[m0-m3] += M1[m0-m3] * M2[m2]
        "fmla    v13.4s, v10.4s, v2.s[1]    \n\t"                   // DST->M[m4-m7] += M1[m4-m7] * M2[m6]
        "fmla    v14.4s, v10.4s, v2.s[2]    \n\t"                   // DST->M[m8-m11] += M1[m8-m11] * M2[m10]
        "fmla    v15.4s, v10.4s, v2.s[3]    \n\t"                   // DST->M[m12-m15] += M1[m12-m15] * M2[m14]

        "fmla    v12.4s, v11.4s, v3.s[0]    \n\t"                   // DST->M[m0-m3] += M1[m0-m3] * M2[m3]
        "fmla    v13.4s, v11.4s, v3.s[1]    \n\t"                   // DST->M[m4-m7] += M1[m4-m7] * M2[m7]
        "fmla    v14.4s, v11.4s, v3.s[2]    \n\t"                   // DST->M[m8-m11] += M1[m8-m11] * M2[m11]
        "fmla    v15.4s, v11.4s, v3.s[3]    \n\t"                   // DST->M[m12-m15] += M1[m12-m15] * M2[m15]

        "st1    {v12.4s, v13.4s, v14.4s, v15.4s}, [%0]  \n\t"       // DST->M[m0-m7]// DST->M[m8-m15]

        : // output
        : "r"(dst), "r"(m1), "r"(m2) // input - note *value* of pointer doesn't change.
        : "memory", "v0", "v1", "v2", "v3", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
    );
}

static inline CX_ATTR_UNUSED void neon64_Vec4_X_Mat4(const float* m, float x, float y, float z, float w, float* dst)
{
    asm volatile
    (
        "ld1    {v0.s}[0],        [%1]    \n\t"                    // V[x]
        "ld1    {v0.s}[1],        [%2]    \n\t"                    // V[y]
        "ld1    {v0.s}[2],        [%3]    \n\t"                    // V[z]
        "ld1    {v0.s}[3],        [%4]    \n\t"                    // V[w]
        "ld1    {v9.4s, v10.4s, v11.4s, v12.4s}, [%5]   \n\t"      // M[m0-m7] M[m8-m15]

        "fmul v13.4s, v9.4s, v0.s[0]           \n\t"               // DST->V = M[m0-m3] * V[x]
        "fmla v13.4s, v10.4s, v0.s[1]           \n\t"              // DST->V += M[m4-m7] * V[y]
        "fmla v13.4s, v11.4s, v0.s[2]           \n\t"              // DST->V += M[m8-m11] * V[z]
        "fmla v13.4s, v12.4s, v0.s[3]           \n\t"              // DST->V += M[m12-m15] * V[w]

        //"st1 {v13.4s}, [%0]               \n\t"                  // DST->V[x, y] // DST->V[z]
        "st1 {v13.2s}, [%0], 8               \n\t"
        "st1 {v13.s}[2], [%0]                \n\t"
        :
        : "r"(dst), "r"(&x), "r"(&y), "r"(&z), "r"(&w), "r"(m)
        : "v0", "v9", "v10","v11", "v12", "v13", "memory"
     );
}

#endif

void Mat4_X_Mat4(const float* m1, const float* m2, float* dst)
{
#ifdef USE_NEON32
    neon32_Mat4_X_Mat4(m1,m2,dst);
#elif defined (USE_NEON64)
    neon64_Mat4_X_Mat4(m1,m2,dst);
#elif defined (INCLUDE_NEON32)
    if(isNeon32Enabled()) neon32_Mat4_X_Mat4(m1,m2,dst);
    else ansic_Mat4_X_Mat4(m1,m2,dst);
#else
    ansic_Mat4_X_Mat4(m1,m2,dst);
#endif
}

void Vec4_X_Mat4(const float *m, float x, float y, float z, float w, float *dst)
{
#ifdef USE_NEON32
    neon32_Vec4_X_Mat4(m, x, y, z, w, dst);
#elif defined (USE_NEON64)
    neon64_Vec4_X_Mat4(m, x, y, z, w, dst);
#elif defined (INCLUDE_NEON32)
    if(isNeon32Enabled()) neon32_Vec4_X_Mat4(m, x, y, z, w, dst);
    else ansic_Vec4_X_Mat4(m, x, y, z, w, dst);
#else
    ansic_Vec4_X_Mat4(m, x, y, z, w, dst);
#endif
}



















