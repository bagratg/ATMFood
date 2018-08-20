# ATMFood

ATMFood is a client-server application I've been developing for the past two months to learn Qt.
The idea is to automate ordering food in offices. Users can add items to their food cart.
The application tracks everyone's order and combines it in a single list.

The project consists of two parts ATMFoodClient and ATMFoodServer. Both are separate Qt projects.

They makes use of Qt's networking classes (QTCPServer and QTCPSocket).
The backend talks to the clients with a custom, JSON-based protocol and stores the information inside an SQLite database.

