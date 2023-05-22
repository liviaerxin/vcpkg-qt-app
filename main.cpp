#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QPushButton button("Hello world !");
    button.setGeometry(200, 200, 200, 200);
    button.show();

    return app.exec();
}