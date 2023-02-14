#pragma once
#include <fstream>
#include <map>
#include "JsonData.h"
class JsonParser {
private:
	// �㷨����
	std::string _extractAttributeName(std::string::const_iterator&);												// ��ȡ������
	JsonDataType _judgeType(std::string::const_iterator&);							// �ж��������ͣ� ���������ַ��ж����ͣ���У�飬У�鲿���ھ������������
	std::string _extractAttributeValue(std::string::const_iterator&);	// ��ȡ����ֵ�����ַ�����ʽ����



public:
	JsonParser() {}

	// ������
	JsonObjectData root;

	// �ļ�����
	std::ifstream loadFile(const std::string& filePath);	// ��ȡ�ļ��������ļ�����
	std::string formatToString(std::ifstream&);					// һ���Զ�ȡ�ļ�����ȥ�����кͿո�

	// ��������
	void parse(const std::string s) {			// �����ַ������Ҵ���Ϊ��
		root.parse(s);
	}
	// ��ӡ����
	void print() {
		root.print(0);
	}
	
};

