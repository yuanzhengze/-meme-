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

struct MemeData {
    QString gifPath;
    QStringList soundPaths;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    void loadMeme(int index);
    QLabel *label;
    QMovie *movie;
    QPoint lastPos;
    QList<MemeData> memeDataList;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QTimer *autoSwitchTimer;
    bool isAutoSwitching;
    QAction *autoSwitchAction;
    QStringList danmuList;  
    QTimer *danmuTimer; 
    QPropertyAnimation *floatAnimation;
    QTimer *floatTimer;
};

#endif // MAINWINDOW_H
