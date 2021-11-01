#include <stdlib.h>
#include "resources.h"


namespace Hash {
    int maxLength = 49;
    int ListSize = 50;

    template <typename Value, typename Key> struct LinkedListInterface {
        struct ListNode {
            ListNode* next;
            Key key;

            Value data;
        };

        ListNode* start = 0;
        ListNode* end = start;

        int addNode(Value value, Key key) {
            ListNode* node_ptr = (ListNode*)malloc(sizeof(ListNode));
            if(node_ptr == NULL) {
                return 0;
            }
            node_ptr->next = NULL;
            node_ptr->key = key;
            node_ptr->data = value;

        if(start == 0) {
            start = node_ptr;
            end = start;
        }
        else {
            end->next = node_ptr;

            end = end->next;
        }
        printf("Linked list start: %p\n", start);
        return 1;

        }

        ListNode* searchNodes(Key key, ListNode* list) {
            if(list == 0) {
                Resources::testOutput("Failed to find node\n");
                return 0;
            }
            else if(list->key == key) {
                return list;
            }
            else {
                return searchNodes(key, list->next);
            }

        }

        Value* getNode(uint8_t key) {
            ListNode* node = searchNodes(key, start);
            if(node == 0) {
                return 0;
            }
            
            return &(node->data);

        }
    };

    template <typename Value, typename Key> struct Table {

        private:
            void* List = 0;
            int TableLength = maxLength;

            void allocateTable() {
                if(List != 0) {
                    Resources::testOutput("This table has already been allocated\n");
                    exit(0);
                }
                List = calloc(TableLength, sizeof(LinkedListInterface<Value, Key>));
                    Resources::testOutput("Allocated the table\n");
                    printf("Value of list: %p\n", List);
            }

            int hash(Key key) {
                return (key*100 + 100)%TableLength;
            }

        public:
            int addElement(Value value, Key key) {
                if(List == 0) {
                    Resources::testOutput("First item on the table: Alocating\n");
                    allocateTable();
                    printf("List: %p\n", List);
                }

                int index = hash(key);
                Resources::testOutput("The index for the hashed key is ");
                Resources::testOutput(index);

                if((((LinkedListInterface<Value, Key>*)List)[index]).addNode(value, key) == 0) {
                    return 0;
                } else {
                    return 1;
                }
            }

            void removeElement(Key key) {
                int index = hash(key);
                ((Value*)List)[index] = (Value){0};
            }

            Value* getElement(Key key) {
                int index = hash(key);
                if(((LinkedListInterface<Value, Key>*)List)[index].start == 0) {
            Resources::testOutput("The value your trying to get at the start of the linked list has been removed\n");
                    return 0;
                }
                else if((((LinkedListInterface<Value, Key>*)List)[index]).getNode(key) == 0) {
            Resources::testOutput("The value your trying to get in the linked list has been removed\n");
                    return 0;
                }

                return (((LinkedListInterface<Value, Key>*)List)[index]).getNode(key);
            }

    };
};
