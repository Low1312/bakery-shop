#include <cstdio>
#include <cstring>
#include <cstdlib>

#define MAX_ITEMS 100
#define NAME_LEN 50
#define MAX_ORDERS 100

struct BakeryItem {
    int id;
    char name[NAME_LEN];
    char category[NAME_LEN];
    double price;
    int quantity;
    char expiryDate[NAME_LEN];
};

struct Order {
    char customerName[NAME_LEN];
    int productId;
    int quantity;
};

BakeryItem bakery[MAX_ITEMS];
int bakeryCount = 0;

Order orders[MAX_ORDERS];
int orderCount = 0;

// Manual string to int
int strToInt(const char* str) {
    int res = 0, i = 0, sign = 1;
    if (str[0] == '-') { sign = -1; i++; }
    for (; str[i] != '\0' && str[i] != '\n'; i++) {
        res = res * 10 + (str[i] - '0');
    }
    return res * sign;
}

// Manual string to double
double strToDouble(const char* str) {
    double res = 0.0, frac = 0.0, div = 1.0;
    int i = 0, sign = 1, point = 0;
    if (str[0] == '-') { sign = -1; i++; }
    for (; str[i] != '\0' && str[i] != '\n'; i++) {
        if (str[i] == '.') { point = 1; i++; break; }
        res = res * 10 + (str[i] - '0');
    }
    if (point) {
        for (; str[i] != '\0' && str[i] != '\n'; i++) {
            frac = frac * 10 + (str[i] - '0');
            div *= 10;
        }
        res += frac / div;
    }
    return res * sign;
}

// Manual int to string
void intToStr(int num, char* str) {
    int i = 0, isNeg = 0;
    if (num < 0) { isNeg = 1; num = -num; }
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num);
    if (isNeg) str[i++] = '-';
    str[i] = '\0';
    // reverse
    for (int j = 0; j < i / 2; j++) {
        char t = str[j]; str[j] = str[i - j - 1]; str[i - j - 1] = t;
    }
}

// Manual double to string (2 decimal places)
void doubleToStr(double num, char* str) {
    int intPart = (int)num;
    double fracPart = num - intPart;
    if (fracPart < 0) fracPart = -fracPart;
    char intStr[20];
    intToStr(intPart, intStr);
    int len = strlen(intStr);
    strcpy(str, intStr);
    str[len] = '.';
    fracPart *= 100;
    int frac = (int)(fracPart + 0.5);
    str[len + 1] = frac / 10 + '0';
    str[len + 2] = frac % 10 + '0';
    str[len + 3] = '\0';
}

// Load from file
void loadFromFile(const char* filename) {
    bakeryCount = 0;
    FILE* file = fopen(filename, "r");
    if (!file) return;
    char line[300];
    while (fgets(line, sizeof(line), file)) {
        char* token;
        token = strtok(line, ",");
        if (!token) continue;
        bakery[bakeryCount].id = strToInt(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(bakery[bakeryCount].name, token, NAME_LEN - 1);
        bakery[bakeryCount].name[NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(bakery[bakeryCount].category, token, NAME_LEN - 1);
        bakery[bakeryCount].category[NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        bakery[bakeryCount].price = strToDouble(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        bakery[bakeryCount].quantity = strToInt(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(bakery[bakeryCount].expiryDate, token, NAME_LEN - 1);
        bakery[bakeryCount].expiryDate[NAME_LEN - 1] = '\0';

        // Remove newline from expiryDate if present
        int n = strlen(bakery[bakeryCount].expiryDate);
        if (n > 0 && bakery[bakeryCount].expiryDate[n - 1] == '\n')
            bakery[bakeryCount].expiryDate[n - 1] = '\0';

        bakeryCount++;
        if (bakeryCount >= MAX_ITEMS) break;
    }
    fclose(file);
}

// Save to file
void saveToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    for (int i = 0; i < bakeryCount; i++) {
        char priceStr[20], idStr[10], qtyStr[10];
        intToStr(bakery[i].id, idStr);
        doubleToStr(bakery[i].price, priceStr);
        intToStr(bakery[i].quantity, qtyStr);
        fprintf(file, "%s,%s,%s,%s,%s,%s\n", idStr, bakery[i].name, bakery[i].category, priceStr, qtyStr, bakery[i].expiryDate);
    }
    fclose(file);
}

// =========================
//   ADMIN/STAFF FUNCTIONS
// =========================
void addItem(const char* filename) {
    if (bakeryCount >= MAX_ITEMS) {
        printf("Bakery is full!\n");
        return;
    }
    BakeryItem item;
    char temp[100];
    printf("Enter item ID: ");
    fgets(temp, sizeof(temp), stdin);
    item.id = strToInt(temp);

    printf("Enter item name: ");
    fgets(item.name, sizeof(item.name), stdin);
    int n = strlen(item.name);
    if (n > 0 && item.name[n - 1] == '\n') item.name[n - 1] = '\0';

    printf("Enter item category: ");
    fgets(item.category, sizeof(item.category), stdin);
    n = strlen(item.category);
    if (n > 0 && item.category[n - 1] == '\n') item.category[n - 1] = '\0';

    printf("Enter item price: ");
    fgets(temp, sizeof(temp), stdin);
    item.price = strToDouble(temp);

    printf("Enter item quantity: ");
    fgets(temp, sizeof(temp), stdin);
    item.quantity = strToInt(temp);

    printf("Enter expiry date: ");
    fgets(item.expiryDate, sizeof(item.expiryDate), stdin);
    n = strlen(item.expiryDate);
    if (n > 0 && item.expiryDate[n - 1] == '\n') item.expiryDate[n - 1] = '\0';

    bakery[bakeryCount++] = item;
    saveToFile(filename);
    printf("Item added and saved successfully!\n");
}

void displayItems(int showExpiry) {
    printf("\n--- Bakery Menu ---\n");
    if (showExpiry) {
        printf("%-5s %-22s %-15s %-10s %-10s %-15s\n", "ID", "Product Name", "Category", "Price", "Quantity", "Expiry Date");
    } else {
        printf("%-5s %-22s %-15s %-10s %-10s\n", "ID", "Product Name", "Category", "Price", "Quantity");
    }
    for (int i = 0; i < bakeryCount; i++) {
        char priceStr[20];
        doubleToStr(bakery[i].price, priceStr);
        if (showExpiry) {
            printf("%-5d %-22s %-15s $%-9s %-10d %-15s\n",
                bakery[i].id,
                bakery[i].name,
                bakery[i].category,
                priceStr,
                bakery[i].quantity,
                bakery[i].expiryDate);
        } else {
            printf("%-5d %-22s %-15s $%-9s %-10d\n",
                bakery[i].id,
                bakery[i].name,
                bakery[i].category,
                priceStr,
                bakery[i].quantity);
        }
    }
    printf("--------------------\n");
}

void updateItem(const char* filename) {
    char temp[100];
    printf("Enter the ID of the item to update: ");
    fgets(temp, sizeof(temp), stdin);
    int id = strToInt(temp);
    int found = 0;
    for (int i = 0; i < bakeryCount; i++) {
        if (bakery[i].id == id) {
            found = 1;
            printf("Enter new name (current: %s): ", bakery[i].name);
            fgets(bakery[i].name, sizeof(bakery[i].name), stdin);
            int n = strlen(bakery[i].name);
            if (n > 0 && bakery[i].name[n - 1] == '\n') bakery[i].name[n - 1] = '\0';

            printf("Enter new category (current: %s): ", bakery[i].category);
            fgets(bakery[i].category, sizeof(bakery[i].category), stdin);
            n = strlen(bakery[i].category);
            if (n > 0 && bakery[i].category[n - 1] == '\n') bakery[i].category[n - 1] = '\0';

            printf("Enter new price (current: %.2f): ", bakery[i].price);
            fgets(temp, sizeof(temp), stdin);
            bakery[i].price = strToDouble(temp);

            printf("Enter new quantity (current: %d): ", bakery[i].quantity);
            fgets(temp, sizeof(temp), stdin);
            bakery[i].quantity = strToInt(temp);

            printf("Enter new expiry date (current: %s): ", bakery[i].expiryDate);
            fgets(bakery[i].expiryDate, sizeof(bakery[i].expiryDate), stdin);
            n = strlen(bakery[i].expiryDate);
            if (n > 0 && bakery[i].expiryDate[n - 1] == '\n') bakery[i].expiryDate[n - 1] = '\0';

            saveToFile(filename);
            printf("Item updated and saved successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Item with ID %d not found.\n", id);
    }
}

void staffModule(const char* filename) {
    system("cls");
    int choice;
    char temp[10];
    do {
        printf("\nStaff/Admin Menu\n");
        printf("1. Edit Product\n");
        printf("2. Display Products\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        fgets(temp, sizeof(temp), stdin);
        choice = strToInt(temp);
        switch (choice) {
            case 1: updateItem(filename); break;
            case 2: displayItems(1); break;
            case 3: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
}

// =========================
//   CUSTOMER FUNCTIONS
// =========================
void placeOrder() {
    if (orderCount >= MAX_ORDERS) {
        printf("Order list is full!\n");
        return;
    }
    char temp[100];
    Order order;
    printf("Enter your name: ");
    fgets(order.customerName, sizeof(order.customerName), stdin);
    int n = strlen(order.customerName);
    if (n > 0 && order.customerName[n - 1] == '\n') order.customerName[n - 1] = '\0';

    while (1) {
        displayItems(0);
        printf("Enter the product ID you want to order (0 to finish): ");
        fgets(temp, sizeof(temp), stdin);
        int productId = strToInt(temp);
        if (productId == 0) break;

        // Check if product exists
        int found = 0;
        for (int i = 0; i < bakeryCount; i++) {
            if (bakery[i].id == productId) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Product ID not found.\n");
            continue;
        }

        printf("Enter quantity: ");
        fgets(temp, sizeof(temp), stdin);
        int quantity = strToInt(temp);

        order.productId = productId;
        order.quantity = quantity;
        if (orderCount < MAX_ORDERS) {
            orders[orderCount++] = order;
            printf("Order placed successfully!\n");
        } else {
            printf("Order list is full!\n");
            break;
        }
    }
}

void displayOrders() {
    printf("\n--- Customer Orders ---\n");
    printf("%-5s %-22s %-10s %-10s %-10s\n", "ID", "Name", "Price", "Quantity", "Total");
    double grandTotal = 0.0;
    for (int i = 0; i < orderCount; i++) {
        // Find product name and price
        const char* productName = "Unknown";
        double price = 0.0;
        for (int j = 0; j < bakeryCount; j++) {
            if (bakery[j].id == orders[i].productId) {
                productName = bakery[j].name;
                price = bakery[j].price;
                break;
            }
        }
        double total = price * orders[i].quantity;
        grandTotal += total;
        printf("%-5d %-22s $%-9.2f %-10d $%-9.2f\n",
            orders[i].productId, productName, price, orders[i].quantity, total);
    }
    printf("Grand Total: $%.2f\n", grandTotal);
    printf("-----------------------\n");
}

void editOrder() {
    if (orderCount == 0) {
        printf("No orders to edit.\n");
        return;
    }
    char temp[100];
    printf("Enter your order number to edit (1-%d): ", orderCount);
    fgets(temp, sizeof(temp), stdin);
    int orderNum = strToInt(temp);
    if (orderNum < 1 || orderNum > orderCount) {
        printf("Invalid order number.\n");
        return;
    }
    Order* order = &orders[orderNum - 1];
    printf("Edit your name (current: %s): ", order->customerName);
    fgets(order->customerName, sizeof(order->customerName), stdin);
    int n = strlen(order->customerName);
    if (n > 0 && order->customerName[n - 1] == '\n') order->customerName[n - 1] = '\0';

    printf("Edit quantity (current: %d): ", order->quantity);
    fgets(temp, sizeof(temp), stdin);
    order->quantity = strToInt(temp);

    printf("Order updated successfully!\n");
}

void customerModule() {
    system("cls");
    int choice;
    char temp[10];
    do {
        printf("\nCustomer Menu\n");
        printf("1. Display Products\n");
        printf("2. Place Order\n");
        printf("3. Edit My Order\n");
        printf("4. Display My Orders\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        fgets(temp, sizeof(temp), stdin);
        choice = strToInt(temp);
        switch (choice) {
            case 1: displayItems(0); break;
            case 2: placeOrder(); break;
            case 3: editOrder(); break;
            case 4: displayOrders(); break;
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}

int main() {
    const char* filename = "bakerymenu.txt";
    loadFromFile(filename);
    int choice;
    char temp[10];
    do {
        system("cls");
        printf("\nBakery Shop System\n");
        printf("1. Customer\n");
        printf("2. Staff/Admin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        fgets(temp, sizeof(temp), stdin);
        choice = strToInt(temp);
        switch (choice) {
            case 1: customerModule(); break;
            case 2: staffModule(filename); break;
            case 3: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
    return 0;
}

