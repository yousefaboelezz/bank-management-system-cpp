# 🏦 Bank Management System (Console Application)

A robust, modular C++ console application that simulates a comprehensive banking system. The project demonstrates core software engineering principles, clean data modeling, custom file-based data persistence, and an advanced **Bitwise Role-Based Access Control (RBAC)** security system.

---

##  Key Features

### 1. Client Management (CRUD)
- **Show Client List:** Tabular view of all registered clients with formatted balances.
- **Add New Client:** Real-time account number validation to prevent duplicates.
- **Delete Client:** Safe soft-delete pattern before flushing updates to storage.
- **Update Client Info:** Dynamic record modification for client credentials and contact details.
- **Find Client:** Instant record lookup by account number.

### 2. Transactions Engine
- **Deposit & Withdraw:** Balance validation and immediate atomic updates.
- **Withdrawal Guard:** Automated checks preventing overdraft beyond available balance.
- **Total Balances:** Real-time calculation and formatted display of total bank assets.

### 3. User Management & Security
- **Authentication System:** Secure credential check before accessing the main system.
- **Granular Permissions (Bitwise RBAC):** Each system feature is mapped to a power-of-two bitmask, enabling fine-grained permission assignments using bitwise logic.
- **Admin Protection:** Built-in safeguards preventing the deletion of the primary `Admin` account.
- **Access Control Enforcement:** Automated permission gates returning clear `Access Denied` feedback on unauthorized operations.

---

##  Concepts & Engineering Highlights

- **Bitwise Operations for Permissions:** Utilizes bitwise `&` (AND) operations against powers of two to evaluate access permissions with high performance and minimal memory footprint.
- **Flat-File Serialization:** Custom parsing and serialization algorithms (`#//#` delimiter) converting between plain text files and in-memory structures without third-party libraries.
- **State Synchronization:** Uses vectors (`std::vector<sClient>`, `std::vector<sUser>`) for in-memory operations and writes back changes reliably using standard file streams (`std::fstream`).
- **Clean Architecture:** Strict separation between presentation logic (Console UI) and core business/data access logic.

---

##  Tech Stack

- **Language:** C++ (C++11 or higher)
- **Standard Libraries:** `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<iomanip>`
- **Storage:** Text-based flat files (`Clients.txt`, `Users.txt`)
