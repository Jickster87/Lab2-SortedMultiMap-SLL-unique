#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
    rel = r;
    head = nullptr;
    totalSize = 0;
}

void SortedMultiMap::add(TKey c, TValue v) {
	
  //  cout << "Adding (" << c << ", " << v << ") to the map." << endl;

    Node *nomad = head;
    Node *prevNomad = nullptr;

    // Point to the correct position according to relation
    while (nomad != nullptr && rel(nomad->key, c)) {
        if (nomad->key == c) {
            // Push the new value in the head of the value linked list
            vNode *newvNode = new vNode(v);
            newvNode->next = nomad->vHead;
            nomad->vHead = newvNode;
            totalSize++;
            return;
        }
        prevNomad = nomad;
        nomad = nomad->next;
    }
    
    // Key not found, create new key and value
    vNode *newValNode = new vNode(v);
    Node *newNode = new Node(c);
    newNode->vHead = newValNode;
    
    //insert (cases?) - next pointer only in SLL to keep track of
    //what cases do i have? (first & mid+last)
    if (prevNomad == nullptr) {
        newNode->next = head;
        head = newNode;
    } else {
        newNode->next = nomad;
        prevNomad->next = newNode;
    }
    totalSize++;
}

bool SortedMultiMap::remove(TKey c, TValue v) {
    Node * nomad = head;
    Node * prevNomad = nullptr;
    
    //point to correct position according to rel (travel with two pointers)
    while (nomad != nullptr && nomad->key != c) {
        prevNomad = nomad;
        nomad = nomad->next;
    }
    
    //case 1 not found
    if (nomad == nullptr) {
        return false;
    }
    
    //case 2 found
    vNode * vNomad = nomad->vHead;
    vNode * prevVNomad = nullptr;
    
    //check if value exists inside key c
    while (vNomad != nullptr && vNomad->value != v) {
        prevVNomad = vNomad;
        vNomad = vNomad->next;
    }
    //0. value not found
    if (vNomad == nullptr) {
        return false; // value not found in key
    }
    //1. value found
    if (vNomad == nomad->vHead) {
        nomad->vHead = vNomad->next;
    } else {
        prevVNomad->next = vNomad->next;
    }
    delete vNomad;
    totalSize--;
    
    //if last value also remove key ??
    if (nomad->vHead == nullptr) {
        //that means we removed last value in key
        if (nomad == head) {
            //check if head
            head = nomad->next;
        } else {
            //check key inbetween+last
            prevNomad->next = nomad->next;
        }
        delete nomad;
    }
    
    return true;
}

vector<TValue> SortedMultiMap::search(TKey c) const {
    Node * traveler = head;
    while (traveler != nullptr) {  // Check traveler instead of head
        if (traveler->key == c) {
            vector<TValue> values;
            vNode * valueTraveler = traveler->vHead;
            while (valueTraveler != nullptr) {
                values.push_back(valueTraveler->value);
                valueTraveler = valueTraveler->next;
            }
            return values;
        }
        traveler = traveler->next;
    }
    return vector<TValue>();
}

int SortedMultiMap::size() const {
    return totalSize;
}

bool SortedMultiMap::isEmpty() const {
    return totalSize == 0;
}

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {
    while (head != nullptr) {  // Keep deleting until head is nullptr
        // First, delete all the values (vNodes) associated with the current key (Node)
        vNode * vTemp = head->vHead;
        while (vTemp != nullptr) {
            vNode * toDelete = vTemp;
            vTemp = vTemp->next;
            delete toDelete;  // Delete each value node
        }

        // Then delete the current key (Node)
        Node * temp = head;
        head = head->next;
        delete temp;  // Delete the key node
    }
}
