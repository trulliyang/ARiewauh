package com.appmagics.demo;

import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

/**
 * Created by menghai on 2017/3/1.
 */

public class MaCamera {

    /**
     * 最大宽高比差
     */
    private static final double MAX_ASPECT_DISTORTION = 0.15;

    /**
     * 最小预览界面的分辨率
     */
    private static final int MIN_PREVIEW_PIXELS = 480 * 320;

    public static final int DEFAULT_PREVIEW_PIXELS = 1280 * 720;

    private Camera camera;

    private SurfaceTexture cameraSurfaceTexture;

    private int cameraTextureId;

    private int width = 1280;

    private int height = 720;
    
    private int format = ImageFormat.NV21;

    private boolean isPreviewing = false;

    private TakePictureInterface mTakePictureInterface;

    public void open() {
        int backCamera = getBackCamera();
        camera = Camera.open(backCamera);
    }

    public int getCameraWidth() {
        return width;
    }
    public int getCameraHeight() {
        return height;
    }
    public int getCameraFormat() {
        return format;
    }

    /**
     * 使用TextureView预览Camera
     *
     * @param surface     SurfaceTexture
     */
    void startPreview(SurfaceTexture surface) {
        if (camera != null) {
            try {
                camera.setPreviewTexture(surface);
            } catch (IOException e) {
                e.printStackTrace();
            }

        }

    }

    public void start(SurfaceTexture surface) throws IOException {
        Camera.Parameters parameters = camera.getParameters();
        final List<String> focusModes = parameters.getSupportedFocusModes();
        // 连续聚焦
        if (focusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        }
        // 自动聚焦
        if (focusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        }
        parameters.setPreviewFormat(format);

        Camera.Size CameraSize = findBestPreviewResolution(parameters);
        if(CameraSize != null) {
            parameters.setPreviewSize(CameraSize.width, CameraSize.height); //预览
            parameters.setPictureSize(CameraSize.width, CameraSize.height); //拍照
        }

        camera.setParameters(parameters);
        cameraSurfaceTexture=surface;
        isPreviewing = true;
        camera.setPreviewTexture(cameraSurfaceTexture);
        camera.startPreview();
    }

    /**
     * 找出最适合的预览界面分辨率
     */
    private Camera.Size findBestPreviewResolution(Camera.Parameters mParams) {
        if (mParams != null) {
            Camera.Size defaultPreviewResolution = mParams.getPreviewSize();

            List<Camera.Size> rawSupportedSizes = mParams.getSupportedPreviewSizes();
            if (rawSupportedSizes == null) {
                return defaultPreviewResolution;
            }

            // 按照分辨率从大到小排序
            List<Camera.Size> supportedPreviewResolutions = new ArrayList<Camera.Size>(rawSupportedSizes);
            Collections.sort(supportedPreviewResolutions, new Comparator<Camera.Size>() {
                @Override
                public int compare(Camera.Size a, Camera.Size b) {
                    int aPixels = a.height * a.width;
                    int bPixels = b.height * b.width;
                    if (bPixels < aPixels) {
                        return -1;
                    }
                    if (bPixels > aPixels) {
                        return 1;
                    }
                    return 0;
                }
            });

            // 移除不符合条件的分辨率
            double screenAspectRatio = (double) 720
                    / (double) 1280;
            Iterator<Camera.Size> it = supportedPreviewResolutions.iterator();

            while (it.hasNext()) {
                Camera.Size supportedPreviewResolution = it.next();
                int width = supportedPreviewResolution.width;
                int height = supportedPreviewResolution.height;

                if (width * height == DEFAULT_PREVIEW_PIXELS) { //有默认的
                    return supportedPreviewResolution;
                }
            }

            while (it.hasNext()) {
                Camera.Size supportedPreviewResolution = it.next();
                int width = supportedPreviewResolution.width;
                int height = supportedPreviewResolution.height;
                // 移除低于下限的分辨率，尽可能取高分辨率
                if (width * height < MIN_PREVIEW_PIXELS) {
                    it.remove();
                    continue;
                }

                // 在camera分辨率与屏幕分辨率宽高比不相等的情况下，找出差距最小的一组分辨率
                // 由于camera的分辨率是width>height，我们设置的portrait模式中，width<height
                // 因此这里要先交换然preview宽高比后在比较
                boolean isCandidatePortrait = width > height;
                int maybeFlippedWidth = isCandidatePortrait ? height : width;
                int maybeFlippedHeight = isCandidatePortrait ? width : height;
                double aspectRatio = (double) maybeFlippedWidth / (double) maybeFlippedHeight;
                double distortion = Math.abs(aspectRatio - screenAspectRatio);
                if (distortion > MAX_ASPECT_DISTORTION) {
                    it.remove();
                    continue;
                }

                // 找到与屏幕分辨率完全匹配的预览界面分辨率直接返回
                if (maybeFlippedWidth == 720
                        && maybeFlippedHeight == 1280) {
                    return supportedPreviewResolution;
                }
            }


            // 如果没有找到合适的，并且还有候选的像素，则设置其中最大比例的，对于配置比较低的机器不太合适
            if (!supportedPreviewResolutions.isEmpty()) {
                return supportedPreviewResolutions.get(0);
            }

            // 没有找到合适的，就返回默认的
            return defaultPreviewResolution;
        }
        return null;
    }

    public void stop(){
        if(camera != null){
            isPreviewing = false;
            camera.stopPreview();
        }
    }

    public void updateTexture() {
        if (cameraSurfaceTexture != null) {
            cameraSurfaceTexture.updateTexImage();
        }
    }

    public void release() {
        if (cameraSurfaceTexture != null) {
            cameraSurfaceTexture.release();
            cameraSurfaceTexture = null;

        }
        if (camera != null) {
            isPreviewing = false;
            camera.stopPreview();
            camera.release();
        }
    }

    public void registerCameraPreviewListener(Camera.PreviewCallback listener) {
        if (camera != null && listener != null) {
            Camera.Size size = camera.getParameters().getPreviewSize();
            camera.setPreviewCallbackWithBuffer(listener);
            camera.addCallbackBuffer(new byte[size.width * size.height * ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat()) / 8]);

        }
    }

    public void unRegisterCameraPreviewListener() {
        if (camera != null) {
            camera.setPreviewCallback(null);

            camera.addCallbackBuffer(null);
        }
    }

    public void setTakePicture(TakePictureInterface takePicture) {
        this.mTakePictureInterface = takePicture;
    }

    public void unTakePicture() {
        this.mTakePictureInterface = null;
    }

    private int getBackCamera() {
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        final int numberOfCameras = Camera.getNumberOfCameras();

        for (int i = 0; i < numberOfCameras; ++i) {
            Camera.getCameraInfo(i, cameraInfo);
            if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                return i;
            }
        }
        return Camera.CameraInfo.CAMERA_FACING_FRONT;
    }


    public void takePicture() {
        if (isPreviewing && (camera != null)) {
            camera.takePicture(null, null, mJpegPictureCallback);
        }
    }

    //对jpeg图像数据的回调,最重要的一个回调
    private Camera.PictureCallback mJpegPictureCallback = new Camera.PictureCallback() {
        public void onPictureTaken(byte[] data, Camera camera) {

            if(mTakePictureInterface != null) {
                mTakePictureInterface.onTakePicture(data);
            }
        }
    };

    public interface TakePictureInterface {
        void onTakePicture(byte[] data);
    }
}
