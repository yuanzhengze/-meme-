#include "mainwindow.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), label(new QLabel(this)), movie(nullptr), 
      player(new QMediaPlayer(this)), audioOutput(new QAudioOutput(this)),
      autoSwitchTimer(new QTimer(this)), isAutoSwitching(false), danmuList(), danmuTimer(nullptr) {

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    
    // 设置窗口样式
    setStyleSheet("background: transparent;");
    
    // 设置标签样式
    label->setStyleSheet("background: transparent; border: none;");
    label->setAttribute(Qt::WA_TranslucentBackground);

    // 设置右键菜单
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(label, &QWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // 初始化音频播放器
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    // 初始化定时器
    autoSwitchTimer->setInterval(10000); // 10秒切换一次
    connect(autoSwitchTimer, &QTimer::timeout, this, &MainWindow::onAutoSwitchTimeout);

    // 初始化表情包数据
    // 普通表情包，无音频
    MemeData meme1;
    meme1.gifPath = ":/images/meme1.gif";
    memeDataList.append(meme1);

    MemeData meme2;
    meme2.gifPath = ":/images/meme2.gif";
    memeDataList.append(meme2);

    MemeData meme3;
    meme3.gifPath = ":/images/meme3.gif";
    memeDataList.append(meme3);

    // 带音频的表情包
    MemeData meme8;
    meme8.gifPath = ":/images/meme8.gif";
    meme8.soundPaths << ":/sounds/meme8.mp4";
    memeDataList.append(meme8);

    MemeData meme9;
    meme9.gifPath = ":/images/meme9.gif";
    meme9.soundPaths << ":/sounds/meme9.mp3";
    memeDataList.append(meme9);

    // 加载第一个表情包
    loadMeme(0);

    danmuList << "又摸鱼了吧？" << "老板来了快关！" << "记得喝水~" << "加油打工人！" << "摸鱼要低调哦~";

    danmuTimer = new QTimer(this);
    danmuTimer->setInterval(15000); // 15秒弹一次
    connect(danmuTimer, &QTimer::timeout, this, &MainWindow::showRandomDanmu);
    danmuTimer->start();

    floatAnimation = new QPropertyAnimation(this, "pos", this);
    floatTimer = new QTimer(this);
    floatTimer->setInterval(8000); // 每8秒漂浮/弹跳一次
    connect(floatTimer, &QTimer::timeout, this, &MainWindow::startFloatAnimation);
    floatTimer->start();
}

void MainWindow::loadMeme(int index) {
    if (index < 0 || index >= memeDataList.size()) return;

    const MemeData &meme = memeDataList[index];
    
    if (movie) {
        movie->stop();
        delete movie;
        movie = nullptr;
    }

    movie = new QMovie(meme.gifPath);
    if (!movie->isValid()) {
        qDebug() << "Failed to load movie:" << meme.gifPath;
        return;
    }
    
    label->setMovie(movie);
    movie->start();
    if (movie->state() != QMovie::Running) {
        qDebug() << "Failed to start movie";
        return;
    }

    // 如果表情包有音频，播放第一个音频
    if (!meme.soundPaths.isEmpty()) {
        player->setSource(QUrl(meme.soundPaths.first()));
        player->play();
    }

    label->setScaledContents(true);
    label->adjustSize();
    resize(movie->frameRect().size());
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPos = event->globalPosition().toPoint();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint currentPos = event->globalPosition().toPoint();
        move(pos() + currentPos - lastPos);
        lastPos = currentPos;
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(this);

    // 设置菜单样式
    contextMenu.setStyleSheet(
        "QMenu {"
        "  background-color: #ffffff;"
        "  color: #222222;"
        "  border: 1px solid #cccccc;"
        "  border-radius: 8px;"
        "  padding: 6px;"
        "  font-size: 15px;"
        "  min-width: 120px;"
        "}"
        "QMenu::item {"
        "  padding: 6px 24px 6px 24px;"
        "  background: transparent;"
        "}"
        "QMenu::item:selected {"
        "  background: #e6f0fa;"
        "  color: #0078d7;"
        "  border-radius: 6px;"
        "}"
        "QMenu::separator {"
        "  height: 1px;"
        "  background: #e0e0e0;"
        "  margin: 4px 0 4px 0;"
        "}"
    );

    autoSwitchAction = contextMenu.addAction("开启自动切换", this, SLOT(toggleAutoSwitch()));
    autoSwitchAction->setCheckable(true);
    autoSwitchAction->setChecked(isAutoSwitching);
    contextMenu.addAction("切换表情", this, SLOT(switchRandomMeme()));
    contextMenu.addAction("退出桌宠", this, SLOT(close()));
    contextMenu.exec(label->mapToGlobal(pos));
}

void MainWindow::toggleAutoSwitch() {
    isAutoSwitching = !isAutoSwitching;
    if (isAutoSwitching) {
        autoSwitchTimer->start();
        autoSwitchAction->setText("关闭自动切换");
    } else {
        autoSwitchTimer->stop();
        autoSwitchAction->setText("开启自动切换");
    }
    autoSwitchAction->setChecked(isAutoSwitching);
}

void MainWindow::onAutoSwitchTimeout() {
    switchRandomMeme();
}

void MainWindow::switchRandomMeme() {
    int newIndex = QRandomGenerator::global()->bounded(memeDataList.size());
    loadMeme(newIndex);
}

void MainWindow::showRandomDanmu() {
    // 随机选一句弹幕
    QString text = danmuList[QRandomGenerator::global()->bounded(danmuList.size())];

    // 创建弹幕标签
    QLabel *danmuLabel = new QLabel(text, this);
    danmuLabel->setStyleSheet(
        "QLabel {"
        " color: #ff4081;"
        " font: bold 16px '微软雅黑';"
        " background: rgba(255,255,255,180);"
        " border-radius: 8px;"
        " padding: 2px 10px;"
        "}"
    );
    danmuLabel->adjustSize();

    // 弹幕起始和结束位置
    int y = 10; // 距离顶部10像素
    int startX = width();
    int endX = -danmuLabel->width();

    danmuLabel->move(startX, y);
    danmuLabel->show();

    // 动画
    QPropertyAnimation *animation = new QPropertyAnimation(danmuLabel, "pos");
    animation->setDuration(4000); // 4秒飞过
    animation->setStartValue(QPoint(startX, y));
    animation->setEndValue(QPoint(endX, y));
    animation->setEasingCurve(QEasingCurve::Linear);

    // 动画结束后删除标签和动画
    connect(animation, &QPropertyAnimation::finished, danmuLabel, &QLabel::deleteLater);
    connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);

    animation->start();
}

void MainWindow::startFloatAnimation() {
    // 随机选择是"弹跳"还是"漂浮"
    bool jump = QRandomGenerator::global()->bounded(2);

    QPoint startPos = this->pos();
    QPoint midPos = startPos + QPoint(0, jump ? -40 : -10); // 弹跳高一点，漂浮低一点
    QPoint endPos = startPos;

    floatAnimation->stop();
    floatAnimation->setDuration(jump ? 600 : 2000); // 弹跳快，漂浮慢
    floatAnimation->setStartValue(startPos);
    floatAnimation->setKeyValueAt(0.5, midPos);
    floatAnimation->setEndValue(endPos);
    floatAnimation->setEasingCurve(jump ? QEasingCurve::OutBounce : QEasingCurve::InOutSine);
    floatAnimation->start();
}

MainWindow::~MainWindow() {
    if (movie) {
        movie->stop();
        delete movie;
    }
    delete player;
    delete audioOutput;
    delete autoSwitchTimer;
    delete danmuTimer;
    delete floatAnimation;
    delete floatTimer;
}
