#include "server.h"

bool test = false;
void debug(const char words[]) {
    if(test == true)
        printf("[#]%s\n", words);
}

struct Node* first = 0;
struct Node* last = 0;

int stringCompare(uint8_t* str1, uint8_t* str2) {
    int i = 0;
    while(true) {
        if(str1[i] != str2[i]) {
            return 0;
        }
        else if(str1[i] == '\0' || str1[i] == 0) {
            return 1;
        }
        else{
            i += 1;
        }
    }
}

struct Node* searchNodes(struct Node* node, uint8_t* name) {
    if(stringCompare(node->name, name) == 1) {
        return node;
    } else if(node->next == 0) {
        return 0;
    } else {
        return searchNodes(node->next, name);
    }
}

struct Node* printAllNodes(struct Node* node) {
    if(node == last) {
        printf("Name: %s\nEnd Of Nodes\n", node->name);
        return last;
    } else if(node->next != 0 && node != last) {
        printf("Name: %s\n", node->name);
        return printAllNodes(node->next);
    } else {
        perror("Break in Linked node list");
        return 0;
    }
}

void copyName(uint8_t* name, uint8_t* nameToCopy, int index) {
    int i = 0;
    while(i < 20) {
        if(nameToCopy[i] == '\0')
            break;

        name[index] = nameToCopy[i];
        i += 1;
        index += 1;
    }
}

uint8_t uidCounter;
uint8_t genUID() {
    return (uidCounter += 1);
}

struct KeyValue* findKeyPair(struct KeyValue* dests, uint8_t op) {
    if(dests->op == op) {
        printf("found KeyPair with OP: %d\n", dests->op);
        return dests;
    }
    else if(dests->next == 0) {
        printf("None of this op\n");
        dests->next = (struct KeyValue*)malloc(sizeof(dests));
        dests->next->op = op;
        printf("No KeyPair associated with OP: %d, Created KeyPair\n", op);
        return dests->next;
    }
    else {
        return findKeyPair(dests->next, op);
    }
}

std::vector<uint8_t> nextName(uint8_t* names) {
    int i = 0;
    std::vector<uint8_t> name;

    while(names[i] != ' ' && names[i] != '\0') {
        name.push_back(names[i]);
        i += 1;
    }

    //MUTATION
    names += (i + 2);
    //END OF MUTATION

    return name;
}

struct NodePtr* lastNodePtr(struct NodePtr* value) {
    printf("Node to start searching from: %s\n", value->node->name);
    printf("started looking for last node\n");
    if(value->next == 0) {
        printf("Found the last nodePtr which points to: %s\n", value->node->name);
        return value;
    } else {
        printf("did not find last nodePtr, recursing\n");
        return lastNodePtr(value->next);
    }
}

struct ListWrap searchDests(struct NodePtr* nodePtr, uint8_t* name, uint8_t* uid) {
    printf("Searching dests\n");
    if(name[0] == '\0' && uid[0] != '\0') {
        //search for uid
    } else if(name[0] != '\0' && uid[0] == '\0') {
        struct NodePtr* temp = nodePtr;
        if(stringCompare(temp->node->name, name) == 1) {
                struct ListWrap wrapNodePtr;
                wrapNodePtr.type = 1;
                wrapNodePtr.prev = 0;
                wrapNodePtr.current = temp;
                wrapNodePtr.next = temp->next;
                return wrapNodePtr;
        }
        while(true) {
            if(temp->next == 0) {
                struct ListWrap wrapNodePtr;
                wrapNodePtr.type = 1;
                wrapNodePtr.current = 0;
                printf("Failed to find node in dests\n");
                return wrapNodePtr;
            }
            else if(stringCompare(name, temp->next->node->name) == 1) {
                struct ListWrap wrapNodePtr;
                wrapNodePtr.type = 1;
                wrapNodePtr.prev = temp;
                wrapNodePtr.current = temp->next;
                wrapNodePtr.next = temp->next->next;
                return wrapNodePtr;
            }
            temp = temp->next;
        }
    }

}

//rewrite using SearchDests
int addToDests(struct KeyValue* pair, uint8_t* names, uint8_t* uids) {
    printf("Names still in list\n");
    std::vector<uint8_t> name = nextName(names);
    struct Node* newNode = searchNodes(first, &name[0]);
    struct NodePtr* ptr = (struct NodePtr*)malloc(sizeof(struct NodePtr));
    ptr->node = newNode;
    ptr->next = 0;
    printf("Allocated a new nodePtr that points to: %s\n", ptr->node->name);

    if(pair->value == 0) {
        printf("The list of NodePtr's was 0, initialised\n");
        pair->value = ptr;
    } else {
        struct NodePtr* lastDest = lastNodePtr(pair->value);
        lastDest->next = ptr;
    }

    if(*(names + name.size()) == '\0') {
        printf("Finished all names\n");
        if(uids[0] == '\0') {
            printf("FInished all uid\nReturning state\n");
            return 1;
        }
    }
    return addToDests(pair, names + name.size() + 1, uids);
}

bool checkIfNodeToRemove(struct KeyValue* pair, struct NodePtr* nodePtr, struct Node* nodeToRemove) {
    if(pair->value->node == nodeToRemove) {
        //deallocate nodePtr for node to remove
        pair->value = pair->value->next;
        return true;
    } else if(nodePtr->next->node == nodeToRemove) {
        //deallocate nodePtr->next
        if(nodePtr->next->next == 0) {
            nodePtr->next = 0;
            return true;
        } else{
            nodePtr->next = nodePtr->next->next;
            return true;
        }
    } else {
        return false;
    }
}

//rewrite using SearchDests
int removeFromDests(struct KeyValue* pair, struct NodePtr* nodePtr, uint8_t* names, uint8_t* uids) {
    std::vector<uint8_t> name = nextName(names);
    struct Node* nodeToRemove = searchNodes(first, &name[0]);

    if(checkIfNodeToRemove(pair, nodePtr, nodeToRemove) == true) {
        if(*(names + name.size()) == '\0') {
            printf("Finished all names\n");
            if(uids[0] == '\0') {
                printf("FInished all uid\nReturning state\n");
                return 1;
            }
        }
        return removeFromDests(pair, pair->value, names + name.size() + 1, uids);
    } 
    else if(nodePtr->next == 0) {
        printf("Failed to find node %s in destination list", &name[0]);
        if(*(names + name.size()) == '\0') {
            printf("Finished all names\n");
            if(uids[0] == '\0') {
                printf("FInished all uid\nReturning state\n");
                return 1;
            }
        }
        return removeFromDests(pair, pair->value, names + name.size() + 1, uids);
    }
    else {
        return removeFromDests(pair, nodePtr->next, names, uids);
    }


}

int changeDict(struct Node* node, struct MutateDest dests) {
    //Find key-par
    struct KeyValue* keyValue = findKeyPair(node->DestinationDictionary, dests.op);

    if(dests.sign == '+') {
        addToDests(keyValue, dests.names, dests.uids);
        return 1;
    } else if(dests.sign == '-') {
        removeFromDests(keyValue, keyValue->value, dests.names, dests.uids);
        return 1;
    }
    return 0;
}

//Return uid
uint8_t addNode(struct Join Client, struct sockaddr_in Address, socklen_t len) {
    uint8_t uid = genUID();
    if(last == 0) {
        last = (struct Node*)malloc(sizeof(struct Node));
        //CHeck for failed allocation

        last->next = 0;
        last->uid = uid;
        last->temperature = 100;
        last->Address = Address;
        last->len = len;
        copyName(last->name, Client.name, 0);

        first = last;
    }
    else {
        last->next = (struct Node*)malloc(sizeof(struct Node));
        //CHeck for failed allocation

        last->next->next = 0;
        last->next->uid = uid;
        last->next->temperature = 100;
        last->next->Address = Address;
        last->next->len = len;
        copyName(last->next->name, Client.name, 0);

        last = last->next;
    }

    return uid;
}

void deallocateNode(struct ListWrap* wrapper) {
    if(wrapper->prev == 0) {
        printf("Invalid trying to delete first node\n");
    }
    else if(wrapper->current == 0) {
        printf("Trying to delete invalid node: node does not exist\n");
    }
    else if(wrapper->next == 0) {
        //delete no problem as is at the end of list
        ((struct Node*)wrapper->prev)->next = 0;
        free((struct Node*)wrapper->current);
    }
    else {
        //prev next current all not 0 so in middle of list
        ((struct Node*)wrapper->prev)->next = ((struct Node*)wrapper->next);
        free((struct Node*)wrapper->current);
    }
}

void deallocateNodePtr(struct KeyValue* pair, struct ListWrap* wrapper) {
    if(wrapper->current == 0) {
    }
    else if(wrapper->prev == 0) {
        pair->value = ((struct NodePtr*)wrapper->next);
        free(wrapper->current);
        printf("Reassigned first node\n");
    }
    else if(wrapper->next == 0) {
        printf("Deleted last node\n");
        ((struct NodePtr*)wrapper->prev)->next = 0;
        free((struct NodePtr*)wrapper->current);
    }
    else {
        //prev next current all not 0 so in middle of list
        ((struct NodePtr*)wrapper->prev)->next = ((struct NodePtr*)wrapper->next);
        free((struct NodePtr*)wrapper->current);
    }
}

void deallocateKeyPair(struct ListWrap* wrapper) {
}

void unlink(void* pointedStructure, struct ListWrap* wrapper) {
    switch(wrapper->type) {
        case 0:
                debug("Dellocating Node\n");
                deallocateNode(wrapper);
                printf("Deallocated Node\n");
                break;
        case 1:
                debug("Dellocating NodePtr");
                deallocateNodePtr((struct KeyValue*)&pointedStructure, wrapper);
                printf("Deallocated NodePtr\n");
                break;
        case 2:
                debug("Dellocating KeyPair\n");
                deallocateKeyPair(wrapper);
                printf("Deallocated KeyPair\n");
                break;
        default:
            printf("Default\n");
            break;
    };
    printf("Finished Switch in unlink\n");
    return;
}

void testAddNode() {
    printf("[*]TEST: AddNode\n");
    struct sockaddr_in Addr;
    socklen_t len;

    struct Join Aaron;
    uint8_t name1[20] = "Aaron";
    copyName(Aaron.name, name1, 0);

    struct Join Roger;
    uint8_t name2[20] = "Roger";
    copyName(Roger.name, name2, 0);

    struct Join Mike;
    uint8_t name3[20] = "Mike";
    copyName(Mike.name, name3, 0);

    addNode(Aaron, Addr, len);
    addNode(Roger, Addr, len);
    addNode(Mike, Addr, len);

    last = printAllNodes(first);
    printf("[*]END TEST: AddNode\n\n");
}

void testFindKeyValue() {
    printf("[*]TEST: FindKeyValue\n");
    first->DestinationDictionary = (KeyValue*)malloc(sizeof(struct KeyValue));
    first->DestinationDictionary->op = 1;
    first->DestinationDictionary->value = 0;

    first->DestinationDictionary->next = (KeyValue*)malloc(sizeof(KeyValue*));
    first->DestinationDictionary->next->op = 2;
    first->DestinationDictionary->next->value = 0;

    first->DestinationDictionary->next->next = (KeyValue*)malloc(sizeof(struct KeyValue));
    first->DestinationDictionary->next->next->op = 3;
    first->DestinationDictionary->next->next->value = 0;

    uint8_t op = 3;
    KeyValue* dests = findKeyPair(first->DestinationDictionary, op);
    if(dests->op == op) {
        printf("[*]TEST: PASSED \n");
        printf("OP found is: %d\n", dests->op);
    } else {
        printf("[!]TEST: FAILED \n");
        printf("Did not find OP\n");
    }
    printf("Value of op1: %d\n", first->DestinationDictionary->value);
    printf("[*]END TEST: FindKeyValue\n\n");
}

void testSearchNodes() {
    printf("[*]TEST: SearchNodes\n");
    uint8_t name[20] = "Mike";
    struct Node* foundNode = searchNodes(first, name);
    if(stringCompare(name, foundNode->name) == 1) {
        printf("[*]TEST: PASSED \n");
        printf("Found node. Searched %s, found %s\n", name, foundNode->name);
    } else {
        printf("[!]TEST: FAILED \n");
        printf("Failed to find node with name %s\n", name);
    }
    printf("[*]END TEST: SearchNodes\n\n");
}

void testNextName() {
    printf("[*]TEST: NextName\n");

    uint8_t names[] = "Aaron Boby K Joe";
    uint8_t* namesPtr = names;
    std::vector<uint8_t> name;

    while(namesPtr[0] != '\0') {
        name = nextName(namesPtr);
        printf("Next name: %s\n", &name[0]);
        namesPtr += name.size() + 1;
        printf("vec size: %d\n", (int)name.size());
    }

    printf("[*]END TEST: NextName\n\n");
}

void testAddToDests() {
    printf("[*]TEST: AddToDests\n");
    uint8_t nodesToAdd[] = "Mike Roger";
    uint8_t nodesToChange[] = "Aaron";
    uint8_t uids[] = "";

    struct Node* node = searchNodes(first, nodesToChange);
    printf("Our nodes name %s\n", node->name);

    struct KeyValue* keyPair = findKeyPair(node->DestinationDictionary, 1);
    addToDests(keyPair, nodesToAdd, uids);
    printf("Node1: %s\n", node->DestinationDictionary->value->node->name);
    printf("Node2: %s\n", node->DestinationDictionary->value->next->node->name);
    printf("[*]END TEST: AddToDests\n\n");
}


void testSearchDict() {
    printf("[*]TEST: SearchDict\n");
    uint8_t name[] = "Aaron";
    uint8_t nameToFind[] = "Roger";
    uint8_t uids[] = "";

    struct Node* node = searchNodes(first, name);
    struct ListWrap nodePtrWrap = searchDests(node->DestinationDictionary->value, nameToFind, uids);
    printf("Node found: %s\n", ((struct NodePtr*)nodePtrWrap.current)->node->name);
    printf("[*]END TEST: SearchDict\n\n");
}

void testUnlink() {
    printf("[*]TEST: Unlink\n");
    uint8_t name[] = "Aaron";
    uint8_t nameToFind[] = "Mike";
    uint8_t uids[] = "";

    struct Node* node = searchNodes(first, name);
    struct ListWrap nodePtrWrap = searchDests(node->DestinationDictionary->value, nameToFind, uids);
    printf("Type returned: %d\n", nodePtrWrap.type);
    printf("Current Node: %s\n", ((struct NodePtr*)nodePtrWrap.current)->node->name);
    printf("Prev: %d\n", nodePtrWrap.prev);
    printf("Current: %d\n", nodePtrWrap.current);
    printf("Next: %d\n", nodePtrWrap.next);
    unlink(node->DestinationDictionary, &nodePtrWrap);
    printf("Unlinked\n");
    searchDests(node->DestinationDictionary->value, nameToFind, uids);
    printf("[*]END TEST: Unlink\n\n");
}
int main(int argc, const char* argv[]) {
    if(argc == 1) {
    test = true;
    testAddNode();
    testFindKeyValue();
    testSearchNodes();
    testNextName();
    printf("First is %s\n", first->name);
    testAddToDests();
    testSearchDict();
    testUnlink();
    }
    else if(argc == 2) {
    test = true;
    printf("Run as normal but no normal exists\n");
    }
    else if(argc == 3) {
    test = false;
    printf("Run as release\n");
    }

}
