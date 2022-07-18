#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED, // 記号
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

struct Token{
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークン文字列
};

Token *token; // メインのtoken...

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);

    fprintf(stderr, "\n");

    exit(1);
}

bool consume_token(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;

    return true;
}

void expect_token(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("'%c'ではありません", op);
    
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM)
        error("not count");
    
    int val = token->val;
    token = token->next;

    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// tokenを引数とし、Tokenを返す。
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));

    tok->kind = kind;
    tok->str  = str;
    cur->next = tok;

    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);

            continue;
        }

        error("can not tokenize");
    }

    new_token(TK_EOF, cur, p);

    return head.next;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    // token に する。
    token = tokenize(argv[1]);

    // 前準備、とでもいいます、か。
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");


    printf("  mov rax, %d\n", expect_number());
    // happy new year

    // + or - で、アセンブリ出力 
    while (!at_eof()) {
        if (consume_token('+')) {
            printf("  add rax, %d\n", expect_number()); // 2が出力され，"-4"の'-'へのポインタがpに格納される．
            continue;
        }

        expect_token('-');
        printf("  sub rax, %d\n", expect_number());
    }


    printf("  ret\n");
    return 0;
}
