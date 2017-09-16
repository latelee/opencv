/**
* ����haar�ĳ���Ŀ����ʾ����
* ʹ��
* ./a.out cascade.xml foo/
* ���fooĿ¼������ͼƬ��

* linux + opencv3.3
* ע����ǰ���õ�ģ���ļ�׼ȷ�Բ��ߣ���ѵ����
*     ͼƬ������Ҫ̫���ӣ����ֻ��һ������
*     ��ʹֻ��һ������Ҳ���ж�������
*     Ӧ�ú�ģ���ļ��йء�
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

// ȥ����׺��
std::string splitfile(const std::string &file)
{
    std::size_t pos = file.find_last_of(".");
    // ��ȡ��׺����
    // file.substr(pos==std::string::npos ? file.length() : pos+1);
    return file.substr(0, pos==std::string::npos ? file.length() : pos);
}

// ֻ�����ļ����ƣ���ҪĿ¼�ͺ�׺
std::string splitonlyfile(const std::string &file)
{
    std::size_t pos1 = file.find_last_of("/");
    std::size_t pos2 = file.find_last_of(".");

    return file.substr(pos1==std::string::npos ? 0 : pos1+1, pos2-pos1-1);
}

// ��ȡĿ¼
std::string splitdir(const std::string &file)
{
    std::size_t pos = file.find_last_of("/");
    // ��ȡ��׺����
    // file.substr(pos==std::string::npos ? file.length() : pos+1);
    return file.substr(0, pos==std::string::npos ? file.length() : pos);
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, char* outfile)
{
    double t = 0;
    vector<Rect> cars;

    Mat gray, smallImg;
    Mat src_clone = img.clone();
    
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
        Size(200, 150));

    t = (double)getTickCount() - t;
    printf( "detection time = %g ms total: %d\n", t*1000/getTickFrequency(), cars.size());
    for ( size_t i = 0; i < cars.size(); i++ )
    {
        Rect rect = cars[i];

        double aspect_ratio = (double)rect.width/rect.height;
        //printf("w: %d h: %d ratio: %.2f\n", rect.width, rect.height, aspect_ratio);
        
        rectangle(img, cvPoint(cvRound(rect.x*scale), cvRound(rect.y*scale)),
                       cvPoint(cvRound((rect.x + rect.width-1)*scale), cvRound((rect.y + rect.height-1)*scale)),
                       Scalar(0, 0, 255), 2);
        #if 0
        // ��������Ĳ��ֱ�����ļ�
        Mat smallImgROI = src_clone(rect);
        char filename[256] = {0};
        sprintf(filename, "%s_car_%d.jpg", outfile, i);
        imwrite(filename, smallImgROI);
        #endif
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
        double scale = 3;
        
        detectAndDraw(image, cascade, scale, outfile);
        
        printf("saving file: %s\n", outfile);
        imwrite(outfile, image);
    
        printf("===================\n");
        
    }

    return 0;
}