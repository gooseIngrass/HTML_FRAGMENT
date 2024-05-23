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

    char c;                 //Текущий символ
    int fragmentNum = 0;    //Номер фрагмента
    int charCount = 0;      //Счётчик символов в текущем фрагменте

    while((fscanf(input, "%c", &c)) != EOF){ //Считываем по символу
        printf("%c", c);

        if(c == '<'){       //Если нашли скобку, то формируем тег из последующих символов
            while ((c != '>') + (c != ' ') > 1){
                buffer.push_back(c);
                fscanf(input, "%c", &c);
                printf("%c", c);
            }
            while (c != '>'){ // Выводим атрибуты тега, если есть
                fscanf(input, "%c", &c);
                printf("%c", c);
            }

            buffer.push_back(c);
            closeTag = buffer;
        }
        else{
            if(charCount == max_len){
                if(!Stack.empty()){ //Если достигли лимита символов, проверяем остались ли незакрытые теги
                    for(string s : Stack){
                        if(Tags.count(s) == 0){ //Если незакрытым остался неблочный тег, ошибка(Тут пусть Данил веселится)
                            return 404;
                        }
                        printf("%s", Tags[s].c_str());
                    }

                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount);
                    fragmentNum++;

                    for(string s : Stack){ //Открываем теги заново
                        printf("%s", s.c_str());
                    }
                }
                else{
                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount); //Это мракобесие надо исправить
                    fragmentNum++;
                }

            }
            charCount++;
            continue; //Если тега нет то считаем текущий символ и в следующую итерацию
        }

        //Тут в условии тоже костыль, надо будет придумать по другому
        //А так: Если в стеке лежит открыващий, а в буфере закрывающий, то убираем из стека открывающий
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
