/**
opencv画矩形示例：
在一张图片中，根据坐标、宽高画矩形
*/
#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include<opencv/CvxText.h>
using namespace cv;

void draw_rect(Mat& imaged)
{
    // 方法1：指定坐标，宽高
    Rect rect;
    // 左上角坐标
    rect.x = 0;
    rect.y = 0;
    // 宽高
    rect.width = 100;
    rect.height = 100;
    
    printf("draw rectangle...\n");
    rectangle(imaged, rect, Scalar(0, 255, 255), 2);

    // 方法2：指定中心坐标，宽高
    Point2f points[4];
    RotatedRect rrect;
    // 中心坐标
    rrect.center.x = 100;
    rrect.center.y = 100;
    // 矩形宽高
    rrect.size.width = 20;
    rrect.size.height = 20;

    rrect.points(points);

    printf("draw RotatedRect...\n");
    for (int j = 0; j < 4; j++)
    {
        line(imaged, points[j], points[(j+1)%4], Scalar(0, 0, 255), 2);
    }

    //CvxText text("simsun.ttc");

    putText(imaged, "This is text 中文", cv::Point(0, 150), 1, 1, cvScalar(0, 0, 255));
}

void rect2rrect(Rect& rect, RotatedRect& rrect)
{
    // 中心坐标
    rrect.center.x = rect.x + rect.width/2;
    rrect.center.y = rect.y + rect.height/2;
    // 矩形宽高
    rrect.size.width = rect.width;
    rrect.size.height = rect.height;
}

// 从rect转换成RotatedRect
void draw_rect2(Mat& imaged)
{
    Rect rect;
    // 左上角坐标
    rect.x = 100;
    rect.y = 0;
    // 宽高
    rect.width = 100;
    rect.height = 100;
    
    Point2f points[4];
    RotatedRect rrect;
    
    
    rect2rrect(rect, rrect);

    rrect.points(points);

    printf("draw RotatedRect...\n");
    for (int j = 0; j < 4; j++)
    {
        line(imaged, points[j], points[(j+1)%4], Scalar(0, 0, 255), 2);
    }
    
}

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
    printf("got res: %dx%d\n", image.cols, image.rows);
    // 没有界面的话，下面几句不需要
    //namedWindow("Display Image", WINDOW_AUTOSIZE );
    //imshow("Display Image", image);
    //waitKey(0);

    //////////////////////////////
    Mat imaged;
    // 拷贝一份，不影响原图
    image.copyTo(imaged);


    //draw_rect(imaged);
    draw_rect2(imaged);
    
    imwrite("foo_draw.jpg", imaged);

    return 0;
}