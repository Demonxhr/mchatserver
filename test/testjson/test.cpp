#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
using namespace std;

//json序列化实例1
string func1()
{
     json js;
     js["from"] = "zhang san";
     js["to"] = "li si";
     js["msg"] = "hello,";
     string sendBuf = js.dump();
     return sendBuf;
}

//json 序列化实例2
void func2()
{
     json js;
     js["id"] = {1,2,3,4,5};
     js["to"] = "li si";
     js["cc"]["liu shuo"] = "hello,aaa";
     js["msg"] = {{"af","fadh"},{"asdfg","asdg"}};
     string sendBuf = js.dump();
     cout << sendBuf <<endl;
}

//json 序列化实例3
void func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    cout << js << endl;
}
int main()
{
    //func1();
    //func2();
    string recBuf = func1();
    json buf = json::parse(recBuf);
    cout << buf["from"] << buf["to"]  << buf["msg"] << endl;
    return 0;
}