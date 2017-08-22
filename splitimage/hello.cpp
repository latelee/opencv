/**
opencv分割图片示例：

在一张图片中，分割出指定坐标、分辨率的图片

注：
本文编码格式为UTF8无BMO，方便处理中文字符

*/
#include <string.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <signal.h>
#include <unistd.h>

#include "common.h"

using namespace cv;

static int current = 0;
static int total_cnt = 0;
static void sig_alarm(int signum)
{
   printf("progress=================>>>: %.2f%%\n", (float)current / (float)total_cnt * 100);

   alarm(3);
}

void write_image(const std::string& outdir, const std::string& file, Mat& image, int x, int y, int width, int height)
{
    // 参数分别：x、y坐标，宽、高
    Rect rect(x, y, width, height);

    Mat image_roi = image(rect);
    char filename[128] = {0};
    sprintf(filename, "%s/%s.jpg", outdir.c_str(), splitonlyfile(file).c_str());
    imwrite(filename, image_roi);
        
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage: %s <source dir> <output dir>\n", argv[0]);
        return -1;
    }
    
    std::vector<std::string> outfile;
    std::string outdir;

    signal(SIGALRM, sig_alarm);
	alarm(3);
    
    // 查找指定目录所有图片
    find_file_dir(argv[1], outfile);
    outdir = argv[2];
    
    total_cnt = outfile.size();

    for (unsigned int i = 0; i < total_cnt; i++)
    {
        current = i;
        printf("procesing %s\n", outfile[i].c_str());
        Mat image;
        image = imread(outfile[i].c_str());
        if (!image.data)
        {
            printf("No image data, continue...\n");
            continue;
        }
        write_image(outdir, outfile[i].c_str(), image, 118, 75, 3500-118-118, 2075-75);
        //write_image(outdir, outfile[i].c_str(), image, 0, 2080, 3500-0, 4080-2080);        
        
        printf("===================\n");
        
    }
    alarm(0);
    printf("done...\n");

    return 0;  
}