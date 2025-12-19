# Expense Tracker – C Programming (PF Final Project)

![Language](https://img.shields.io/badge/Language-C-blue)
![Project](https://img.shields.io/badge/Project-Academic-success)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)
![Platform](https://img.shields.io/badge/Platform-CLI-lightgrey)

A **console-based Expense Tracker application written in C**, developed as a **Final Semester Project for Programming Fundamentals (CT-175)**. The project focuses on applying core C concepts to solve a real-world problem: **personal expense and income management with persistent storage**.

---

## 📌 Project Overview

The Expense Tracker allows multiple users to **sign up, log in, and manage their financial data independently**. Each user can record income, add categorized expenses, and generate summaries — all while ensuring expenses do not exceed income.

Data is stored persistently using **CSV and TXT files**, demonstrating practical file handling and structured programming in C.

---

## ✨ Key Features

- 👤 **Multi-user account system** (Sign Up / Login)
- 💰 **Income management** with overspending prevention
- 🧾 **Expense recording** (item, category, amount, date)
- 📊 **Expense summary & analysis**
  - Total income, expenses, remaining balance
  - Category-wise breakdown
  - Top spending items
- 🗑️ Delete most recent expense
- 💾 **Persistent storage** using CSV & TXT files
- 📂 Separate data file for each user

---

## 🛠️ Core Concepts Demonstrated

- C Programming Fundamentals
- Structures
- File Handling (CSV & TXT)
- Modular programming using functions
- Conditional logic & loops
- Basic data analysis
- Menu-driven programs

---

## 📁 File Structure

```
Expense-Tracker-C/
│
├── Final.c                 # Main source code
├── PF-Project-Report.pdf   # Detailed project documentation
├── users.txt               # Stores user credentials
├── all_incomes.csv         # Stores income data of users
├── <username>.csv          # Expense data (auto-created per user)
├── README.md               # Project documentation
```

> ⚠️ Note: `<username>.csv` files are automatically generated when a user adds expenses.
>           The added demo user csv file - is not compatible to call data into the program directly - its just there to view how the file will look...

---

## ▶️ How to Run

1. Clone the repository:

```bash
git clone https://github.com/your-username/Expense-Tracker-C.git
```

2. Compile the program:

```bash
gcc Final.c -o expense_tracker
```

3. Run the executable:

```bash
./expense_tracker
```

---

## 👥 Team Contributions

- **Muhammad Mahad Amir** — Program architecture and complete menu-driven system
- **Faaiq Ahmed** — File handling, persistent storage, and summary analytics
- **Syed Muhammad Areeb** — User authentication, date handling, and input validation

---

## 🔮 Future Improvements

- Predefined & user-defined expense categories
- Advanced sorting and analytical reports
- Enhanced delete and filtering options
- Improved CLI readability or GUI-based version

##

---

## 📬 Feedback

Suggestions and feedback are always welcome. This project was built as a learning experience and can be extended further with advanced concepts.

