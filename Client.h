/****************************************************************************
** Form interface generated from reading ui file 'Client.ui'
**
** Created: mer. oct. 22 03:38:31 2014
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CLIENT_H
#define CLIENT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QCheckBox;

class Client : public QDialog
{
    Q_OBJECT

public:
    Client( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Client();

    QLabel* textNomLogin;
    QLineEdit* lineNomLogin;
    QLabel* textNom;
    QLineEdit* lineNom;
    QLineEdit* lineGsm;
    QLabel* textRenseignement;
    QPushButton* ButtonLogin;
    QPushButton* ButtonRechercher;
    QPushButton* ButtonTerminer;
    QPushButton* ButtonModifier;
    QLabel* textConnecte;
    QPushButton* ButtonEnvoyer;
    QLineEdit* lineMessage;
    QLabel* textGsm;
    QLineEdit* linePublicite;
    QTextEdit* textMessageRecu;
    QLineEdit* linePersonne[5];
    QCheckBox* checkBox[5];


public slots:
    virtual void Login();
    virtual void Terminer();
    virtual void Rechercher();
    virtual void Modifier();
    virtual void Envoyer();
    virtual void Selection1();
    virtual void Selection2();
    virtual void Selection3();
    virtual void Selection4();
    virtual void Selection0();
    virtual void Selection(int);
    virtual void setPersonne(int, const char*);
    virtual const char* getPersonne(int) const ;
    virtual void setMessage(const char*);
    virtual void setPublicite(const char*);
    virtual void setCheckbox(int, const bool);
    virtual void setGsm(const char*);
    virtual void setConnexion();

protected:

protected slots:
    virtual void languageChange();

};

#endif // CLIENT_H
