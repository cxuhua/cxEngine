package com.xginx.cxengine;

import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.View;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.HashMap;
import java.util.UUID;

public class EngineActivity extends NativeActivity {

	static {
		System.loadLibrary("cxengine");
	}
	//安装更新文件
    public void installApp(final String file){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Intent intent = new Intent();
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                intent.setAction(Intent.ACTION_VIEW);
                intent.setDataAndType(Uri.parse(file), "application/vnd.android.package-archive");
                getApplicationContext().startActivity(intent);
            }
        });
    }
	//显示虚拟按键
	public void showVirtualKey(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < 19) {
                    View v = getWindow().getDecorView();
                    v.setSystemUiVisibility(View.VISIBLE);
                } else if (Build.VERSION.SDK_INT >= 19) {
                    View decorView = getWindow().getDecorView();
                    int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
                    decorView.setSystemUiVisibility(uiOptions);
                }
            }
        });
	}

	//隐藏虚拟按键
	public void hideVirtualKey(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < 19) {
                    View v = getWindow().getDecorView();
                    v.setSystemUiVisibility(View.GONE);
                } else if (Build.VERSION.SDK_INT >= 19) {
                    View decorView = getWindow().getDecorView();
                    int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_FULLSCREEN;
                    decorView.setSystemUiVisibility(uiOptions);
                }
            }
        });
    }

    private HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();

	//获取UUID
	public String NewUUID() {
		UUID uuid = UUID.randomUUID();
		return uuid.toString();
	}

	//获取versioncode
	public int getVersionCode() {
		try {
			Context ctx = getApplicationContext();
			PackageInfo packageInfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(),0);
			return packageInfo.versionCode;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}
		return 0;
	}

    public String getVersionName() {
        try {
            Context ctx = getApplicationContext();
            PackageInfo packageInfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(),0);
            return packageInfo.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }

	//退出程序
	public void EngineTerminate() {
		this.runOnUiThread(new Runnable() {
			public void run() {
				EngineActivity.this.finish();
			}
		});
	}
	//
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
		int size = width * height * 4;
		byte[] wb = packInt(width);
		byte[] hb = packInt(height);
		byte[] pixels = new byte[size + 8];
		ByteBuffer buf = ByteBuffer.wrap(pixels);
		buf.order(ByteOrder.nativeOrder());
		pBitmap.copyPixelsToBuffer(buf);
		System.arraycopy(wb, 0, pixels, size, 4);
		System.arraycopy(hb, 0, pixels, size + 4, 4);
		return pixels;
	}

	public synchronized Typeface getTypeface(String pAssetName) {
		if (!sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/")) {
				typeface = Typeface.createFromFile(pAssetName);
			} else {
				typeface = Typeface.createFromAsset(this.getAssets(), pAssetName);
			}
			sTypefaceCache.put(pAssetName, typeface);
		}
		return sTypefaceCache.get(pAssetName);
	}

	private TextPaint newPaint(String fontName, int fontSize, boolean bold) {
		TextPaint paint = new TextPaint();
		paint.setTextSize(fontSize);
		paint.setAntiAlias(true);
		if (fontName != null && fontName.endsWith(".ttf")) {
			try {
				Typeface typeFace = getTypeface(fontName);
				paint.setTypeface(typeFace);
			} catch (Exception e) {
				paint.setTypeface(Typeface.create(fontName, bold ? Typeface.BOLD : Typeface.NORMAL));
			}
		} else if (fontName != null) {
			paint.setTypeface(Typeface.create(fontName, bold ? Typeface.BOLD : Typeface.NORMAL));
		} else {
			paint.setTypeface(bold ? Typeface.DEFAULT_BOLD : Typeface.DEFAULT);
		}
		paint.setFlags(Paint.ANTI_ALIAS_FLAG);
		paint.setFakeBoldText(bold);
		return paint;
	}

	private String numberFormat(String str, int format) {
		//Decimal
		if (format == 1) {
			int v = Integer.parseInt(str);
			DecimalFormat fmt = new DecimalFormat("###,###");
			return fmt.format(v);
		}
		//Currency
		if (format == 2) {
			int v = Integer.parseInt(str);
			NumberFormat fmt = NumberFormat.getCurrencyInstance();
			return fmt.format(v);
		}
		return str;
	}

	public byte[] CreateTextBitmap(String pString,
								   String pFontName, boolean bold, int fontSize,
								   int align, int format,
								   float fw,
								   float r, float g, float b, float a,
								   float sw, float sr, float sg, float sb, float sa, float sx, float sy) {
		if (pString.length() == 0) {
			return null;
		}
		if (format != 0) {
			pString = numberFormat(pString, format);
		}
		Layout.Alignment textAlign = Layout.Alignment.ALIGN_NORMAL;
		if (align == 1) {
			textAlign = Layout.Alignment.ALIGN_NORMAL;
		} else if (align == 2) {
			textAlign = Layout.Alignment.ALIGN_CENTER;
		} else {
			textAlign = Layout.Alignment.ALIGN_OPPOSITE;
		}
		//normal
		TextPaint paint = newPaint(pFontName, fontSize, bold);
		paint.setStyle(Style.FILL);
		paint.setARGB((int) (a * 255), (int) (r * 255), (int) (g * 255), (int) (b * 255));
		//stroke
		TextPaint stroke = null;
		if (sw > 0) {
			stroke = newPaint(pFontName, fontSize, bold);
			stroke.setStrokeWidth(sw);
			stroke.setStrokeJoin(Paint.Join.ROUND);
			stroke.setStrokeCap(Paint.Cap.ROUND);
			stroke.setStyle(Style.FILL_AND_STROKE);
			stroke.setARGB((int) (sa * 255), (int) (sr * 255), (int) (sg * 255), (int) (sb * 255));
		}
		int height = 0;
		int width = 0;
		int fwidth = 0;
		if (paint != null) {
			int iw = (int) Math.ceil(paint.measureText(pString));
			if (iw > fwidth) fwidth = iw;
		}
		if (stroke != null) {
			int iw = (int) Math.ceil(stroke.measureText(pString));
			if (iw > fwidth) fwidth = iw;
		}
		if (fw > 0) {
			fwidth = (int) Math.ceil(fw);
		}
		StaticLayout strokesl = null;
		if (stroke != null) {
			strokesl = new StaticLayout(pString, stroke, fwidth, textAlign, 1.0f, 0.0f, false);
			if (strokesl.getWidth() > width) width = strokesl.getWidth();
			if (strokesl.getHeight() > height) height = strokesl.getHeight();
		}
		StaticLayout paintsl = null;
		if (paint != null) {
			paintsl = new StaticLayout(pString, paint, fwidth, textAlign, 1.0f, 0.0f, false);
			if (paintsl.getWidth() > width) width = paintsl.getWidth();
			if (paintsl.getHeight() > height) height = paintsl.getHeight();
		}
		Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		if (strokesl != null) {
			canvas.translate(sx, sy);
			strokesl.draw(canvas);
		}
		if (paintsl != null) {
			canvas.translate(-sx, -sy);
			paintsl.draw(canvas);
		}
		return getPixels(bitmap);
	}
}
