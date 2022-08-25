#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;

char token;
int num = 0;
string input;
int input_count = 0;
int paren = 0;


float expr();

void error(){
    cout << "syntax error!!" << endl;
    exit(1);
}

void lex(){
    if(isdigit(token)){
        if(num==0 && token=='0')
            error();
        num = num * 10 + token - 48;
    }
    else if(token=='*' || token=='/' || token=='+' || token=='-')
        num = 0;

    token = input[input_count++];

    while(isspace(token)){
        token = input[input_count++];
    }
}

int digit(){
	if(isdigit(token))
		lex();
	else
		error();

    return num;
}

int number(){
    digit();
    if(isdigit(token) || token=='+' || token=='-' || token=='*' || token=='/' || token==')' || token=='\0'){
        if(token==')' && paren<1)
            error();
        while(isdigit(token))
            digit();
    }
    else
        error();

    return num;
}

float factor(){
    float value;
    float minus = 1;

    if(token == '-'){
        lex();
        minus = -1;
    }
    if(token == '('){
        paren++;
		lex();
		value = expr();
		if(token == ')'){
            paren--;
			lex();
        }
		else
			error();
	}
	else
		value = number();

    return minus * value;
}

float term(){
    float value = factor();
	while(token == '*' || token == '/'){
        switch(token) {
        case '*':
            lex();
            value *= factor();
            break;
        case '/':
            lex();
            value /= factor();
            break;
        default:
            error();
        }
	}
    return value;
}

float expr() {
    float value = term();
	while(token == '+' || token == '-'){
        switch(token) {
        case '+':
            lex();
            value += term();
            break;
        case '-':
            lex();
            value -= term();
            break;
        default:
            error();
        }
	}
    return value;
}

int main(void) {
    while(true){
        getline(cin, input);
        token = input[input_count++];
        float result = expr();
        cout << result << endl;

        token = 0;
        num = 0;
        input_count = 0;
    }
    return 0;
}