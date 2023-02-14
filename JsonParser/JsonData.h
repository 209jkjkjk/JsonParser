#pragma once
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
enum JsonDataType {
	object,
	array,
	null,
	string,
	number,
	boolean
};
class JsonData{
public:
	JsonData() {}
	JsonDataType type;
	virtual JsonDataType judgeType(std::string::const_iterator& it) {
		if (*it == '"') return JsonDataType::string;
		else if (*it == 'n') return JsonDataType::null;
		else if ((*it >= '0' && *it <= '9') || *it == '+' || *it == '-')return JsonDataType::number;
		else if (*it == '{') return JsonDataType::object;
		else if (*it == '[') return JsonDataType::array;
		else if (*it == 'f' || *it == 't') return JsonDataType::boolean;
		else throw std::runtime_error("�Ƿ�json��������");
	}
	virtual void print(unsigned indent) = 0;
};

// ��ƽ��������
class JsonPlainData: public JsonData {
public:
	JsonPlainData(){}
	virtual void parseValue(std::string::const_iterator& it) = 0;
protected:
	// ��json�ַ�������ȡ����ֵ
	std::string extractStringValue(std::string::const_iterator& it);
};

// Ƕ����������
class JsonNestedData :public JsonData {
public:
	JsonNestedData(){}
	virtual void parse(std::string::const_iterator&) = 0;
	JsonData* newJsonData(JsonDataType type);
};

class JsonNullData : public JsonPlainData {
public:
	JsonNullData() {}
	void parseValue(std::string::const_iterator& it) {	// ��Ҫ��У������
		std::string strValue = extractStringValue(it);
		if (strValue != "null") throw std::runtime_error("����ֵ" + strValue + "��������������:null");
	};
	void print(unsigned indent) {
		// ��ӡ����
		//printf("%*s", indent, " ");
		// ��ӡֵ
		std::cout << "null";
	}
};

class JsonNumberData : public JsonPlainData {
public:
	JsonNumberData(){}
	long value;
	void parseValue(std::string::const_iterator& it) {
		std::string strValue = extractStringValue(it);
		std::istringstream iss;
		iss.str(strValue);
		try {
			iss >> value;
		}
		catch (std::runtime_error e) {
			throw std::runtime_error("����ֵ" + strValue + "��������������:number");
		}
	};
	void print(unsigned indent) {
		// ��ӡ����
		//printf("%*s", indent, " ");
		// ��ӡֵ
		std::cout << std::to_string(value);
	}
};

class JsonBooleanData : public JsonPlainData {
public:
	JsonBooleanData(){}
	bool value;
	void parseValue(std::string::const_iterator& it) {
		std::string strValue = extractStringValue(it);
		if (strValue == "true") value = true;
		else if (strValue == "false") value = false;
		else throw std::runtime_error("����ֵ" + strValue + "��������������:Boolean");
	};
	void print(unsigned indent) {
		// ��ӡ����
		//printf("%*s", indent, " ");
		// ��ӡֵ
		std::cout << (value ? "true" : "false");
	}
};

class JsonStringData : public JsonPlainData {
public:
	JsonStringData(){}
	std::string value;
	void parseValue(std::string::const_iterator& it) {
		std::string strValue = extractStringValue(it);
		// ��������strValue[0] != '"'��Ϊfalse���ʷ��ں��治Ӱ������
		if (strValue[strValue.size() - 1] != '"' || strValue[0] != '"') throw std::runtime_error("����ֵ" + strValue + "��������������:String");
		value = strValue.substr(1,strValue.size()-2);	// ȥ����������
	};
	void print(unsigned indent) {
		// ��ӡ����
		//printf("%*s", indent, " ");
		// ��ӡֵ
		std::cout << '\"' << value << '\"';
	}
};


class JsonObjectData : public JsonNestedData {
public:
	JsonObjectData() {}
	std::map<std::string, JsonData*> attributes;
	
	// ��ΪJson��ʱ
	void parse(std::string);
	// ��Ϊ����ֵʱ
	void parse(std::string::const_iterator&);

	void print(unsigned indent);
private:
	std::string extractAttributeName(std::string::const_iterator& it);
};

class JsonArrayData : public JsonNestedData {
public:
	JsonArrayData() {}
	std::vector<JsonData*> elements;
	void parse(std::string::const_iterator&);

	void print(unsigned indent);
};
