#include "serverTypes.h"

int main() {
Test = 1;
    Hash::Table<Server::Client, char> main_table;

    //Server::Client1
    Server::Client Rick;
    Rick.uid = 8;
    Rick.temperature = 57;
    //

    //Server::Client2
    Server::Client Roger;
    Roger.uid = 38;
    Roger.temperature = 28;
    //

    main_table.addElement(Rick, Rick.uid);
    main_table.addElement(Roger, Roger.uid);
    
    Server::Client* RickPtr = (Server::Client*)main_table.getElement(Rick.uid);
    printf("Ricks uid stored in the table: %d\n", RickPtr->uid);
}

