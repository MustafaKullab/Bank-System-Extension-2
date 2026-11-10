# Bank System – Extension 2 (C++)

A console-based Bank Management System developed in C++ as part of my programming learning journey.

This project extends the previous Bank System by introducing **user authentication, user management, and permission-based access control**.

The project was implemented independently as a practical exercise inspired by the **Programming Advices** courses by Mohammed Abu-Hadhoud.

## Features

### Client Management
- List all clients
- Add new clients
- Delete existing clients
- Update client information
- Find clients by account number

### Banking Transactions
- Deposit money
- Withdraw money with balance validation
- Display total balances

### User Management
- List system users
- Add users with custom permissions
- Delete users with confirmation
- Update user passwords and permissions
- Find users by username
- Prevent deletion of the Admin account

### Authentication & Permissions
- Login using a username and password
- Retry login when credentials are incorrect
- Logout and log in as another user
- Control access to system operations using bitwise permissions
- Display an Access Denied message for unauthorized operations

## Permission System

The project uses bitwise flags to represent user permissions.

| Permission | Value |
|---|---:|
| List Clients | 1 |
| Add Client | 2 |
| Delete Client | 4 |
| Update Client | 8 |
| Find Client | 16 |
| Transactions | 32 |
| Manage Users | 64 |
| Full Access | -1 |

Multiple permissions can be combined using the bitwise OR (`|`) operator and checked using the bitwise AND (`&`) operator.

## Technologies Used

- **Language:** C++
- **STL:** `vector`, `string`
- **File Handling:** `fstream`
- **Console Formatting:** `iomanip`
- **Data Storage:** Text files

## How to Run

1. Clone or download this repository.
2. Open the C++ source file in your preferred IDE.
3. Compile the project using a C++ compiler.
4. Create the required data files in the program's working directory.
5. Run the application and log in with a test account.

The project uses Windows-specific console commands such as `system("cls")` and `system("pause>0")`.

### Data Files

**Users.txt**

Stores user credentials and permission values using the following format:

`Username#//#Password#//#Permissions`

**Client.txt**

Stores client information using the following format:

`AccountNumber#//#PinCode#//#Name#//#Phone#//#Balance`

For testing, you can create a local administrator entry in `Users.txt`:

`Admin#//#your_test_password#//#-1`

Use a test password and do not upload real credentials or customer information.

## Concepts Practiced

Through this project, I practiced:

- Structures and enumerations
- Functions and modular programming
- Vectors and file handling
- Searching, adding, updating, and deleting records
- User authentication
- Bitwise operators and permission flags
- Input validation and handling user confirmations

## Learning Purpose

This is an educational project focused on strengthening my understanding of C++ and applying programming concepts in a larger, multi-feature application.

It is not intended for real banking use. Passwords are stored as plain text, and the application does not provide production-grade security.

## Acknowledgments

Inspired by the **Programming Advices** programming courses by **Mohammed Abu-Hadhoud**.

The code in this repository represents my own learning implementation of the project requirements.
