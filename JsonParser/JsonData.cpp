#pragma once
#include <iostream>

#include "JsonData.h"

std::string JsonPlainData::extractStringValue(std::string::const_iterator& it) {
	// 扫描计算值长度
	unsigned attributeValueLength = 0;
	do {
		++attributeValueLength;
	} while (*(it + attributeValueLength) != ',' && *(it + attributeValueLength) != '}' &&*(it + attributeValueLength) != ']');
	// 截取字符串得到值
	std::string value(it, it + attributeValueLength);
	std::cout << "得到属性值：" << value << std::endl;
	// 移动迭代器至值后一个字符
	it += attributeValueLength;

	return value;
}

JsonData* JsonNestedData::newJsonData(JsonDataType type) {
	switch (type)
	{
	case object:
		return new JsonObjectData();
	case array:
		return new JsonArrayData();
	case null:
		return new JsonNullData();
	case string:
		return new JsonStringData();
	case number:
		return new JsonNumberData();
	case boolean:
		return new JsonBooleanData();
	}
}


void JsonObjectData::parse(std::string s) {
	// 初始化string迭代器
	auto it = s.begin();
	// 使用迭代器开始解析
	parse(it);
	if (it != s.end()) throw std::runtime_error("根对象结尾错误");
}

void JsonObjectData::parse(std::string::const_iterator& it) {
	std::cout << "进入对象结构" << std::endl << std::endl;
	// 确保json格式合法性：①花括号开头
	//std::cout << "开头字符们：" << *it << *(it + 1) << std::endl;
	if (*it != '{') throw std::runtime_error("object类型值，开头错误");

	// 循环添加键值对
	do {
		std::cout << "对象解析中..." << std::endl;
		it++;
		// 提取键
		std::string attributeName = extractAttributeName(it);
		// 判断值类型
		auto type = judgeType(it);
		//std::cout << "根据" << *it << "得到数据类型：" << type << std::endl;
		// 添加键
		attributes.insert(std::make_pair(attributeName, newJsonData(type)));
		// 添加值
		switch (type)
		{
		case object:
			((JsonNestedData*)attributes[attributeName])->parse(it); break;
		case array:
			((JsonNestedData*)attributes[attributeName])->parse(it); break;
		case null:
		case string:
		case number:
		case boolean:
			((JsonPlainData*)attributes[attributeName])->parseValue(it);break;
		}
	} while (*it == ',');		// 逗号说明还有未解析完的属性

	// 已到根对象末尾，字符应为｝
	//std::cout << "末尾字符们：" << *it << *(it + 1) << std::endl;
	if (*it != '}') throw std::runtime_error("object类型值，结尾错误");
	it++;
	std::cout << std::endl << "离开对象结构" << std::endl;
}

std::string JsonObjectData::extractAttributeName(std::string::const_iterator& it) {
	// 确保json格式合法性：①双引号开头 ②属性名长度不为0
	if (*it++ != '"' || *it == '"') throw std::runtime_error("json格式错误");

	// 扫描计算属性名长度
	unsigned attributeNameLength = 0;
	do {
		++attributeNameLength;
	} while (*(it + attributeNameLength) != '"');
	// 截取字符串得到属性名
	std::string name(it, it + attributeNameLength);
	std::cout << "得到属性名：" << name << std::endl;;
	// 移动迭代器至属性名后一个字符（理应为双引号)
	it += attributeNameLength;

	// 确保json格式合法性：①双引号结尾 ②后面有冒号
	if (*it++ != '"' || *it++ != ':') throw std::runtime_error("json格式错误");

	return name;
}

void JsonArrayData::parse(std::string::const_iterator& it) {
	std::cout << "进入数组结构" << std::endl << std::endl;
	// 确保json格式合法性：①方括号开头
	if (*it != '[') throw std::runtime_error("json格式错误");

	// 循环添加值
	do {
		std::cout << "数组解析中..." << std::endl;
		it++;
		// 判断数据类型
		auto type = judgeType(it);
		// 在数组尾部创建值
		elements.push_back(newJsonData(type));
		switch (type)
		{
		case object:
			((JsonNestedData*)elements[elements.size() - 1])->parse(it); break;
		case array:
			((JsonNestedData*)elements[elements.size() - 1])->parse(it); break;
		case null:
		case string:
		case number:
		case boolean:
			((JsonPlainData*)elements[elements.size() - 1])->parseValue(it);
			break;
		}
	} while (*it == ',');		// 逗号说明还有未解析完的属性

	// 已到根对象末尾，字符应为]
	//std::cout << "末尾字符们：" << *it << *(it + 1) << std::endl;
	if (*it != ']') throw std::runtime_error("array类型值，结尾错误");
	it++;
	std::cout << std::endl << "离开数组结构" << std::endl;
}

// 打印函数
inline void printSpace(unsigned num) {
	std::cout.width(num);
	std::cout << ' ';
}

void JsonObjectData::print(unsigned indent=0) {
	// 打印{
	std::cout << '{' << std::endl;
	for (auto it = attributes.begin(); it != attributes.end();) {
		// 缩进
		printf("%*s", indent+4, " ");
		// 键:
		std::cout << '\"' << it->first << "\":";
		// 值
		it->second->print(indent +4);
		// 逗号
		if ((++it) != attributes.end())std::cout << ',';
		// 换行
		std::cout << std::endl;
	}
	// 打印缩进和}
	printf("%*s", indent, " "); std::cout << '}';
}

void JsonArrayData::print(unsigned indent=0) {
	// 打印[
	std::cout << '[' << std::endl;
	for (auto it = elements.begin(); it != elements.end();) {
		// 缩进
		printf("%*s", indent+4, " ");
		// 值
		(*it)->print(indent+4);
		// 逗号
		if ((++it) != elements.end())std::cout << ',';
		// 换行
		std::cout << std::endl;
	}
	// 打印缩进和}
	printf("%*s", indent, " ");; std::cout << ']';
}
