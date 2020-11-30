#include "textcipher.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextCipher w;
    w.setFixedSize(470, 660);
    w.show();
    return a.exec();
}
