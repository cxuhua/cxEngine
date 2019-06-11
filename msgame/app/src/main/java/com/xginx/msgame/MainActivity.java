package com.xginx.msgame;

import android.Manifest;
import android.content.pm.PackageManager;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.xginx.cxengine.EngineActivity;

public class MainActivity extends EngineActivity {
    static {
        System.loadLibrary("game");
    }
    //当申请到权限时
    @Override
    public void onRequestPermissionsResult(int code, String permissions[], @NonNull int[] grant) {
        super.onRequestPermissionsResult(code,permissions,grant);
    }

    //动态申请权限
    private void requestPermissions(){
        int hasw = ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO);
        if(hasw == PackageManager.PERMISSION_GRANTED){
            return;
        }
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, 10001);
    }

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        requestPermissions();
    }
}
