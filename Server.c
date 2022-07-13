#include"csapp.h"
#include <string.h>
#include <stdio.h>
#define BUFFER 256

int normal_count = 100;
int deluxe_count = 50;
int premium_count = 30;
int *ticket_no;
int Normal = 0, Deluxe = 0, Premium = 0;
sem_t n, d, p;

void *Ticket_Booking(void * vargp);

int email_check(char *ptr);

int check_name(char *ptr);

int main(int argc, char **argv)
{
    pthread_t server_thread;
    int listenfd, *connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
   
    if(argc != 2)
    {
        fprintf(stderr, "usage : %s <port>\n", argv[0]);
        exit(0);
    }
    
    listenfd = Open_listenfd(argv[1]);
    while(1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        Pthread_create(&server_thread, NULL, Ticket_Booking, connfd);
    }
    return 0;
}

void *Ticket_Booking(void *vargp)
{
    Sem_init(&n, 0, 1);
    Sem_init(&d, 0, 1);
    Sem_init(&p, 0, 1);

    int connfd = *((int *)vargp), tickets;
    char *token[5], name[50], email[50], category[50], tickets_char[50], mail[300];

    Pthread_detach(pthread_self());
    
    int request;
    char client_request[BUFFER];

    request = recv(connfd, client_request, BUFFER, 0);
    
    if(request < 0) 
    {
        puts("Request receiving failed...\n");
    }

    else if(request == 0)
    {
        puts("Oops! Connection disconnected unexpectedly...\n");
    }

    else
    {
        printf("%s", client_request);
        token[0] = strtok(client_request, " ");

        if(strcmp(token[0], "GET") == 0)
        {
            token[1] = strtok(NULL, " ");
            token[1]+=1;
            token[2] = strtok(NULL," ");
            token[3] = strtok(NULL, " ");
            token[4] = strtok(NULL, " ");

            if((strcmp(token[1], "") == 0) || check_name(token[1]) == 1)
                printf("Invalid name entered!\n");

            else
            {
                strcpy(name, token[1]);

                if(atoi(token[2]) <= 0)
                    printf("%s-Invalid quantity requested\n", name);

                else
                {
                    strcpy(tickets_char, token[2]);
                    tickets = atoi(tickets_char);

                    if((strcmp(token[3], "Normal") != 0) && (strcmp(token[3], "Deluxe") != 0) && (strcmp(token[3], "Premium") != 0))
                        printf("Invalid category entered!\n");

                    else
                    {
                        strcpy(category, token[3]);

                        if(email_check(token[4]) || strcmp(token[4], "") == 0)
                        {
                            strcpy(email, token[4]);

                            if(strcmp(category, "Normal") == 0)
                            {
                                P(&n);
                                char ticket_name[100], confirmation_ticket[100], temp[100];
                                FILE *fileptr;

                                if(normal_count >= tickets)
                                {
                                    normal_count = normal_count - tickets;
                                    printf("%s-The ticket booking was successful. Ticket numbers: ", name);

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        printf("%d, ", ++Normal);
                                    }

                                    printf("%d.\n", ++Normal);
                                    printf("Total tickets booked %d. Sent to %s\n", tickets, email);
                                    strcpy(ticket_name, "Ticket.txt");
                                    fileptr = fopen(ticket_name, "w");
                                    strcpy(confirmation_ticket, "Thanks ");
                                    strcat(confirmation_ticket, name);
                                    strcat(confirmation_ticket, " for booking ticket(s).");
                                    strcat(confirmation_ticket, "\nYour ticket(s) has been booked.\n");
                                    strcat(confirmation_ticket, "Seat Category : ");
                                    strcat(confirmation_ticket, category);
                                    strcat(confirmation_ticket, "\nTotal number of booked tickets are : ");
                                    strcat(confirmation_ticket, tickets_char);
                                    strcat(confirmation_ticket, "\nSeat numbers booked are : ");

                                    Normal = Normal - tickets;
                                    char helper[300];

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        Normal = Normal + 1;
                                        int temp = Normal;
                                        sprintf(helper, "%d", temp);
                                        strcat(confirmation_ticket, helper);
                                        strcat(confirmation_ticket, ", ");
                                    }

                                    Normal = Normal + 1;
                                    int temp = Normal;
                                    sprintf(helper, "%d", temp);
                                    strcat(confirmation_ticket, helper);
                                    strcat(confirmation_ticket, ".\n");

                                    if(fileptr)
                                    {
                                        fputs(confirmation_ticket, fileptr);
                                    }

                                    fclose(fileptr);
                                    strcpy(mail, "sendmail ");
                                    strcat(mail, email);
                                    strcat(mail, " < ./");
                                    strcat(mail, ticket_name);
                                    system(mail);
                                    V(&n);
                                }

                                else
                                {
                                    printf("%s-No sufficient tickets are available in this category\n", name);
                                }
                            }

                            else if(strcmp(category, "Deluxe") == 0)
                            {
                                P(&d);
                                char ticket_name[100], confirmation_ticket[100], temp[100];
                                FILE *fileptr;

                                if(deluxe_count >= tickets)
                                {
                                    deluxe_count = deluxe_count - tickets;
                                    printf("%s-The ticket booking was successful. Ticket numbers: ", name);

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        printf("%d, ", ++Deluxe);
                                    }

                                    printf("%d.\n", ++Deluxe);
                                    printf("Total tickets booked %d. Sent to %s\n", tickets, email);
                                    strcpy(ticket_name, "Ticket.txt");
                                    fileptr = fopen(ticket_name, "w");
                                    strcpy(confirmation_ticket, "Thanks ");
                                    strcat(confirmation_ticket, name);
                                    strcat(confirmation_ticket, " for booking ticket(s).");
                                    strcat(confirmation_ticket, "\nYour ticket(s) has been booked.\n");
                                    strcat(confirmation_ticket, "Seat Category : ");
                                    strcat(confirmation_ticket, category);
                                    strcat(confirmation_ticket, "\nTotal number of booked tickets are : ");
                                    strcat(confirmation_ticket, tickets_char);
                                    strcat(confirmation_ticket, "\nSeat numbers booked are : ");

                                    Deluxe = Deluxe - tickets;
                                    char helper[300];

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        Deluxe = Deluxe + 1;
                                        int temp = Deluxe;
                                        sprintf(helper, "%d", temp);
                                        strcat(confirmation_ticket, helper);
                                        strcat(confirmation_ticket, ", ");
                                    }

                                    Deluxe = Deluxe + 1;
                                    int temp = Deluxe;
                                    sprintf(helper, "%d", temp);
                                    strcat(confirmation_ticket, helper);
                                    strcat(confirmation_ticket, ".\n");

                                    if(fileptr)
                                    {
                                        fputs(confirmation_ticket, fileptr);
                                    }

                                    fclose(fileptr);
                                    strcpy(mail, "sendmail ");
                                    strcat(mail, email);
                                    strcat(mail, " < ./");
                                    strcat(mail, ticket_name);
                                    system(mail);
                                    V(&d);
                                }

                                else
                                {
                                    printf("%s-No sufficient tickets are available in this category\n", name);
                                }
                            }

                            else if(strcmp(category, "Premium") == 0)
                            {
                                P(&p);
                                char ticket_name[100], confirmation_ticket[100], temp[100];
                                FILE *fileptr;

                                if(premium_count >= tickets)
                                {
                                    premium_count = premium_count - tickets;
                                    printf("%s-The ticket booking was successful. Ticket numbers: ", name);

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        printf("%d, ", ++Premium);
                                    }

                                    printf("%d.\n", ++Premium);
                                    printf("Total tickets booked %d. Sent to %s\n", tickets, email);
                                    strcpy(ticket_name, "Ticket.txt");
                                    fileptr = fopen(ticket_name, "w");
                                    strcpy(confirmation_ticket, "Thanks ");
                                    strcat(confirmation_ticket, name);
                                    strcat(confirmation_ticket, " for booking ticket(s).");
                                    strcat(confirmation_ticket, "\nYour ticket(s) has been booked.\n");
                                    strcat(confirmation_ticket, "Seat Category : ");
                                    strcat(confirmation_ticket, category);
                                    strcat(confirmation_ticket, "\nTotal number of booked tickets are : ");
                                    strcat(confirmation_ticket, tickets_char);
                                    strcat(confirmation_ticket, "\nSeat numbers booked are : ");

                                    Premium = Premium - tickets;
                                    char helper[300];

                                    for(int i = 0; i < (tickets - 1); i++)
                                    {   
                                        Premium = Premium + 1;
                                        int temp = Premium;
                                        sprintf(helper, "%d", temp);
                                        strcat(confirmation_ticket, helper);
                                        strcat(confirmation_ticket, ", ");
                                    }

                                    Premium = Premium + 1;
                                    int temp = Premium;
                                    sprintf(helper, "%d", temp);
                                    strcat(confirmation_ticket, helper);
                                    strcat(confirmation_ticket, ".\n");

                                    if(fileptr)
                                    {
                                        fputs(confirmation_ticket, fileptr);
                                    }

                                    fclose(fileptr);
                                    strcpy(mail, "sendmail ");
                                    strcat(mail, email);
                                    strcat(mail, " < ./");
                                    strcat(mail, ticket_name);
                                    system(mail);
                                    V(&p);
                                }

                                else
                                {
                                    printf("%s-No sufficient tickets are available in this category\n", name);
                                }
                            }
                        }

                        else
                            printf("%s-Invalid email address.\n", name);
                    }
                }
            }
        }
    }

    free(vargp);
    Close(connfd);
    return NULL;
}

int email_check(char *ptr)
{
    int at_rate = -1, dot = -1, i;
    int slen = strlen(ptr);

    for(i = 0; i < slen; i++)
    {
        if(ptr[i]=='@')
            at_rate = i;
        else if(ptr[i]=='.')
            dot = i;
    }

    if(at_rate == -1 || dot == -1)
        return 0;
    
    if(dot >= (slen - 1))
        return 0;
}

int check_name(char *ptr)
{
    if(ptr[0] >= '0' && ptr[0] <= '9')
        return 1;
    else
        return 0;
}