token = 0
num = 0
input_data = 0
input_count = 0
paren = 0

def error():
    print("syntax error!!")
    exit(1)

def lex():
    global token
    global input_data
    global input_count
    global num
    if token.isdigit():
        if num == 0 and token == '0':
            error()
        num = num * 10 + int(token)
    elif token=='*' or token=='/' or token=='+' or token=='-':
        num = 0

    if(len(input_data)>input_count):
        token = input_data[input_count]
        input_count += 1
        while token == ' ':
            if (len(input_data) > input_count):
                token = input_data[input_count]
                input_count += 1

def digit():
    global token
    global num
    if token.isdigit():
        lex()
    else:
        error()
    return num

def number():
    global token
    global num
    global paren
    digit()
    if token.isdigit() or token=='+' or token=='-' or token=='*' or token=='/' or token==')' or token=='\0':
        if token==')' and paren<1:
            error()
        while token.isdigit():
            digit()
    else:
        error()
    return num

def factor():
    global token
    global paren
    minus = 1

    if token == '-':
        lex()
        minus = -1
    if token == '(':
        paren += 1
        lex()
        value = expr()
        if token == ')':
            paren -= 1
            lex()
        else:
            error()
    else:
        value = number()
    return minus * value

def term():
    global token
    value = factor()
    while token == '*' or token == '/':
        if token == '*':
            lex()
            value *= factor()
        elif token == '/':
            lex()
            value /= factor()
        else:
            error()
    return value


def expr():
    global token
    value = term()
    while token == '+' or token == '-':
        if token == '+':
            lex()
            value += term()
        elif token == '-':
            lex()
            value -= term()
        else:
            error()
    return value

if __name__ == '__main__':
    while True:
        input_data = input()
        input_data = input_data + '\0'
        token = input_data[input_count]
        input_count += 1
        result = expr()
        print(result)

        token = 0
        num = 0
        input_data = 0
        input_count = 0