#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "DatabaseManager.h"
#include "TableModel.h"
#include "CustomQuickImageProvider.h"

#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>

#include <QApplication>
#include <QTableView>

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("Attraction Sales Service");
    QGuiApplication::setOrganizationName("Baxulio");

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //QGuiApplication app(argc, argv);
    QApplication app(argc,argv);
    QIcon::setThemeName("gallery");

    QSettings settings;
    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());

//    if(settings.contains("host")){
//        DatabaseManager::instance().connectToDatabase(settings.value("host").toString(),
//                              settings.value("user").toString(),
//                              settings.value("password").toString(),
//                              settings.value("port").toInt());
//    }
//    else
        DatabaseManager::instance().connectToDatabase();

    ////////////
    /// \brief productTypesModel
    ///
    TableModel *productTypesModel = new TableModel(), *productsModel = new TableModel();
    productTypesModel->setTable("product_types");
    productTypesModel->generateRoleNames();
    productTypesModel->select();

    productsModel->setTable("products");
    productsModel->generateRoleNames();
    productsModel->select();

    QTableView *view = new QTableView(), *view2 = new QTableView();
    view->setModel(productTypesModel);
    view2->setModel(productsModel);
    view->show();
    view2->show();
    //////////////////////

    qmlRegisterSingletonType(QUrl("qrc:/Core.qml"),"Core", 1,0,"Core");
    qmlRegisterType<TableModel>("baxulio.models.tableModel",1,0,"TableModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("availableStyles", QQuickStyle::availableStyles());
    engine.rootContext()->setContextProperty("databaseManager", &DatabaseManager::instance());
    engine.rootContext()->setContextProperty("productTypesModel", productTypesModel);
    engine.rootContext()->setContextProperty("productsModel", productsModel);

    CustomQuickImageProvider *productTypesImageProvider = new CustomQuickImageProvider();
    productTypesImageProvider->setTableModel(*productTypesModel);
    CustomQuickImageProvider *productsImageProvider = new CustomQuickImageProvider();
    productsImageProvider->setTableModel(*productsModel);

    engine.addImageProvider("product_types",productTypesImageProvider); //"image://product_types/"
    engine.addImageProvider("products",productsImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
