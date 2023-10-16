/********************************************************
 *          DSA TERM Project
 *        Abdul Manan - 303323
 *       Tafheem Ul Islam - 321906
 *             BSCS 9-A
 *        Research Publication Database
 *******************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Forward declaration of classes and functions

class Article;                   // data type for article
class ArticleTreeNode;           // tree node for article AVL tree
class ArticleTree;               // AVL tree for Article
class Author;                    // data type for author
class AuthorTreeNode;            // tree node for author AVL tree
class AuthorTree;                // AVL tree for author
class AuthorsInArticleListNode;  // Linked List node for Authors in a article list.
class ArticlesByAuthorListNode;  // Linked List node for Aritcles written by author list.
class YearWiseTreeNodeForAuthor; // YearWise tree node for each author.
class YearWiseTreeForAuthor;     // AVL tree yearwise for each author
class AuthorGraphNode;           // Graph node for Authors' graph
class AuthorGraph;               // Authors' Graph

void parser(ArticleTree *article_tree, AuthorTree *author_tree);
AuthorTreeNode *InsertAuthor(AuthorTree *author_tree, string full_name, ArticleTreeNode *article_node, short unsigned int year);

void Function01TotalNumberOfArticlesPublished(AuthorTree *author_tree, string name);
void Function02NumberofPublicationPerYear(AuthorTree *author_tree, string name);
void Function03NumberofCoauthorsPerPublication(AuthorTree *author_tree, string name);
void Function04NumberofPapersForEachPosition(AuthorTree *author_tree, string name);
void Function05PrintNameofJournal(AuthorTree *author_tree, string name);
void Function06PrintAtDistance(string name, int distance, AuthorTree *author_tree, AuthorGraph *author_graph);
void Function07CheckIfCoauthors(string name1, string name2, AuthorTree *author_tree, AuthorGraph *author_graph);
void Function08PrintCoauthors(string name, AuthorTree *author_tree, AuthorGraph *author_graph);
void Function09PrintNumberofArticlesCoauthored(string name1, string name2, AuthorTree *author_tree, AuthorGraph *author_graph);
void Function10PrintNameofArticlesofAuthor(AuthorTree *author_tree, string name);
void Function11PrintNameofAllAuthors(AuthorTree *author_tree);

void traverseReverseList(AuthorTree *author_tree, AuthorsInArticleListNode *temp, int *array, string name, int position);
void TraverseTreeForGraph(AuthorTreeNode *node, AuthorGraph *author_graph);
void NodeAtDistance(AuthorGraphNode *node, int d, AuthorGraph *author_graph);
void GraphPreOrderTraversalForFunction06(AuthorGraphNode *node, int d, AuthorGraph *author_graph);
void GraphTreeInorderTraversal(AuthorGraphNode *node, int index_of_author2, AuthorGraph *author_graph, int flag);
void CreateEdges(AuthorGraph *author_graph, AuthorTree *author_tree);
void MainScreen();

//stores article data
class Article
{
public:
    AuthorsInArticleListNode *pstart_authors = nullptr;
    string identifier;
    string title;
    string journal;
    string month;
    short unsigned int year;
    string publisher;
    string type;
};

//node of article tree that will be formed
class ArticleTreeNode
{
public:
    Article data;
    ArticleTreeNode *left_child;
    ArticleTreeNode *right_child;
};

//article tree
class ArticleTree
{
public:
    ArticleTreeNode *root;

public:
    void printAuthors(ArticleTreeNode *node);
    //constructor
    ArticleTree()
    {
        root = nullptr;
    }

    //utility function for insertion
    void UseInsert(Article data)
    {
        root = Insert(root, data);
    }

    //insert article data
    ArticleTreeNode *Insert(ArticleTreeNode *node, Article data)
    {
        // Find the correct position and insert the new node
        if (node == nullptr)
        {
            auto *node = new ArticleTreeNode();
            node->data = data;
            node->left_child = nullptr;
            node->right_child = nullptr;
            return node;
        }
        if (data.identifier < node->data.identifier)
            node->left_child = Insert(node->left_child, data);
        else if (data.identifier > node->data.identifier)
            node->right_child = Insert(node->right_child, data);
        else
        {
            return node;
        }
        //re-balancing
        if (BalanceFactor(node) > 1)
        {
            if (Height(node->left_child->left_child) >= Height(node->left_child->right_child))
            {
                // Left Left Heavy Case
                return RightRotate(node);
            }
            else
            {
                // Left Right Heavy
                return LeftRightRotate(node);
            }
        }
        else if (BalanceFactor(node) < -1)
        {
            if (Height(node->right_child->right_child) >= Height(node->right_child->left_child))
            {
                // Right Right Heavy
                return LeftRotate(node);
            }
            else
            {
                // Right Left heavy
                return RightLeftRotate(node);
            }
        }
        return node;
    }

    int Height(ArticleTreeNode *node)
    {
        if (node == nullptr)
            return 0;
        else
            return max(Height(node->left_child), Height(node->right_child)) + 1;
    }

    int BalanceFactor(ArticleTreeNode *node)
    {
        if (node == nullptr)
            return 0;
        return Height(node->left_child) - Height(node->right_child);
    }

    ArticleTreeNode *LeftRotate(ArticleTreeNode *node)
    {
        ArticleTreeNode *temp = node->right_child;
        node->right_child = temp->left_child;
        temp->left_child = node;
        return temp;
    }

    ArticleTreeNode *RightRotate(ArticleTreeNode *node)
    {
        ArticleTreeNode *temp = node->left_child;
        node->left_child = temp->right_child;
        temp->right_child = node;
        return temp;
    }

    ArticleTreeNode *LeftRightRotate(ArticleTreeNode *node)
    {
        node->left_child = LeftRotate(node->left_child);
        return RightRotate(node);
    }

    ArticleTreeNode *RightLeftRotate(ArticleTreeNode *node)
    {
        node->right_child = RightRotate(node->right_child);
        return LeftRotate(node);
    }

    //print tree inorder
    void printInorder(ArticleTreeNode *node)
    {
        if (node == NULL)
            return;
        printInorder(node->left_child);
        cout << node->data.title << endl;
        printAuthors(node);
        printInorder(node->right_child);
    }
};

//author data class
class Author
{
public:
    short unsigned int index;
    string name;
    short unsigned int total_number_of_articles_published = 0;
    ArticlesByAuthorListNode *pstart_articles = nullptr;
    YearWiseTreeNodeForAuthor *proot_yearwiselist = nullptr;
};

//author node
class AuthorTreeNode
{
public:
    Author data;
    AuthorTreeNode *left_child;
    AuthorTreeNode *right_child;
};

//author tree
class AuthorTree
{
public:
    short unsigned int author_count = 0;
    AuthorTreeNode *root;
    AuthorTreeNode *loc;
    AuthorTreeNode *ploc;

    short unsigned int coauthor_count = 0;

    //constructor
    AuthorTree()
    {
        root = nullptr;
        loc = nullptr;
        ploc = nullptr;
    }

    //utility function to use insert
    void UseInsert(Author data)
    {
        root = Insert(root, data);
    }

    //insert into tree
    AuthorTreeNode *Insert(AuthorTreeNode *node, Author data)
    {
        // Find the correct position and insert the new node
        if (node == nullptr)
        {
            auto *node = new AuthorTreeNode();
            node->data = data;
            node->left_child = nullptr;
            node->right_child = nullptr;
            author_count++;
            return node;
        }
        if (data.name < node->data.name)
            node->left_child = Insert(node->left_child, data);
        else if (data.name > node->data.name)
            node->right_child = Insert(node->right_child, data);
        else
        {
            return node;
        }
        //re-balancing
        if (BalanceFactor(node) > 1)
        {
            if (Height(node->left_child->left_child) >= Height(node->left_child->right_child))
            {
                // Left Left Heavy Case
                return RightRotate(node);
            }
            else
            {
                // Left Right Heavy
                return LeftRightRotate(node);
            }
        }
        else if (BalanceFactor(node) < -1)
        {
            if (Height(node->right_child->right_child) >= Height(node->right_child->left_child))
            {
                // Right Right Heavy
                return LeftRotate(node);
            }
            else
            {
                // Right Left heavy
                return RightLeftRotate(node);
            }
        }
        return node;
    }

    int Height(AuthorTreeNode *node)
    {
        if (node == nullptr)
            return 0;
        else
            return max(Height(node->left_child), Height(node->right_child)) + 1;
    }

    int BalanceFactor(AuthorTreeNode *node)
    {
        if (node == nullptr)
            return 0;
        return Height(node->left_child) - Height(node->right_child);
    }

    AuthorTreeNode *LeftRotate(AuthorTreeNode *node)
    {
        AuthorTreeNode *temp = node->right_child;
        node->right_child = temp->left_child;
        temp->left_child = node;
        return temp;
    }

    AuthorTreeNode *RightRotate(AuthorTreeNode *node)
    {
        AuthorTreeNode *temp = node->left_child;
        node->left_child = temp->right_child;
        temp->right_child = node;
        return temp;
    }

    AuthorTreeNode *LeftRightRotate(AuthorTreeNode *node)
    {
        node->left_child = LeftRotate(node->left_child);
        return RightRotate(node);
    }

    AuthorTreeNode *RightLeftRotate(AuthorTreeNode *node)
    {
        node->right_child = RightRotate(node->right_child);
        return LeftRotate(node);
    }

    //print authors inorder
    void printInorder(AuthorTreeNode *node)
    {
        if (node == NULL)
            return;
        printInorder(node->left_child);
        cout << node->data.name << endl;
        printInorder(node->right_child);
    }

    //utility function to search an author by his name
    void SearchAuthor(string name)
    {
        loc = root;
        ploc = nullptr;
        if (root == nullptr)
            return;
        while (loc != nullptr && loc->data.name != name)
        {
            if (name < loc->data.name)
            {
                ploc = loc;
                loc = loc->left_child;
            }
            else
            {
                ploc = loc;
                loc = loc->right_child;
            }
        }
    }
};

//list of authors in an article
class AuthorsInArticleListNode
{
public:
    AuthorTreeNode *data;
    AuthorsInArticleListNode *next;
};

void ArticleTree::printAuthors(ArticleTreeNode *node)
{
    AuthorsInArticleListNode *temp_node = node->data.pstart_authors;
    while (temp_node != NULL)
    {
        cout << temp_node->data->data.name << ", ";
        temp_node = temp_node->next;
    }
}

//list of articles by an author
class ArticlesByAuthorListNode
{
public:
    ArticleTreeNode *data;
    ArticlesByAuthorListNode *next;
};

//yearwise tree for author
class YearWiseTreeNodeForAuthor
{
public:
    short unsigned int year;
    short unsigned int count_of_articles = 0;
    ArticlesByAuthorListNode *pstart_article_list = nullptr;
    YearWiseTreeNodeForAuthor *left_child;
    YearWiseTreeNodeForAuthor *right_child;
};

class YearWiseTreeForAuthor
{
public:
    YearWiseTreeNodeForAuthor *InsertYearNode(YearWiseTreeNodeForAuthor *node, int year)
    {
        // Find the correct position and insert the new node
        if (node == nullptr)
        {
            auto *node = new YearWiseTreeNodeForAuthor();
            node->year = year;
            node->left_child = nullptr;
            node->right_child = nullptr;
            return node;
        }
        if (year < node->year)
            node->left_child = InsertYearNode(node->left_child, year);
        else if (year > node->year)
            node->right_child = InsertYearNode(node->right_child, year);
        else
        {
            return node;
        }
        //re-balancing
        if (BalanceFactor(node) > 1)
        {
            if (Height(node->left_child->left_child) >= Height(node->left_child->right_child))
            {
                // Left Left Heavy Case
                return RightRotate(node);
            }
            else
            {
                // Left Right Heavy
                return LeftRightRotate(node);
            }
        }
        else if (BalanceFactor(node) < -1)
        {
            if (Height(node->right_child->right_child) >= Height(node->right_child->left_child))
            {
                // Right Right Heavy
                return LeftRotate(node);
            }
            else
            {
                // Right Left heavy
                return RightLeftRotate(node);
            }
        }
        return node;
    }

    int Height(YearWiseTreeNodeForAuthor *node)
    {
        if (node == nullptr)
            return 0;
        else
            return max(Height(node->left_child), Height(node->right_child)) + 1;
    }

    int BalanceFactor(YearWiseTreeNodeForAuthor *node)
    {
        if (node == nullptr)
            return 0;
        return Height(node->left_child) - Height(node->right_child);
    }

    YearWiseTreeNodeForAuthor *LeftRotate(YearWiseTreeNodeForAuthor *node)
    {
        YearWiseTreeNodeForAuthor *temp = node->right_child;
        node->right_child = temp->left_child;
        temp->left_child = node;
        return temp;
    }

    YearWiseTreeNodeForAuthor *RightRotate(YearWiseTreeNodeForAuthor *node)
    {
        YearWiseTreeNodeForAuthor *temp = node->left_child;
        node->left_child = temp->right_child;
        temp->right_child = node;
        return temp;
    }

    YearWiseTreeNodeForAuthor *LeftRightRotate(YearWiseTreeNodeForAuthor *node)
    {
        node->left_child = LeftRotate(node->left_child);
        return RightRotate(node);
    }

    YearWiseTreeNodeForAuthor *RightLeftRotate(YearWiseTreeNodeForAuthor *node)
    {
        node->right_child = RightRotate(node->right_child);
        return LeftRotate(node);
    }

    void printInorderAllTitles(YearWiseTreeNodeForAuthor *node)
    {
        if (node == NULL)
            return;
        printInorderAllTitles(node->left_child);
        auto *temp_node = node->pstart_article_list;
        cout << "Year: " << node->year << endl;
        while (temp_node != NULL)
        {
            cout << temp_node->data->data.title << endl;
            temp_node = temp_node->next;
        }
        printInorderAllTitles(node->right_child);
    }

    void printInorderCounts(YearWiseTreeNodeForAuthor *node)
    {
        if (node == NULL)
            return;
        printInorderCounts(node->left_child);
        cout << "Year: " << node->year;
        cout << " Total articles: " << node->count_of_articles << endl;
        printInorderCounts(node->right_child);
    }

    void printInorderCountofAuthors(YearWiseTreeNodeForAuthor *node, string name)
    {
        if (node == NULL)
            return;
        printInorderCountofAuthors(node->left_child, name);
        countAuthors(node, name);
        printInorderCountofAuthors(node->right_child, name);
    }

    void countAuthors(YearWiseTreeNodeForAuthor *node, string name)
    {
        auto *temp_node = node->pstart_article_list;
        int count_of_coauthors = 0;
        int publication_count = 0;
        while (temp_node != NULL)
        {
            auto *temp_author = temp_node->data->data.pstart_authors;
            publication_count++;

            while (temp_author != NULL)
            {
                if (temp_author->data->data.name != name)
                {
                    count_of_coauthors++;
                }
                temp_author = temp_author->next;
            }

            cout << "Publication: " << publication_count << " " << temp_node->data->data.title << endl;
            cout << "Year: " << node->year;
            cout << " No of Couthors: " << count_of_coauthors << endl;
            count_of_coauthors = 0;
            temp_node = temp_node->next;
        }
    }

    void printInorderPublisher(YearWiseTreeNodeForAuthor *node, string name)
    {
        if (node == NULL)
            return;
        printInorderPublisher(node->left_child, name);
        auto *temp_article = node->pstart_article_list;
        while (temp_article != NULL)
        {
            cout << "Year: " << node->year << " Journal:" << temp_article->data->data.journal << endl;
            temp_article = temp_article->next;
        }
        printInorderPublisher(node->right_child, name);
    }

    YearWiseTreeNodeForAuthor *SearchYear(YearWiseTreeNodeForAuthor *root, int year)
    {
        YearWiseTreeNodeForAuthor *loc = root;
        YearWiseTreeNodeForAuthor *ploc = nullptr;
        if (root == NULL)
            return nullptr;
        while (loc != nullptr && loc->year != year)
        {
            if (year < loc->year)
            {
                ploc = loc;
                loc = loc->left_child;
            }
            else
            {
                ploc = loc;
                loc = loc->right_child;
            }
        }

        return loc;
    }
};

class AuthorGraphNode
{
public:
    int index, weight;
    AuthorsInArticleListNode *author;
    AuthorGraphNode *right_child;
    AuthorGraphNode *left_child;
};

//Graph of authors using avl tree
class AuthorGraph
{
public:
    int number_of_vertices;
    bool are_coauthor = false;
    bool *is_visited;
    AuthorGraphNode **root;

    AuthorGraph(AuthorTree *author_tree)
    {

        number_of_vertices = author_tree->author_count;
        root = new AuthorGraphNode *[number_of_vertices]();
        for (int i = 0; i < number_of_vertices; i++)
        {
            root[i] = nullptr;
        }
        is_visited = new bool[number_of_vertices];
        for (int i = 0; i < number_of_vertices; i++)
        {
            is_visited[i] = false;
        }
    }

    void printPreorder(AuthorGraphNode *node)
    {
        if (node == NULL)
            return;
        cout << node->author->data->data.name << endl;
        printPreorder(node->left_child);
        printPreorder(node->right_child);
    }

    void AddEdge(int source, int destination, AuthorsInArticleListNode *DestAuthor)
    {
        root[source] = Insert(root[source], destination, DestAuthor);
    }

    AuthorGraphNode *Insert(AuthorGraphNode *node, int destination, AuthorsInArticleListNode *author)
    {
        // Find the correct position and insert the new node
        if (node == nullptr)
        {
            auto *node = new AuthorGraphNode();
            node->index = destination;
            node->weight = 1;
            node->author = author;
            node->left_child = nullptr;
            node->right_child = nullptr;
            return node;
        }
        if (destination < node->index)
            node->left_child = Insert(node->left_child, destination, author);
        else if (destination > node->index)
            node->right_child = Insert(node->right_child, destination, author);
        else
        {
            node->weight++;
            return node;
        }
        //re-balancing
        if (BalanceFactor(node) > 1)
        {
            if (Height(node->left_child->left_child) >= Height(node->left_child->right_child))
            {
                // Left Left Heavy Case
                return RightRotate(node);
            }
            else
            {
                // Left Right Heavy
                return LeftRightRotate(node);
            }
        }
        else if (BalanceFactor(node) < -1)
        {
            if (Height(node->right_child->right_child) >= Height(node->right_child->left_child))
            {
                // Right Right Heavy
                return LeftRotate(node);
            }
            else
            {
                // Right Left heavy
                return RightLeftRotate(node);
            }
        }
        return node;
    }

    int Height(AuthorGraphNode *node)
    {
        if (node == nullptr)
            return 0;
        else
            return max(Height(node->left_child), Height(node->right_child)) + 1;
    }

    int BalanceFactor(AuthorGraphNode *node)
    {
        if (node == nullptr)
            return 0;
        return Height(node->left_child) - Height(node->right_child);
    }

    AuthorGraphNode *LeftRotate(AuthorGraphNode *node)
    {
        AuthorGraphNode *temp = node->right_child;
        node->right_child = temp->left_child;
        temp->left_child = node;
        return temp;
    }

    AuthorGraphNode *RightRotate(AuthorGraphNode *node)
    {
        AuthorGraphNode *temp = node->left_child;
        node->left_child = temp->right_child;
        temp->right_child = node;
        return temp;
    }

    AuthorGraphNode *LeftRightRotate(AuthorGraphNode *node)
    {
        node->left_child = LeftRotate(node->left_child);
        return RightRotate(node);
    }

    AuthorGraphNode *RightLeftRotate(AuthorGraphNode *node)
    {
        node->right_child = RightRotate(node->right_child);
        return LeftRotate(node);
    }
};

// ---- Following two functions read the data and create trees  ----

AuthorTreeNode *InsertAuthor(AuthorTree *author_tree, string full_name, ArticleTreeNode *article_node, short unsigned int year)
{

    auto *articles_by_author = new ArticlesByAuthorListNode();
    articles_by_author->data = article_node;

    auto *articles_for_year = new ArticlesByAuthorListNode();
    articles_for_year->data = article_node;

    auto *article_in_year = new YearWiseTreeNodeForAuthor();
    article_in_year->count_of_articles++;

    auto *temp_tree = new YearWiseTreeForAuthor();

    AuthorTreeNode *author_node;
    author_tree->SearchAuthor(full_name);

    if (author_tree->loc == nullptr)
    {

        Author author_data;
        author_data.index = author_tree->author_count;
        author_data.name = full_name;
        author_data.pstart_articles = articles_by_author;
        author_data.total_number_of_articles_published++;
        author_data.proot_yearwiselist = temp_tree->InsertYearNode(author_data.proot_yearwiselist, year);

        author_data.proot_yearwiselist->pstart_article_list = articles_for_year;
        author_data.proot_yearwiselist->count_of_articles++;
        author_tree->UseInsert(author_data);

        author_tree->SearchAuthor(author_data.name);
        author_node = author_tree->loc;
    }
    else
    {
        author_node = author_tree->loc;
        articles_by_author->next = author_node->data.pstart_articles;
        author_node->data.pstart_articles = articles_by_author;
        author_node->data.total_number_of_articles_published++;

        article_in_year = temp_tree->SearchYear(author_node->data.proot_yearwiselist, year);

        if (article_in_year == NULL)
        {
            author_node->data.proot_yearwiselist = temp_tree->InsertYearNode(author_node->data.proot_yearwiselist, year);
            article_in_year = temp_tree->SearchYear(author_node->data.proot_yearwiselist, year);
            articles_for_year->next = article_in_year->pstart_article_list;
            article_in_year->pstart_article_list = articles_for_year;
            article_in_year->count_of_articles++;
        }
        else
        {
            articles_for_year->next = article_in_year->pstart_article_list;
            article_in_year->pstart_article_list = articles_for_year;

            article_in_year->count_of_articles++;
        }
    }
    return author_node;
}

void parser(ArticleTree *article_tree, AuthorTree *author_tree)
{
    ifstream myFileStream("data.csv");
    if (!(myFileStream.is_open()))
    {
        cout << "Failed to open file  " << endl;
        return;
    }

    string line;
    string temp;
    string first_name;
    string last_name;
    string temp_full_name = "";
    string full_name;
    int count = 0;
    getline(myFileStream, line);
    while (getline(myFileStream, line))
    {
        auto *article_node = new ArticleTreeNode();

        stringstream ss(line);
        getline(ss, article_node->data.identifier, ',');
        getline(ss, article_node->data.title, ',');
        getline(ss, article_node->data.journal, ',');
        getline(ss, article_node->data.month, ',');
        getline(ss, temp, ',');
        article_node->data.year = stoi(temp);
        getline(ss, article_node->data.publisher, ',');
        getline(ss, article_node->data.type, ',');

        int current_char = 0;

        AuthorTreeNode *author_node;
        getline(ss, temp, ',');
        do
        {
            temp_full_name += temp[current_char];
            current_char++;
            if (temp[current_char] == ';')
            {
                int position = temp_full_name.find('_');
                last_name = temp_full_name.substr(0, position);
                first_name = temp_full_name.substr(position + 1);
                full_name = first_name + "" + last_name;
                position = full_name.find(';');
                full_name[position] = ' ';
                author_node = InsertAuthor(author_tree, full_name, article_node, article_node->data.year);
                auto *temp_author = new AuthorsInArticleListNode();
                temp_author->data = author_node;

                if (article_node->data.pstart_authors == NULL)
                {
                    article_node->data.pstart_authors = temp_author;
                }
                else
                {
                    temp_author->next = article_node->data.pstart_authors;
                    article_node->data.pstart_authors = temp_author;
                }

                temp_full_name = "";
                full_name = "";
            }

        } while (temp[current_char] != '\0');
        temp = "";
        article_tree->UseInsert(article_node->data);
    }
}

// --------

// ---- Program Functions start here

// Gives total number of articles published by given author.
void Function01TotalNumberOfArticlesPublished(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Total Number of Articles published by " << author_tree->loc->data.name;
        cout << ": " << author_tree->loc->data.total_number_of_articles_published << endl;
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// Gives number of publication per year (sorted year wise) of given author
void Function02NumberofPublicationPerYear(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Number of Publiciations of author " << author_tree->loc->data.name;
        cout << " per year (Sorted Year Wise)" << endl;
        auto *temp_year = new YearWiseTreeForAuthor();
        temp_year->printInorderCounts(author_tree->loc->data.proot_yearwiselist);
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// Gives number of coauthors of each publication of given author (sorted year wise)
void Function03NumberofCoauthorsPerPublication(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Coauthors of " << author_tree->loc->data.name;
        cout << " per publication (Sorted Year Wise)" << endl;
        auto *temp_year = new YearWiseTreeForAuthor();
        temp_year->printInorderCountofAuthors(author_tree->loc->data.proot_yearwiselist, name);
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// this function is used by Function04NumberofPapersForEachPosition, counts the position from reverse
void traverseReverseList(AuthorTree *author_tree, AuthorsInArticleListNode *temp, int *array, string name, int position)
{
    if (temp == NULL)
    {
        author_tree->coauthor_count = position + 1;
        return;
    }

    traverseReverseList(author_tree, temp->next, array, name, ++position);
    if (temp->data->data.name == name)
    {
        array[author_tree->coauthor_count - position]++;
    }
}

// calculates the position of author in each article
void Function04NumberofPapersForEachPosition(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Numbers of papers published by " << author_tree->loc->data.name;
        cout << " for each author position" << endl;

        auto *temp_article = author_tree->loc->data.pstart_articles;
        int count_for_each_position[11] = {0};
        int position = 0;
        while (temp_article != NULL)
        {
            auto *temp_author = temp_article->data->data.pstart_authors;

            traverseReverseList(author_tree, temp_author, count_for_each_position, name, 1);

            temp_article = temp_article->next;
        }

        for (int i = 1; i < 11; i++)
        {
            cout << " Author Position: " << i;
            cout << " Papers published for this position: " << count_for_each_position[i] << endl;
        }
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// gives name of journals of each article by given author
void Function05PrintNameofJournal(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Name of Journals " << author_tree->loc->data.name;
        cout << " has published articles in (Sorted Year Wise)" << endl;
        auto *temp_year = new YearWiseTreeForAuthor();
        temp_year->printInorderPublisher(author_tree->loc->data.proot_yearwiselist, name);
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// this function creates edges for author gaph using author AVL tree.
void TraverseTreeForGraph(AuthorTreeNode *node, AuthorGraph *author_graph)
{
    if (node == NULL)
        return;
    TraverseTreeForGraph(node->left_child, author_graph);
    auto *temp_article = node->data.pstart_articles;
    while (temp_article != NULL)
    {
        auto *temp_author = temp_article->data->data.pstart_authors;
        while (temp_author != NULL)
        {
            if (temp_author->data->data.name != node->data.name)
            {
                author_graph->AddEdge(node->data.index, temp_author->data->data.index, temp_author);
            }
            temp_author = temp_author->next;
        }
        temp_article = temp_article->next;
    }
    TraverseTreeForGraph(node->right_child, author_graph);
}

// prints coauthores at distance d of given author
void NodeAtDistance(AuthorGraphNode *node, int d, AuthorGraph *author_graph)
{
    if (d <= 0)
        return;
    auto *temp_node = author_graph->root[node->index];

    if (author_graph->is_visited[temp_node->index] == false)
    {
        author_graph->is_visited[temp_node->index] = true;
        NodeAtDistance(temp_node, --d, author_graph);
        cout << endl
             << "At distance: " << d << endl;
        author_graph->printPreorder(temp_node);
    }
}

// used by function 06 for pre order traversal
void GraphPreOrderTraversalForFunction06(AuthorGraphNode *node, int d, AuthorGraph *author_graph)
{
    if (node == NULL)
        return;

    cout << endl
         << "Coauthors of " << node->author->data->data.name << endl
         << endl;
    NodeAtDistance(node, --d, author_graph);
    GraphPreOrderTraversalForFunction06(node->left_child, d, author_graph);
    GraphPreOrderTraversalForFunction06(node->right_child, d, author_graph);
}

// uses above two functions to find authors at distance d of given author.
void Function06PrintAtDistance(string name, int distance, AuthorTree *author_tree, AuthorGraph *author_graph)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc == nullptr)
    {
        cout << "Author Not found" << endl;
        return;
    }
    auto *graph_node = author_graph->root[author_tree->loc->data.index];
    GraphPreOrderTraversalForFunction06(graph_node, distance, author_graph);
}

// used by function 07 and 08 for Inordertraversal
void GraphTreeInorderTraversal(AuthorGraphNode *node, int index_of_author2, AuthorGraph *author_graph, int flag)
{
    if (node == NULL)
        return;

    if (node->index == index_of_author2)
    {
        author_graph->are_coauthor = true;
        if (flag == 2)
        {
            cout << "They are coauthors in " << node->weight << " articles" << endl;
        }
        return;
    }
    GraphTreeInorderTraversal(node->left_child, index_of_author2, author_graph, flag);
    GraphTreeInorderTraversal(node->right_child, index_of_author2, author_graph, flag);
}

// checks if given two authors are coauthors
void Function07CheckIfCoauthors(string name1, string name2, AuthorTree *author_tree, AuthorGraph *author_graph)
{
    author_tree->SearchAuthor(name1);
    if (author_tree->loc == NULL)
    {
        cout << "Author " << name1 << " not found" << endl;
        return;
    }
    auto *temp_author1 = author_tree->loc;
    author_tree->SearchAuthor(name2);
    if (author_tree->loc == NULL)
    {
        cout << "Author " << name2 << " not found" << endl;
        return;
    }
    auto *temp_author2 = author_tree->loc;
    auto *graph_root_node = author_graph->root[temp_author1->data.index];

    GraphTreeInorderTraversal(graph_root_node, temp_author2->data.index, author_graph, 1);

    if (author_graph->are_coauthor == true)
    {
        cout << name1 << " and " << name2 << " are coauthors" << endl;
    }
    else
    {
        cout << name1 << " and " << name2 << " are not coauthors" << endl;
    }
    author_graph->are_coauthor = false;
}

// Prints all the coauthors of a given author
void Function08PrintCoauthors(string name, AuthorTree *author_tree, AuthorGraph *author_graph)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc == NULL)
    {
        cout << "Author " << name << " not found" << endl;
        return;
    }
    auto *temp_author = author_tree->loc;
    auto *graph_root_node = author_graph->root[temp_author->data.index];
    author_graph->printPreorder(graph_root_node);
}

// Print Numbers of articles two authors have written together.
void Function09PrintNumberofArticlesCoauthored(string name1, string name2, AuthorTree *author_tree, AuthorGraph *author_graph)
{
    author_tree->SearchAuthor(name1);
    if (author_tree->loc == NULL)
    {
        cout << "Author " << name1 << " not found" << endl;
        return;
    }
    auto *temp_author1 = author_tree->loc;
    author_tree->SearchAuthor(name2);
    if (author_tree->loc == NULL)
    {
        cout << "Author " << name2 << " not found" << endl;
        return;
    }
    auto *temp_author2 = author_tree->loc;
    auto *graph_root_node = author_graph->root[temp_author1->data.index];

    GraphTreeInorderTraversal(graph_root_node, temp_author2->data.index, author_graph, 2);

    if (author_graph->are_coauthor == false)
    {
        cout << name1 << " and " << name2 << " are not coauthors" << endl;
    }

    author_graph->are_coauthor = false;
}

// Print names of all articles for given author.
void Function10PrintNameofArticlesofAuthor(AuthorTree *author_tree, string name)
{
    author_tree->SearchAuthor(name);
    if (author_tree->loc != NULL)
    {
        cout << "Name of Articles " << author_tree->loc->data.name;
        cout << " has published articles in (Sorted Year Wise)" << endl;
        auto *temp_year = new YearWiseTreeForAuthor();
        temp_year->printInorderAllTitles(author_tree->loc->data.proot_yearwiselist);
    }
    else
    {
        cout << "Author Not found" << endl;
    }
}

// Print name of all authors all with total number of authors
void Function11PrintNameofAllAuthors(AuthorTree *author_tree)
{
    cout << "Total Number of Authors " << author_tree->author_count << endl;
    author_tree->printInorder(author_tree->root);
}

// create edges for author graph using TraverseTreeForGraph function
void CreateEdges(AuthorGraph *author_graph, AuthorTree *author_tree)
{
    TraverseTreeForGraph(author_tree->root, author_graph);
}

// prompts menu
void MainScreen()
{
    cout << endl;

    cout << "1. Total number of articles published by an author" << endl;
    cout << "2. Total number of publications per year by an author" << endl;
    cout << "3. Number of coauthors per publications of an author" << endl;
    cout << "4. Number of papers for each position of an author" << endl;
    cout << "5. Print name of journals of each article by an author" << endl;
    cout << "6. Print name of authors at distance d of given author" << endl;
    cout << "7. Check if given authors have worked together" << endl;
    cout << "8. Print coauthors of given author" << endl;
    cout << "9. Number of Articles given authors have coauthored" << endl;
    cout << "10. Print name of all articles by an author" << endl;
    cout << "11. Print names of all authors" << endl;
}

int main()
{
    ArticleTree *article_tree = new ArticleTree();
    AuthorTree *author_tree = new AuthorTree();
    parser(article_tree, author_tree);
    AuthorGraph *author_graph = new AuthorGraph(author_tree);
    CreateEdges(author_graph, author_tree);
    cout << "_________________________________________________________" << endl;
    cout << "             DSA SEMESTER PROJECT" << endl;
    cout << "_________________________________________________________" << endl;
    string name = "Mohsen Guizani";
    string name1 = "Mahamod Ismail";
    string name2 = "Syed Hassan Ahmed";

    int choice = 0;
    do
    {
        MainScreen();
        cout << "Please Enter a choice:" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function01TotalNumberOfArticlesPublished(author_tree, name1);
            break;
        case 2:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function02NumberofPublicationPerYear(author_tree, name1);
            break;
        case 3:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function03NumberofCoauthorsPerPublication(author_tree, name1);
            break;
        case 4:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function04NumberofPapersForEachPosition(author_tree, name1);
            break;
        case 5:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function05PrintNameofJournal(author_tree, name1);
            break;
        case 6:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            int distance;
            cout << "Enter distance: ";
            cin >> distance;
            Function06PrintAtDistance(name1, distance, author_tree, author_graph);
            break;
        case 7:
            cout << "Enter author 1 name: ";
            cin.ignore();
            getline(cin, name1);
            cout << "Enter author 2 name: ";
            getline(cin, name2);
            Function07CheckIfCoauthors(name1, name2, author_tree, author_graph);
            break;
        case 8:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function08PrintCoauthors(name1, author_tree, author_graph);
            break;
        case 9:
            cout << "Enter author 1 name: ";
            cin.ignore();
            getline(cin, name1);
            cout << "Enter author 2 name: ";
            getline(cin, name2);
            Function09PrintNumberofArticlesCoauthored(name1, name2, author_tree, author_graph);
            break;
        case 10:
            cout << "Enter author name: ";
            cin.ignore();
            getline(cin, name1);
            Function10PrintNameofArticlesofAuthor(author_tree, name1);
            break;
        case 11:
            Function11PrintNameofAllAuthors(author_tree);
            break;
        default:
            break;
        }
        cout << endl;
        cout << "Do you want to continue? Press any key to continue or (-1) to exit" << endl;
        cin >> choice;
        cout << endl;
    } while (choice > 0);
}