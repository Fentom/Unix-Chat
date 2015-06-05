/****************************************************************************
** Form implementation generated from reading ui file 'Client.ui'
**
** Created: mar. oct. 21 04:20:43 2014
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Client.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "Fichier.ini"

extern MESSAGE		M;

extern int idMsg;
extern Client* F1;
extern pid_t pidRecherche;

/*
 *  Constructs a Client as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */


Client::Client( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Client" );
    setMinimumSize( QSize( 800, 450 ) );

    textNomLogin = new QLabel( this, "textNomLogin" );
    textNomLogin->setGeometry( QRect( 20, 60, 60, 20 ) );

    lineNomLogin = new QLineEdit( this, "lineNomLogin" );
    lineNomLogin->setGeometry( QRect( 80, 60, 120, 21 ) );

    textNom = new QLabel( this, "textNom" );
    textNom->setGeometry( QRect( 550, 130, 77, 20 ) );

    lineNom = new QLineEdit( this, "lineNom" );
    lineNom->setGeometry( QRect( 660, 130, 120, 21 ) );

    lineGsm = new QLineEdit( this, "lineGsm" );
    lineGsm->setGeometry( QRect( 660, 160, 120, 21 ) );

    textRenseignement = new QLabel( this, "textRenseignement" );
    textRenseignement->setGeometry( QRect( 550, 100, 125, 20 ) );

    ButtonLogin = new QPushButton( this, "ButtonLogin" );
    ButtonLogin->setGeometry( QRect( 550, 60, 100, 29 ) );
    ButtonLogin->setAutoDefault( FALSE );

    ButtonRechercher = new QPushButton( this, "ButtonRechercher" );
    ButtonRechercher->setGeometry( QRect( 550, 190, 100, 29 ) );
    ButtonRechercher->setAutoDefault( FALSE );

    ButtonTerminer = new QPushButton( this, "ButtonTerminer" );
    ButtonTerminer->setGeometry( QRect( 680, 60, 100, 29 ) );
    ButtonTerminer->setAutoDefault( FALSE );

    ButtonModifier = new QPushButton( this, "ButtonModifier" );
    ButtonModifier->setGeometry( QRect( 680, 190, 100, 29 ) );
    ButtonModifier->setAutoDefault( FALSE );

    textConnecte = new QLabel( this, "textConnecte" );
    textConnecte->setGeometry( QRect( 550, 270, 260, 20 ) );
    //textConnecte->setGeometry( QRect( 550, 270, 160, 20 ) );

    ButtonEnvoyer = new QPushButton( this, "ButtonEnvoyer" );
    ButtonEnvoyer->setGeometry( QRect( 430, 420, 100, 29 ) );
    ButtonEnvoyer->setAutoDefault( FALSE );

    lineMessage = new QLineEdit( this, "lineMessage" );
    lineMessage->setGeometry( QRect( 20, 390, 510, 21 ) );

    textGsm = new QLabel( this, "textGsm" );
    textGsm->setGeometry( QRect( 550, 160, 77, 20 ) );

    linePublicite = new QLineEdit( this, "linePublicite" );
    linePublicite->setGeometry( QRect( 20, 20, 760, 21 ) );
    linePublicite->setReadOnly( TRUE );

    textMessageRecu = new QTextEdit( this, "textMessageRecu" );
    //textMessageRecu->setEnabled( FALSE );
    textMessageRecu->setReadOnly( TRUE );
    textMessageRecu->setGeometry( QRect( 20, 100, 510, 270 ) );

    linePersonne[0] = new QLineEdit( this, "linePersonne0" );
    linePersonne[0]->setGeometry( QRect( 550, 305, 120, 21 ) );
    linePersonne[0]->setReadOnly( TRUE );
    linePersonne[1] = new QLineEdit( this, "linePersonne1" );
    linePersonne[1]->setGeometry( QRect( 550, 335, 120, 21 ) );
    linePersonne[1]->setReadOnly( TRUE );
    linePersonne[2] = new QLineEdit( this, "linePersonne2" );
    linePersonne[2]->setGeometry( QRect( 550, 365, 120, 21 ) );
    linePersonne[2]->setReadOnly( TRUE );
    linePersonne[3] = new QLineEdit( this, "linePersonne3" );
    linePersonne[3]->setGeometry( QRect( 550, 395, 120, 21 ) );
    linePersonne[3]->setReadOnly( TRUE );
    linePersonne[4] = new QLineEdit( this, "linePersonne4" );
    linePersonne[4]->setGeometry( QRect( 550, 425, 120, 21 ) );
    linePersonne[4]->setReadOnly( TRUE );

    checkBox[0] = new QCheckBox( this, "checkBox0" );
    checkBox[0]->setGeometry( QRect( 685, 305, 90, 20 ) );
    checkBox[1] = new QCheckBox( this, "checkBox1" );
    checkBox[1]->setGeometry( QRect( 685, 335, 90, 20 ) );
    checkBox[2] = new QCheckBox( this, "checkBox2" );
    checkBox[2]->setGeometry( QRect( 685, 365, 90, 20 ) );
    checkBox[3] = new QCheckBox( this, "checkBox3" );
    checkBox[3]->setGeometry( QRect( 685, 395, 90, 20 ) );
    checkBox[4] = new QCheckBox( this, "checkBox4" );
    checkBox[4]->setGeometry( QRect( 685, 425, 90, 20 ) );
    languageChange();
    resize( QSize(800, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ButtonLogin, SIGNAL( clicked() ), this, SLOT( Login() ) );
    connect( ButtonTerminer, SIGNAL( clicked() ), this, SLOT( Terminer() ) );
    connect( ButtonRechercher, SIGNAL( clicked() ), this, SLOT( Rechercher() ) );
    connect( ButtonModifier, SIGNAL( clicked() ), this, SLOT( Modifier() ) );
    connect( ButtonEnvoyer, SIGNAL( clicked() ), this, SLOT( Envoyer() ) );
    connect( checkBox[0], SIGNAL( clicked() ), this, SLOT( Selection0() ) );
    connect( checkBox[1], SIGNAL( clicked() ), this, SLOT( Selection1() ) );
    connect( checkBox[2], SIGNAL( clicked() ), this, SLOT( Selection2() ) );
    connect( checkBox[3], SIGNAL( clicked() ), this, SLOT( Selection3() ) );
    connect( checkBox[4], SIGNAL( clicked() ), this, SLOT( Selection4() ) );

    lineNom->setReadOnly( TRUE );
    lineGsm->setReadOnly( TRUE );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Client::~Client()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Client::languageChange()
{
    setCaption( tr( "Dossier Unix 2014" ) );
    textNomLogin->setText( tr( "Nom :" ) );
    textNom->setText( tr( "Nom :" ) );
    textRenseignement->setText( tr( "<b>Renseignements :</b>" ) );
    ButtonLogin->setText( tr( "Login" ) );
    ButtonRechercher->setText( tr( "Rechercher" ) );
    ButtonTerminer->setText( tr( "Terminer" ) );
    ButtonModifier->setText( tr( "---" ) );
    textConnecte->setText( tr( "<b>Personnes connectees</b>" ) );
    ButtonEnvoyer->setText( tr( "Envoyer" ) );
    textGsm->setText( tr( "Gsm :" ) );
    checkBox[0]->setText( tr( "Refuse" ) );
    checkBox[1]->setText( tr( "Refuse" ) );
    checkBox[2]->setText( tr( "Refuse" ) );
    checkBox[3]->setText( tr( "Refuse" ) );
    checkBox[4]->setText( tr( "Refuse" ) );
}

void Client::Login()
{
    M.Type = 1L;
    M.Requete = LOGIN;
    M.idPid = getpid();
    strcpy(M.Donnee, lineNomLogin->text());
    if (strcmp(M.Donnee, "") == 0)
    {
        setMessage("Login vide\n");
    }
    else
    {
        alarm(120);
        F1->textMessageRecu->clear();
        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        }
    }
}

void Client::Terminer()
{
    int i; 

    M.Type = 1L;
    M.Requete = TERMINER;
    M.idPid = getpid();
    strcpy(M.Donnee, lineNomLogin->text());

    if(strcmp(M.Donnee, "") == 0)
    {
        setMessage("Vous n'etes pas connecte\n");
    }
    else
    {
        alarm(0);
        
        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        } 

        for(i = 0; i < 5; i++)
        {
            F1->setPersonne(i, "");
            checkBox[i]->setChecked(false);
        }


        lineNomLogin->setReadOnly( FALSE );
        lineNom->setReadOnly( TRUE );
        lineGsm->setReadOnly( TRUE );

        lineNomLogin->setText("");

        lineGsm->setText("");
        lineNom->setText("");

        setMessage("Vous avez bien ete deconnecte\n");
    }
}

void Client::Rechercher()
{
    char	B[255];
    strcpy(B, ButtonRechercher->text());
    strcpy(M.Donnee, lineNom->text());

    M.Type = 1L;
    M.idPid = getpid();

    if (strcmp(lineNomLogin->text(), "") == 0 )
    {
        setMessage("Vous n'etes pas connecte\n");
    }
    else
    {
        alarm(120);

        if (strcmp(B,"Rechercher") == 0)
        {
            if(strcmp(M.Donnee, lineNomLogin->text()) == 0)
            {
                ButtonModifier->setText( tr( "Modifier" ) );
                ButtonRechercher->setText( tr( "Annuler") );
            }
        

            M.Requete = RECHERCHER;

            if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
            { 
                perror("Erreur:");
                 exit(1);
            }

            // if( msgrcv(idMsg,&M,sizeof(M) - sizeof(long), getpid(), 0) == -1 )
            // {
            //     perror("Erreur:");
            //     exit(0);
            // }

            // lineGsm->setText(M.Donnee);

            return;
        }

        M.Requete = ANNULER;
        M.Type = pidRecherche;

        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        }

        ButtonModifier->setText( tr( "---" ) );
        ButtonRechercher->setText( tr( "Rechercher") );
    }
    return;
}

void Client::Modifier()
{
    alarm(120);

    M.Type = pidRecherche;
    M.idPid = getpid();
    strcpy(M.Donnee, lineGsm->text());
    M.Requete = MODIFIER;

    if (strcmp(lineNomLogin->text(), "") != 0 )
    {
        alarm(120);
        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        }

        ButtonModifier->setText( tr( "---" ) );
        ButtonRechercher->setText( tr( "Rechercher" ) );
    }
}

void Client::Envoyer()
{
// M.Donnee[0] = '\0';
// strcat(M.Donnee,lineMessage->text());
// strcat(M.Donnee,"\n");
// setMessage(M.Donnee);
// lineMessage->clear();

    alarm(120);

    M.Type = 1L;
    M.Requete = ENVOYER;
    M.idPid = getpid();
    strcpy(M.Donnee, lineMessage->text());

    if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
    { 
        perror("Erreur:");
         exit(1);
    }

    lineMessage->clear();
}

void Client::Selection1()
{
    Selection(1);
}

void Client::Selection2()
{
    Selection(2);
}

void Client::Selection3()
{
    Selection(3);
}

void Client::Selection4()
{
    Selection(4);
}

void Client::Selection0()
{
    Selection(0);
}

void Client::Selection(int i)
{
    alarm(120);

    if (checkBox[i]->isChecked()) 
    { 
        checkBox[i]->setText( tr( "Accepte" ) );
        M.Type = 1L;
        M.idPid = getpid();
        M.Requete = ACCEPTER;
        strcpy(M.Donnee, linePersonne[i]->text());
        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        }
    }
    else
    { 
        checkBox[i]->setText( tr( "Refuse" ) );
        M.Type = 1L;
        M.idPid = getpid();
        M.Requete = REFUSER;
        strcpy(M.Donnee, linePersonne[i]->text());
        if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
        { 
            perror("Erreur:");
             exit(1);
        }
    }

}

void Client::setPersonne(int i,const char *T)
{
    linePersonne[i]->setText(T);
}

const char* Client::getPersonne(int i) const 
{
    return linePersonne[i]->text();
}

void Client::setMessage(const char *T)
{
    QString	TInter = textMessageRecu->text();
    TInter = TInter.append(T);
    textMessageRecu->setText(TInter);
}

void Client::setPublicite(const char *T)
{
    linePublicite->setText(T);
}

void Client::setCheckbox(int i,const bool b)
{
    checkBox[i]->setChecked(b);
    if (b) checkBox[i]->setText( tr("Accepte" ) );
    else checkBox[i]->setText( tr("Refuse" ) );
}

void Client::setGsm(const char *T)
{
    lineGsm->setText(T);

}

void Client::setConnexion()
{
    lineNomLogin->setReadOnly( TRUE );
    lineNom->setReadOnly( FALSE );
    lineGsm->setReadOnly( FALSE );
}
