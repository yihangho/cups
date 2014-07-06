#include <stdio.h>

struct Computer
{
    int price;
    int main;
};

typedef struct Computer Computer;

typedef struct
{
    int index;
    int cost;
} Node;

int main()
{
    Node x  = {1,2};
    Computer a = {1, 2};
    printf("%d %d\n" , x.index , x.cost);
    printf("%d %d\n" , a.price , a.main);
    return 0;
}
