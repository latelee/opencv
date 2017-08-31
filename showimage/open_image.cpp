/**
* ʹ��C++�汾�ӿ�ע�����
* 1��ʹ��cv�����ռ�
* 2�������Ŀ�������Debug��Release�汾��
*    ����Debug���̱������d���Ŀ⣬��opencv_core244d.lib
*        Release���̱����ǲ�����d���Ŀ⣬��opencv_core244.lib
*
*/
#include <stdio.h>

#include "opencv2/opencv.hpp"


using namespace cv;

int openImage(char* filename, Mat& src, int gray)
{
    //���ļ��ж���ͼ��
    src = imread(filename);

    //�������ͼ��ʧ��
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

    //��ʾͼ��
    imshow("image", img);

    //�˺����ȴ�������������������ͷ���
    waitKey(0);

    return 0;
}

// �ɰ汾����
int openImage_old(char* filename)
{
    const char *pstrWindowsTitle = "pic";

    //���ļ��ж�ȡͼ��
    IplImage *pImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
    if (pImage == NULL)
    {
        printf("Open file %s error.\n", filename);
        return -1;
    }

    //��������
    cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);

    //��ָ����������ʾͼ��
    cvShowImage(pstrWindowsTitle, pImage);

    //�ȴ������¼�
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