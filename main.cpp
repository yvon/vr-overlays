#include <QApplication>
#include <QWebEngineView>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QWebEngineView view;
    view.setUrl(QUrl("http://localhost:8080/ui/#/chatoverlays/twitch"));
    view.resize(550, 300);
    view.show();

    view.page()->setBackgroundColor(Qt::darkGray);

    return app.exec();
}
