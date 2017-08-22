/**
�򵥵�opencv����ʾ����
��ȡһ��ͼƬ��ת���ɻҶ�ͼ��Ȼ�󱣴档
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
    // û�н���Ļ������漸�䲻��Ҫ
    //namedWindow("Display Image", WINDOW_AUTOSIZE );  
    //imshow("Display Image", image);  
    //waitKey(0);  
    
    // תΪ�Ҷ�ͼ
    cv::Mat grayImg;
    cv::cvtColor(image, grayImg, CV_BGR2GRAY);
    // ����
    imwrite("foo.jpg", grayImg);
    
    return 0;  
}