#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPainter>

musicplayer::musicplayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::musicplayer)
{
    ui->setupUi(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(mediaPlayer, &QMediaPlayer::durationChanged,this, [=](qint64 duration)
{
        ui->totalLabel->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60));
        ui->playCourseSlider->setRange(0,duration);
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this,[=](qint64 pos)
{
        ui->curLabel->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60));
        ui->playCourseSlider->setValue(pos);
        connect(ui->playCourseSlider, &QSlider::sliderMoved,mediaPlayer, &QMediaPlayer::setPosition);
    });

    connect(ui->playCourseSlider,&QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);
}

musicplayer::~musicplayer()
{
    delete ui;
}

void musicplayer::on_file_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this,"OPEN","C:/Users/lenovo/Desktop/Report 8/Report8/music/VipSongsDownload");
    QDir dir(path);
    auto musicList = dir.entryList(QStringList()<<"*.mgg");
    ui->listWidget->addItems(musicList);
    ui->listWidget->setCurrentRow(0);
    for(auto file : musicList)
        playList.append(QUrl::fromLocalFile(path+"/"+file));
    qInfo()<<playList;
}


void musicplayer::on_presong_clicked()
{
    curPlayIndex = (curPlayIndex-1) % playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}


void musicplayer::on_start_clicked()
{
    if(playList.empty())
    {
        return;
    }
    switch (mediaPlayer->playbackState())
    {
    case QMediaPlayer::PlaybackState::StoppedState:
    {
        curPlayIndex = ui->listWidget->currentRow();
        mediaPlayer->setSource(playList[curPlayIndex]);
        mediaPlayer->play();
        break;
    }
    case QMediaPlayer::PlaybackState::PlayingState:
        mediaPlayer->pause();
        break;
    case QMediaPlayer::PlaybackState::PausedState:
        mediaPlayer->play();
        break;
    }
}


void musicplayer::on_nextsong_clicked()
{
    curPlayIndex = (curPlayIndex+1) % playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}


void musicplayer::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}

void musicplayer::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
