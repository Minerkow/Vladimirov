struct list_t
{
    struct node_t *start;
    struct node_t *end;
};

struct node_t *read_list(FILE *inp);
void delete_list(struct node_t *top);
struct node_t* create_node(int data);
struct node_t* insert(struct node_t* parent, int data);

struct node_t *read_list(FILE *inp)
{
    struct node_t *even_start = NULL;
    struct node_t *odd_start = NULL;
    int data = 0;
    int flag_even = 0;
    int flag_odd = 0;

    struct node_t *ptr_even = NULL;
    struct node_t *ptr_odd = NULL;

    while(fscanf(inp, "%d", &data) == 1)
    {
        if ((data % 2) == 0 && flag_even == 0)
        {
            flag_even++;
            ptr_even = create_node(data);
            even_start = ptr_even;
            continue;
        }
        if ((data % 2) != 0 && flag_odd == 0)
        {
            flag_odd++;
            ptr_odd = create_node(data);
            odd_start = ptr_odd;
            continue;
        }
        if((data % 2) == 0 )
            ptr_even = insert(ptr_even, data);
        if((data % 2) != 0 )
            ptr_odd = insert(ptr_odd, data);
    }
    if (flag_even == 0)
        return odd_start;
    ptr_even->next = odd_start;
    return even_start;
}

struct node_t* create_node(int data)
{
    struct node_t *ptr = (struct node_t*)calloc(1, sizeof(struct node_t));
    ptr->data = data;
    ptr->next = NULL;
    return ptr;
}

struct node_t* insert(struct node_t* parent, int data)
{
    parent->next = (struct node_t*)calloc(1, sizeof(struct node_t));
    parent->next->data = data;
    parent->next->next = NULL;
    return parent->next;
}

void delete_list(struct node_t *top)
{
    while(top != NULL)
    {
        struct node_t* tmp = top->next;
        free(top);
        top = tmp;
    }
}
