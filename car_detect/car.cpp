/**
* 基于haar的车牌目标检测示例。
* 使用
* ./a.out cascade.xml foo/
* 检测foo目录下所有图片。

* linux + opencv3.3
* 注：当前所用的模型文件准确性不高，待训练。
*     图片背景不要太复杂，最好只有一辆车。
*     即使只有一辆车，也会有多检情况。
*     应该和模型文件有关。
*
*/

#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "opencv2/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
CascadeClassifier cascade;

int filter_fn(const struct dirent *dirp)
{
    if(dirp->d_type != DT_REG)
        return 0;

    return (strncasecmp(dirp->d_name + (strlen(dirp->d_name) - 4), "jpeg", 4) == 0 ||
            strncasecmp(dirp->d_name + (strlen(dirp->d_name) - 3), "jpg", 3) == 0 || 
            strncasecmp(dirp->d_name + (strlen(dirp->d_name) - 3), "bmp", 3) == 0
            );
}

int find_file_dir(const std::string &dir, std::vector<std::string>& outfile)
{
    int num = 0;
    struct dirent **namelist;
    std::string tmpfile = dir;

    num = scandir(dir.c_str(), &namelist, filter_fn, alphasort);
    if (num < 0)
    {
        printf("faile to read dir %s\n", dir.c_str());
        return 01;
    }
    for (int i = 0; i < num; i++)
    {
        if(strcmp(namelist[i]->d_name, ".")==0 || strcmp(namelist[i]->d_name, "..")==0)
			continue;
        //printf("%d file: %s\n", i, namelist[i]->d_name);
        tmpfile += namelist[i]->d_name;
        outfile.push_back(tmpfile);
        tmpfile = dir; // rock back
        free(namelist[i]);
    }
    free(namelist);

    return 0;
}

// 去掉后缀名
std::string splitfile(const std::string &file)
{
    std::size_t pos = file.find_last_of(".");
    // 截取后缀名：
    // file.substr(pos==std::string::npos ? file.length() : pos+1);
    return file.substr(0, pos==std::string::npos ? file.length() : pos);
}

// 只保留文件名称，不要目录和后缀
std::string splitonlyfile(const std::string &file)
{
    std::size_t pos1 = file.find_last_of("/");
    std::size_t pos2 = file.find_last_of(".");

    return file.substr(pos1==std::string::npos ? 0 : pos1+1, pos2-pos1-1);
}

// 获取目录
std::string splitdir(const std::string &file)
{
    std::size_t pos = file.find_last_of("/");
    // 截取后缀名：
    // file.substr(pos==std::string::npos ? file.length() : pos+1);
    return file.substr(0, pos==std::string::npos ? file.length() : pos);
}


void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale)
{
    double t = 0;
    vector<Rect> cars;

    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();

    cascade.detectMultiScale(smallImg, cars,
        1.1, 5, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 9));

    t = (double)getTickCount() - t;
    printf( "detection time = %g ms total: %d\n", t*1000/getTickFrequency(), cars.size());
    for ( size_t i = 0; i < cars.size(); i++ )
    {
        Rect r = cars[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        printf("w: %d h: %d ratio: %.2f\n", r.width, r.height, aspect_ratio);
        
        rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                       cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                       Scalar(0, 0, 255), 2);
        // TODO: 将框出来的部分保存成文件
    }
}

int main(int argc, char* argv[])
{
    Mat image;
    std::vector<std::string> infile;
    char outfile[256] = {0};
    char outdir[256] = {0};
    
    if (argc < 3)
    {
        printf("usage: %s <cascade.xml> <image file dir>.\n", argv[0]);
        return 0;
    }
    
    if( !cascade.load(argv[1]) )
    {
        printf("ERROR: Could not load classifier cascade\n");
        return -1;
    }

    find_file_dir(argv[2], infile);
    
    sprintf(outdir, "%s/result", argv[2]);
    mkdir(outdir, 0775);
    for (unsigned int i = 0; i < infile.size(); i++)
    {
        printf("procesing %s\n", infile[i].c_str());
        
        image = imread(infile[i].c_str());
    
        sprintf(outfile, "%s/%s_detect.jpg", outdir, splitonlyfile(infile[i]).c_str());
        double scale = 1.0;
        
        detectAndDraw(image, cascade, scale);
        
        printf("saving file: %s\n", outfile);
        imwrite(outfile, image);
    
        printf("===================\n");
        
    }

    return 0;
}