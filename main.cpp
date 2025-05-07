#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QList>
#include <QPointer>
#include <functional>
#include <QDebug>
#include <QScreen>        // 新增：包含 QScreen
#include <QGuiApplication> // 新增：包含 QGuiApplication

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DesktopPet_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 用于存储所有宠物窗口指针的列表
    QList<QPointer<MainWindow>> petWindows;

    // 预先声明 lambda 函数，以便互相引用
    std::function<void(MainWindow*)> handleDismissArmy;
    std::function<void()> handleSummonArmy;
    std::function<void()> createNewPet;

    createNewPet = [&]() {
        MainWindow *newPet = new MainWindow();
        QPointer<MainWindow> petPtr = newPet;
        petWindows.append(petPtr);

        // 连接所有需要的信号
        QObject::connect(newPet, &MainWindow::summonNewPetRequested, createNewPet);
        QObject::connect(newPet, &MainWindow::summonArmyRequested, handleSummonArmy); // 连接召唤军团信号
        QObject::connect(newPet, &MainWindow::dismissArmyRequested, handleDismissArmy); // 连接解散军团信号

        QObject::connect(newPet, &QObject::destroyed, [&petWindows, petPtr]() {
            petWindows.removeAll(petPtr);
            qDebug() << "Pet window closed. Remaining pets:" << petWindows.count();
        });

        newPet->show();
        qDebug() << "New pet summoned. Total pets:" << petWindows.count();
    };

    handleSummonArmy = [&]() {
        qDebug() << "Summoning the army!";
        int armySize = 15; // 定义军团大小
        int currentPetCount = petWindows.count();
        int maxPets = 50; // 设置一个最大数量限制，防止卡死

        for (int i = 0; i < armySize && (currentPetCount + i) < maxPets; ++i) {
            // 可以考虑加一点点延迟，但可能会让体验变慢，暂时不加
            createNewPet();
        }
         if (currentPetCount + armySize >= maxPets) {
             qDebug() << "Army limit reached.";
         }
    };

    handleDismissArmy = [&](MainWindow* sender) {
        qDebug() << "Dismissing the army, keeping sender:" << sender;
        if (!sender) return; // 如果发送者无效，则不执行任何操作

        // 创建一个要关闭的窗口列表的副本，避免在迭代时修改列表
        QList<QPointer<MainWindow>> windowsToClose;
        for (const auto& petPtr : petWindows) {
            if (petPtr && petPtr.data() != sender) {
                windowsToClose.append(petPtr);
            }
        }

        qDebug() << "Attempting to close" << windowsToClose.count() << "windows.";
        // 关闭副本列表中的所有窗口
        for (const auto& petPtr : windowsToClose) {
            if (petPtr) { // 再次检查指针是否有效
                petPtr->close(); // 请求关闭窗口
            }
        }
        // 注意：窗口关闭是异步的，petWindows 列表会在 destroyed 信号槽中更新
    };

    // 创建第一个宠物
    createNewPet();

    // 如果列表为空 (例如第一个窗口创建失败或立即关闭)，则退出
    if (petWindows.isEmpty()) {
        return 0;
    }

    return a.exec();
}
