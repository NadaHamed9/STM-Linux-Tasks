#ifndef SHOP_H
#define SHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
/*************************** DEFINED VARIABLES **************************/
#define STORAGE_FILE "storage.txt"
#define LOG_FILE "transactions.log"
#define SHM_KEY 1234
#define SEM_NAME "/shop_semaphore"
/************************** GLOBAL VARIABLES ***************************/
typedef struct {
	int item_id;
	char name[20];
	int stock;
	float price;
} InventoryItem;

extern InventoryItem *inventory;
extern int *inventory_size;
extern sem_t *sem;

/*********************************** FUNCTIONS ***************************/

/* Function to setup the shared memory between 3 processes for IPC*/
void setup_shared_memory();
/* Function to display what's in inventory from struct*/
void display_inventory();
/* Fuction to log transaction happened from customer/manager */
void log_transaction(const char *message);

#endif

