#ifndef CHECKBOXARRAY_H
#define CHECKBOXARRAY_H

#include <QWidget>
#include "types.h"

class QListWidget;

namespace Ui {
class CheckBoxarray;
}

class CheckBoxarray : public QWidget
{
    Q_OBJECT

public:
    explicit CheckBoxarray(QWidget *parent = nullptr);
    ~CheckBoxarray();
    void setBits(value_t);
    value_t getBits();
private:
    Ui::CheckBoxarray *ui;
    QListWidget *listWidget = nullptr;
    QStringList itemLabels;
};

#endif // CHECKBOXARRAY_H
