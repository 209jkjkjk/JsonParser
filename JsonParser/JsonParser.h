#pragma once
#include <fstream>
#include <map>
#include "JsonData.h"
class JsonParser {
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

