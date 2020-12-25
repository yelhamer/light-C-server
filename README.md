# light-C-server
A C implementation of a bare bone tcp server that supports multiple simultaneous connections.
The default action of the server is that of an ftp server, it interprets received input as a filename and serves the contents of that file to the client that requested it (so be cautious when running this as a user which has access to sensitive files). The default action can be changed by modifying the "action()" function found in "source/action.c".

# TODO:
* Add thread support (start a new thread for each event that pops up)

