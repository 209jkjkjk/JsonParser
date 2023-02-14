#pragma once
#include <fstream>
#include <map>
#include "JsonData.h"
class JsonParser {
private:
	// 算法函数
	std::string _extractAttributeName(std::string::const_iterator&);												// 提取属性名
	JsonDataType _judgeType(std::string::const_iterator&);							// 判断属性类型（ 仅根据首字符判断类型，不校验，校验部分在具体数据类型里）
	std::string _extractAttributeValue(std::string::const_iterator&);	// 提取属性值，以字符串形式保存



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

