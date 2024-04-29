#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_player = new QMediaPlayer();
    m_player->setVolume(70);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number((m_player->volume()))));
    ui->horizontalSliderVolume->setValue(m_player->volume());
}

Widget::~Widget()
{
    delete m_player;
    delete ui;
}



void Widget::on_pushButtonOpen_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"Open file",NULL,"Audio files(*.mp3 *.flac)");
    ui->labelFile->setText(file);
    m_player->setMedia(QUrl::fromLocalFile(file));
    int duration =
    m_player->play();
}


void Widget::on_horizontalSliderVolume_sliderMoved(int position)
{
    m_player->setVolume(position);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number((m_player->volume()))));
}




void Widget::on_pushButtonStop_clicked()
{
    if(!m_player->StoppedState){
        m_player->stop();
    }
}


void Widget::on_pushButtonPause_clicked()
{
    if(m_player->PlayingState){
        m_player->pause();
    }
}


void Widget::on_pushButtonPlay_clicked()
{
    if(m_player->PausedState){
        m_player->play();
    }
}




