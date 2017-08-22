/**
opencv大图上叠加一张小图
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage: %s <bit picture> <small pciture>\n", argv[0]);
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