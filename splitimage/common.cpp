#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

int filter_fn(const struct dirent *dirp)
{
    if(dirp->d_type != DT_REG)
        return 0;

    return (strncmp(dirp->d_name + (strlen(dirp->d_name) - 4), "jpeg", 4) == 0 ||
            strncmp(dirp->d_name + (strlen(dirp->d_name) - 3), "jpg", 3) == 0 || 
            strncmp(dirp->d_name + (strlen(dirp->d_name) - 3), "bmp", 3) == 0
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