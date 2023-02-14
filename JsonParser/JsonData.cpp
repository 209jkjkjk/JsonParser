#pragma once
#include <iostream>

#include "JsonData.h"

std::string JsonPlainData::extractStringValue(std::string::const_iterator& it) {
	// ɨ�����ֵ����
	unsigned attributeValueLength = 0;
	do {
		++attributeValueLength;
	} while (*(it + attributeValueLength) != ',' && *(it + attributeValueLength) != '}' &&*(it + attributeValueLength) != ']');
	// ��ȡ�ַ����õ�ֵ
	std::string value(it, it + attributeValueLength);
	std::cout << "�õ�����ֵ��" << value << std::endl;
	// �ƶ���������ֵ��һ���ַ�
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
	// ��ʼ��string������
	auto it = s.begin();
	// ʹ�õ�������ʼ����
	parse(it);
	if (it != s.end()) throw std::runtime_error("�������β����");
}

void JsonObjectData::parse(std::string::const_iterator& it) {
	std::cout << "�������ṹ" << std::endl << std::endl;
	// ȷ��json��ʽ�Ϸ��ԣ��ٻ����ſ�ͷ
	//std::cout << "��ͷ�ַ��ǣ�" << *it << *(it + 1) << std::endl;
	if (*it != '{') throw std::runtime_error("object����ֵ����ͷ����");

	// ѭ����Ӽ�ֵ��
	do {
		std::cout << "���������..." << std::endl;
		it++;
		// ��ȡ��
		std::string attributeName = extractAttributeName(it);
		// �ж�ֵ����
		auto type = judgeType(it);
		//std::cout << "����" << *it << "�õ��������ͣ�" << type << std::endl;
		// ��Ӽ�
		attributes.insert(std::make_pair(attributeName, newJsonData(type)));
		// ���ֵ
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
	} while (*it == ',');		// ����˵������δ�����������

	// �ѵ�������ĩβ���ַ�ӦΪ��
	//std::cout << "ĩβ�ַ��ǣ�" << *it << *(it + 1) << std::endl;
	if (*it != '}') throw std::runtime_error("object����ֵ����β����");
	it++;
	std::cout << std::endl << "�뿪����ṹ" << std::endl;
}

std::string JsonObjectData::extractAttributeName(std::string::const_iterator& it) {
	// ȷ��json��ʽ�Ϸ��ԣ���˫���ſ�ͷ �����������Ȳ�Ϊ0
	if (*it++ != '"' || *it == '"') throw std::runtime_error("json��ʽ����");

	// ɨ���������������
	unsigned attributeNameLength = 0;
	do {
		++attributeNameLength;
	} while (*(it + attributeNameLength) != '"');
	// ��ȡ�ַ����õ�������
	std::string name(it, it + attributeNameLength);
	std::cout << "�õ���������" << name << std::endl;;
	// �ƶ�����������������һ���ַ�����ӦΪ˫����)
	it += attributeNameLength;

	// ȷ��json��ʽ�Ϸ��ԣ���˫���Ž�β �ں�����ð��
	if (*it++ != '"' || *it++ != ':') throw std::runtime_error("json��ʽ����");

	return name;
}

void JsonArrayData::parse(std::string::const_iterator& it) {
	std::cout << "��������ṹ" << std::endl << std::endl;
	// ȷ��json��ʽ�Ϸ��ԣ��ٷ����ſ�ͷ
	if (*it != '[') throw std::runtime_error("json��ʽ����");

	// ѭ�����ֵ
	do {
		std::cout << "���������..." << std::endl;
		it++;
		// �ж���������
		auto type = judgeType(it);
		// ������β������ֵ
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
	} while (*it == ',');		// ����˵������δ�����������

	// �ѵ�������ĩβ���ַ�ӦΪ]
	//std::cout << "ĩβ�ַ��ǣ�" << *it << *(it + 1) << std::endl;
	if (*it != ']') throw std::runtime_error("array����ֵ����β����");
	it++;
	std::cout << std::endl << "�뿪����ṹ" << std::endl;
}

// ��ӡ����
inline void printSpace(unsigned num) {
	std::cout.width(num);
	std::cout << ' ';
}

void JsonObjectData::print(unsigned indent=0) {
	// ��ӡ{
	std::cout << '{' << std::endl;
	for (auto it = attributes.begin(); it != attributes.end();) {
		// ����
		printf("%*s", indent+4, " ");
		// ��:
		std::cout << '\"' << it->first << "\":";
		// ֵ
		it->second->print(indent +4);
		// ����
		if ((++it) != attributes.end())std::cout << ',';
		// ����
		std::cout << std::endl;
	}
	// ��ӡ������}
	printf("%*s", indent, " "); std::cout << '}';
}

void JsonArrayData::print(unsigned indent=0) {
	// ��ӡ[
	std::cout << '[' << std::endl;
	for (auto it = elements.begin(); it != elements.end();) {
		// ����
		printf("%*s", indent+4, " ");
		// ֵ
		(*it)->print(indent+4);
		// ����
		if ((++it) != elements.end())std::cout << ',';
		// ����
		std::cout << std::endl;
	}
	// ��ӡ������}
	printf("%*s", indent, " ");; std::cout << ']';
}
