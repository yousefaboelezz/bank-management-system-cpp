# 🏦 Bank Management System (Console Application)

A robust, modular C++ console application designed for bank client operations, financial transaction processing, and user administration. The project demonstrates core software engineering principles, clean data modeling, persistent text-file storage, transaction validation logic, and an advanced **Bitwise Role-Based Access Control (RBAC)** security system.

---

##  Key Features

### 1. Client Management (CRUD)
- **Show Client List:** Tabular view of all registered clients formatted with structured columns and active balances.
- **Add New Client:** Record creation with duplicate account number validation and initial balance assignment.
- **Delete Client:** Safe soft-delete pattern before flushing updates back to persistent storage.
- **Update Client Info:** Dynamic record modification for client credentials and contact details.
- **Find Client:** Instant record search and lookup by account number.

### 2. Transactions Engine (Financial Operations)
- **Deposit:** Adds funds to any existing client account with immediate balance update and file synchronization.
- **Withdrawal:** Executes cash withdrawals with strict validation checks preventing overdrafts (ensures requested amount does not exceed available balance).
- **Total Balances Screen:** Global calculation and tabular listing displaying all client names alongside their balances, accompanied by the bank's aggregate total funds.

### 3. User Management & Security
- **Authentication System:** Secure login gate validating username and credentials before granting access to the system.
- **Granular Permissions (Bitwise RBAC):** Each system feature is mapped to a bitmask power-of-two, enabling fine-grained permission assignments using bitwise logic.
- **Admin Protection:** Built-in safeguards preventing the deletion or permission stripping of the primary `Admin` account.
- **Access Control Enforcement:** Automated permission gates returning clear `Access Denied` feedback on unauthorized operations.

---

##  Concepts & Engineering Highlights

- **Bitwise Operations for Permissions:** Utilizes bitwise `&` (AND) operations against bitmasks to evaluate access permissions with high performance and zero memory overhead.
- **Balance Validation & Edge Cases:** Defends against negative withdrawals and amounts exceeding current client balances.
- **Flat-File Serialization:** Custom parsing and serialization algorithms (`#//#` delimiter) converting between plain text files and in-memory structures without external libraries.
- **State Synchronization:** Uses standard vectors (`std::vector<sClient>`, `std::vector<sUser>`) for in-memory operations and writes back changes reliably using file streams (`std::fstream`).
- **Clean Architecture:** Strict separation between presentation logic (Console UI) and core business/data logic.

---

##  Tech Stack

- **Language:** C++ (C++11 or higher)
- **Standard Libraries:** `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<iomanip>`
- **Storage:** Text-based flat files (`Clients.txt`, `Users.txt`)

---

##  Getting Started

### Prerequisites
A modern C++ compiler (MSVC, GCC, or Clang) supporting C++11 or later.

