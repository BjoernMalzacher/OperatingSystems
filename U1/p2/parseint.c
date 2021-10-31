#include "parseint.h"

/*
 * Returns the value of c or -1 on error
 */
int parseDecimalChar(char c) {
    c -='0';
    int ic = (int)c;
    if(ic>=0 && ic <10){
        
        return ic;
    }else{
        return -1;
    }
}

int octalOrDecimal(char octal, char *string) {    
    int output = 0; 
    char *currentChar = string;

    while (*currentChar != '\0' ) {
        if (octal == 1){
            if(*currentChar>= '0' && *currentChar <'8'){
                output += (int)(*currentChar-'0');
                currentChar +=1; 
            }else{
                return -1;
            }
        }else{
            int currentInt =parseDecimalChar(*currentChar);
            if(currentInt == -1){
                return -1;  
            } 
            output += currentInt;
            currentChar +=1; 
        }
        if(*currentChar != '\0'){
            output *=10;        
        }
    }
    return output;
}
/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string) {
    
    if(*string == '0' ){
        
        string +=1;
        return octalOrDecimal( 1, string);
    }else{
        return octalOrDecimal( 0, string);
    } 
    
    return -1;
}
