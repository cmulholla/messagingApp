# messagingApp
A messaging test app that uses threading, SFML, and TCP communication.
The app only works over LAN but could theoretically work over a port-forwarded server.
This uses only the console, and so if one person sends a message when the other person is still typing then it will break up what they're trying to type, but they can still send it properly.
This is simply a proof of concept that I could write a program that uses threading with 2-way TCP communication.
If given more time, x amount of connections could be done with having a thread listen for incoming connections, as well as sending out messages the server recieves to other clients with flags in a list.
A better way to do this would probably just be to use UDP, but eh whatever. I might remake this using UDP one day to create chaotic LAN chatrooms.
