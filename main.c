#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 177

typedef struct {
    char isbn[MAX_LENGTH];
    char authors[MAX_LENGTH];
    char title[MAX_LENGTH];
    int totalBooks;
    int availableBooks;
} Book;

int readBooksFromFile(Book* books, int maxBooks, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 0;
    }

    int count = 0;
    while (count < maxBooks && fscanf(file, "%[^,],%[^,],%[^,],%d,%d\n",
                                      books[count].isbn,
                                      books[count].authors,
                                      books[count].title,
                                      &books[count].totalBooks,
                                      &books[count].availableBooks) == 5) {
        count++;
    }
    fclose(file);
    return count;
}

void writeBooksToFile(const Book* books, int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%d,%d\n", books[i].isbn, books[i].authors, books[i].title,
                books[i].totalBooks, books[i].availableBooks);
    }
    fclose(file);
}

void addBook(Book* books, int* count) {
    if (*count >= MAX_LENGTH) {
        printf("Maximum number of books reached.\n");
        return;
    }

    Book newBook;
    printf("Enter the ISBN number: ");
    scanf("%s", newBook.isbn);

    for (int i = 0; i < *count; i++) {
        if (strcmp(newBook.isbn, books[i].isbn) == 0) {
            printf("A book with the same ISBN number already exists.\n");
            return;
        }
    }

    printf("Enter the authors' names: ");
    scanf(" %[^\n]s", newBook.authors);

    printf("Enter the book title: ");
    scanf(" %[^\n]s", newBook.title);

    printf("Enter the total number of books: ");
    scanf("%d", &newBook.totalBooks);

    printf("Enter the number of available books: ");
    scanf("%d", &newBook.availableBooks);

    books[*count] = newBook;
    (*count)++;

    printf("Book successfully added.\n");
}

void removeBook(Book* books, int* count) {
    char isbn[MAX_LENGTH];
    printf("Enter the ISBN number of the book to remove: ");
    scanf("%s", isbn);

    for (int i = 0; i < *count; i++) {
        if (strcmp(isbn, books[i].isbn) == 0) {
            for (int j = i; j < *count - 1; j++) {
                books[j] = books[j + 1];
            }
            (*count)--;
            printf("Book successfully removed.\n");
            return;
        }
    }
    printf("Book with that ISBN number not found.\n");
}

void viewBook(const Book* books, int count) {
    char isbn[MAX_LENGTH];
    printf("Enter the ISBN number of the book to view information: ");
    scanf("%s", isbn);

    for (int i = 0; i < count; i++) {
        if (strcmp(isbn, books[i].isbn) == 0) {
            printf("ISBN: %s\n", books[i].isbn);
            printf("Authors: %s\n", books[i].authors);
            printf("Title: %s\n", books[i].title);
            printf("Total Books: %d\n", books[i].totalBooks);
            printf("Available Books: %d\n", books[i].availableBooks);
            return;
        }
    }
    printf("Book with that ISBN number not found.\n");
}

void sortBooksByISBN(Book* books, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(books[j].isbn, books[j + 1].isbn) > 0) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}

void displayAllBooks(const Book* books, int count) {
    printf("Book List:\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| ISBN |    Authors    |      Title      | Total books | Available books |\n");
    printf("------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("| %s | %s | %s | %d | %d |\n", books[i].isbn, books[i].authors, books[i].title, books[i].totalBooks, books[i].availableBooks);
    }
    printf("------------------------------------------------------------------------------\n");
}

int main() {
    Book books[MAX_LENGTH];
    int count = 0;

    count = readBooksFromFile(books, MAX_LENGTH, "books.csv");

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add a new book\n");
        printf("2. Remove a book by ISBN number\n");
        printf("3. View information for a book\n");
        printf("4. All books in table\n");
        printf("5. Exit\n");
        printf("Enter the number: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(books, &count);
                break;
            case 2:
                removeBook(books, &count);
                break;
            case 3:
                viewBook(books, count);
                break;
            case 4:
                sortBooksByISBN(books, count);
                displayAllBooks(books, count);
                break;
            case 5:
                writeBooksToFile(books, count, "books.csv");
                printf("Program terminated. Current library state saved to file.\n");
                break;
            default:
                printf("Invalid input.\n");
        }
    } while (choice != 5);
    return 0;
}
