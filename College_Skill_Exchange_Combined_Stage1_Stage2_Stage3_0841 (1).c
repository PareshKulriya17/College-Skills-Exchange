#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

struct SkillNode {
    int skillID;
    char skillName[NAME_LEN];
    struct SkillNode *next;
};

struct RequestNode {
    int studentID;
    char studentName[NAME_LEN];
    char mentorName[NAME_LEN];
    char skillName[NAME_LEN];
    struct RequestNode *next;
};

struct BSTNode {
    int skillID;
    char skillName[NAME_LEN];
    struct BSTNode *left;
    struct BSTNode *right;
};

struct SkillRecord {
    int skillID;
    char skillName[NAME_LEN];
};

struct SkillNode *skillListHead = NULL;
struct RequestNode *front = NULL;
struct RequestNode *rear = NULL;
struct BSTNode *bstRoot = NULL;

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int readInt(const char *prompt, int *value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        printf("Invalid input. Please enter a whole number.\n");
        clearInputBuffer();
        return 0;
    }
    clearInputBuffer();
    return 1;
}

int readText(const char *prompt, char *buffer, size_t size) {
    size_t length;

    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        clearInputBuffer();
    }

    if (buffer[0] == '\0') {
        printf("Input cannot be empty.\n");
        return 0;
    }

    return 1;
}

struct SkillNode *findSkillInList(int skillID) {
    struct SkillNode *temp = skillListHead;

    while (temp != NULL) {
        if (temp->skillID == skillID) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

struct BSTNode *createBSTNode(int id, const char *name) {
    struct BSTNode *newNode =
        (struct BSTNode *)malloc(sizeof(struct BSTNode));

    if (newNode == NULL) {
        printf("Memory allocation failed for BST node.\n");
        return NULL;
    }

    newNode->skillID = id;
    snprintf(newNode->skillName, sizeof(newNode->skillName), "%s", name);
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

struct BSTNode *insertBST(struct BSTNode *root, int id, const char *name) {
    if (root == NULL) {
        return createBSTNode(id, name);
    }

    if (id < root->skillID) {
        root->left = insertBST(root->left, id, name);
    } else if (id > root->skillID) {
        root->right = insertBST(root->right, id, name);
    }

    return root;
}

struct BSTNode *searchSkillBST(struct BSTNode *root, int skillID) {
    if (root == NULL || root->skillID == skillID) {
        return root;
    }

    if (skillID < root->skillID) {
        return searchSkillBST(root->left, skillID);
    }

    return searchSkillBST(root->right, skillID);
}

void addSkill(void) {
    struct SkillNode *newNode;
    struct BSTNode *newBSTRoot;
    int id;
    char name[NAME_LEN];

    if (!readInt("\nEnter Skill ID: ", &id)) {
        return;
    }

    if (id <= 0) {
        printf("Skill ID must be a positive integer.\n");
        return;
    }

    if (findSkillInList(id) != NULL || searchSkillBST(bstRoot, id) != NULL) {
        printf("This Skill ID already exists. Please use a unique ID.\n");
        return;
    }

    if (!readText("Enter Skill Name: ", name, sizeof(name))) {
        printf("Skill was not added.\n");
        return;
    }

    newNode = (struct SkillNode *)malloc(sizeof(struct SkillNode));
    if (newNode == NULL) {
        printf("Memory allocation failed for skill node.\n");
        return;
    }

    newNode->skillID = id;
    snprintf(newNode->skillName, sizeof(newNode->skillName), "%s", name);
    newNode->next = skillListHead;
    skillListHead = newNode;

    newBSTRoot = insertBST(bstRoot, id, name);
    if (newBSTRoot == NULL && bstRoot == NULL) {
        skillListHead = newNode->next;
        free(newNode);
        printf("Skill could not be indexed in the BST.\n");
        return;
    }
    bstRoot = newBSTRoot;

    printf("Skill '%s' (ID: %d) added to the Linked List and BST.\n", name, id);
}

void addLearningRequest(void) {
    struct RequestNode *newRequest;
    int studentID;
    char studentName[NAME_LEN];
    char mentorName[NAME_LEN];
    char skillName[NAME_LEN];

    if (!readInt("\nEnter Student ID: ", &studentID) || studentID <= 0) {
        printf("Student ID must be a positive integer.\n");
        return;
    }

    if (!readText("Enter Student Name: ", studentName, sizeof(studentName)) ||
        !readText("Enter Mentor Name: ", mentorName, sizeof(mentorName)) ||
        !readText("Enter Skill to Learn: ", skillName, sizeof(skillName))) {
        printf("Learning request was not added. Please try again.\n");
        return;
    }

    newRequest = (struct RequestNode *)malloc(sizeof(struct RequestNode));
    if (newRequest == NULL) {
        printf("Memory allocation failed for learning request.\n");
        return;
    }

    newRequest->studentID = studentID;
    snprintf(newRequest->studentName, sizeof(newRequest->studentName), "%s", studentName);
    snprintf(newRequest->mentorName, sizeof(newRequest->mentorName), "%s", mentorName);
    snprintf(newRequest->skillName, sizeof(newRequest->skillName), "%s", skillName);
    newRequest->next = NULL;

    if (front == NULL) {
        front = rear = newRequest;
    } else {
        rear->next = newRequest;
        rear = newRequest;
    }

    printf("Learning request added to the Queue successfully.\n");
}

void displaySkills(void) {
    struct SkillNode *temp = skillListHead;

    if (temp == NULL) {
        printf("\nNo skills available.\n");
        return;
    }

    printf("\n===== STUDENT SKILL PROFILE (LINKED LIST) =====\n");
    while (temp != NULL) {
        printf("Skill ID: %d | Skill Name: %s\n",
               temp->skillID, temp->skillName);
        temp = temp->next;
    }
}

void displayRequests(void) {
    struct RequestNode *temp = front;

    if (temp == NULL) {
        printf("\nNo learning requests are waiting.\n");
        return;
    }

    printf("\n===== LEARNING REQUEST QUEUE (FIFO) =====\n");
    while (temp != NULL) {
        printf("Student ID: %d | Student: %s | Mentor: %s | Skill: %s\n",
               temp->studentID, temp->studentName,
               temp->mentorName, temp->skillName);
        temp = temp->next;
    }
}

void processNextRequest(void) {
    struct RequestNode *temp;

    if (front == NULL) {
        printf("\nNo learning requests to process.\n");
        return;
    }

    temp = front;
    printf("\nProcessing request (FIFO):\n");
    printf("Student ID: %d | Student: %s | Mentor: %s | Skill: %s\n",
           temp->studentID, temp->studentName,
           temp->mentorName, temp->skillName);

    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }

    free(temp);
    printf("Request processed and removed from the Queue.\n");
}

void deleteSkill(struct SkillNode **headRef, int skillID) {
    struct SkillNode *temp = *headRef;
    struct SkillNode *prev = NULL;

    while (temp != NULL && temp->skillID != skillID) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Skill ID %d not found in the Linked List profile.\n", skillID);
        return;
    }

    if (prev == NULL) {
        *headRef = temp->next;
    } else {
        prev->next = temp->next;
    }

    printf("Skill ID %d ('%s') deleted from the Linked List profile.\n",
           temp->skillID, temp->skillName);
    free(temp);
    printf("Note: The BST index remains unchanged, as in the Stage 2 report.\n");
}

void deleteSkillMenu(void) {
    int id;

    if (!readInt("\nEnter Skill ID to delete from profile: ", &id)) {
        return;
    }

    deleteSkill(&skillListHead, id);
}

void displayBSTInOrder(struct BSTNode *root) {
    if (root == NULL) {
        return;
    }

    displayBSTInOrder(root->left);
    printf("Skill ID: %d | Skill Name: %s\n", root->skillID, root->skillName);
    displayBSTInOrder(root->right);
}

void displayBST(void) {
    if (bstRoot == NULL) {
        printf("\nBST index is empty.\n");
        return;
    }

    printf("\n===== GLOBAL SKILL INDEX (BST IN-ORDER) =====\n");
    displayBSTInOrder(bstRoot);
}

void searchSkillBSTMenu(void) {
    int id;
    struct BSTNode *result;

    if (!readInt("\nEnter Skill ID to search in BST: ", &id)) {
        return;
    }

    result = searchSkillBST(bstRoot, id);

    if (result != NULL) {
        printf("\n[BST SEARCH FOUND]\nSkill ID: %d | Skill Name: %s\n",
               result->skillID, result->skillName);
    } else {
        printf("\n[BST SEARCH FAILED] Skill ID %d not found in BST index.\n", id);
    }
}

void binarySearchSkillID(struct SkillNode *head) {
    struct SkillNode *temp = head;
    struct SkillRecord *arr;
    struct SkillRecord key;
    int count = 0;
    int i, j;
    int low, high, mid, targetID;

    if (head == NULL) {
        printf("\nNo skills available to search.\n");
        return;
    }

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    arr = (struct SkillRecord *)malloc((size_t)count * sizeof(struct SkillRecord));
    if (arr == NULL) {
        printf("Memory allocation failed for temporary search array.\n");
        return;
    }

    temp = head;
    for (i = 0; i < count; i++) {
        arr[i].skillID = temp->skillID;
        snprintf(arr[i].skillName, sizeof(arr[i].skillName), "%s", temp->skillName);
        temp = temp->next;
    }

    for (i = 1; i < count; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j].skillID > key.skillID) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }

    if (!readInt("\nEnter Skill ID to search using Binary Search: ", &targetID)) {
        free(arr);
        return;
    }

    low = 0;
    high = count - 1;

    while (low <= high) {
        mid = low + (high - low) / 2;

        if (arr[mid].skillID == targetID) {
            printf("\n[BINARY SEARCH FOUND]\nSkill ID: %d\nSkill Name: %s\n",
                   arr[mid].skillID, arr[mid].skillName);
            free(arr);
            return;
        } else if (arr[mid].skillID < targetID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    printf("\n[BINARY SEARCH FAILED] Skill ID %d not found in the Linked List records.\n",
           targetID);
    free(arr);
}

void freeSkillList(void) {
    struct SkillNode *temp;

    while (skillListHead != NULL) {
        temp = skillListHead;
        skillListHead = skillListHead->next;
        free(temp);
    }
}

void freeRequestQueue(void) {
    struct RequestNode *temp;

    while (front != NULL) {
        temp = front;
        front = front->next;
        free(temp);
    }

    rear = NULL;
}

void freeBST(struct BSTNode *root) {
    if (root == NULL) {
        return;
    }

    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

int main(void) {
    int choice = 0;

    do {
        printf("\n============================================\n");
        printf("          COLLEGE SKILL EXCHANGE\n");
        printf("============================================\n");
        printf("1. Add Skill (Linked List and BST)\n");
        printf("2. Add Learning Request (Queue)\n");
        printf("3. Display Skills (Linked List)\n");
        printf("4. Display Learning Requests (Queue)\n");
        printf("5. Process Next Learning Request (FIFO)\n");
        printf("6. Search Skill in BST (Stage 2)\n");
        printf("7. Delete Skill from Linked List (Stage 2)\n");
        printf("8. Display BST Index (In-Order)\n");
        printf("9. Search Skill using Binary Search (Stage 3)\n");
        printf("10. Exit\n");
        printf("============================================\n");

        if (!readInt("Enter your choice: ", &choice)) {
            continue;
        }

        switch (choice) {
            case 1:
                addSkill();
                break;
            case 2:
                addLearningRequest();
                break;
            case 3:
                displaySkills();
                break;
            case 4:
                displayRequests();
                break;
            case 5:
                processNextRequest();
                break;
            case 6:
                searchSkillBSTMenu();
                break;
            case 7:
                deleteSkillMenu();
                break;
            case 8:
                displayBST();
                break;
            case 9:
                binarySearchSkillID(skillListHead);
                break;
            case 10:
                printf("\nExiting College Skill Exchange System.\n");
                break;
            default:
                printf("\nInvalid choice. Please select an option from 1 to 10.\n");
        }
    } while (choice != 10);

    freeSkillList();
    freeRequestQueue();
    freeBST(bstRoot);
    bstRoot = NULL;

    return 0;
}
