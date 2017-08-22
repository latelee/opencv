/**
简单的opencv测试示例：
读取一张图片，转换成灰度图，然后保存。
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        printf("usage: %s <Image_Path>\n", argv[0]);
        return -1;
    }
    Mat image;
    image = imread(argv[1]);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    printf("res: %dx%d\n", image.cols, image.rows);
    // 没有界面的话，下面几句不需要
    //namedWindow("Display Image", WINDOW_AUTOSIZE );  
    //imshow("Display Image", image);  
    //waitKey(0);  
    
    // 转为灰度图
    cv::Mat grayImg;
    cv::cvtColor(image, grayImg, CV_BGR2GRAY);
    // 保存
    imwrite("foo.jpg", grayImg);
    
    return 0;  
}