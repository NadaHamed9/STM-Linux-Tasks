
# 🛒 Clothing Shop - IPC System with Shared Memory

This project simulates a **multi-process clothing shop system** in C using **POSIX shared memory (`shmget`, `shmat`)** and **semaphores (`sem_open`, `sem_wait`, `sem_post`)** to ensure synchronized access to inventory.

## 📌 Overview
The system consists of **three processes**:
1. **Shop (`store.c`)** - Displays the inventory continuously (auto-refresh every 5 seconds).
2. **Customer (`customer.c`)** - Allows customers to place orders, updating the inventory.
3. **Manager (`manager.c`)** - Allows inventory managers to update stock and prices.

✅ **Real-time updates** across all processes (Customer, Manager, and Shop).  
✅ **Shared memory ensures immediate reflection** of inventory changes.  
✅ **Semaphores prevent race conditions** and synchronize access.  


## 🚀 How It Works
- **The `store` process runs continuously**, refreshing inventory every 5 seconds.
- **Customers place orders (`customer.c`)**, and the inventory updates immediately.
- **Managers update stock (`manager.c`)**, and the changes appear in real-time.
- **All processes use shared memory**, meaning **changes reflect across all processes immediately**.
- **Semaphores prevent data corruption** when multiple processes access the inventory.


