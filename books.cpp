// TODO FICHIER MANQUANT : books.cpp
#include "book.h"
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

// Constructeurs
//(Constructeur par défaut)
Book::Book()
    : title(""), author(""), isbn(""), isAvailable(true), borrowerName("") {}

//Constructeur avec paramètres :crée un livre avec ses infos de base
Book::Book(const string& title, const string& author, const string& isbn)
    : title(title), author(author), isbn(isbn), isAvailable(true), borrowerName("") {}

// Getters
//Retourne les infos du livre selon l'attribut demandé
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getISBN() const { return isbn; }
bool Book::getAvailability() const { return isAvailable; }
string Book::getBorrowerName() const { return borrowerName; }

// Setters
//Modifie les infos du livre selon ce qui est passé en paramètre
void Book::setTitle(const string& title) { this->title = title; }
void Book::setAuthor(const string& author) { this->author = author; }
void Book::setISBN(const string& isbn) { this->isbn = isbn; }
void Book::setAvailability(bool available) { this->isAvailable = available; }
void Book::setBorrowerName(const string& name) { this->borrowerName = name; }

// Méthodes

// checkOut: C'est quand le user emprunte le livre
void Book::checkOut(const string& borrower) {
    if (isAvailable) {
        isAvailable = false;
        borrowerName = borrower;
    } else {
        cout << "Le livre \"" << title << "\" est déjà emprunté par " << borrowerName << ".\n";
    }
}

// returnBook : quand le livre est retourné dans la bibli
void Book::returnBook() {
    if (!isAvailable) {
        isAvailable = true;
        borrowerName = "";
    } else {
        cout << "Le livre \"" << title << "\" est déjà disponible.\n";
    }
}

// toString : Pour afficher les infos du livre
string Book::toString() const {
    string status = isAvailable ? "Disponible" : "Emprunté par " + borrowerName;
    return "Titre : " + title + "\nAuteur : " + author + "\nISBN : " + isbn + "\nStatut : " + status;
}

// toFileFormat : Sauvegarde un livre dans le fichier books.txt
// Meme format que dans les fichiers d'exemple (séparé par |)
string Book::toFileFormat() const {
    // 1 = disponible, 0 = emprunté
    return title + "|" + author + "|" + isbn + "|" + (isAvailable ? "1" : "0") + "|" + borrowerName;
}

// fromFileFormat : sert à charger un livre depuis une ligne du fichier
void Book::fromFileFormat(const string& line) {
    stringstream ss(line);
    string token;
    vector<string> parts;

    // Découpe la ligne par caractère '|'
    while (getline(ss, token, '|')) {
        parts.push_back(token);
    }

    // Si la ligne contient bien les 5 infos attendues
    if (parts.size() >= 5) {
        title = parts[0];
        author = parts[1];
        isbn = parts[2];
        isAvailable = (parts[3] == "1");
        borrowerName = parts[4];
    }
}
