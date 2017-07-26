#include<stdio.h>
#include<signal.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>

// Max token count = 20
#define TOKEN_COUNT 20

// token enum
typedef enum _TOKEN_TYPE{
    TOKEN_STRING,
    TOKEN_NUMBER,
}TOKEN_TYPE;

// Token structure
typedef struct _TOKEN{
    TOKEN_TYPE type;
    union{
        char *string;
        double number;
    };
    bool isArray;
}TOKEN;

// JSON structure
typedef struct _JSON {
    TOKEN tokens[TOKEN_COUNT];
}JSON;


// Read JSON
void parseJSON(char *file, int size, JSON *json){
    int tokenIndex = 0; // token index
    int pos = 0; // store position of text

    if(file[pos] != '{'){ // make sure it is JSON
        return ;
    }

    pos ++; // next character

    
    while(pos < size){ // loop as long as filesize
        switch (file[pos]){ // the kind of the character
            case '"' :
                {                 
                    // search starting point. +1 -> except for "
                    char *begin = file + pos + 1;

                    // search ending point. until "
                    char *end = strchr(begin, '"');
                    if(end == NULL){ // if there isn't "
                        break; // end
                    }
                    
                    // real length of file
                    int length = end - begin;
    
                    // The type of the token is a String
                    json->tokens[tokenIndex].type = TOKEN_STRING;
                    // allocate memory
                    json->tokens[tokenIndex].string = malloc(length + 1);
                    // initailize allocated memory
                    memset(json->tokens[tokenIndex].string, 0, length + 1);
                    // copy only the length of the string from begin to begin+length
                    memcpy(json->tokens[tokenIndex].string, begin, length);
    
                    tokenIndex++; // next tokenindex
    
                    pos = pos + length + 1; // now position + the length of string + "(+1)
                }
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': 
            case '6': case '7': case '8': case '9': case '-':       
                {
                    // search starting point
                    char *begin = file + pos;
                    char *end;
                    char *buffer;
                    
                    // search ending point
                    end = strchr(file + pos, ',');
                    if(end == NULL){
                        end == strchr(file + pos, '}');
                        if(end == NULL){
                            break;
                        }
                    }
                    
                    // real length of file
                    int length = end - begin;

                    // allocate memory
                    buffer = malloc(length + 1);

                    // initialize allocated memory
                    memset(buffer, 0, length + 1);

                    /// copy only the length of the string from begin to begin+length 
                    memcpy(buffer, begin, length);

                     // The type of the token is a Number
                    json->tokens[tokenIndex].type = TOKEN_NUMBER;
                    /// Convert string to number and store it in tokens
                    json->tokens[tokenIndex].number = atof(buffer);

                    // free buffer
                    free(buffer);

                    tokenIndex++; // next tokenindex

                    pos = pos + length + 1; // now position + the length of string + "(+1)
 
                }
                break;
        }
        pos++;
    }
}

// Free JSON and tokens
void freeJSON(JSON *json){
    int i;
    // free all of the tokens
    for(i = 0; i < TOKEN_COUNT; i++){
        if(json->tokens[i].type == TOKEN_STRING){
            free(json->tokens[i].string);
        }
    }
}


// Read file, and Return content and file_size
char *readFile(char *filename, int *fileSize){
    int size;
    char *buffer;

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        return NULL;
    }

    // seek fileSize
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // allocate memory and initialize that
    buffer = malloc(size + 1);
    memset(buffer, 0 , size);

    // read file
    if(fread(buffer, size, 1, fp) < 1){
        *fileSize = 0;
        free(buffer);
        fclose(fp);
        return NULL;
    }

    // hand over size
    *fileSize = size;

    // finalize
    fclose(fp);
    return buffer;
}

// Handle signal
void signalhandler(int signal){
    int size; // the size of the JSON
    char *file = readFile("ex.json", &size); // file is the varialbe of JSON
    if(file == NULL){
        return ;
    }

    // initialize json varialbe
    JSON json = {0, };

    // parsing JSON file
    parseJSON(file, size, &json);

    // print JSON content
    printf("%s : %s\n", json.tokens[0].string, json.tokens[1].string);
    printf("%s : %s\n", json.tokens[2].string, json.tokens[3].string);
    printf("%s : %f\n", json.tokens[4].string, json.tokens[5].number);

    // release json
    freeJSON(&json);
    // release file
    free(file);

    return ;

}

int main(void){
    while(1){
        signal( SIGUSR1 , signalhandler );
    }
}

