#ifndef BASETRANSIVERWIDGET_H
#define BASETRANSIVERWIDGET_H

#include <QObject>
#include <QWidget>

class BaseTransiverWidget : public QWidget
{
public:
    explicit BaseTransiverWidget(QObject* parent = nullptr, int ch = 0);
    int chanell = 0;
};

#endif // BASETRANSIVERWIDGET_H
