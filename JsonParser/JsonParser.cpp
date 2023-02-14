#pragma once
#include <iostream>
#include <fstream>
#include <sstream> 
#include "JsonParser.h"



std::ifstream JsonParser::loadFile(const std::string& filepath) {
    // 文件读取
    std::ifstream ifs(filepath, std::ios::in);
    // 判断是否成功开启
    if (!ifs.is_open()) {
        throw new std::runtime_error("文件打开失败");
    }
    return ifs;
}

std::string JsonParser::formatToString(std::ifstream& ifs) {
    // 判断文件大小，以为优化做判断
    //ifs.seekg(0, std::ios::end);
    //unsigned len = ifs.tellg();
    //ifs.seekg(0, std::ios::beg);
    //std::cout << "文件长度=" << len << std::endl;

    // 读取文件到字符串
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string s(buffer.str());

    // 去除回车以及换行
    for (auto it = s.begin(); it != s.end(); it++) {
        if (*it == ' ' || *it == '\n' || *it == '\t') {
            s.erase(it);
            it--;       // 保持迭代器指向正确
        }
    }

    return s;
}


int main()
{
    JsonParser jsp;
    auto ifs = jsp.loadFile("jsontest.json");
    std::string s = jsp.formatToString(ifs);
    std::cout << s << std::endl;
    try {
        jsp.parse(s);
        jsp.print();
    }
    catch (std::runtime_error e) {
        std::cout << e.what();
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
