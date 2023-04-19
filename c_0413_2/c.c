#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define MAZE_SIZE 10
int Stacksize = 30;
typedef struct {
    short r;
    short c;
} element;
typedef struct {
    element* s_tack;
    int top;
}stackType;

char maze[MAZE_SIZE][MAZE_SIZE] = {
        {'1','1','1','1','1','1','1','1','1','1'},
        {'e','1','0','1','0','0','0','1','0','1'},
        {'0','0','0','1','0','0','0','1','0','1'},
        {'0','1','0','0','0','1','1','0','0','1'},
        {'1','0','0','0','1','0','0','0','0','1'},
        {'1','0','0','0','1','0','0','0','0','1'},
        {'1','0','0','0','0','0','1','0','1','1'},
        {'1','0','1','1','1','0','1','1','0','1'},
        {'1','1','0','0','0','0','0','0','0','x'},
        {'1','1','1','1','1','1','1','1','1','1'}
};

element here = { 1,0 }, entry = { 1,0 };
stackType move;                                                             //1
stackType r_move;

int boolean = 0, number = 0;

int is_empty(stackType* s) {
    return (s->top == -1);
}
int is_full(stackType* s) {
    return (s->top == (Stacksize - 1));
}
element peek(stackType* s, int t) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1);
    }
    else {
        element e;
        e.r = s->s_tack[t].r;
        e.c = s->s_tack[t].c;
        return e;
    }
}
void push(stackType* s, element* e) {
    if (is_full(s)) {
        //fprintf(stderr, "\nfull. \n");
        Stacksize *= 2;
        s->s_tack = realloc(s->s_tack, Stacksize * sizeof(element));
        if (s->s_tack == NULL) {
            fprintf(stderr, "메모리 할당 오류");
            exit(1);
        }

    }
    ++s->top;
    s->s_tack[s->top].r = e->r;
    s->s_tack[s->top].c = e->c;

}
int check(element* e, element* e2) {
    int c = e->c, r = e->r;
    int c2 = e2->c, r2 = e2->r;

    if (c == c2 && r == r2 + 1 || c == c2 && r == r2 - 1 || r == r2 && c == c2 - 1 || r == r2 && c == c2 + 1)
        return 1;

    else return 0;
}
// here에 리턴해주어야함.
element pop(stackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "empty. \n");
        exit(1);
    }
    else {
        //stack
        element tmp;
        tmp.r = s->s_tack[s->top].r;
        tmp.c = s->s_tack[s->top].c;

        if (boolean == 4) {                                                     //4
            boolean = 0;
            int t_top = move.top;
            //move
            tmp.r = s->s_tack[s->top].r;
            tmp.c = s->s_tack[s->top].c;
            s->top--;

            while (1) {

                element tmp2;
                tmp2.r = move.s_tack[move.top].r;
                tmp2.c = move.s_tack[move.top].c;
                move.top--;

                if (check(&tmp, &tmp2))
                    break;
              /*  else {
                    //t_top--;
                    //tmp2 = peek(&move, t_top);

                    //push(&move, &tmp2);
                    
                }*/

                //t_top++;
            }
            return tmp;
        }
        if (s != &move && s != &r_move) {
            //  printf("\n a : %d %d \n", tmp.r, tmp.c);
            push(&move, &tmp);                                                  //3_작동확인필요.&를 사용하거나하면 될듯
        }
        /*if (s == &move) {
        //    printf("\n b : %d %d \n", tmp.r, tmp.c);
            push(&r_move, &tmp);
        }*/
        s->top--;
        return  tmp;
    }
}
void push_loc(stackType* s, int r, int c) {
    if (r < 0 || c < 0) return;
    if (maze[r][c] != '1' && maze[r][c] != '.') {

        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s, &tmp);
    }
    else boolean++;

}
/*void maze_print(char maze[MAZE_SIZE][MAZE_SIZE]) {
    printf("\n");

    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            printf("%c", maze[r][c]);
        }
        printf("\n");
    }
    printf("\n------------------------\n");
}*/
void init_stack(stackType* s) {
    s->s_tack = malloc(Stacksize * sizeof(element));
    if (s->s_tack == NULL) {
        printf("stack NULL");
        exit(1);
    }
    s->top = -1;
}
int main(void) {
    int r, c;
    stackType s;

    init_stack(&s);
    init_stack(&move);                                                        //2

    here = entry;
    while (maze[here.r][here.c] != 'x') {

        r = here.r;
        c = here.c;

        maze[r][c] = '.';
        //maze_print(maze);
        push_loc(&s, r - 1, c);
        push_loc(&s, r + 1, c);
        push_loc(&s, r, c - 1);
        push_loc(&s, r, c + 1);

        if (is_empty(&s)) {
            printf("실패\n");
            return 0;
        }
        //이동
        else {
            //모든 길이 막혔을때
            if (boolean == 4)
                number++;

            here = pop(&s);
        }
        boolean = 0;
    }
    //printf("성공\n돌아간 횟수는 %d \n", number);

    init_stack(&r_move);

    //바꿔 저장할 반복문
    while (1) {
        if (is_empty(&move)) break;
        element e = pop(&move);
        push(&r_move, &e);
    }
    //출력
    while (1) {                         //5_strlen부분
        if (is_empty(&r_move)) break;
        element e = pop(&r_move);
        printf("(%d,%d) -> ", e.r, e.c);
    }
    printf("도착");

    free(s.s_tack);
    free(move.s_tack);
    free(r_move.s_tack);


    return 0;
}