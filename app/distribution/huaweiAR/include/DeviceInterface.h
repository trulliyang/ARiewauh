#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

#define ONI_MAX_STR 50

/** The source of the stream */
typedef enum
{
    SENSOR_COLOR = 1,
    SENSOR_DEPTH = 2
} StreamType;

/** All available formats of the output of a stream  */
typedef enum{
    //Depth
    PIXEL_FORMAT_DEPTH_1_MM = 100,
    PIXEL_FORMAT_DEPTH_1_M = 101,
    //Color
    PIXEL_FORMAT_YUV420 = 200,
} PixelFormat;

/** Description of the output: format and resolution  */
typedef struct
{
    PixelFormat pixelFormat;
    int resolutionX;
    int resolutionY;
    int fps;
} VideoMode;

/** All information of the current frame  */
typedef struct
{
    int dataSize;               //上层传入大小
    void * data;                //底层负责写数据

    unsigned long timestamp;    //低层传出大小
    int frameIndex;             //低层传出大小

    int width;                  //低层传出大小
    int height;                 //低层传出大小
    int stride;                 //低层传出大小
} VideoFrame;

/** Basic intrinsic of sensor */
typedef struct
{
    float fx;//
    float fy;//(fx, fy): focal length
    float cx;//
    float cy;//(cx, cy): principal point
    float width;
    float height;
    float distort[5];//distortion of camera
} CameraIntrInfo;

/** Basic extrinsic of two sensors */
typedef struct
{
    float baseLine;     //baseline between ir and projector
    float z0;           //depth of structure light reference image
    float dispShift;
    float H[3][3];

    float rotate_rgb2ir[3][3];  //rotation matrix between ir and rgb camera
    float trans_rgb2ir[3];      //translation matrix between ir and rgb camera
    float extrinsic_rgb2ir[4][4];
} CameraExtrInfo;

/** Basic description of a device */
typedef struct
{
    char uri[ONI_MAX_STR];
    char vendor[ONI_MAX_STR];
    char name[ONI_MAX_STR];
    unsigned short usbVendorId;
    unsigned short usbProductId;
} DeviceInfo;

//----------------------------------------------------
/** interfaces */

class Device{
public:

    Device();
    ~Device();

    int open(const char* uri);
    void close();

    const DeviceInfo* getDeviceInfo();
    const CameraIntrInfo* getSensorInfo(StreamType streamType);
    const CameraExtrInfo* getCameraExtInfo();

    int setRGBAEType(bool manually);
    int setRGBexp(unsigned int exp);    //if set exp, AE type will be set manually directly. [1 ~ 30000]ms
    int setRGBgain(unsigned int gain);  //if set gain, AE type will be set manually directly. [1 ~ 16]

    bool isValid() const;

private:
    StreamType mStreamType;
};

class VideoStream
{
public:
    /** Default constructor */
    VideoStream();

    /** Default Destructor */
    ~VideoStream();

    /** @returns Status code indicating success or failure for this operation */
    int create(Device device, StreamType streamType);

    /** Destroy this stream.  */
    void destroy();

    /** Starts data generation from this video stream */
    int start();

    /** Stops data generation from this video stream */
    void stop();

    /** @returns true if this object has been previously initialized, false otherwise. */
    bool isValid();

    /** @returns Current video mode information for this video stream.
        [videoMode]
        0:YUV420, 1280x800 15 fps; Depth 800x1280 15 fps;
        1:YUV420, 1280x800 30 fps; Depth 800x1280 15 fps;
        2:YUV420, 1280x800 15 fps; Depth 400*640  15 fps;
     */
    const int getVideoMode();

    /** 
    @Param [in] videoMode Desired new video mode for this stream.
    @returns Status code indicating success or failure of this operation.
    [videoMode]
        0:YUV420, 1280x800 15 fps; Depth 800x1280 15 fps;
        1:YUV420, 1280x800 30 fps; Depth 800x1280 15 fps;
        2:YUV420, 1280x800 15 fps; Depth 400*640  15 fps;
    */
    int setVideoMode(int videoMode);

    /** 
    Read the next frame from this video stream, delivered as a @ref VideoFrameRef.
    This is the primary method for manually obtaining frames of video data.
     */
    int readFrame(VideoFrame* pFrame);

private:
    static void* read_depth_frame_function(void* vptr_args);
    static void* read_rgb_frame_function(void* vptr_args);

    StreamType mStreamType;
};

#endif // DEVICE_INTERFACE_H