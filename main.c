#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MENU 10
#define MAX_ORDERS 100
#define TAX_RATE 0.05      
#define SERVICE_CHARGE 0.05
typedef struct {
    int id;
    char name[30];
    float price;
    int orderCount; 
} MenuItem;
typedef struct {
    int orderId;
    int tableNo;
    int itemId;
    char itemName[30];
    int quantity;
    float totalPrice;
    char status[15]; 
    int hour; 
} Order;
MenuItem menu[MAX_MENU] = {
    {1, "Burger", 150.0, 0},
    {2, "Pizza", 350.0, 0},
    {3, "Pasta", 250.0, 0},
    {4, "French Fries", 100.0, 0},
    {5, "Coke", 60.0, 0},
    {6, "Sandwich", 120.0, 0},
    {7, "Coffee", 90.0, 0},
    {8, "Ice Cream", 110.0, 0}
};
int menuSize = 8;
Order orderQueue[MAX_ORDERS];
int front = 0;
int rear = -1;
int orderCount = 0;
float dailyRevenue = 0.0;
int hourlyOrders[24] = {0}; 
void displayMenu();
void placeOrder();
void updateOrderStatus();
void displayQueue();
void generateBill();
void showAnalytics();
int main() {
    int choice;
    while (1) {
        printf("\n=========================================\n");
        printf("   RESTAURANT ORDER MANAGEMENT SYSTEM    \n");
        printf("=========================================\n");
        printf("1. Display Menu\n");
        printf("2. Place New Order (Kitchen Queue)\n");
        printf("3. Update Order Status\n");
        printf("4. Display Kitchen Queue & Orders\n");
        printf("5. Generate Bill & Checkout\n");
        printf("6. View Reports & Analytics\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                displayMenu();
                break;
            case 2:
                placeOrder();
                break;
            case 3:
                updateOrderStatus();
                break;
            case 4:
                displayQueue();
                break;
            case 5:
                generateBill();
                break;
            case 6:
                showAnalytics();
                break;
            case 7:
                printf("\nExiting program. Have a great day!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}
void displayMenu() {
    printf("\n--- RESTAURANT MENU ---\n");
    printf("%-5s %-20s %-10s\n", "ID", "Item Name", "Price (INR)");
    printf("------------------------------------\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%-5d %-20s %-10.2f\n", menu[i].id, menu[i].name, menu[i].price);
    }
}
void placeOrder() {
    if (orderCount >= MAX_ORDERS) {
        printf("\nKitchen Queue is full! Cannot take more orders right now.\n");
        return;
    }

    displayMenu();
    int itemId, qty, table, hour;

    printf("\nEnter Table Number: ");
    scanf("%d", &table);
    printf("Enter Menu Item ID: ");
    scanf("%d", &itemId);
    printf("Enter Quantity: ");
    scanf("%d", &qty);
    printf("Enter Current Hour (0-23): ");
    scanf("%d", &hour);

    if (hour < 0 || hour > 23) {
        hour = 12; 
    }
    int foundIndex = -1;
    for (int i = 0; i < menuSize; i++) {
        if (menu[i].id == itemId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("\nInvalid Item ID selected!\n");
        return;
    }
    rear++;
    orderQueue[rear].orderId = 1000 + rear;
    orderQueue[rear].tableNo = table;
    orderQueue[rear].itemId = itemId;
    strcpy(orderQueue[rear].itemName, menu[foundIndex].name);
    orderQueue[rear].quantity = qty;
    orderQueue[rear].totalPrice = menu[foundIndex].price * qty;
    strcpy(orderQueue[rear].status, "Preparing");
    orderQueue[rear].hour = hour;
    menu[foundIndex].orderCount += qty;
    hourlyOrders[hour] += qty;
    orderCount++;

    printf("\nOrder successfully placed! Assigned Order ID: %d\n", orderQueue[rear].orderId);
}
void updateOrderStatus() {
    if (front > rear || orderCount == 0) {
        printf("\nNo active orders in the queue.\n");
        return;
    }
    displayQueue();
    int targetId, found = 0;
    printf("\nEnter Order ID to update: ");
    scanf("%d", &targetId);
    for (int i = front; i <= rear; i++) {
        if (orderQueue[i].orderId == targetId) {
            found = 1;
            printf("Current Status: %s\n", orderQueue[i].status);
            printf("Select new status:\n");
            printf("1. Preparing\n2. Ready\n3. Served\n4. Cancelled\n");
            int choice;
            scanf("%d", &choice);
            switch (choice) {
                case 1: strcpy(orderQueue[i].status, "Preparing"); break;
                case 2: strcpy(orderQueue[i].status, "Ready"); break;
                case 3: strcpy(orderQueue[i].status, "Served"); break;
                case 4: 
                    strcpy(orderQueue[i].status, "Cancelled"); 
                    break;
                default: printf("Invalid choice. Status unchanged.\n"); return;
            }
            printf("Order status updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("\nOrder ID not found!\n");
    }
}
void displayQueue() {
    if (front > rear || orderCount == 0) {
        printf("\n--- Kitchen Queue is Empty ---\n");
        return;
    }
    printf("\n-------------------------------------------------------------\n");
    printf("%-10s %-8s %-18s %-8s %-12s %-10s\n", "Order ID", "Table", "Item", "Qty", "Status", "Price");
    printf("-------------------------------------------------------------\n");
    for (int i = front; i <= rear; i++) {
        printf("%-10d %-8d %-18s %-8d %-12s ₹%-9.2f\n", 
            orderQueue[i].orderId, 
            orderQueue[i].tableNo, 
            orderQueue[i].itemName, 
            orderQueue[i].quantity, 
            orderQueue[i].status, 
            orderQueue[i].totalPrice);
    }
    printf("-------------------------------------------------------------\n");
}

// 5. Generate Bill & Checkout (Processes completed/served orders)
void generateBill() {
    if (front > rear || orderCount == 0) {
        printf("\nNo active orders to bill.\n");
        return;
    }

    int targetTable;
    printf("\nEnter Table Number to generate bill: ");1
    scanf("%d", &targetTable);

    float subtotal = 0.0;
    int itemsFound = 0;

    printf("\n=========================================\n");
    printf("           ITEMIZED INVOICE              \n");
    printf("=========================================\n");
    printf("Table Number: %d\n", targetTable);
    printf("-----------------------------------------\n");
    printf("%-20s %-5s %-10s\n", "Item", "Qty", "Price");
    printf("-----------------------------------------\n");

    for (int i = front; i <= rear; i++) {
        if (orderQueue[i].tableNo == targetTable && strcmp(orderQueue[i].status, "Cancelled") != 0) {
            printf("%-20s %-5d ₹%-9.2f\n", orderQueue[i].itemName, orderQueue[i].quantity, orderQueue[i].totalPrice);
            subtotal += orderQueue[i].totalPrice;
            itemsFound = 1;
        }
    }

    if (!itemsFound) {
        printf("No valid/active orders found for Table %d.\n", targetTable);
        printf("=========================================\n");
        return;
    }

    float tax = subtotal * TAX_RATE;
    float service = subtotal * SERVICE_CHARGE;
    float grandTotal = subtotal + tax + service;

    printf("-----------------------------------------\n");
    printf("Subtotal:       ₹%.2f\n", subtotal);
    printf("GST (5%%):       ₹%.2f\n", tax);
    printf("Service Charge: ₹%.2f\n", service);
    printf("========================================-\n");
    printf("GRAND TOTAL:    ₹%.2f\n", grandTotal);
    printf("========================================-\n");

    // Add to daily revenue
    dailyRevenue += grandTotal;

    // Remove billed items from queue tracking (advance front or clear table orders)
    // For simplicity, we flag or let them clear out.
    char confirm;
    printf("Confirm payment and close orders for Table %d? (y/n): ", targetTable);
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        // Clear orders for this table by marking status or shifting queue
        for (int i = front; i <= rear; i++) {
            if (orderQueue[i].tableNo == targetTable) {
                strcpy(orderQueue[i].status, "Billed/Closed");
            }
        }
        printf("Payment processed successfully. Thank you!\n");
    }
}

// 6. Reports & Analytics (Sorting, Busiest hours, Most-ordered items, Revenue)
void showAnalytics() {
    // Copy menu items to a temporary array to sort them by frequency (most-ordered)
    MenuItem tempMenu[MAX_MENU];
    for (int i = 0; i < menuSize; i++) {
        tempMenu[i] = menu[i];
    }

    // Bubble Sort for Most-Ordered Items (Descending Order)
    for (int i = 0; i < menuSize - 1; i++) {
        for (int j = 0; j < menuSize - i - 1; j++) {
            if (tempMenu[j].orderCount < tempMenu[j + 1].orderCount) {
                MenuItem temp = tempMenu[j];
                tempMenu[j] = tempMenu[j + 1];
                tempMenu[j + 1] = temp;
            }
        }
    }

    // Find Busiest Hour
    int maxHour = 0;
    int maxOrdersCount = hourlyOrders[0];
    for (int i = 1; i < 24; i++) {
        if (hourlyOrders[i] > maxOrdersCount) {
            maxOrdersCount = hourlyOrders[i];
            maxOrdersCount = hourlyOrders[i];
            maxHour = i;
        }
    }

    printf("\n=========================================\n");
    printf("           RESTAURANT REPORTS            \n");
    printf("=========================================\n");
    printf("Total Daily Revenue: ₹%.2f\n", dailyRevenue);
    printf("Busiest Hour:        %02d:00 - %02d:00 (%d items ordered)\n", maxHour, (maxHour + 1) % 24, maxOrdersCount);
    
    printf("\n--- Most Ordered Items Ranking ---\n");
    printf("%-5s %-20s %-15s\n", "Rank", "Item Name", "Total Quantity");
    printf("-----------------------------------------\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%-5d %-20s %-15d\n", i + 1, tempMenu[i].name, tempMenu[i].orderCount);
    }
    printf("=========================================\n");
}
