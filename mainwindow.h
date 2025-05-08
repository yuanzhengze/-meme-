#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMovie>
#include <QStringList>
#include <QPoint>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QEasingCurve>
#include <QList>
#include <QPointer>
#include "catrungamewidget.h" // 新增：包含猫猫快跑游戏窗口头文件

struct MemeData {
    QString gifPath;
    QStringList soundPaths;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void summonNewPetRequested();
    void summonArmyRequested(); // 新增：召唤军团信号
    void dismissArmyRequested(MainWindow* sender); // 新增：解散军团信号（传递发送者）

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void switchRandomMeme();
    void showContextMenu(const QPoint &pos);
    void toggleAutoSwitch();
    void onAutoSwitchTimeout();
    void showRandomDanmu();
    void startFloatAnimation();
    void summonNewPet();
    void summonArmy(); // 新增：召唤军团槽
    void dismissArmy(); // 新增：解散军团槽
    void startCatRunGame(); // 新增：猫猫快跑游戏槽函数
    public:
        void startPomodoro();
    void pausePomodoro();
    void resetPomodoro();

private:
    void loadMeme(int index);
    QLabel *label;
    QMovie *movie;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QTimer *autoSwitchTimer;
    bool isAutoSwitching;
    QList<MemeData> memeDataList;
    QList<QString> danmuList;
    QTimer *danmuTimer;
    QAction *autoSwitchAction;
    QPoint lastPos;
    QPropertyAnimation *floatAnimation;
    QTimer *floatTimer;
    
    // 新增成员变量
    int clickCount; // 点击计数器
    bool isAngry; // 生气状态标志
    QList<QString> angryDanmuList; // 生气弹幕列表
    // 番茄钟相关成员变量
    QTimer *pomodoroTimer;
    QLabel *pomodoroLabel;
    int pomodoroDuration; // 单位：秒
    int pomodoroRemaining; // 剩余秒数
    bool isPomodoroRunning;
};
 
#endif
