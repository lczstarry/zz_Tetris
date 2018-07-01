#pragma once
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include "BackgroundNet.h"
using namespace std;

function<void(const char* sJson)> callFunc;


// 生成协议与处理函数对应关系数组
const map<string, function<void(const char* sJson)>>::value_type init_value[] = {
	map<string, function<void(const char* sJson)>>::value_type("show", GS2CShowServerInfo),
};

// 用对应关系数组初始化 协议与处理函数对应关系字典
map<string, function<void(const char* sJson)>> funcMap(init_value, init_value + 1);
map<string, function<void(const char* sJson)>>::iterator funcIter;
