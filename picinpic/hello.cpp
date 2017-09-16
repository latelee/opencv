/**
opencv大图上叠加一张小图
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

int onepic(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage: %s <big picture> <small pciture>\n", argv[0]);
        return -1;
    }
    
    Mat image;
    image = imread(argv[1], 1);
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }
    printf("big picture res: %dx%d\n", image.cols, image.rows);
    
    Mat smallImg = imread(argv[2], 1);
    if (!smallImg.data)
    {
        printf("No smallImg data \n");
        return -1;
    }
    
    printf("small picture res: %dx%d\n", smallImg.cols, smallImg.rows);

    cv::Mat imageROI;
    // 在大图中扣出小图的位置(前2个参数)和分辨率(后2个参数) －－在左上角叠加
    imageROI = image(cv::Rect(0, 0, smallImg.cols, smallImg.rows));
    // 不透明，小图覆盖式
    cv::addWeighted(imageROI, 0, smallImg, 1, 0, imageROI);
    //cv::addWeighted(imageROI, 0.2, smallImg, 0.4, 0, imageROI);
    imwrite("foo.jpg", image);

    return 0;  
}

int morepic(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("usage: %s <big picture> \n", argv[0]);
        return -1;
    }
    
    Mat image;
    image = imread(argv[1], 1);
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }
    printf("big picture res: %dx%d\n", image.cols, image.rows);
    
    for (int i = 0; i < 7; i++)
    {
        char filename[64] = {0};
        sprintf(filename, "char/%02d.jpg", i);
        Mat smallImg = imread(filename);
        if (!smallImg.data)
        {
            printf("No smallImg data \n");
            return -1;
        }
        
        printf("small picture res: %dx%d\n", smallImg.cols, smallImg.rows);

        cv::Mat resizeImg;
        // 车牌小图放大约整个图像高的0.05
        float scale = image.rows * 0.05 / (float)smallImg.rows;
        resize(smallImg, resizeImg, Size(), scale, scale, CV_INTER_AREA);

        cv::Mat imageROI;
        // 在大图中扣出小图的位置(前2个参数)和分辨率(后2个参数) －－在左上角叠加
        imageROI = image(cv::Rect(i*(resizeImg.cols+5), 0, resizeImg.cols, resizeImg.rows));
        // 不透明，小图覆盖式
        cv::addWeighted(imageROI, 0, resizeImg, 1, 0, imageROI);
        //cv::addWeighted(imageROI, 0.2, smallImg, 0.4, 0, imageROI);
    }
    
    imwrite("foo.jpg", image);

    return 0;  
}

int main(int argc, char* argv[])
{
    //onepic(argc, argv);
    morepic(argc, argv);
}