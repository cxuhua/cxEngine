package cn.rockygame;

import android.util.Log;

import com.cxengine.EngineActivity;

public class TestActivity extends EngineActivity {
	static {
	    System.loadLibrary("cxEngineAndroid");
	}
	
	@Override
	public void OnRecvJson(String json){
		Log.e("json", json);
	}
}
