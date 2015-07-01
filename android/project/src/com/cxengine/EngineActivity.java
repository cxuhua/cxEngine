package com.cxengine;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.HashMap;
import java.util.UUID;
import android.app.NativeActivity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Typeface;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.Layout;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;

public class EngineActivity extends NativeActivity {
	static {
	    System.loadLibrary("openal");
	}
	private MediaPlayer mediaPlayer = null;
	private String mediaFile = null;
	private HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();
	
	//发送json到gl线程
	public native void SendJson(String json);
	//从gl线程收到json
	public void RecvJson(final String json){
		this.runOnUiThread(new Runnable(){
			public void run(){
				EngineActivity.this.OnRecvJson(json);
			}
		});
	}
	//开始播放音乐
	public void cxPlayMusic(String file,float volume,boolean loop){
		if(mediaPlayer != null){
			mediaPlayer.stop();
			mediaPlayer.release();
			mediaPlayer = null;
		}
		this.mediaFile = file;
		mediaPlayer = new MediaPlayer();
		try {
			AssetFileDescriptor fd = this.getAssets().openFd(file);
			mediaPlayer.setVolume(volume, volume);
			mediaPlayer.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
			mediaPlayer.prepare();
			mediaPlayer.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public void cxSetMusicVolume(float volume){
		if(mediaPlayer != null){
			mediaPlayer.setVolume(volume, volume);
		}
	}
	public void cxStopMusic(){
		if(mediaPlayer != null){
			mediaPlayer.stop();
		}
	}
	public void cxPauseMusic(){
		if(mediaPlayer != null){
			mediaPlayer.pause();
		}
	}
	public void cxResumeMusic(){
		if(mediaPlayer != null){
			mediaPlayer.start();
		}
	}
	//当收到json数据时主线程处理
	public void OnRecvJson(String json){
		
	}
	//退出程序
	public void EngineTerminate(){
		this.runOnUiThread(new Runnable(){
			public void run(){
				EngineActivity.this.finish();
			}
		});
	}
	//获取UUID
    public String EngineUUID(){
    	UUID uuid = UUID.randomUUID();
    	return uuid.toString();
    }
    //获取默认语言路径
    public String EngineDefaultLocalized(){
    	String ret = null;
    	try {
			String[] files = this.getAssets().list("strings");
			if(files.length >= 1){
				ret = "strings/"+files[0];
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
    	return ret;
    }
    private byte[] packInt(int v) {
		byte[] wb = new byte[4];
		wb[0] = (byte) (v & 0xFF);
		wb[1] = (byte) ((v >> 8) & 0xFF);
		wb[2] = (byte) ((v >> 16) & 0xFF);
		wb[3] = (byte) ((v >> 24) & 0xFF);
		return wb;
	}
	private byte[] getPixels(Bitmap pBitmap) {
		int width = pBitmap.getWidth();
		int height = pBitmap.getHeight();
		int isize = width * height * 4;
		byte[] wb = packInt(width);
		byte[] hb = packInt(height);
		byte[] pixels = new byte[isize + 8];
		ByteBuffer buf = ByteBuffer.wrap(pixels);
		buf.order(ByteOrder.nativeOrder());
		pBitmap.copyPixelsToBuffer(buf);
		System.arraycopy(wb, 0, pixels, isize, 4);
		System.arraycopy(hb, 0, pixels, isize + 4, 4);
		return pixels;
	}
	public synchronized Typeface getTypeface(String pAssetName) {
		if (!sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/")){
				typeface = Typeface.createFromFile(pAssetName);
			}else{
				typeface = Typeface.createFromAsset(this.getAssets(), pAssetName);
			}
			sTypefaceCache.put(pAssetName, typeface);
		}
		return sTypefaceCache.get(pAssetName);
	}
	private TextPaint newPaint(String fontName,int fontSize,boolean bold) {
		TextPaint paint = new TextPaint();
		paint.setTextSize(fontSize); 
		paint.setAntiAlias(true);
		if (fontName != null && fontName.endsWith(".ttf")) {
			try {
				Typeface typeFace = getTypeface(fontName);
				paint.setTypeface(typeFace);
			} catch (Exception e) {
				paint.setTypeface(Typeface.create(fontName, bold?Typeface.BOLD:Typeface.NORMAL));
			}
		} else if(fontName != null) {
			paint.setTypeface(Typeface.create(fontName, bold?Typeface.BOLD:Typeface.NORMAL));
		} else {
			paint.setTypeface(bold?Typeface.DEFAULT_BOLD:Typeface.DEFAULT);
		}
		paint.setFlags(Paint.ANTI_ALIAS_FLAG);
		paint.setFakeBoldText(bold);
		return paint;
	}
	private String numberFormat(String str,int format){
		//Decimal
		if(format == 1){
			int v = Integer.parseInt(str);
			DecimalFormat fmt = new DecimalFormat("###,###");
			return fmt.format(v);
		}
		//Currency
		if(format == 2){
			int v = Integer.parseInt(str);
			NumberFormat fmt = NumberFormat.getCurrencyInstance();
			return fmt.format(v);
		}
		//
		return str;
	}
	public byte[] createTextBitmap(String pString, 
			String pFontName, boolean bold,int fontSize,
			int align,int format,
			float fw,
			float r,float g,float b,float a,
			float sw,float sr,float sg,float sb,float sa,float sx,float sy) {
		if(pString.length() == 0){
			return null;
		}
		if(format != 0){
			pString = numberFormat(pString,format);
		}
		Layout.Alignment textAlign = Layout.Alignment.ALIGN_NORMAL;
		if(align == 1){
			textAlign = Layout.Alignment.ALIGN_NORMAL;
		}else if(align == 2){
			textAlign = Layout.Alignment.ALIGN_CENTER;
		}else{
			textAlign = Layout.Alignment.ALIGN_OPPOSITE;
		}
		//normal
		TextPaint paint = newPaint(pFontName, fontSize, bold);
		paint.setStyle(Style.FILL);
		paint.setARGB((int)(a*255),(int)(r*255), (int)(g*255), (int)(b*255));
		//stroke
		TextPaint stroke = null;
		if(sw > 0){
			stroke = newPaint(pFontName, fontSize, bold);
			stroke.setStrokeWidth(sw);
			stroke.setStrokeJoin(Paint.Join.ROUND);
			stroke.setStrokeCap(Paint.Cap.ROUND);
			stroke.setStyle(Style.FILL_AND_STROKE);
			stroke.setARGB((int)(sa*255),(int)(sr*255), (int)(sg*255), (int)(sb*255));
		}
		int height = 0;
		int width = 0;
		int fixwidth = 0;
		if(paint != null){
			int iw = (int)Math.ceil(paint.measureText(pString));
			if(iw > fixwidth)fixwidth = iw;
		}
		if(stroke != null){
			int iw = (int)Math.ceil(stroke.measureText(pString));
			if(iw > fixwidth)fixwidth = iw;
		}
		if(fw > 0){
			fixwidth = (int)Math.ceil(fw);
		}
		StaticLayout strokesl = null;
		if(stroke != null){
			strokesl = new StaticLayout(pString, stroke, fixwidth, textAlign, 1.0f, 0.0f, false);
			if(strokesl.getWidth() > width)width = strokesl.getWidth();
			if(strokesl.getHeight() > height)height = strokesl.getHeight();
		}
		StaticLayout paintsl = null;
		if(paint != null){
			paintsl = new StaticLayout(pString, paint, fixwidth, textAlign, 1.0f, 0.0f, false);
			if(paintsl.getWidth() > width)width = paintsl.getWidth();
			if(paintsl.getHeight() > height)height = paintsl.getHeight();
		}
		Bitmap bitmap = Bitmap.createBitmap(width,height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		if(strokesl != null){
			canvas.translate(sx, sy);
			strokesl.draw(canvas);
		}
		if(paintsl != null){
			canvas.translate(-sx, -sy);
			paintsl.draw(canvas);
		}
		return getPixels(bitmap);
	}
}
