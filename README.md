# BiblioBrain – Library Management System #
## Project Description
**BiblioBrain** is a backend **Library Management System** implemented using **C++** and the **Crow framework**.  
The system provides a modern and efficient solution for managing library operations through **RESTful APIs** using **JSON-based communication**.

---

## Project Objectives
- Build a RESTful library system using C++
- Apply backend development concepts using the Crow framework
- Implement secure user authentication and role-based authorization
- Manage library data efficiently using structured APIs
- Follow clean architectural design principles (MVC)

---

## Technologies & Tools
- **Programming Language:** C++
- **Framework:** Crow
- **Architecture:** MVC (Model–View–Controller)
- **API Style:** RESTful APIs
- **Data Format:** JSON
- **Testing Tool:** Postman

---

## Features
- User authentication with role-based authorization  
  (Admin – Librarian – Regular User)
- Book management:
  - Add books
  - Update book information
  - Delete books
  - Retrieve book details
- Search and filter books by:
  - Title
  - Author
  - Category
- Inventory management:
  - Track total copies
  - Track available copies
- Borrowing and returning operations with automatic updates to book availability

---

## OOP Concepts Used
- Classes and Objects for:
  -- Users
  -- Books
  -- Borrowing records
- Encapsulation for data protection
- Separation of responsibilities across system components

---

< System Architecture >
The project follows the **Model–View–Controller (MVC)** architecture:

- **Models:** Handle data representation and business logic  
- **Controllers:** Manage API endpoints and application logic  
- **Views:** JSON-based API responses  

This structure enhances maintainability and scalability.

---

## How to Run the Project

< Prerequisites >
Make sure the following are installed on your system:
- C++ compiler (GCC or MinGW)
- CMake
- Git
- Crow framework
- Postman (for API testing)

 < Steps to Run >
1. Clone the repository:
   ```bash
   git clone https://github.com/USERNAME/REPOSITORY_NAME.git
   
2. Navigate to the project directory:
cd BiblioBrain

3. Create a build directory:
mkdir build
cd build

4. Configure the project using CMake:
cmake ..

5. Build the project:
cmake --build .

6. Run the server:
./BiblioBrain

7. The server will start on the default port (e.g., http://localhost:18080).

8. Use Postman to test the API endpoints.

< API Testing >
--> All APIs were tested using Postman to ensure:
-Correct request handling
-Valid JSON responses
-Proper authorization behavior

< Team Members >
--> Name:
1. Mariem Ahemed Attia Ahmed
2. Omnia Ehab Mohamed Asaad
3. Shorouk Muhammad Ammar
4. Toka Osama El Aggan

--> Conclusion
< This project demonstrates how C++ can be used to build scalable backend systems using RESTful APIs.
By applying MVC architecture and clean API design, BiblioBrain delivers a reliable and maintainable library management solution. >

