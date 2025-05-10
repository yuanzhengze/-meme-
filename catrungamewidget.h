#ifndef CATRUNGAMEWIDGET_H
#define CATRUNGAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QPushButton>
#include <QList>
#include <QRect>
#include <QKeyEvent> 

class CatRunGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CatRunGameWidget(QWidget *parent = nullptr);
    ~CatRunGameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override; 
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updateGame();
    void restartGame();
    void quitGame();

private:
    void initGame();
    void spawnObstacle();
    bool checkCollision();
    void gameOver();

    QLabel *catLabel;
    QMovie *catMovie;
    QLabel *backgroundLabel; 
    QList<QLabel*> obstacles;
    QTimer *gameTimer;
    QTimer *obstacleTimer;
    int score;
    QLabel *scoreLabel;
    QPushButton *restartButton;
    QPushButton *quitButton;

    bool isJumping;
    int jumpHeight;
    int initialCatY;
    int catVelocityY;
    const int gravity = 1;
    const int jumpStrength = -15;
    const int catSpeedX = 10; // 猫猫水平移动速度

    QStringList catGifPaths; 
    QStringList dogImagePaths; 
    QStringList backgroundImagePaths; 

    bool gameIsOver;
    int currentCatX; // 猫猫当前X坐标
    bool moveLeftPressed; 
    bool moveRightPressed;
};

#endif 