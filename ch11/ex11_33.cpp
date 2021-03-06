#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;
map<string, string> buildMap(ifstream &map_file)
{
    map<string, string> trans_map;  //存放转换规则
    string key;                     //存放要转换的单词
    string value;                   //替换后的内容
    //读取第一个单词存入key中，行中剩余内容存入value
    while (map_file >> key && getline(map_file, value))
    {
        if (value.size() > 1)       //检查是否有转换规则
            trans_map[key] = value.substr(1);   //跳过前导空格
        else
            throw runtime_error("no rule for " + key);
    }
    return trans_map;
}
const string & transform(const string &s, const map<string, string> &m)
{
    //实际完成转换工作；此部分是程序的核心部分
    auto map_it = m.find(s);
    //如果单词在转换规则中map中
    if (map_it != m.cend())
        return map_it->second;  //使用替换短语
    else
        return s;               //否则返回原string
}
void word_transform(ifstream &map_file, ifstream &input)
{
    auto trans_map = buildMap(map_file);        //保存转换规则
    string text;                                //保存输入的第一行
    while (getline(input, text))                //读取一行输入
    {
        istringstream stream(text);
        string word;
        bool firstword = true;                  //控制是否打印空格
        while (stream >> word)
        {
            if (firstword)
                firstword = false;
            else
                cout << " ";                    //在单词间打印一个空格
            cout << transform(word, trans_map); //打印输出
        }
        cout << endl;                           //完成一行转换
    }
}
int main()
{
    ifstream ifs_rules("../ch11/transform_rules.txt");
    ifstream ifs_txt("../ch11/for_transform.txt");

    word_transform(ifs_rules, ifs_txt);

    return EXIT_SUCCESS;
}