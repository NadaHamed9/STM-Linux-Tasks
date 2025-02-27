#include "shop.h"
/****************************************** STORE ************************************/
InventoryItem *inventory;
int *inventory_size;
sem_t *sem;

/* Load inventory from file (only at startup) */
void load_inventory() {
	/*load from file as startup*/
	FILE *file = fopen(STORAGE_FILE, "r");
	if (!file) {
		perror("Error opening inventory file");
		exit(1);
	}
	*inventory_size = 0;
	/*edits the struct*/
	while (fscanf(file, "%d %s %d %f", &inventory[*inventory_size].item_id,
				inventory[*inventory_size].name, &inventory[*inventory_size].stock,
				&inventory[*inventory_size].price) != EOF) {
		(*inventory_size)++;
	}
	fclose(file);
}

/* Display inventory */
void display_inventory() {
	sem_wait(sem);
	printf("\n--- Shop Inventory ---\n");
	for (int i = 0; i < *inventory_size; i++) {
		printf("ID: %d, Item: %s, Stock: %d, Price: %.2f\n",
				inventory[i].item_id, inventory[i].name, inventory[i].stock, inventory[i].price);
	}
	sem_post(sem);
}

/* Setup shared memory */
void setup_shared_memory() {
	int shm_id = shmget(SHM_KEY, sizeof(InventoryItem) * 10 + sizeof(int), IPC_CREAT | 0666);
	if (shm_id == -1) {
		perror("Shared memory allocation failed");
		exit(1);
	}
	void *shm_ptr = shmat(shm_id, NULL, 0);
	if (shm_ptr == (void *)-1) {
		perror("Shared memory attachment failed");
		exit(1);
	}
	inventory = (InventoryItem *)shm_ptr;
	inventory_size = (int *)(shm_ptr + sizeof(InventoryItem) * 10);
	sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
}

/* Main Shop Process */
int main() {
	setup_shared_memory();
	load_inventory();
	while (1) {
		display_inventory();
		sleep(5);
	}
	return 0;
}

