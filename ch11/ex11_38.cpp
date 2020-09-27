#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;
unordered_map<string, string> buildMap(ifstream &map_file)
{
    unordered_map<string, string> trans_map;  //���ת������
    string key;                     //���Ҫת���ĵ���
    string value;                   //�滻�������
    //��ȡ��һ�����ʴ���key�У�����ʣ�����ݴ���value
    while (map_file >> key && getline(map_file, value))
    {
        if (value.size() > 1)       //����Ƿ���ת������
            trans_map[key] = value.substr(1);   //����ǰ���ո�
        else
            throw runtime_error("no rule for " + key);
    }
    return trans_map;
}
const string & transform(const string &s, const unordered_map<string, string> &m)
{
    //ʵ�����ת���������˲����ǳ���ĺ��Ĳ���
    auto map_it = m.find(s);
    //���������ת��������map��
    if (map_it != m.cend())
        return map_it->second;  //ʹ���滻����
    else
        return s;               //���򷵻�ԭstring
}
void word_transform(ifstream &map_file, ifstream &input)
{
    auto trans_map = buildMap(map_file);        //����ת������
    string text;                                //��������ĵ�һ��
    while (getline(input, text))                //��ȡһ������
    {
        istringstream stream(text);
        string word;
        bool firstword = true;                  //�����Ƿ��ӡ�ո�
        while (stream >> word)
        {
            if (firstword)
                firstword = false;
            else
                cout << " ";                    //�ڵ��ʼ��ӡһ���ո�
            cout << transform(word, trans_map); //��ӡ���
        }
        cout << endl;                           //���һ��ת��
    }
}
int main()
{
    ifstream ifs_rules("../ch11/transform_rules.txt");
    ifstream ifs_txt("../ch11/for_transform.txt");

    word_transform(ifs_rules, ifs_txt);

    return EXIT_SUCCESS;
}