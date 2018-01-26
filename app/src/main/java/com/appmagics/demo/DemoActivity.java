package com.appmagics.demo;

import android.app.Activity;
import android.content.IntentFilter;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.os.Bundle;
import android.system.Os;
import android.view.Surface;
import android.view.TextureView;

import com.huawei.ArCameraAccessories.ArCameraAccessories;

import java.io.IOException;

import static com.huawei.ArCameraAccessories.ArCameraAccessories.ACTION_USB_PERMISSION;


/**
 * Created by menghai on 2017/4/1.
 */

public class DemoActivity extends Activity {

private TextureView textureView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    
        registerReceiver(ArCameraAccessories.mUsbReceiver, new IntentFilter(ACTION_USB_PERMISSION));
        ArCameraAccessories.init(this, this);
        ArCameraAccessories.openUSB();
        
        setContentView(R.layout.content_main);
        textureView= (TextureView) findViewById(R.id.textID);
        textureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
                Engine engine=new Engine();
                engine.setSurface(new Surface(surface));
                engine.init();
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
                return false;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface) {

            }
        });


    }
}
