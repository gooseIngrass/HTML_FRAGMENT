#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "string.h"

using namespace std;

int split_message(FILE *input, int max_len){
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
    int fragmentNum = 0;    //Fragment number
    int charCount = 0;      //The number of characters in the fragment

    while((fscanf(input, "%c", &c)) != EOF){ //read file
        printf("%c", c);

        if(c == '<'){       //creating a tag
            while ((c != '>') + (c != ' ') > 1){
                buffer.push_back(c);
                fscanf(input, "%c", &c);
                printf("%c", c);
            }
            while (c != '>'){ // If the tag has attributes, skip them
                fscanf(input, "%c", &c);
                printf("%c", c);
            }

            buffer.push_back(c);
            closeTag = buffer;
        }
        else{
            if(charCount == max_len){ 
                if(!Stack.empty()){ 
                    for(string s : Stack){ //At the end of the fragment, we close the open tags
                        if(Tags.count(s) == 0){
                            return 404;
                        }
                        printf("%s", Tags[s].c_str());
                    }

                    printf("\n----------------------\n"
                           "fragment #%d: %d chars"
                           "\n----------------------\n", fragmentNum, charCount);
                    fragmentNum++;

                    for(string s : Stack){ // open the closed tags at the beginning of the next fragment
                        printf("%s", s.c_str());
                    }
                    Stack.clear();
                }
                else{
                    printf("\n----------------------\n"
                           "fragment #%d: %d chars"
                           "\n----------------------\n", fragmentNum, charCount);
                    fragmentNum++;
                }
                charCount = 0;
            }
            charCount++; //If there are no generated tags, we start the next iteration
            continue;
        }


        //If the generated tag is closing, remove the opening one from the stack

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

int main(int argc, char *argv[]) {
    if(strcmp(argv[1], "-help") == 0){
        printf("Example: msg_split 873 ./source.html");
        return 0;
    }

    FILE *input = fopen(argv[2], "r");
    if(input == NULL){
        printf("File doesn't exist or you enter something wrong "
               "\nType msg_split -help for help");
        return 0;
    }

    int max_len = strtol(argv[1], nullptr, 10);
    int error = split_message(input, max_len);

    switch(error){
        case 404:
            printf("\n\nIt is impossible to divide into fragments of length %d %d\n", max_len);
            break;
    }

    fclose(input);
    return 0;
}
