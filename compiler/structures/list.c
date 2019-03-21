/*
//  List.c
//  DataStructures
//
//  Created by Gonzo Fialho on 5/16/14.
//  Copyright (c) 2014 CEAFDC. All rights reserved.
*/

#include <stdlib.h>
#include "list.h"
#include "refcnt.h"


void _list_release(void *aList);

List *
list_init() {
    List * aList = (List *)alloc(sizeof(List), _list_release);
    aList->count = 0;
    aList->firstNode = NULL;
    aList->lastNode = NULL;
    return aList;
}

List *
list_append(List *aList, void *object) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    retain(object);
    newNode->object = object;
    newNode->next = NULL;
    newNode->prev = aList->lastNode;

    if (!aList->count) {
        aList->firstNode = newNode;
    } else {
        aList->lastNode->next = newNode;
    }
    aList->lastNode = newNode;
    aList->count++;
    return aList;
}

List *
list_push(List *aList, void *object) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    retain(object);
    newNode->object = object;
    newNode->next = aList->firstNode;
    newNode->prev = NULL;

    if (!aList->count) {
        aList->lastNode = newNode;
    } else {
        aList->firstNode->prev = newNode;
    }
    aList->firstNode = newNode;
    aList->count++;

    return aList;
}

void *
list_pop(List *aList) {
    Node *node;
    void *object;
    if (!aList->count) {
        return NULL;
    }
    node = aList->firstNode;
    object = node->object;
    node->next->prev = NULL;
    aList->firstNode = node->next;
    free(node);
    aList->count--;
    if (!aList->count) {
        aList->lastNode = NULL;
    }
    return object;
}

void *
list_dequeue(List *aList) {
    Node *node;
    void *object;
    if (!aList->count) {
        return NULL;
    }
    node = aList->lastNode;
    object = node->object;
    node->prev->next = NULL;
    aList->lastNode = node->prev;
    free(node);
    aList->count--;
    if (!aList->count) {
        aList->firstNode = NULL;
    }
    return object;
}

void
list_iterate(List *aList, void (*obj_iteration)(void *obj, int i, int count, bool *stop)) {
    Node *node;
    int i;
    bool stop = false;
    for (node = aList->firstNode, i = 0; node && !stop; node = node->next, i++) {
        obj_iteration(node->object, i, aList->count, &stop);
    }
}

void
list_release(List *aList) {
    Node *node;
    for (node = aList->firstNode; node;) {
        Node *nodeToFree = node;
        release(nodeToFree->object);
        node = node->next;
        free(nodeToFree);
    }
}

void
_list_release(void *aList) {
    list_release(aList);
}
