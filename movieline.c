//Programming Assignment 4
//Thai Nguyen
//th313791
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 12

// Customer struct
typedef struct customer {
    char ptr[50];   // customer's name
    int nt;          // number ticket: 0 < nt <= 500
    int t;           // t <= 10^9 (seconds)
    int line;        // line number
} customer;

// Node struct for linked list of customers
typedef struct node_c {
    struct customer *c;
    struct node_c *next;
} node_c;

// Queue struct
typedef struct queue {
    int size;
    node_c *front;
    node_c *back;
} queue;

int n;    // n <= 500,000 (customer purchasing ticket)
int b;    // b <= 12 (number booths)

// Function prototypes
customer *new_customer();
node_c *new_node_c(customer *c);
void enqueue(queue *line[], customer *c);
int find_min_queue(queue *line[]);
void print_queue(queue *q);
void free_queue(queue *q);


int main() {
    scanf("%d %d", &n, &b);

    // Create an array of pointers to queues
    queue* line[MAXLINE];

    // Initialize queues
    for (int i = 1; i < MAXLINE+1; i++) {
        line[i] = malloc(sizeof(queue));
        line[i]->size = 0;
        line[i]->front = NULL;
        line[i]->back = NULL;
    }

    // Enqueue customers to the queues
    for (int i = 0; i < n; i++) {
        customer *c = new_customer();
        enqueue(line, c);
    }

    // Print the queues
    for (int i = 1; i <= MAXLINE; i++) {
        printf("Queue %d:\n", i);
        print_queue(line[i]);
    }
 
    
    for(int i = 1; i < MAXLINE+1; i++)
    {
         free_queue(line[i]); // Free the memory for each queue
    }
    


    return 0;
}

customer *new_customer() {
    customer *new = malloc(sizeof(customer));

    // Read customer information
    scanf("%s %d %d", new->ptr, &new->nt, &new->t);

    char letter = new->ptr[0];
    if ('A' <= letter && letter <= 'Z') {
        int position = letter - 'A';
        new->line = position % 13;
    }

    return new;
}

node_c *new_node_c(customer *c) {
    node_c *newNode = malloc(sizeof(node_c));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->c = c;
    newNode->next = NULL;

    return newNode;
}

void enqueue(queue *line[], customer *c) {
    int line_number;

    if (c->line != 0) {
        // Customer with line number not equal to 0
        line_number = c->line;
    } else {
        // Find the queue with the fewest customers (excluding queue 0)
        line_number = find_min_queue(line);
    }

    node_c *newNode = new_node_c(c);

    if (line[line_number]->front == NULL) {
        // If the queue is empty, set both front and back to the new node
        line[line_number]->front = newNode;
        line[line_number]->back = newNode;
    } else {
        // Otherwise, add the new node to the back of the queue
        line[line_number]->back->next = newNode;
        line[line_number]->back = newNode;
    }

    // Increment the size of the queue
    line[line_number]->size++;

    // Calculate time (if needed)
    // c->t = 30 + c->nt * 5 + c->t;
}

int find_min_queue(queue *line[]) {
    int min_size = -1;
    int min_queue = -1;

    for (int i = 1; i <= MAXLINE; i++) {
        if (line[i]->size > 0 && ((min_size == -1) || (line[i]->size < min_size))) {
            min_size = line[i]->size;
            min_queue = i;
        }
    }

    return min_queue;
}



void print_queue(queue *q) {
    printf("Queue Size: %d\n", q->size);

    // Traverse the queue and print customer information
    node_c *current = q->front;
    while (current != NULL) {
        printf("Name: %s, Number of Tickets: %d, Time: %d, Line: %d\n",
               current->c->ptr, current->c->nt, current->c->t, current->c->line);

        current = current->next;
    }

    printf("\n");
}

void free_queue(queue *q) {
    // Traverse the queue and free memory for each node
    node_c *current = q->front;
    while (current != NULL) {
        node_c *temp = current;
        current = current->next;
        free(temp->c);  // Free memory for customer struct
        free(temp);     // Free memory for node
    }

    // After all nodes are freed, free memory for the queue
    free(q);
}