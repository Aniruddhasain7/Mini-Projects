#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int burst;
    int remaining;
} Process;

typedef struct Node {
    Process p;
    struct Node* next;
} Node;

Node* front = NULL;
Node* rear = NULL;

void enqueue(Process p) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->p = p;
    temp->next = NULL;
    if (rear == NULL) {
        front = rear = temp;
        return;
    }
    rear->next = temp;
    rear = temp;
}

Process dequeue() {
    Node* temp = front;
    Process p = temp->p;
    front = front->next;
    if (front == NULL) rear = NULL;
    free(temp);
    return p;
}

int isEmpty() {
    return front == NULL;
}

int main() {
    int n = 5, tq = 2;
    Process p[5];

    int waiting[5] = {0};
    int turnaround[5] = {0};

    printf("Enter burst times for 5 processes:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d: ", p[i].pid);
        scanf("%d", &p[i].burst);
        p[i].remaining = p[i].burst;
        enqueue(p[i]);
    }

    printf("\nGantt Chart:\n|");

    int time = 0;

    while (!isEmpty()) {
        Process cur = dequeue();

        if (cur.remaining > tq) {
            time += tq;
            cur.remaining -= tq;
            enqueue(cur);
            printf(" P%d |", cur.pid);
        } 
        else {
            time += cur.remaining;
            printf(" P%d |", cur.pid);

            turnaround[cur.pid - 1] = time;
            waiting[cur.pid - 1] = turnaround[cur.pid - 1] - p[cur.pid - 1].burst;

            cur.remaining = 0;
        }
    }

    printf("\n\nProcess\tBurst\tWaiting\tTurnaround\n");

    float totalW = 0, totalT = 0;

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n", 
               i + 1, 
               p[i].burst, 
               waiting[i], 
               turnaround[i]);

        totalW += waiting[i];
        totalT += turnaround[i];
    }

    printf("\nAverage Waiting Time = %.1f\n", totalW / n);
    printf("Average Turnaround Time = %.1f\n", totalT / n);

    return 0;
}
