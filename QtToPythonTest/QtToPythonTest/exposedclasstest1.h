#ifndef EXPOSEDCLASSTEST1_H
#define EXPOSEDCLASSTEST1_H

#include <QObject>

class ExposedClassTest1 : public QObject
{
    Q_OBJECT
public:
    explicit ExposedClassTest1(QObject *parent = 0);

signals:

public slots:
    bool printHello();
};

#endif // EXPOSEDCLASSTEST1_H
