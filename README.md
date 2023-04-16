# MAX485 gateway

__Description__-

* On gateway side,
    * Read data over RS485 communication protocol and upload data to server over MQTT protocol.
* On server side, 
    * Recieve data data over MQTT protocol.
    * Store data on MongoDB server.
    * Host a UI with login access and data display features.
    * Send critical data to cloud.
    * __Seperate serverless Docker containers for backend, frontend and mongoDB.__
* On cloud side [AWS],
   * Implementation of API gateway.
   * Lambda methods to process on recieved data.
   * DynamoDB to store received data.
   * AWS SNS to inform user about critical data using EMAIL.

---------------------------------------------------------------

__Contributors__:


* Shweta Mahale.
* Vrushali Lobhe.
* Saurav Pimpalkar.
* Rohit Akurdekar.
