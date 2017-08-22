/**
opencv画矩形示例：
在一张图片中，根据坐标、宽高画矩形
*/
#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include<opencv/CvxText.h>
using namespace cv;

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        printf("usage: %s <Image_Path>\n", argv[0]);
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
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

    //////////////////////////////
    Mat imaged;
    // 拷贝一份，不影响原图
    image.copyTo(imaged);

    // 方法1：指定坐标，宽高
    Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = 100;
    rect.height = 100;
    rectangle(imaged, rect, Scalar(0, 255, 255), 2);

    // 方法2：指定中心坐标，宽高
    Point2f points[4];
    RotatedRect rrect;
    rrect.center.x = 100;
    rrect.center.y = 100;
    rrect.size.width = 20;
    rrect.size.height = 20;

    rrect.points(points);
    for (int j = 0; j < 4; j++)
    {
        line(imaged, points[j], points[(j+1)%4], Scalar(0, 0, 255), 2);
    }


    //CvxText text("simsun.ttc");

    putText(imaged, "This is text 中文", cv::Point(0, 150), 2, 1, cvScalar(0, 0, 255));

    imwrite("foo_draw.jpg", imaged);

    return 0;
}