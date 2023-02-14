#pragma once
#include <fstream>
#include <map>
#include "JsonData.h"
class JsonParser {
public:
	JsonParser() {}

	// 根对象
	JsonObjectData root;

	// 文件处理
	std::ifstream loadFile(const std::string& filePath);	// 读取文件（假设文件不大）
	std::string formatToString(std::ifstream&);					// 一次性读取文件，并去除换行和空格

	// 分析函数
	void parse(const std::string s) {			// 解析字符串并且储存为树
		root.parse(s);
	}
	// 打印函数
	void print() {
		root.print(0);
	}
	
};

