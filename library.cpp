#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime>

#include "library.h"

using namespace std;

// Fonction pour enregistrer les activités dans un fichier texte(BONUS)
static void logActivity(const string &message)
{
    ofstream log("logs.txt", ios::app); // fin du fichier
    if (!log.is_open())
        return;

    // Ajout de la date et heure actuelle
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    log << "[" << buf << "] " << message << "\n";
}

// Constructor
Library::Library() {}

// Add book to library
void Library::addBook(const Book &book)
{
    // On fait une copie du livre pour pouvoir modifier le titre et l'auteur
    Book formattedBook = book;

    string title = formattedBook.getTitle();
    string author = formattedBook.getAuthor();

    // Fonction utilitaire inline pour mettre en majuscule la première lettre
    auto format = [](string &str)
    {
        if (!str.empty())
        {
            // tout en minuscule d'abord
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            // première lettre en majuscule
            str[0] = toupper(str[0]);
        }
    };

    format(title);
    format(author);

    // setters :
    formattedBook.setTitle(title);
    formattedBook.setAuthor(author);

    // Ajout du livre formaté
    books.push_back(make_unique<Book>(formattedBook));

    // Tri automatique par titre
    sort(books.begin(), books.end(),
         [](const unique_ptr<Book> &a, const unique_ptr<Book> &b)
         {
             return a->getTitle() < b->getTitle();
         });
}


// Remove book from library
bool Library::removeBook(const string &isbn)
{
    auto it = find_if(books.begin(), books.end(),
                      [&isbn](const unique_ptr<Book> &book)
                      {
                          return book->getISBN() == isbn;
                      });

    if (it != books.end())
    {
        books.erase(it);
        return true;
    }
    return false;
}

// Find book by ISBN
Book *Library::findBookByISBN(const string &isbn)
{
    auto it = find_if(books.begin(), books.end(),
                      [&isbn](const unique_ptr<Book> &book)
                      {
                          return book->getISBN() == isbn;
                      });

    return (it != books.end()) ? it->get() : nullptr;
}

// Search books by title (case-insensitive partial match)
vector<Book *> Library::searchBooksByTitle(const string &title)
{
    vector<Book *> results;
    string lowerTitle = title;
    transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

    for (auto &book : books)
    {
        string bookTitle = book->getTitle();
        transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);

        if (bookTitle.find(lowerTitle) != string::npos)
        {
            results.push_back(book.get());
        }
    }
    return results;
}

// Search books by author (case-insensitive partial match)
vector<Book *> Library::searchBooksByAuthor(const string &author)
{
    vector<Book *> results;
    string lowerAuthor = author;
    transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);

    for (auto &book : books)
    {
        string bookAuthor = book->getAuthor();
        transform(bookAuthor.begin(), bookAuthor.end(), bookAuthor.begin(), ::tolower);

        if (bookAuthor.find(lowerAuthor) != string::npos)
        {
            results.push_back(book.get());
        }
    }
    return results;
}

// Get all available books
vector<Book *> Library::getAvailableBooks()
{
    vector<Book *> available;
    for (auto &book : books)
    {
        if (book->getAvailability())
        {
            available.push_back(book.get());
        }
    }
    return available;
}

// Get all books
vector<Book *> Library::getAllBooks()
{
    vector<Book *> allBooks;
    for (auto &book : books)
    {
        allBooks.push_back(book.get());
    }
    return allBooks;
}

// Add user to library
void Library::addUser(const User &user)
{
    users.push_back(make_unique<User>(user));
}

// Find user by ID
User *Library::findUserById(const string &userId)
{
    auto it = find_if(users.begin(), users.end(),
                      [&userId](const unique_ptr<User> &user)
                      {
                          return user->getUserId() == userId;
                      });

    return (it != users.end()) ? it->get() : nullptr;
}

// Get all users
vector<User *> Library::getAllUsers()
{
    vector<User *> allUsers;
    for (auto &user : users)
    {
        allUsers.push_back(user.get());
    }
    return allUsers;
}

// Check out book
bool Library::checkOutBook(const string &isbn, const string &userId)
{
    Book *book = findBookByISBN(isbn);
    User *user = findUserById(userId);

    if (book && user && book->getAvailability())
    {
        book->checkOut(user->getName());
        user->borrowBook(isbn);

        // loggue l'activité d'emprunt
        logActivity("BORROW user=" + user->getName() + " isbn=" + isbn + " titre=" + book->getTitle());
        return true;
    }
    return false;
}

// Return book
bool Library::returnBook(const string &isbn)
{
    Book *book = findBookByISBN(isbn);

    if (book && !book->getAvailability())
    {
        // Find the user who borrowed the book
        for (auto &user : users)
        {
            if (user->hasBorrowedBook(isbn))
            {
                // loggue l'activité de retour
                logActivity("RETURN user=" + user->getName() +
                            " isbn=" + isbn +
                            " titre=" + book->getTitle());

                user->returnBook(isbn);
                book->returnBook();
                return true;
            }
        }
    }

    return false;
}

// Display all books
void Library::displayAllBooks()
{
    if (books.empty())
    {
        cout << "Aucun livre dans la bibliothèque.\n";
        return;
    }

    //Tri des livres par titre (insensible à la casse)
    sort(books.begin(), books.end(), [](const unique_ptr<Book> &a, const unique_ptr<Book> &b)
    {
        string titleA = a->getTitle();
        string titleB = b->getTitle();

        //Met en minuscules pour une comparaison uniforme
        transform(titleA.begin(), titleA.end(), titleA.begin(), ::tolower);
        transform(titleB.begin(), titleB.end(), titleB.begin(), ::tolower);

        return titleA < titleB;
    });

    cout << "\n=== TOUS LES LIVRES (triés par titre) ===\n";
    for (size_t i = 0; i < books.size(); ++i)
    {
        cout << "\nLivre " << (i + 1) << " :\n";
        cout << books[i]->toString() << "\n";
        cout << "-------------------------\n";
    }
}



// Display available books
void Library::displayAvailableBooks()
{
    auto available = getAvailableBooks();

    if (available.empty())
    {
        cout << "Aucun livre disponible pour emprunt.\n";
        return;
    }

    // Tri des livres disponibles par auteur avant affichage
    sort(available.begin(), available.end(),
         [](const Book *a, const Book *b)
         {
             return a->getAuthor() < b->getAuthor();
         });

    cout << "\n=== LIVRES DISPONIBLES (triés par auteur) ===\n";

    for (size_t i = 0; i < available.size(); ++i)
    {
        cout << "\nLivre " << (i + 1) << " :\n";
        cout << "Titre  : " << available[i]->getTitle() << "\n";
        cout << "Auteur : " << available[i]->getAuthor() << "\n";
        cout << "ISBN   : " << available[i]->getISBN() << "\n";
        cout << "Statut : Disponible\n";
        cout << "---------------------------\n";
    }
}

// Display all users
void Library::displayAllUsers()
{
    if (users.empty())
    {
        cout << "Aucun utilisateur enregistré.\n";
        return;
    }

    cout << "\n=== TOUS LES UTILISATEURS ===\n";
    for (size_t i = 0; i < users.size(); ++i)
    {
        cout << "\nUtilisateur " << (i + 1) << " :\n";
        cout << users[i]->toString() << "\n";
        cout << "------------------------------\n";
    }
}

// Statistics
int Library::getTotalBooks() const { return books.size(); }
int Library::getAvailableBookCount() const
{
    return count_if(books.begin(), books.end(),
                    [](const unique_ptr<Book> &book)
                    {
                        return book->getAvailability();
                    });
}
int Library::getCheckedOutBookCount() const { return getTotalBooks() - getAvailableBookCount(); }