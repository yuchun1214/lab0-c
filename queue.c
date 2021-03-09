#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#include "harness.h"
#include "queue.h"

#define show(list)                                                 \
    for (list_ele_t *node = list; node != NULL; node = node->next) \
        printf("%s ", node->value);                                \
    printf("\n");

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    return (q) ? (q->tail = q->head = NULL), (q->size = 0), (q) : q;
    // q->head = NULL;
    // return q;
}

void list_ele_free(list_ele_t *ele)
{
    if (ele) {
        free(ele->value);
        free(ele);
    }
}

list_ele_t *list_ele_new(char *s)
{
    list_ele_t *ele = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!ele) {
        return NULL;
    }
    size_t str_length = strlen(s) + 1;
    ele->value = (char *) malloc(sizeof(char) * str_length);
    if (!ele->value) {
        free(ele);
        return NULL;
    }
    strncpy(ele->value, s, str_length);
    ele->next = NULL;
    return ele;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q) {
        return;
    }
    list_ele_t *next;
    while (q->head) {
        next = (q->head)->next;
        list_ele_free(q->head);
        q->head = next;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q == NULL) {
        return false;
    }
    newh = list_ele_new(s);
    if (!newh)  // fail to malloc
        return false;
    newh->next = q->head;
    q->head = newh;
    ++q->size;
    if (q->tail == NULL) {
        q->tail = q->head;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL) {
        return false;
    }
    list_ele_t *newt;
    newt = list_ele_new(s);
    if (!newt)
        return false;
    if (q->tail) {
        q->tail->next = newt;
    } else {
        q->head = newt;
    }
    q->tail = newt;
    ++q->size;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q && q->size) {
        list_ele_t *head = q->head;
        q->head = q->head->next;
        if (sp) {
            strncpy(sp, head->value, (bufsize - 1) * sizeof(char));
            sp[bufsize - 1] = '\0';
        }
        --q->size;
        list_ele_free(head);
        return true;
    }
    return false;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    return (q) ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || q->size == 0)
        return;
    list_ele_t *prev = q->head;
    list_ele_t *iter = q->head->next;
    list_ele_t *next;
    while (iter && iter->next) {
        next = iter->next;
        iter->next = prev;
        prev = iter;
        iter = next;
    }

    if (!iter)
        return;

    iter->next = prev;

    // swap head and tail
    next = q->tail;
    q->tail = q->head;
    q->head = next;
    q->tail->next = NULL;
}

list_ele_t *list_concat(list_ele_t **lhs,
                        list_ele_t **lhs_tail,
                        list_ele_t *rhs)
{
    if (!*lhs) {  // if lhs is null
        *lhs = rhs;
        *lhs_tail = rhs;
    } else {
        (*lhs_tail)->next = rhs;
        *lhs_tail = (rhs) ? (rhs) : *lhs_tail;
    }
    return *lhs;
}

void list_ele_add_node(list_ele_t **head, list_ele_t **tail, list_ele_t *node)
{
    node->next = NULL;
    if (*head) {  // test if ele is null or not
        // the newest version is add node on the tail
        (*tail)->next = node;  // connection
        (*tail) = node;        // move the tail


        // the old version is add node on the head
        // node->next = (*ele)->next;
        // (*ele)->next = node;

    } else {  // if head is NULL; both head and tail point on the node
        *head = node;
        *tail = node;
    }
}

void list_quick_sort(list_ele_t **ele, list_ele_t **tail)
{
    if (!*ele) {
        return;
    }
    list_ele_t *lhs, *rhs, *lhs_tail, *rhs_tail;
    list_ele_t *pivot, *iter, *next;
    lhs_tail = rhs_tail = lhs = rhs = NULL;


    pivot = *ele;        // set pivot point to the first element of the list
    iter = pivot->next;  // iterator start on the second element
    pivot->next = NULL;

    while (iter) {
        next = iter->next;
        if (strcmp(iter->value, pivot->value) < 0) {
            list_ele_add_node(&lhs, &lhs_tail, iter);
            // assert(lhs_tail == iter); // check if lhs_tail certantily move
        } else {
            list_ele_add_node(&rhs, &rhs_tail, iter);
            // assert(rhs_tail == iter); // check if rhs_tail move
        }
        iter = next;
    }

    list_quick_sort(&lhs, &lhs_tail);
    list_quick_sort(&rhs, &rhs_tail);

    list_concat(&lhs, &lhs_tail, pivot);
    list_concat(&lhs, &lhs_tail, rhs);
    *ele = lhs;
    *tail = (rhs_tail) ? rhs_tail : lhs_tail;
    return;
}

list_ele_t *list_merge(list_ele_t *lhs, list_ele_t *rhs)
{
    list_ele_t *result, *result_iter;

    if (strcmp(lhs->value, rhs->value) < 0) {
        result = lhs;
        lhs = lhs->next;
    } else {
        result = rhs;
        rhs = rhs->next;
    }

    result_iter = result;

    while (lhs && rhs) {
        if (strcmp(lhs->value, rhs->value) < 0) {
            result_iter->next = lhs;
            lhs = lhs->next;
        } else {
            result_iter->next = rhs;
            rhs = rhs->next;
        }
        result_iter = result_iter->next;
    }
    if (lhs) {
        result_iter->next = lhs;
    } else if (rhs) {
        result_iter->next = rhs;
    }
    return result;
}

list_ele_t *list_merge_sort(list_ele_t *head)
{
    if (!head || !(head->next))
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // find the middle of list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;
    // printf("Slow : "); show(head);
    // printf("Fast : "); show(fast);
    list_ele_t *lhs = list_merge_sort(head);
    list_ele_t *rhs = list_merge_sort(fast);
    return list_merge(lhs, rhs);
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    // list_quick_sort(&q->head, &q->tail);
    if (q) {
        q->head = list_merge_sort(q->head);
        list_ele_t *iter = q->head;
        if (iter)
            while (iter->next)
                iter = iter->next;
        q->tail = iter;
    }
    // printf("Tail : %s \n", q->tail->value);
    return;
}
