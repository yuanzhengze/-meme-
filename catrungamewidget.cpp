#include "catrungamewidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QPixmap> // 新增：用于加载图片

CatRunGameWidget::CatRunGameWidget(QWidget *parent)
    : QWidget(parent),
      catLabel(new QLabel(this)),
      catMovie(nullptr),
      backgroundLabel(new QLabel(this)), // 新增：初始化背景标签
      gameTimer(new QTimer(this)),
      obstacleTimer(new QTimer(this)),
      score(0),
      scoreLabel(new QLabel(this)),
      restartButton(new QPushButton("重新开始", this)),
      quitButton(new QPushButton("退出游戏", this)),
      isJumping(false),
      jumpHeight(0),
      initialCatY(200),
      catVelocityY(0),
      gameIsOver(false),
      currentCatX(50), // 初始化猫猫X坐标
      moveLeftPressed(false),
      moveRightPressed(false)
{
    // 设置窗口基本属性
    setFixedSize(800, 400); // 游戏窗口大小
    setWindowTitle("猫猫快跑");
    setStyleSheet("background-color: #f0f0f0;"); // 淡灰色背景

    // 初始化猫猫GIF路径列表 (假设图片在images目录下)
    // TODO: 用户需要确保这些GIF文件在 :images/ 资源路径下
    catGifPaths << ":/images/meme1.gif" << ":/images/meme2.gif" << ":/images/meme3.gif" << ":/images/meme4.gif" << ":/images/meme5.gif" << ":/images/meme11.gif" << ":/images/meme12.GIF" << ":/images/meme13.GIF"; // 可添加更多
    dogImagePaths << ":/images/dog1.png" << ":/images/dog3.png" << ":/images/dog4.png"<< ":/images/dog2.png";

    // 初始化背景图片路径列表
    for (int i = 1; i <= 13; ++i) {
        backgroundImagePaths << QString(":/images/buildingimages/building%1.jpg").arg(i);
    }

    initGame();

    connect(gameTimer, &QTimer::timeout, this, &CatRunGameWidget::updateGame);
    connect(obstacleTimer, &QTimer::timeout, this, &CatRunGameWidget::spawnObstacle);
    connect(restartButton, &QPushButton::clicked, this, &CatRunGameWidget::restartGame);
    connect(quitButton, &QPushButton::clicked, this, &CatRunGameWidget::quitGame);
}

CatRunGameWidget::~CatRunGameWidget()
{
    // 清理动态分配的内存
    delete catMovie; // catLabel 会被 Qt 自动管理
    // backgroundLabel 也会被 Qt 自动管理，因为它是 this 的子对象
    qDeleteAll(obstacles);
    obstacles.clear();
}

void CatRunGameWidget::initGame()
{
    gameIsOver = false;
    score = 0;
    isJumping = false;
    catVelocityY = 0;
    currentCatX = 50; // 重置猫猫X坐标
    moveLeftPressed = false;
    moveRightPressed = false;

    // 清理旧的障碍物
    qDeleteAll(obstacles);
    obstacles.clear();

    // 设置背景图片
    if (!backgroundImagePaths.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(backgroundImagePaths.size());
        QPixmap bgPixmap(backgroundImagePaths[randomIndex]);
        if (!bgPixmap.isNull()) {
            backgroundLabel->setPixmap(bgPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backgroundLabel->setGeometry(0, 0, this->width(), this->height());
            backgroundLabel->lower(); // 确保背景在最底层
            backgroundLabel->show();
        } else {
            qWarning() << "Failed to load background image:" << backgroundImagePaths[randomIndex];
            backgroundLabel->setText("Error: BG not found"); // 显示错误信息
            backgroundLabel->setStyleSheet("background-color: #f0f0f0;"); // 默认背景色
            backgroundLabel->setGeometry(0, 0, this->width(), this->height());
            backgroundLabel->lower();
            backgroundLabel->show();
        }
    } else {
        backgroundLabel->setStyleSheet("background-color: #f0f0f0;"); // 默认背景色，如果没有图片路径
        backgroundLabel->setGeometry(0, 0, this->width(), this->height());
        backgroundLabel->lower();
        backgroundLabel->show();
    }

    // 设置猫猫
    if (!catGifPaths.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(catGifPaths.size());
        if (catMovie) delete catMovie;
        catMovie = new QMovie(catGifPaths[randomIndex]);
        if (!catMovie->isValid()) {
            qWarning() << "Failed to load cat GIF:" << catGifPaths[randomIndex];
            // 可以加载一个默认的占位图或者显示错误信息
            catLabel->setText("Error: Cat GIF not found");
        } else {
            catLabel->setMovie(catMovie);
            catMovie->start();
        }
    }
    catLabel->setFixedSize(80, 80); // 猫猫大小
    catLabel->setScaledContents(true);
    catLabel->setAttribute(Qt::WA_TranslucentBackground);
    catLabel->setStyleSheet("background-color: transparent;");
    initialCatY = height() - catLabel->height() - 20; // 猫猫初始Y坐标，地面以上20像素
    catLabel->move(currentCatX, initialCatY);
    catLabel->show();

    // 设置分数标签
    scoreLabel->setText(QString("分数: %1").arg(score));
    scoreLabel->setStyleSheet("font-size: 16px; color: #333;");
    scoreLabel->adjustSize();
    scoreLabel->move(width() - scoreLabel->width() - 20, 20);
    scoreLabel->show();

    // 设置按钮
    restartButton->setFixedSize(100, 30);
    quitButton->setFixedSize(100, 30);
    restartButton->move(width() / 2 - restartButton->width() - 10, height() / 2 - restartButton->height() / 2);
    quitButton->move(width() / 2 + 10, height() / 2 - quitButton->height() / 2);
    restartButton->hide();
    quitButton->hide();

    // 启动定时器
    gameTimer->start(30); // 约33 FPS
    obstacleTimer->start(2000); // 每2秒产生一个障碍物

    setFocus(); // 允许窗口接收键盘事件
}

void CatRunGameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // 背景图片由 backgroundLabel 处理，这里不再绘制纯色背景
    // 如果需要自定义绘制地面等，可以在这里进行，但要确保它在背景图片之上
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#cccccc")); // 地面颜色
    painter.drawRect(0, height() - 20, width(), 20); // 绘制地面
}

void CatRunGameWidget::keyPressEvent(QKeyEvent *event)
{
    if (gameIsOver) return;

    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
        if (!isJumping) {
            isJumping = true;
            catVelocityY = jumpStrength;
        }
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        moveLeftPressed = true;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        moveRightPressed = true;
    }
}

void CatRunGameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (gameIsOver) return;

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        moveLeftPressed = false;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        moveRightPressed = false;
    }
}

void CatRunGameWidget::mousePressEvent(QMouseEvent *event)
{
    if (!gameIsOver && event->button() == Qt::LeftButton) {
        if (!isJumping) {
            isJumping = true;
            catVelocityY = jumpStrength;
        }
    }
    QWidget::mousePressEvent(event);
}

void CatRunGameWidget::updateGame()
{
    if (gameIsOver) return;

    // 水平移动逻辑
    if (moveLeftPressed) {
        currentCatX -= catSpeedX;
        if (currentCatX < 0) {
            currentCatX = 0;
        }
    }
    if (moveRightPressed) {
        currentCatX += catSpeedX;
        if (currentCatX + catLabel->width() > width()) {
            currentCatX = width() - catLabel->width();
        }
    }

    // 猫猫跳跃逻辑
    if (isJumping) {
        int currentCatY = catLabel->y();
        currentCatY += catVelocityY;
        catVelocityY += gravity;

        if (currentCatY >= initialCatY) {
            currentCatY = initialCatY;
            isJumping = false;
            catVelocityY = 0;
        }
        catLabel->move(currentCatX, currentCatY); // 更新X坐标以实现跳跃惯性
    } else {
        // 如果不在跳跃，也需要根据按键更新X坐标
        catLabel->move(currentCatX, catLabel->y());
    }

    // 移动障碍物并检测碰撞
    QMutableListIterator<QLabel*> it(obstacles);
    while (it.hasNext()) {
        QLabel *obstacle = it.next();
        obstacle->move(obstacle->x() - 5, obstacle->y()); // 障碍物移动速度

        if (obstacle->x() + obstacle->width() < 0) { // 障碍物移出屏幕
            it.remove();
            delete obstacle;
            score++; // 躲过一个障碍物得分
            scoreLabel->setText(QString("分数: %1").arg(score));
            scoreLabel->adjustSize(); // 确保文本能完整显示
            scoreLabel->move(width() - scoreLabel->width() - 20, 20); // 重新定位以防大小变化
        } else if (catLabel->geometry().intersects(obstacle->geometry())) {
            gameOver();
            return;
        }
    }
}

void CatRunGameWidget::spawnObstacle()
{
    if (gameIsOver) return;

    QLabel *obstacle = new QLabel(this);
    if (dogImagePaths.isEmpty()) {
        qWarning() << "No dog images available!";
        obstacle->setText("E"); // Error placeholder
        obstacle->setFixedSize(50, 50);
    } else {
        int randomIndex = QRandomGenerator::global()->bounded(dogImagePaths.size());
        QString selectedDogImagePath = dogImagePaths[randomIndex];
        QPixmap dogPixmap(selectedDogImagePath);
        if (dogPixmap.isNull()) {
            qWarning() << "Failed to load dog image:" << selectedDogImagePath;
            obstacle->setText("X"); // Placeholder for failed load
            obstacle->setFixedSize(50, 50);
        } else {
            obstacle->setPixmap(dogPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            obstacle->setFixedSize(50, 50);
        }
    }
    obstacle->setAttribute(Qt::WA_TranslucentBackground);
    obstacle->setStyleSheet("background-color: transparent;");
    
    // 障碍物随机高度，但确保在地面上
    int obstacleY = height() - obstacle->height() - 20; 
    obstacle->move(width(), obstacleY);
    obstacle->show();
    obstacles.append(obstacle);
}

void CatRunGameWidget::gameOver()
{
    gameIsOver = true;
    gameTimer->stop();
    obstacleTimer->stop();

    if (catMovie) catMovie->stop();

    // 显示最终得分和按钮
    scoreLabel->setText(QString("最终分数: %1").arg(score));
    scoreLabel->adjustSize();
    scoreLabel->move(width() / 2 - scoreLabel->width() / 2, height() / 2 - 50);
    
    restartButton->show();
    quitButton->show();
}

void CatRunGameWidget::restartGame()
{
    // 清理旧的障碍物
    qDeleteAll(obstacles);
    obstacles.clear();

    initGame();
}

void CatRunGameWidget::quitGame()
{
    close(); // 关闭游戏窗口
}