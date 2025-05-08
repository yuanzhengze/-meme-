#ifndef CATRUNGAMEWIDGET_H
#define CATRUNGAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QPushButton>
#include <QList>
#include <QRect>
#include <QKeyEvent> // Required for keyPressEvent

class CatRunGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CatRunGameWidget(QWidget *parent = nullptr);
    ~CatRunGameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override; // 添加 keyReleaseEvent 声明
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
    QLabel *backgroundLabel; // 新增：用于显示背景图片
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

    QStringList catGifPaths; // To store paths to cat GIFs
    QStringList dogImagePaths; // Paths to dog images
    QStringList backgroundImagePaths; // 新增：存储背景图片路径

    bool gameIsOver;
    int currentCatX; // 猫猫当前X坐标
    bool moveLeftPressed;  // 新增：跟踪向左移动键是否按下
    bool moveRightPressed; // 新增：跟踪向右移动键是否按下
};

#endif // CATRUNGAMEWIDGET_H