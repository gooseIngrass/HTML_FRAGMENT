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

    char c;                
    int fragmentNum = 0;    //nomer fragmenta
    int charCount = 0;      //kol-vo simvolov v tek fragmente

    while((fscanf(input, "%c", &c)) != EOF){ //schityvaem fail
        printf("%c", c);

        if(c == '<'){       //formiruem teg
            while ((c != '>') + (c != ' ') > 1){
                buffer.push_back(c);
                fscanf(input, "%c", &c);
                printf("%c", c);
            }
            while (c != '>'){ // yesli u tega est atributy, propuskaem ih
                fscanf(input, "%c", &c);
                printf("%c", c);
            }

            buffer.push_back(c);
            closeTag = buffer;
        }
        else{
            if(charCount == max_len){ 
                if(!Stack.empty()){ 
                    for(string s : Stack){ //Po okonchaniu fragmenta zakryvaem vse otkrytye tegi
                        if(Tags.count(s) == 0){
                            return 404;
                        }
                        printf("%s", Tags[s].c_str());
                    }

                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount);
                    fragmentNum++;

                    for(string s : Stack){ // otkryvaem zanovo v nachale sled fragmenta
                        printf("%s", s.c_str());
                    }
                    Stack.clear();
                }
                else{
                    printf("\n\nfragment #%d: %d chars\n\n", fragmentNum, charCount);
                    fragmentNum++;
                }

            }
            charCount++; //Yesli net sformirovannyh tegov i limit ne prevyshen, nachinaem sled iteraciu
            continue;
        }


        //yesli sformirovannyi teg zakryvaushiy, ubiraem iz steka otkryvaushiy

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
    FILE *input = fopen("source.html", "r");
    if(input == NULL){
        printf("FILE DOESNT EXIST");
    }
    int error = split_message(input);
    switch(error){
        case 404:
            printf("%d\n", error);
            break;
    }    
    fclose(input);
    return 0;
}
