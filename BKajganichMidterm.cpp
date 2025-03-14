// BKajganichMidterm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK 100

typedef struct StudentNode {
    int studentAge;
    int studentID;
    StudentNode* next;
} StudentNode;

typedef struct Stack {
	StudentNode* student;
	int topIndex;
} Stack;

// Prototypes

Stack* initializeStack();  //Initializes an empty stack.
void push(Stack* stack, StudentNode* student);   //Adds a new student to the stack.s
StudentNode* pop(Stack* stack); //Removes and returns the most recent student.
StudentNode* peek(Stack* stack); //Returns the most recent student without removing it.
bool isStackEmpty(Stack* stack); //Checks if the stack is empty.
bool isStackFull(Stack* stack); //Check if the stack is full.
bool doesIDExist(Stack* stack, int desiredID); //Check if ID is already occupied


void insertSorted(StudentNode** head, StudentNode** tail, StudentNode* newStudent);  // Inserts a student into the sorted linked list.
void deleteStudent(StudentNode** head, StudentNode** tail, int studentId);  //Deletes a student from the linked list by Student ID.
void printList(StudentNode* head); // Displays all student records in sorted order.
StudentNode* createNewStudent(int studentID, int age); //Creates a new student node

int getNum();
int displayMenu();

//enumeration to save "magic numbers" of menu input
enum Menu
{
	addNewStudent = 1,
	removeLastStudent = 2,
	displayStudents = 3,
	exitApp = 4,
};

int main(void)
{
	int choice;
	int newID;
	int newAge;
	Stack* stack = initializeStack();
	StudentNode* deletedStudent;
    StudentNode* head = NULL;
    StudentNode* tail = NULL;
	StudentNode* newStudent;
	int deleteID;
	bool exit = false;
	do
	{
		choice = displayMenu();
		switch (choice)
		{
		case addNewStudent:
			printf("Please enter new student ID:");
			newID = getNum();
			if (newID < 0)
			{
				printf("Student ID must be a positive integer. Student not added");
				break;
			}
			if (doesIDExist(stack, newID))
			{
				printf("Student ID is already taken. Student not added");
				break;
			}
			printf("Please enter new student Age:");
			newAge = getNum();
			if (newAge < 5 || newAge > 70)
			{
				printf("Student Age too old/young for school. Student not added");
				break;
			}
			newStudent = createNewStudent(newID, newAge); 
			push(stack, newStudent);
			insertSorted(&head, &tail, newStudent);
			break;
		case removeLastStudent:
			if (peek(stack) != NULL)
			{
				deleteID = pop(stack)->studentID;
				deleteStudent(&head, &tail, deleteID);
			}
			break;
		case displayStudents:
			printList(head);
			break;
		case exitApp:
			exit = true;
			break;
		default:
			printf("That input is not a valid choice. Please try again\n");
			break;
		}
	} while (!exit);

	StudentNode* current = head;
	StudentNode* temp = NULL;
	while (current != NULL)
	{
		temp = current->next;
		free(current);
		current = temp;
	}
	free(stack);
	free(stack->student);
    return 0;
}

int displayMenu(void)
{
	int choice;
	printf("\nPlease select one of the following options:\n");
	printf("1. Register a new student\n");
	printf("2. Remove the most recently registered student.\n");
	printf("3. Display all students\n");
	printf("4. Exit\n");
	choice = (int)(getNum());
	return choice;
}

void deleteStudent(StudentNode** head, StudentNode** tail, int studentID)
{
		if (*head != NULL)
		{
			//List is not empty
			StudentNode* current = *head;
			StudentNode* prior = NULL;
			while (current->studentID != studentID)
			{
				prior = current;
				current = current->next;
			}
			if (prior != NULL)
			{
				prior->next = current->next;
			}
			else if (current->next != NULL)
			{
				*head = current->next;
			}
			if (current == *head)
			{
				*head = NULL;
			}
			free(current);
		}
	
	return;
}
void insertSorted(StudentNode** head, StudentNode** tail, StudentNode* newStudent)
{
	if (*head == NULL)
	{
		*head = newStudent;
		*tail = newStudent;
	}
	else if ((*head)->studentID > newStudent->studentID)
	{
		newStudent->next = *head;
		*head = newStudent;
	}
	else
	{
		StudentNode* current = *head;
		while (current->next != NULL && current->next->studentID < newStudent->studentID)
		{
			current = current->next;
		}
		newStudent->next = current->next;
		current->next = newStudent;
	}
}
// FUNCTION : initializeStack
// DESCRIPTION : This function initialize a Stack struct
// PARAMETERS : void
// RETURNS : Stack* - pointer to initialized Stack

Stack* initializeStack()
{
	Stack* newStack = (Stack*)malloc(sizeof(Stack));
	if (!newStack)
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	newStack->student = (StudentNode*)malloc(sizeof(StudentNode) * MAX_STACK);
	if (!newStack->student)
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	newStack->topIndex = -1;
	return newStack;
}

bool isStackEmpty(Stack* stack)
{
	return stack->topIndex == -1;
}

bool isStackFull(Stack* stack)
{
	return stack->topIndex >= MAX_STACK;
}

bool doesIDExist(Stack* stack, int desiredID)
{
	int sizeOfStack = stack->topIndex;
	int currentID;
	while (stack->topIndex != -1)
	{
		currentID = stack->student[stack->topIndex].studentID;
		if (currentID == desiredID)
		{
			stack->topIndex = sizeOfStack;
			return true;
		}
		stack->topIndex--;
	}
	stack->topIndex = sizeOfStack;
	return false;
}
StudentNode* createNewStudent(int studentID, int age)
{
	StudentNode* newStudent = (StudentNode*)malloc(sizeof(StudentNode));
	if (!newStudent)
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	newStudent->studentAge = age;
	newStudent->studentID = studentID;
	newStudent->next = NULL;
	return newStudent;
}


// FUNCTION : push
// DESCRIPTION : the will move the given StudentNode to the top of the stack
// PARAMETERS : stack* stack - pointer to the stack of registered students StudentNode* - pointer to StudentNode to be pushed
// RETURNS : void
void push(Stack* stack, StudentNode* newStudent)
{
	if (isStackFull(stack))
	{
		printf("Student registry is full");
		exit(EXIT_FAILURE);
	}
	stack->topIndex++;
	stack->student[stack->topIndex] = *newStudent;
}

// FUNCTION : pop
// DESCRIPTION : the will remove the most recently added student from the stack
// PARAMETERS : stack* stack - pointer to the stack of completed tasks
// RETURNS : StudentNode* StudentNode pointer to the student popped off the stack
StudentNode* pop(Stack* stack)
{
	if (isStackEmpty(stack))
	{
		printf("Registry is empty");
		return NULL;
	}
	stack->topIndex--;
	return &stack->student[stack->topIndex+1];
}

// FUNCTION : peek
// DESCRIPTION : the will move return the most recently added student without changing the index
// PARAMETERS : stack* stack - pointer to the stack of completed tasks
// RETURNS : StudentNode* StudentNode pointer to the top-most student on the stack
StudentNode* peek(Stack* stack)
{
	if (isStackEmpty(stack))
	{
		printf("Registry is empty");
		return NULL;
	}
	return &stack->student[stack->topIndex];
}

void printList(StudentNode* head)
{
	if (head == NULL)
	{
		printf("Registry is empty");
		return;
	}
	while (head != NULL)
	{
		printf("Student ID: %d, Student Age: %d\n", head->studentID, head->studentAge);
		head = head->next;
	}
	return;
}

/* FUNCTION: GetNum
 PARAMETERS: void
	 ACTION: Scans a user input for a number, returns  a double of that number
			 if a valid input is not given, returns -1
	***This function was explicitly provided to all students last semester***
	 RETURN: double: value of user input, -1 if invalid */
int getNum(void)
{
	char record[121] = { 0 }; // Buffer to store input string
	double number = 0.0; // Variable to store the converted number
	// Prompt the user for input and read the input string
	fgets(record, 121, stdin);
	// Attempt to convert the string to a double
	if (sscanf(record, "%lf", &number) != 1) {
		// If conversion fails, set the number to -1
		number = -1.0;
	}
	return number; // Return the double (or -1 if conversion failed)
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
