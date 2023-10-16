# Web-of-Knowledge
## A data structures and algorithms project

## Dataset
This project revolves around a Research Publication Database. The dataset includes information about authors, articles, and their relationships.

## Data Structures Used
The project utilizes the following data structures:
- AVL Trees
- Graph
- Linked Lists
- Arrays

## Functionalities
1. Total number of articles published by an author
2. Total number of publications per year by an author
3. Number of coauthors per publications of an author
4. Number of papers for each position of an author
5. Print the names of journals of each article by an author
6. Print the names of authors at a distance 'd' from a given author
7. Check if given authors have worked together
8. Print coauthors of a given author
9. Number of articles given authors have coauthored
10. Print the names of all articles by an author
11. Print names of all authors

## Brief Description
- The project organizes data as follows:
  - Articles are stored in an AVL tree.
  - Each article has a list of authors in linked list form.
  - Authors are stored in an AVL tree.
  - Each author has a list of articles in linked list form.
  - Each author has a year-wise tree in AVL form.
  - Each year-wise node has articles published in that year stored in a linked list.
  - A graph is created for authors.

- **Why We Chose These Data Structures:**
  - Linked Lists: Used for insertions with constant time complexity (O(1)).
  - AVL Trees: Used when search operations were required (log(n)).
  - AVL Tree in Graph: Used to reduce search time complexity.

## Project Diagram
![Project Flow](https://drive.google.com/file/d/1ebvuMe8oiljm6Y_ZqBdUfHYUOa0NQ6pW/view?usp=drive_link)

## How to Run
To run the program, execute the `finalproject.cpp` file. Data is provided in the `data.csv` file.
