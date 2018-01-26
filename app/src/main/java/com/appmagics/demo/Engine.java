package com.appmagics.demo;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by menghai on 2017/4/1.
 */

public class Engine implements Runnable {

    static{
        System.loadLibrary("engine");
//        System.loadLibrary("FacialLandmarkDetector");
    }

    private SurfaceTexture surfaceTexture;
    private  Surface surface;

    private ArrayList<Runnable> queneEvent = new ArrayList<>();
    private Thread engineThread = null;
    private boolean isStop = false;

    public Engine() {
    }

    public void setSurface( Surface surface){

        this.surface=surface;
    }

    public void setSurfaceTexture(SurfaceTexture surfaceTexture) {
        this.surfaceTexture = surfaceTexture;
    }

    public void init() {

        if (engineThread == null) {
            isStop = false;
            engineThread = new Thread(this);
            engineThread.start();
        }
    }

    public void pushQueneEvent(Runnable run) {
        synchronized (this) {
            queneEvent.add(run);
        }
    }

    private int mCameraWidth, mCameraHeight;


    private final Object mLock = new Object();
    private ByteBuffer mCameraDataBuffer;
    long engineAddress = 0;
    private byte[] fakeData;// = new byte[800*1280*3/2];

    @Override
    public void run() {
        long address= nativeCreateShareEGLContext(720, 1280, surface);
        nativeMakeCurrent(address);
        long engine=nativeCreateEngine();
        engineAddress = engine;
        int  cameraTextureId = MyGLUtils.genTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES);
        surfaceTexture= new SurfaceTexture(cameraTextureId);
        
        
//                MaCamera maCamera=new MaCamera();
//                maCamera.open();
//                try {
//
//                    maCamera.start(surfaceTexture);
//                    maCamera.registerCameraPreviewListener(new Camera.PreviewCallback() {
//                        @Override
//                        public void onPreviewFrame(final byte[] data, final Camera camera) {
////                            synchronized (mLock) {
////                                time = System.currentTimeMillis();
//                                try {
//                                    Camera.Parameters param = camera.getParameters();
//                                    Camera.Size t_size = param.getPreviewSize();
//                                    int cameraWidth = t_size.width;
//                                    int cameraHeight = t_size.height;
//                                    Log.e("shiyang", "shiyang camera w="+cameraWidth+",h="+cameraHeight);
//
//                                    mCameraWidth = cameraWidth;
//                                    mCameraHeight = cameraHeight;
//                                    int size = cameraWidth * cameraHeight * 3 / 2;
//
//                                    if (mCameraDataBuffer == null) {
//                                        mCameraDataBuffer = ByteBuffer.allocateDirect(size);//分配空间
//                                    }
//                                    mCameraDataBuffer.rewind();
//                                    mCameraDataBuffer.put(data);
//                                    nativeSendCameraData(mCameraDataBuffer, engineAddress);
////                                    if (null != mFLJNI) {
////                                        mFLJNI.FLSetImageData(mCameraDataBuffer, mCameraWidth, mCameraHeight, size, 1);
////                                    }
//                                } catch (Exception e) {
//                                    e.printStackTrace();
//                                }
//
////                                FLGLSurfaceView.this.requestRender();
////                                try {
////                                    mLock.wait();
////                                } catch (InterruptedException e) {
////                                    e.printStackTrace();
////                                }
////                            }
//
//                            camera.addCallbackBuffer(data);
//
//                        }
//                    });
//
//
//
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
        while (!isStop) {
            if (!queneEvent.isEmpty()) {
                Runnable event;
                synchronized (this) {
                    event = queneEvent.remove(0);
                }
                event.run();
            }
            if(surfaceTexture!=null) {
                surfaceTexture.updateTexImage();
            }
//            if (fakeData == null) {
//                fakeData = new byte[800*1280*3/2];
//                for (int i=0; i<800*1280; i++) {
//                    fakeData[i] = (byte)255;
//                }
//                for (int i=800*1280; i<800*1280*3/2; i++) {
//                    fakeData[i] = (byte)127;
//                }
//            }
//            if (mCameraDataBuffer == null) {
//                mCameraDataBuffer = ByteBuffer.allocateDirect(800*1280*3/2);//分配空间
//            }
//            mCameraDataBuffer.rewind();
//            mCameraDataBuffer.put(fakeData);
//            nativeSendCameraData(mCameraDataBuffer , address);
            nativeGetCameraData(engine);
            nativeDraw(cameraTextureId, engine);
            nativeWrapBuffer(address);
//            mLock.notifyAll();
        }
    }

    public void destory() {
        isStop = true;
        engineThread.interrupt();
    }

    public native long nativeCreateEngine();
    
    public native void nativeGetCameraData(long address);
    
    public native void nativeSendCameraData(ByteBuffer data ,long address);

    public native void nativeDraw(int id ,long address);

    public native long nativeCreateShareEGLContext(int width, int height,Surface surfaceTexture);

    public native void nativeRelease(long nativeAddress);

    public native void nativeMakeCurrent(long nativeAddress);

    public native void nativeWrapBuffer(long nativeAddress);



}
