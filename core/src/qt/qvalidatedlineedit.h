/***********************************************************************
***********Copyright (c) 2011-2013 The Bitcoin developers***************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_QT_QVALIDATEDLINEEDIT_H
#define MUBDI_QT_QVALIDATEDLINEEDIT_H

#include <QLineEdit>

/** Line edit that can be marked as "invalid" to show input validation feedback. When marked as invalid,
   it will get a red background until it is focused.
 */
class QValidatedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QValidatedLineEdit(QWidget* parent);
    void clear();
    void setCheckValidator(const QValidator* v);

protected:
    void focusInEvent(QFocusEvent* evt);
    void focusOutEvent(QFocusEvent* evt);

private:
    bool valid;
    const QValidator* checkValidator;

public Q_SLOTS:
    void setValid(bool valid);
    void setEnabled(bool enabled);

private Q_SLOTS:
    void markValid();
    void checkValidity();
};

#endif /* MUBDI_QT_QVALIDATEDLINEEDIT_H */
