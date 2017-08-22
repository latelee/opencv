#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

int find_file_dir(const std::string &dir, std::vector<std::string>& outfile);

std::string splitfile(const std::string &file);
std::string splitonlyfile(const std::string &file);
std::string splitdir(const std::string &file);

#endif // 