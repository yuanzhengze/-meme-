#include "mainwindow.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>
#include <QScreen> // 新增：包含头文件以获取屏幕信息
#include <QGuiApplication> // 新增：包含头文件
#include <QAction> // 确保包含 QAction 头文件
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), label(new QLabel(this)), movie(nullptr),
      player(new QMediaPlayer(this)), audioOutput(new QAudioOutput(this)),
      autoSwitchTimer(new QTimer(this)),
      isAutoSwitching(true), // 修改：将 isAutoSwitching 默认设置为 true
      danmuList(), danmuTimer(nullptr),
      autoSwitchAction(nullptr),
      clickCount(0), // 新增：点击计数器
      isAngry(false), // 新增：生气状态标志
      angryDanmuList() // 新增：生气弹幕列表
{

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
    if (isAutoSwitching) { // 新增：如果默认开启，则启动定时器
        autoSwitchTimer->start();
    }

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

    MemeData meme11;
    meme11.gifPath = ":/images/meme11.GIF";
    memeDataList.append(meme11);

    MemeData meme12;
    meme12.gifPath = ":/images/meme12.GIF";
    memeDataList.append(meme12);

    MemeData meme13;
    meme13.gifPath = ":/images/meme13.GIF";
    memeDataList.append(meme13);

    MemeData meme14;
    meme14.gifPath = ":/images/meme14.GIF";
    memeDataList.append(meme14);

    MemeData meme15;
    meme15.gifPath = ":/images/meme15.GIF";
    memeDataList.append(meme15);

    MemeData meme16;
    meme16.gifPath = ":/images/meme16.GIF";
    memeDataList.append(meme16);

    MemeData meme17;
    meme17.gifPath = ":/images/meme17.GIF";
    memeDataList.append(meme17);

    MemeData meme18;
    meme18.gifPath = ":/images/meme18.GIF";
    memeDataList.append(meme18);

    MemeData meme19;
    meme19.gifPath = ":/images/meme19.GIF";
    memeDataList.append(meme19);

    MemeData meme21;
    meme21.gifPath = ":/images/meme21.GIF";
    memeDataList.append(meme21);

    MemeData meme22;
    meme22.gifPath = ":/images/meme22.GIF";
    memeDataList.append(meme22);

    MemeData meme23;
    meme23.gifPath = ":/images/meme23.GIF";
    memeDataList.append(meme23);

    MemeData meme24;
    meme24.gifPath = ":/images/meme24.GIF";
    memeDataList.append(meme24);

    MemeData meme25;
    meme25.gifPath = ":/images/meme25.GIF";
    memeDataList.append(meme25);

    MemeData meme26;
    meme26.gifPath = ":/images/meme26.GIF";
    memeDataList.append(meme26);

    MemeData meme27;
    meme27.gifPath = ":/images/meme27.GIF";
    memeDataList.append(meme27);

    MemeData meme28;
    meme28.gifPath = ":/images/meme28.GIF";
    memeDataList.append(meme28);

    MemeData meme29;
    meme29.gifPath = ":/images/meme29.GIF";
    memeDataList.append(meme29);

    MemeData meme30;
    meme30.gifPath = ":/images/meme30.GIF";
    memeDataList.append(meme30);

    MemeData meme31;
    meme31.gifPath = ":/images/meme31.GIF";
    memeDataList.append(meme31);

    MemeData meme32;
    meme32.gifPath = ":/images/meme32.GIF";
    memeDataList.append(meme32);

    MemeData meme33;
    meme33.gifPath = ":/images/meme33.GIF";
    memeDataList.append(meme33);

    MemeData meme34;
    meme34.gifPath = ":/images/meme34.GIF";
    memeDataList.append(meme34);

    MemeData meme35;
    meme35.gifPath = ":/images/meme35.GIF";
    memeDataList.append(meme35);

    MemeData meme4;
    meme4.gifPath = ":/images/meme4.gif";
    memeDataList.append(meme4);

    MemeData meme5;
    meme5.gifPath = ":/images/meme5.gif";
    memeDataList.append(meme5);

    MemeData meme7;
    meme7.gifPath = ":/images/meme7.gif";
    memeDataList.append(meme7);

    MemeData meme8;
    meme8.gifPath = ":/images/meme8.gif";
    meme8.soundPaths << ":/sounds/meme8.mp4";
    memeDataList.append(meme8);

    MemeData meme9;
    meme9.gifPath = ":/images/meme9.gif";
    meme9.soundPaths << ":/sounds/meme9.mp3";
    memeDataList.append(meme9);

    // 加载第一个表情包 -> 修改为加载随机表情包
    if (!memeDataList.isEmpty()) { // 确保列表不为空
        int randomIndex = QRandomGenerator::global()->bounded(memeDataList.size());
        loadMeme(randomIndex); // 使用随机索引加载表情包
    } else {
        qWarning() << "Meme data list is empty, cannot load initial meme.";
    }

    // 设置随机初始位置
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        QRect screenGeometry = primaryScreen->availableGeometry();
        int maxPosX = screenGeometry.width() - width() - 50;
        int maxPosY = screenGeometry.height() - height() - 100;
        if (maxPosX > 0 && maxPosY > 0) {
            int randomX = QRandomGenerator::global()->bounded(50, maxPosX);
            int randomY = QRandomGenerator::global()->bounded(50, maxPosY);
            move(randomX, randomY);
        } else {
            move(50, 50);
        }
    } else {
        move(100, 100);
    }
    danmuList << "又摸鱼了吧？" << "老板来了快关！" << "记得喝水~" << "加油打工人！" << "摸鱼要低调哦~" << "今天的你也很棒！" << "工作再忙也要休息哦~" << "来点音乐放松一下吧~" << "别忘了微笑~" << "努力工作，快乐生活！" << "小心老板在你身后~" << "喝杯咖啡提提神！" << "工作累了就伸个懒腰~" << "保持好心情，事半功倍！" << "今天的目标是：不加班！" << "记得吃午饭哦~" << "工作再忙也要保持健康！" << "来个深呼吸，放松一下~" << "加油，胜利就在眼前！" << "保持专注，效率更高！" << "今天的你真帅！" << "工作再忙也要保持微笑！" << "来点音乐放松一下吧~" << "别忘了微笑~" << "努力工作，快乐生活！" << "小心老板在你身后~" << "喝杯咖啡提提神！" << "工作累了就伸个懒腰~" << "保持好心情，事半功倍！" << "今天的目标是：不加班！";
    danmuTimer = new QTimer(this);
    danmuTimer->setInterval(15000);
    connect(danmuTimer, &QTimer::timeout, this, &MainWindow::showRandomDanmu);
    danmuTimer->start();

    floatAnimation = new QPropertyAnimation(this, "pos", this);
    floatTimer = new QTimer(this);
    floatTimer->setInterval(8000);
    connect(floatTimer, &QTimer::timeout, this, &MainWindow::startFloatAnimation);
    floatTimer->start();

    // 初始化生气弹幕列表
    angryDanmuList << "我生气了喵！" << "不要一直点我！" << "喵喵喵！" << "再点我就咬你！" 
                   << "哼！不理你了！" << "喵呜！好烦！" << "生气气！" << "喵喵喵喵喵！";

    // 在构造函数中添加信号槽连接
    QPushButton *startBtn = findChild<QPushButton*>("startButton");
    if (startBtn) {
        connect(startBtn, &QPushButton::clicked, this, &MainWindow::startPomodoro);
    }
    QPushButton *pauseBtn = findChild<QPushButton*>("pauseButton");
    if (pauseBtn) {
        connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::pausePomodoro);
    }
    QPushButton *resetBtn = findChild<QPushButton*>("resetButton");
    if (resetBtn) {
        connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetPomodoro);
    }
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

    // 重置点击计数器和生气状态
    clickCount = 0;
    isAngry = false;
    
    // 重置窗口和标签大小
    label->setScaledContents(true);
    label->setFixedSize(200, 200);
    label->adjustSize();
    resize(movie->frameRect().size());
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPos = event->globalPosition().toPoint();
        
        // 处理点击计数和生气效果
        clickCount++;
        if (clickCount >= 10 && !isAngry) {
            isAngry = true;
            loadMeme(16); // 切换到 meme17（生气表情）
            
            // 发送生气弹幕
            QString angryText = angryDanmuList[QRandomGenerator::global()->bounded(angryDanmuList.size())];
            QLabel *angryDanmu = new QLabel(angryText, this);
            angryDanmu->setStyleSheet(
                "QLabel {"
                " color: #ff0000;"
                " font: bold 20px '微软雅黑';"
                " background: rgba(255,255,255,180);"
                " border-radius: 8px;"
                " padding: 4px 12px;"
                "}"
            );
            angryDanmu->adjustSize();
            
            int y = QRandomGenerator::global()->bounded(height() - angryDanmu->height());
            int startX = width();
            int endX = -angryDanmu->width();
            
            angryDanmu->move(startX, y);
            angryDanmu->show();
            
            QPropertyAnimation *animation = new QPropertyAnimation(angryDanmu, "pos");
            animation->setDuration(3000);
            animation->setStartValue(QPoint(startX, y));
            animation->setEndValue(QPoint(endX, y));
            animation->setEasingCurve(QEasingCurve::Linear);
            
            connect(animation, &QPropertyAnimation::finished, angryDanmu, &QLabel::deleteLater);
            connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);
            
            animation->start();
        }
        
        // 如果已经生气，继续点击会使表情变大
        if (isAngry) {
            QSize currentSize = size();
            QSize newSize = currentSize * 1.2; // 每次点击增大20%

            // 如果新尺寸超过屏幕尺寸，则设置为屏幕尺寸
            QScreen *screen = QGuiApplication::primaryScreen();
            if (screen) {
                QRect screenGeometry = screen->availableGeometry();
                if (newSize.width() > screenGeometry.width() || newSize.height() > screenGeometry.height()) {
                    newSize = screenGeometry.size();
                }
            }

            // 使用动画平滑放大窗口和label
            QPropertyAnimation *winAnim = new QPropertyAnimation(this, "size");
            winAnim->setDuration(400); // 400ms动画
            winAnim->setStartValue(currentSize);
            winAnim->setEndValue(newSize);
            winAnim->setEasingCurve(QEasingCurve::OutCubic);
            winAnim->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation *labelAnim = new QPropertyAnimation(label, "size");
            labelAnim->setDuration(400);
            labelAnim->setStartValue(label->size());
            labelAnim->setEndValue(newSize);
            labelAnim->setEasingCurve(QEasingCurve::OutCubic);
            labelAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }
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
    // 确保 autoSwitchAction 在这里被创建或更新
    autoSwitchAction->setChecked(isAutoSwitching);
    contextMenu.addAction(autoSwitchAction);
    contextMenu.addAction("切换表情", this, SLOT(switchRandomMeme()));
    contextMenu.addAction("召唤新宠物", this, SLOT(summonNewPet()));
    contextMenu.addAction("召唤猫咪军团", this, SLOT(summonArmy())); // 新增：召唤军团菜单项
    contextMenu.addAction("解散猫咪军团", this, SLOT(dismissArmy())); // 新增：解散军团菜单项
    contextMenu.addAction("猫猫快跑", this, SLOT(startCatRunGame())); // 新增：猫猫快跑游戏
    contextMenu.addSeparator();
    contextMenu.addAction("退出桌宠", this, SLOT(close()));
    contextMenu.exec(label->mapToGlobal(pos));
}

void MainWindow::toggleAutoSwitch() {
    isAutoSwitching = !isAutoSwitching;
    if (isAutoSwitching) {
        autoSwitchTimer->start();
    } else {
        autoSwitchTimer->stop();
    }
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

// 新增：处理召唤新宠物的槽函数
void MainWindow::summonNewPet() {
    emit summonNewPetRequested(); // 发射信号，通知主程序创建新宠物
}

// 新增：处理召唤军团的槽函数
void MainWindow::summonArmy() {
    emit summonArmyRequested(); // 发射召唤军团信号
}

// 新增：处理解散军团的槽函数
void MainWindow::startCatRunGame()
{
    CatRunGameWidget *gameWidget = new CatRunGameWidget();
    gameWidget->setAttribute(Qt::WA_DeleteOnClose); // Ensure it's deleted when closed
    gameWidget->show();
}

void MainWindow::dismissArmy() {
    emit dismissArmyRequested(this); // 发射解散军团信号，并传递自身指针
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

void MainWindow::startPomodoro() {
    if (!isPomodoroRunning && pomodoroRemaining > 0) {
        pomodoroTimer->start();
        isPomodoroRunning = true;
    }
}


void MainWindow::pausePomodoro() {
    if (isPomodoroRunning) {
        pomodoroTimer->stop();
        isPomodoroRunning = false;
    }
}

void MainWindow::resetPomodoro() {
    pomodoroTimer->stop();
    pomodoroRemaining = pomodoroDuration;
}
