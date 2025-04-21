#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), label(new QLabel(this)) {

    // 设置窗口透明 + 无边框 + 置顶
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    // 加载 gif
    movie = new QMovie(":/images/meme.gif");  // 资源路径
    label->setMovie(movie);
    movie->start();

    // 调整窗口大小为 gif 大小
    label->setScaledContents(true);
    label->adjustSize();
    resize(movie->frameRect().size());
}

MainWindow::~MainWindow() {
    delete movie;
}

// 鼠标拖动逻辑
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPos = event->globalPosition().toPoint();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        int dx = event->globalPosition().x() - lastPos.x();
        int dy = event->globalPosition().y() - lastPos.y();
        move(x() + dx, y() + dy);
        lastPos = event->globalPosition().toPoint();
    }
}
