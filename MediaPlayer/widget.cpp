#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStyle>
#include <QTime>
#include <QInputDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Buttons style
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButtonOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));


    m_player = new QMediaPlayer();
    m_player->setVolume(70);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number((m_player->volume()))));
    ui->horizontalSliderVolume->setValue(m_player->volume());
    connect(ui->pushButtonPlay, &QPushButton::clicked, this->m_player, &QMediaPlayer::play);
    connect(ui->pushButtonStop, &QPushButton::clicked, this->m_player, &QMediaPlayer::stop);
    connect(ui->pushButtonPause, &QPushButton::clicked, this->m_player, &QMediaPlayer::pause);


    connect(this->m_player,&QMediaPlayer::durationChanged, this,&Widget::on_durationChanged);
    connect(this->m_player,&QMediaPlayer::positionChanged,this,&Widget::on_positionCahnged);

    //              INIT PLAYLSIT
    m_playlist_model = new QStandardItemModel(this);
    ui->tableViewPlaylist->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList()<<"Track"<<"File");
    ui->tableViewPlaylist->hideColumn(1);
    ui->tableViewPlaylist->horizontalHeader()->setStretchLastSection(1);
    ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_playlist = new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(ui->pushButtonNext, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(ui->pushButtonPrev, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Widget::on_current_index_changed);
}

Widget::~Widget()
{
    delete m_player;
    delete ui;
    delete m_playlist_model;
    delete m_playlist;
}



void Widget::on_pushButtonOpen_clicked()
{
//    QString file = QFileDialog::getOpenFileName(this,"Open file",NULL,"Audio files(*.mp3 *.flac)");
//    ui->labelFile->setText(file);
//    m_player->setMedia(QUrl::fromLocalFile(file));
//    m_player->play();
//    m_player->media();
//    this->setWindowTitle(QString("Media Player ").append(file.split('/').last()));
    QStringList files = QFileDialog::getOpenFileNames
        (
            this,
            "Open files",
            "C:\\Users\\hoxy_\\Music",
            "Audio Files (*mp3 *flac);;mp3 (*mp3;;FLAC (*flac)"
            );

    for(QString file:files){
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QDir(file).dirName()));
        items.append(new QStandardItem(file));
        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl(file));
    }
}

void Widget::on_horizontalSliderVolume_sliderMoved(int position)
{
    m_player->setVolume(position);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
}

void Widget::on_durationChanged(qint64 duration)
{
    ui->horizontalSliderProgress->setMaximum(duration);

    QTime qt_duration = QTime::fromMSecsSinceStartOfDay(duration);

    ui->labelDuration->setText(QString("Duration: ")
                               .append(qt_duration.toString(duration< 3600000 ? "mm:ss" : "hh:mm:ss")));
}

void Widget::on_positionCahnged(qint64 position)
{
    ui->horizontalSliderProgress->setValue(position);
    QTime qt_duration = QTime::fromMSecsSinceStartOfDay(position);

    ui->labelProgress->setText(QString("Duration: ")
                               .append(qt_duration.toString(position< 3600000 ? "mm:ss" : "hh:mm:ss")));
}

void Widget::on_pushButtonMute_clicked()
{
    m_player->setMuted(!m_player->isMuted());
    ui->pushButtonMute->setIcon(style()->standardIcon(m_player->isMuted()?QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}

void Widget::on_horizontalSliderProgress_sliderMoved(int position)
{
    m_player->setPosition(position);
}

void Widget::on_current_index_changed(int position)
{
    ui->tableViewPlaylist->selectRow(position);
    QStandardItem* song = m_playlist_model->item(position,0);
    this->setWindowTitle(QString("Media Player: ").append(song->text()));
    QStandardItem* file = m_playlist_model->item(position,1);
    ui->labelFile->setText(file->text());
}


void Widget::on_pushButtonClear_clicked()
{
   m_playlist_model->removeRows(0,m_playlist_model->rowCount());
   m_playlist->clear();
}


void Widget::on_pushButtonDelete_clicked()
{
    QItemSelectionModel* selectedItems = ui->tableViewPlaylist->selectionModel();
    if(selectedItems->hasSelection()){
        for(QModelIndex item : selectedItems->selectedIndexes()){
             m_playlist_model->removeRows(item.row(),1);
             m_playlist->removeMedia(item.row());
        }
    }

}

void Widget::on_radioButtonLoop_clicked()
{
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}


void Widget::on_radioButtonShuffle_clicked()
{
    m_playlist->setPlaybackMode(QMediaPlaylist::Random);
}


void Widget::on_radioButtonNormal_clicked()
{
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
}


void Widget::on_pushButtonSave_clicked()
{
    bool isNameGiven;
    QString fileName = QInputDialog::getText(nullptr, "Add name", "Name:", QLineEdit::Normal, "",&isNameGiven);
    if(isNameGiven && !fileName.isEmpty()){
        QString directory = QFileDialog::getExistingDirectory(this,"Select directory","C://");
        QString fullPath = directory + "/" + fileName+".pls";
        if(m_playlist->save(QUrl::fromLocalFile(fullPath),"pls")){
            QMessageBox::information(this,"Succes","Saved succesfully");
        }
        else{
            QMessageBox::warning(this,"Error","An error occured");
        }
    }

}

