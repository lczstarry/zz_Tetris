#pragma once
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include "BackgroundNet.h"
using namespace std;

function<void(const char* sJson)> callFunc;


// ����Э���봦������Ӧ��ϵ����
const map<string, function<void(const char* sJson)>>::value_type init_value[] = {
	map<string, function<void(const char* sJson)>>::value_type("show", GS2CShowServerInfo),
};

// �ö�Ӧ��ϵ�����ʼ�� Э���봦������Ӧ��ϵ�ֵ�
map<string, function<void(const char* sJson)>> funcMap(init_value, init_value + 1);
map<string, function<void(const char* sJson)>>::iterator funcIter;
