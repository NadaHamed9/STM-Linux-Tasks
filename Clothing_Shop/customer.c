#include "shop.h"
/****************************************** CUSTOMER ******************************/
InventoryItem *inventory;
int *inventory_size;
sem_t *sem;

/* Setup shared memory */
void setup_shared_memory() {
	int shm_id = shmget(SHM_KEY, sizeof(InventoryItem) * 10 + sizeof(int), 0666);
	if (shm_id == -1) {
		perror("Shared memory access failed");
		exit(1);
	}
	void *shm_ptr = shmat(shm_id, NULL, 0);
	if (shm_ptr == (void *)-1) {
		perror("Shared memory attachment failed");
		exit(1);
	}
	inventory = (InventoryItem *)shm_ptr;
	inventory_size = (int *)(shm_ptr + sizeof(InventoryItem) * 10);
	sem = sem_open(SEM_NAME, 0);
}
/* Customer Order */
void customer_order() {
	int item_id, quantity;
	printf("\n[Customer] Enter Item ID to order: ");
	scanf("%d", &item_id);
	if (item_id == 0) return;

	printf("[Customer] Enter Quantity: ");
	scanf("%d", &quantity);

	sem_wait(sem);
	for (int i = 0; i < *inventory_size; i++) {
		/*check if order is valid or not */
		if (inventory[i].item_id == item_id && inventory[i].stock >= quantity) {
			inventory[i].stock -= quantity;
			printf("[Customer] Order Successful: %d x %s\n", quantity, inventory[i].name);
			break;
		}
	}
	sem_post(sem);

}

/* Main Customer Process */
int main() {
	setup_shared_memory();
	while (1) {
		customer_order();
	}
	return 0;
}

