#ifndef IDS_H
#define IDS_H

#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include "opencv2/opencv.hpp"
#include "ueye.h"

#include <pthread.h>

#define IDS_WIDTH 752
#define IDS_HEIGHT 360
#define IMAGE_COUNT 20

class IDS {

    typedef struct _UEYE_IMAGE
    {
        char *pBuf;
        INT nImageID;
        INT nImageSeqNum;
        INT nBufferSize;
    } UEYE_IMAGE, *PUEYE_IMAGE;

    HIDS m_hCamera;
    char *m_pLastBuffer;
    bool algorithm_ready = true;
    double meanfps = 0.0;
    UEYE_IMAGE m_Images[IMAGE_COUNT];

    //Autoparams settings
    double min_exposure = 0.01;
    double max_exposure = 5.0;

    int min_exposure_slider = 1;
    int max_exposure_slider = 100;
    int hysteresis_slider = 1;
    int reference_slider = 2;
    UINT nSizeOfParam;
    AES_CONFIGURATION *pAesConfiguration;
    AES_PEAK_CONFIGURATION *pPeakConfiguration;

    void ProcessFrame ();
    void updateFps (double fps);

    void frameEvent(void);

    bool _AllocImages (int nWidth, int nHeight, int nBitspp);
    INT _GetImageNum (char* pbuf);
    void _FreeImages ();
    void _EmptyImages ();
    INT _GetImageID (char* pbuf);

    pthread_t frame_thread;

    pthread_mutex_t signal_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t frame_signal = PTHREAD_COND_INITIALIZER;
public:

    cv::Mat ids_frame = cv::Mat(IDS_HEIGHT, IDS_WIDTH, CV_8UC3);
    pthread_mutex_t frame_mutex = PTHREAD_MUTEX_INITIALIZER;

    UINT PixelClock = 25;
    int pixelclock_slider = 25;
    double Exposure = 0.5;
    int exposure_slider = 15;
    double FPS = 100, NEWFPS;
    int fps_slider = 100;

    int Master_GAIN_Factor;//=300;
    int Red_GAIN_Factor;//=112;
    int Green_GAIN_Factor;//=100;
    int Blue_GAIN_Factor;//=158;
	
	int nRange[3];
    int sharpness_slider=1;
    UINT Sharpness=1;

    int Gamma =170;

    char* pMem = NULL;
    int memID = 0;

    void setAlgorithmReady();

    double getFPS();
    void get_frame_to(cv::Mat &output);
    HIDS getCameraHID();

    void init();
    void frame_loop();
    void initialize_camera();
    void exit();
    void change_params();
    void setting_auto_params();
    void update_params();
    void create_manual_trackbars(void);
    void create_auto_trackbars();
    void update_autoparams();
};
extern IDS ids;
extern pthread_cond_t algorithm_signal;
extern pthread_mutex_t algorithm_signal_mutex;

extern void sounds_init();
#endif
