
#include <jni.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <strings.h>
#include <stdbool.h>
#include "rtmp.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    #define NAL_SLICE  1
    #define NAL_SLICE_DPA  2
    #define NAL_SLICE_DPB  3
    #define NAL_SLICE_DPC  4
    #define NAL_SLICE_IDR  5
    #define NAL_SEI  6
    #define NAL_SPS  7
    #define NAL_PPS  8
    #define NAL_AUD  9
    #define NAL_FILLER  12
    
    typedef struct {
        char *buf;
        int len;
    }DATA;
    
    
    int send_rtmp_video(RTMP *rtmp,bool keyframe,DATA *data,uint32_t timestamp)
    {
        RTMPPacket packet;
        RTMPPacket_Reset(&packet);
        
        int ret = RTMPPacket_Alloc(&packet, data->len + 9);
        assert(ret == TRUE);
        char * body = packet.m_body;
        
        body[0] = (keyframe?0x17:0x27);
        body[1] = 0x01;   /*nal unit*/
        body[2] = 0x00;
        body[3] = 0x00;
        body[4] = 0x00;
        
        body[5] = (data->len >> 24) & 0xff;
        body[6] = (data->len >> 16) & 0xff;
        body[7] = (data->len >>  8) & 0xff;
        body[8] = (data->len ) & 0xff;
        
        /*copy data*/
        memcpy(&body[9],data->buf,data->len);
        
        packet.m_hasAbsTimestamp = 0;
        packet.m_packetType = RTMP_PACKET_TYPE_VIDEO;
        packet.m_nInfoField2 = rtmp->m_stream_id;
        packet.m_nChannel = 0x04;
        packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
        packet.m_nTimeStamp = timestamp;
        
        /*调用发送接口*/
        ret = RTMP_SendPacket(rtmp,&packet,FALSE);
        assert(ret == TRUE);
        RTMPPacket_Free(&packet);
        return ret;
    }
    
    int send_sps_pps(RTMP *rtmp, DATA *sps,DATA *pps)
    {
        RTMPPacket packet;
        RTMPPacket_Reset(&packet);
        char *body;
        int i;
        int ret =  RTMPPacket_Alloc(&packet, 1024);
        assert(ret == TRUE);
        body=packet.m_body;
        
        i = 0;
        body[i++] = 0x17;
        body[i++] = 0x00;
        
        body[i++] = 0x00;
        body[i++] = 0x00;
        body[i++] = 0x00;
        
        /*AVCDecoderConfigurationRecord*/
        body[i++] = 0x01;
        body[i++] = sps->buf[1];
        body[i++] = sps->buf[2];
        body[i++] = sps->buf[3];
        body[i++] = 0xff;
        
        /*sps*/
        body[i++]   = 0xe1;
        body[i++] = (sps->len >> 8) & 0xff;
        body[i++] = sps->len & 0xff;
        memcpy(&body[i],sps->buf,sps->len);
        i +=  sps->len;
        
        /*pps*/
        body[i++]   = 0x01;
        body[i++] = (pps->len >> 8) & 0xff;
        body[i++] = (pps->len) & 0xff;
        memcpy(&body[i],pps->buf,pps->len);
        i +=  pps->len;
        
        packet.m_packetType = RTMP_PACKET_TYPE_VIDEO;
        packet.m_nBodySize = i;
        packet.m_nChannel = 0x04;
        packet.m_nTimeStamp = 0;
        packet.m_hasAbsTimestamp = 0;
        packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
        packet.m_nInfoField2 = rtmp->m_stream_id;
        
        /*调用发送接口*/
        ret = RTMP_SendPacket(rtmp, &packet,FALSE);
        assert(ret == TRUE);
        RTMPPacket_Free(&packet);
        
        return ret;
    }

    
    RTMP *r=NULL;
    char *buf=NULL;
    DATA sps={NULL,0};
    DATA pps={NULL,0};
    
    JNIEXPORT void JNICALL Java_org_yanzi_camera_RTMP_Init(JNIEnv *env, jclass clasz,jstring url)
    {
        const char *rtmpurl = (*env)->GetStringUTFChars(env, url, 0);
        buf = realloc(buf,1024*1024);
        r = RTMP_Alloc();
        RTMP_Init(r);
        int ret = RTMP_SetupURL(r, (char *)rtmpurl);
        assert(ret == TRUE);
        RTMP_EnableWrite(r);
        ret = RTMP_Connect(r, NULL);
        assert(ret == TRUE);
        ret = RTMP_ConnectStream(r, 0);
        assert(ret == TRUE);
        (*env)->ReleaseStringUTFChars(env, url, rtmpurl);
    }

    static int runFrame(RTMP *r,bool keyframe, char *data,int len,long timestamp){
        DATA idr;
        idr.buf = data;
        idr.len = len;
        int ret = send_rtmp_video(r, keyframe, &idr,timestamp);
        if(keyframe){
            RTMP_Log(RTMP_LOGERROR, "SEND KEY FRAME len=%d %d Time=%ld\n",len, ret, timestamp);
        }
        return ret;
    }
    
    static int runSPSPPSFrame(RTMP *r, char *data,int len){
        int ret = 0;
        uint8_t t = (uint8_t)data[0] & 0x1F;
        if(t == NAL_SPS){
            sps.buf = realloc(sps.buf,len);
            memcpy(sps.buf,data,len);
            sps.len = len;
            RTMP_Log(RTMP_LOGERROR, "SAVE SPS %d",len);
        }else if(t == NAL_PPS){
            pps.buf = realloc(pps.buf,len);
            memcpy(pps.buf,data,len);
            pps.len = len;
            ret = send_sps_pps(r, &sps, &pps);
            RTMP_Log(RTMP_LOGERROR, "SAVE PPS %d",len);
        }
        return ret;
    }
    
    JNIEXPORT void JNICALL Java_org_yanzi_camera_RTMP_SaveVideo(JNIEnv *env, jclass clasz,jbyteArray data)
    {
        jsize len  = (*env)->GetArrayLength(env,data);
        (*env)->GetByteArrayRegion(env, data, 0, len, (jbyte *)buf);
        
        char *body = buf;
        int size = len;
        int beg = -1;
        int end = -1;
        int c = 0;
        bool isget = false;
        int step = 0;
        for(int i=0;i<size;i++){
            char b1 = body[i+0];
            
            if(i+1 >= size){
                break;
            }
            char b2 = body[i+1];
            
            if(i+2 >= size){
                break;
            }
            char b3 = body[i+2];
            isget = false;
            if(b1 == 0 && b2 == 0 && b3 == 1){
                isget = true;
                c++;
                if(beg >= 0)end = i;
                i+=3;
                step = 3;
                if(beg < 0)beg = i;
                goto getframe;
            }
            if(i+3 >= size){
                break;
            }
            char b4 = body[i+3];
            if(b1 == 0 && b2 == 0 && b3 == 0 && b4 == 1){
                isget = true;
                c++;
                if(beg >= 0)end = i;
                i+=4;
                step = 4;
                if(beg < 0)beg = i;
                goto getframe;
            }
        getframe:
            if(isget && beg >=0 && end >= 0){
                int len = end-beg;
                char *data = body + beg;
                runSPSPPSFrame(r, data, len);
                beg = end + step;
            }
        }
        if(size > beg){
            int len = size - beg;
            char *data = body + beg;
            runSPSPPSFrame(r, data, len);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_yanzi_camera_RTMP_PushVideo(JNIEnv *env, jclass clasz,jbyteArray data,jboolean keyframe,jlong timestamp)
    {
        jsize len  = (*env)->GetArrayLength(env,data);
        (*env)->GetByteArrayRegion(env, data, 0, len, (jbyte *)buf);
        runFrame(r, keyframe, buf + 4, len - 4, timestamp);
    }
    
#ifdef __cplusplus
}
#endif
