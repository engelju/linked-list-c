/* ------------------------------------------------------------------- *
 *  @file:     linked_list.c                                           *
 *  @author:   julie.engel@mail.com                                    *
 *  @compiler: gcc version 4.2.1 20080704 (Red Hat 4.1.2-46)           *
 * ------------------------------------------------------------------- */
 
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
 
typedef struct _elem {
    char* name; 
    struct _elem* prev;     /* unused yet */
    struct _elem* next;
} element;
 
/* wrapper for first and last entry.
 * allows us to iterate from the beginning and from backwards. */
typedef struct _list{
    element* anfang;
    element* ende;
} list;
 
list*   list_create();
int     list_populate(list*, char*);
void    list_print(list*);
int     list_free(list*);
int     element_insert(list*, char*);
int     element_delete(list*, char*);
 
int main(int argc, char** argv) {
 
    list*   li;                       /* linked list wrapper structure */
    char    del_name[20];             /* not optimal b/c of limitation */
 
    /* create list */
    li = list_create();
 
    /* populate list */
    list_populate(li, "names.txt");
 
    /* delete name */
    printf("please type in the name you want to delete: ");
    scanf("%s", del_name);
 
    if (element_delete(li, del_name) != 0) {
        printf("element delete went wrong...");
    }
 
    list_print(li);
    list_free(li);
 
    return 0;
}
 
/* helper function */
int list_populate(list* li_p, 
                  char* filename) {
 
    FILE *fp;                       /* file pointer for file operations */
    char  person[100];              /* temporary array for read person name */
    int   cnt_successful = 0;
     
    /* read all names from the file until we get an unexpected entry
     * (bspw. EOF, fewer than 1 name per line or more than 1 name per line) */
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while(fscanf(fp, "%s", person) == 1) {              
            /* create entry for that name within linked list ASAP */
            element_insert(li_p, person);
            cnt_successful++; 
 
            /* TODO determine maxlengt of a name for the del_buffer */
            //printf("max size: %d", (int)(sizeof(person)/sizeof(char)));
        }
        fclose(fp);
    } else {
        perror("couln't open file");
        return -1;
    }
 
    list_print(li_p);
    printf("%d names read in successfull\n\n", cnt_successful);
     
    return cnt_successful;
}
 
/**
 * Creates the linked list wrapper and sets the start and ending
 * to NULL for the beginning.
 *
 * @return  pointer to the created linked list wrapper.
 */
list* list_create() {
 
    list* li = malloc(sizeof(list));
    if (li == NULL) {
        perror("Allocation of list failed\n");
    }
 
    li->anfang = NULL;
    li->ende   = NULL;
 
    return li;
}
 
/**
 * Prints out the pointer of each element within the linked list, 
 * as well as the name of the data in it and the ->next pointer.
 *
 * @param   list_p  pointer to the list to print out
 * @return  void
 */
void list_print(list* li_p) {
 
    element *elem = li_p->anfang;
 
    /* list is empty */
    if (li_p->anfang == NULL) {
        printf("list doesn't contain any elemnts\n");
    }
 
    /* iterate through list and print out name and pointers
     * of every entry */
    while (elem != NULL) {
        printf("[%p]", elem);
        printf(" %s ", elem->name);
        printf("\t-> %p ", elem->next);
        printf("\n");
        elem = elem->next;
    }
 
    return;
}
 
/**
 * Destroys a the content of a linked list.
 *
 * TODO dunno yet if LL should also be destroyed.
 *      evtl create:
 *      
 *          int list_free_content() {
 *
 *          }
 *
 * @param   li_p    pointer to the linked list to "destroy"
 * @return  0 if successful, -1 if list is already empty
 */
int list_free(list* li_p) {
 
    element *elem = li_p->anfang;
    element *elem_tmp;
 
    /* list is empty */
    if (li_p->anfang == NULL) return -1;
 
    while (elem != NULL) {
        /* back up pointer to the next element */
        elem_tmp = elem->next;
 
        /* free the name of the element and set it to NULL */
        free(elem->name);
        elem->name = NULL;
 
        /* eventually free the element itself */
        free(elem);
 
        elem = elem_tmp;
    }
    li_p->anfang = NULL;
    li_p->ende = NULL;
    free(li_p);
 
    return 0;
}
 
/**
 * Insert a element into a linked list.
 *
 * @param   li_p    pointer to the list to insert the element into
 * @param   name    name (data) of the element entry
 * @return  0 if successful, -1 if allocation of new element failed
 */
int element_insert(list* li_p, 
                   char* name) {
 
    element* elem = NULL;
 
    /* create and init element */
    elem = malloc(sizeof(element));
    if (elem == NULL) {
        perror("Allocation of new element failed");
        return -1;
    }
    elem->name = strdup(name);
    elem->prev = NULL;
    elem->next = NULL;
 
    /* insert element */
    if (li_p->anfang == NULL) {
        li_p->anfang = elem;
        elem->prev = li_p->anfang;
    } else {
        li_p->ende->next = elem;
    }
 
    if (li_p->ende != NULL) {
        elem->prev = li_p->ende;
    }
    li_p->ende = elem;
 
    return 0;
}
 
/**
 * Deletes an element out of a linked list.
 *
 * @param   li_p    pointer to the list to insert the element into
 * @param   name    name (data) of the element entry
 * @return  0 on success, -1 if list is empty and -2 if name not found
 */
int element_delete(list* li_p, 
                   char* name) {
 
    element *elem = NULL;
    int      not_done = 1;
 
    /* list is empty */
    if (li_p->anfang == NULL) {
        return -1;
    } else {
        elem = li_p->anfang;
    }
 
    /* loop through list till match found */
    while (not_done) {
 
        /* we have a match */
        if (strcmp(elem->name, name) == 0) {
 
            if(li_p->anfang == elem) {
                /* item is the first in list */
                li_p->anfang = elem->next;
            } else if (li_p->ende == elem) {
                li_p->ende = elem->prev;
                elem->prev->next = NULL;
            } else {
                /* normal case */
                elem->prev->next = elem->next;
                elem->next->prev = elem->prev;
            }
 
            /* free the name of the element and set it to NULL */
            free(elem->name);
            elem->name = NULL;
 
            /* eventually free the element itself */
            free(elem);
 
            /* set notfound to 0 to show the job is done */
            not_done = 0;
        } else {
            if (elem->next != NULL) {
                elem = elem->next;
                not_done++;
            } else {
                /* end of list and couldn't find a match */
                fprintf(stderr, "Couldn't find name\n");
                return -2;
            }
        }
    }
 
    return 0;
}
