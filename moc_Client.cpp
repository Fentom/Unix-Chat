/****************************************************************************
** Client meta object code from reading C++ file 'Client.h'
**
** Created: Wed Oct 22 14:31:45 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "Client.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Client::className() const
{
    return "Client";
}

QMetaObject *Client::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Client( "Client", &Client::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Client::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Client", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Client::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Client", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Client::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Login", 0, 0 };
    static const QUMethod slot_1 = {"Terminer", 0, 0 };
    static const QUMethod slot_2 = {"Rechercher", 0, 0 };
    static const QUMethod slot_3 = {"Modifier", 0, 0 };
    static const QUMethod slot_4 = {"Envoyer", 0, 0 };
    static const QUMethod slot_5 = {"Selection1", 0, 0 };
    static const QUMethod slot_6 = {"Selection2", 0, 0 };
    static const QUMethod slot_7 = {"Selection3", 0, 0 };
    static const QUMethod slot_8 = {"Selection4", 0, 0 };
    static const QUMethod slot_9 = {"Selection0", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"Selection", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"setPersonne", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_charstar, 0, QUParameter::Out },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"getPersonne", 2, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"setMessage", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"setPublicite", 1, param_slot_14 };
    static const QUMethod slot_15 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Login()", &slot_0, QMetaData::Public },
	{ "Terminer()", &slot_1, QMetaData::Public },
	{ "Rechercher()", &slot_2, QMetaData::Public },
	{ "Modifier()", &slot_3, QMetaData::Public },
	{ "Envoyer()", &slot_4, QMetaData::Public },
	{ "Selection1()", &slot_5, QMetaData::Public },
	{ "Selection2()", &slot_6, QMetaData::Public },
	{ "Selection3()", &slot_7, QMetaData::Public },
	{ "Selection4()", &slot_8, QMetaData::Public },
	{ "Selection0()", &slot_9, QMetaData::Public },
	{ "Selection(int)", &slot_10, QMetaData::Public },
	{ "setPersonne(int,const char*)", &slot_11, QMetaData::Public },
	{ "getPersonne(int)", &slot_12, QMetaData::Public },
	{ "setMessage(const char*)", &slot_13, QMetaData::Public },
	{ "setPublicite(const char*)", &slot_14, QMetaData::Public },
	{ "languageChange()", &slot_15, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Client", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Client.setMetaObject( metaObj );
    return metaObj;
}

void* Client::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Client" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Client::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Login(); break;
    case 1: Terminer(); break;
    case 2: Rechercher(); break;
    case 3: Modifier(); break;
    case 4: Envoyer(); break;
    case 5: Selection1(); break;
    case 6: Selection2(); break;
    case 7: Selection3(); break;
    case 8: Selection4(); break;
    case 9: Selection0(); break;
    case 10: Selection((int)static_QUType_int.get(_o+1)); break;
    case 11: setPersonne((int)static_QUType_int.get(_o+1),(const char*)static_QUType_charstar.get(_o+2)); break;
    case 12: static_QUType_charstar.set(_o,getPersonne((int)static_QUType_int.get(_o+1))); break;
    case 13: setMessage((const char*)static_QUType_charstar.get(_o+1)); break;
    case 14: setPublicite((const char*)static_QUType_charstar.get(_o+1)); break;
    case 15: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Client::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Client::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Client::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
