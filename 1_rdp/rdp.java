import java.util.Scanner;

public class rdp {

    static char token;
    static int num = 0;
    static String input;
    static int input_count = 0;
    static int paren = 0;

    static void error(){
        System.out.println("syntax error!!");
        System.exit(1);
    }

    static void lex(){
        if(Character.isDigit(token)) {
            if (num == 0 && token == '0')
                error();
            num = num * 10 + token - 48;
        }
        else if(token=='*' || token=='/' || token=='+' || token=='-')
            num = 0;

        if(input.length()>input_count) {
            token = input.charAt(input_count++);
            while (token == ' ') {
                if (input.length() > input_count)
                    token = input.charAt(input_count++);
            }
        }
    }

    static int digit(){
        if(Character.isDigit(token))
            lex();
        else
            error();

        return num;
    }

    static int number(){
        digit();
        if(Character.isDigit(token) || token=='+' || token=='-' || token=='*' || token=='/' || token==')' || token=='\0') {
            if (token == ')' && paren < 1)
                error();
            while (Character.isDigit(token))
                digit();
        }
        else
            error();

        return num;
    }

    static float factor(){
        float value = 0;
        float minus = 1;
        if(token == '-'){
            lex();
            minus = -1;
        }
        if(token == '('){
            paren++;
            lex();
            value = expr();
            if(token == ')') {
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

    static float term(){
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

    static float expr() {
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

   public static void main(String[] args) {
        while(true){
           Scanner sc = new Scanner(System.in);
           input = sc.nextLine();
           token = input.charAt(input_count++);
           input = input + '\0';
           float result = expr();
           if(result%1.0==0)
               System.out.println((int)result);
           else
               System.out.println(result);
           token = 0;
           num = 0;
           input_count = 0;
       }
   }
}
