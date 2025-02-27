#include "shop.h"
/************************************** MANAGER ********************************/
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
/* Inventory Update */
void update_inventory() {
	int item_id, new_stock;
	float new_price;
	printf("\n[Inventory Manager] Enter Item ID to update: ");
	scanf("%d", &item_id);
	printf("[Inventory Manager] Enter Stock to Add: ");
	scanf("%d", &new_stock);
	printf("[Inventory Manager] Enter New Price: ");
	scanf("%f", &new_price);

	sem_wait(sem);
	/*update inventory*/
	for (int i = 0; i < *inventory_size; i++) {
		if (inventory[i].item_id == item_id) {
			inventory[i].stock += new_stock;
			inventory[i].price = new_price;
			break;
		}
	}
	sem_post(sem);
}

/* Main Manager Process */
int main() {
	setup_shared_memory();
	while (1) {
		update_inventory();
	}
	return 0;
}

