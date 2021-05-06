#include "dialog.h"
#include "skin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Skin::InitSkin();

    Dialog w;

    w.resize(800, 480);
    w.show();

    return a.exec();
}
