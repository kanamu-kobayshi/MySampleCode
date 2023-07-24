#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

char stack[MAX_SIZE];
int top = -1;

void push(char c);
char pop();
char peek();
int is_opening_bracket(char c);
int is_matching_pair(char opening, char closing);
int is_balanced(char *s);

int main()
{
    char s[MAX_SIZE];
    printf("Enter the brackets: ");
    scanf("%s", s);

    if (is_balanced(s))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
    return 0;
}

void push(char c)
{
    if (top == (MAX_SIZE - 1))
    {
        printf("Error: stack overflow\n");
        return;
    }
    stack[++top] = c;
}

char pop()
{
    if (top == -1)
    {
        printf("Error: No elements to pop\n");
        return '\0';
    }
    return stack[top--];
}

char peek()
{
    return stack[top];
}

int is_opening_bracket(char c)
{
    return (c == '(' || c == '{' || c == '[');
}

int is_matching_pair(char opening, char closing)
{
    if (opening == '(' && closing == ')')
        return 1;
    else if (opening == '{' && closing == '}')
        return 1;
    else if (opening == '[' && closing == ']')
        return 1;
    return 0;
}

int is_balanced(char *s)
{
    int i;
    for (i = 0; i < strlen(s); i++)
    {
        if (is_opening_bracket(s[i]))
        {
            push(s[i]);
        }
        else
        {
            if (top == -1 || !is_matching_pair(peek(), s[i]))
            {
                return 0;
            }
            pop();
        }
    }
    return (top == -1) ? 1 : 0;
}
