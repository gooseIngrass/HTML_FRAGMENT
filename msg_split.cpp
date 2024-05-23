#include <cstdio>
#include <string>
#include <map>
#include <vector>

using namespace std;
const int max_len = 893;

int split_message(FILE *input){
    map<string, string>Tags;
    Tags = {{"<strong>", "</strong>"},
            {"<i>", "</i>"},
            {"<p>", "</p>"},
            {"<ul>", "</ul>"},
            {"<ol>", "</ol>"},
            {"<b>", "</b>"},
            {"<div>", "</div>"},
            {"<span>", "</span>"}};

    string buffer, closeTag;
    vector<string> Stack;

    char c;                 //������� ������
    int fragmentNum = 0;    //����� ���������
    int charCount = 0;      //������� �������� � ������� ���������

    while((fscanf(input, "%c", &c)) != EOF){ //��������� �� �������
        printf("%c", c);

        if(c == '<'){       //���� ����� ������, �� ��������� ��� �� ����������� ��������
            while ((c != '>') + (c != ' ') > 1){
                buffer.push_back(c);
                fscanf(input, "%c", &c);
                printf("%c", c);
            }
            while (c != '>'){ // ������� �������� ����, ���� ����
                fscanf(input, "%c", &c);
                printf("%c", c);
            }

            buffer.push_back(c);
            closeTag = buffer;
        }
        else{
            if(charCount == max_len){
                if(!Stack.empty()){ //���� �������� ������ ��������, ��������� �������� �� ���������� ����
                    for(string s : Stack){
                        if(Tags.count(s) == 0){ //���� ���������� ������� ��������� ���, ������(��� ����� ����� ���������)
                            return 404;
                        }
                        printf("%s", Tags[s].c_str());
                    }

                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount);
                    fragmentNum++;

                    for(string s : Stack){ //��������� ���� ������
                        printf("%s", s.c_str());
                    }
                }
                else{
                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount); //��� ���������� ���� ���������
                    fragmentNum++;
                }

            }
            charCount++;
            continue; //���� ���� ��� �� ������� ������� ������ � � ��������� ��������
        }

        //��� � ������� ���� �������, ���� ����� ��������� �� �������
        //� ���: ���� � ����� ����� ����������, � � ������ �����������, �� ������� �� ����� �����������
        if(!buffer.empty() && !Stack.empty() && Stack.back() == buffer.erase(1, 1)){
            Stack.pop_back();
        }
        else{
            Stack.push_back(closeTag);
        };
        buffer.clear();
        closeTag.clear();
    }
    return 1;
}

int main() {
    FILE *input = fopen("F:\\HTML_Fragmentation\\source.html", "r");
    if(input == NULL){
        printf("FILE DOESNT EXIST");
    }
    int error = split_message(input);
    printf("%d\n", error);
    fclose(input);
    return 0;
}
