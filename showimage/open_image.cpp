/**
* 使用C++版本接口注意事项：
* 1、使用cv命名空间
* 2、包含的库必须分清Debug和Release版本，
*    即：Debug工程必须带“d”的库，如opencv_core244d.lib
*        Release工程必须是不带“d”的库，如opencv_core244.lib
*
*/
#include <stdio.h>

#include "opencv2/opencv.hpp"


using namespace cv;

int openImage(char* filename, Mat& src, int gray)
{
    //从文件中读入图像
    src = imread(filename);

    //如果读入图像失败
    if(src.empty())
    {
        fprintf(stderr, "Can not load image %s\n", filename);
        return -1;
    }

    if (gray == 1)
        cvtColor(src, src, CV_BGR2GRAY);

    return 0;
}

int openImageTest(char* filename)
{
    Mat img;

    openImage(filename, img, 0);

    //显示图像
    imshow("image", img);

    //此函数等待按键，按键盘任意键就返回
    waitKey(0);

    return 0;
}

// 旧版本函数
int openImage_old(char* filename)
{
    const char *pstrWindowsTitle = "pic";

    //从文件中读取图像
    IplImage *pImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
    if (pImage == NULL)
    {
        printf("Open file %s error.\n", filename);
        return -1;
    }

    //创建窗口
    cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);

    //在指定窗口中显示图像
    cvShowImage(pstrWindowsTitle, pImage);

    //等待按键事件
    cvWaitKey();

    cvDestroyWindow(pstrWindowsTitle);
    cvReleaseImage(&pImage);
    return 0;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("usage: %s <image file>.\n", argv[0]);
        return 0;
    }
    openImageTest(argv[1]);
    return 0;
}