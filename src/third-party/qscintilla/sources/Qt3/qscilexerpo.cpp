// This module implements the QsciLexerPO class.
//
// Copyright (c) 2014 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerpo.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPO::QsciLexerPO(QObject *parent, const char *name)
    : QsciLexer(parent,name),
      fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerPO::~QsciLexerPO()
{
}


// Returns the language name.
const char *QsciLexerPO::language() const
{
    return "PO";
}


// Returns the lexer name.
const char *QsciLexerPO::lexer() const
{
    return "po";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPO::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPO::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS", 9);
#elif defined(Q_OS_MAC)
        f = QFont("Georgia", 13);
#else
        f = QFont("Bitstream Vera Serif", 9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the user name of a style.
QString QsciLexerPO::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case MessageId:
        return tr("Message identifier");

    case MessageIdText:
        return tr("Message identifier text");

    case MessageString:
        return tr("Message string");

    case MessageStringText:
        return tr("Message string text");

    case MessageContext:
        return tr("Message context");

    case MessageContextText:
        return tr("Message context text");

    case Fuzzy:
        return tr("Fuzzy flag");

    case ProgrammerComment:
        return tr("Programmer comment");

    case Reference:
        return tr("Reference");

    case Flags:
        return tr("Flags");

    case MessageIdTextEOL:
        return tr("Message identifier text end-of-line");

    case MessageStringTextEOL:
        return tr("Message string text end-of-line");

    case MessageContextTextEOL:
        return tr("Message context text end-of-line");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerPO::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerPO::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    bool ok, flag;

    flag = qs.readBoolEntry(prefix + "foldcomments", false, &ok);

    if (ok)
        fold_comments = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "foldcompact", true, &ok);

    if (ok)
        fold_compact = flag;
    else
        rc = false;

    return rc;
}


// Write properties to the settings.
bool QsciLexerPO::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    if (!qs.writeEntry(prefix + "foldcomments", fold_comments))
        rc = false;

    if (!qs.writeEntry(prefix + "foldcompact", fold_compact))
        rc = false;

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerPO::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerPO::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerPO::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerPO::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerPO::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPO::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
