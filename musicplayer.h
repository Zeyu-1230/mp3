#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class musicplayer;
}
QT_END_NAMESPACE
class QAudioOutput;
class QMediaPlayer;
class musicplayer : public QWidget
{
    Q_OBJECT

public:
    musicplayer(QWidget *parent = nullptr);
    ~musicplayer();
    void paintEvent(QPaintEvent *e);

private slots:
    void on_file_clicked();

    void on_presong_clicked();

    void on_start_clicked();

    void on_nextsong_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::musicplayer *ui;
    QList<QUrl>playList;
    int curPlayIndex = 0;
    QAudioOutput* audioOutput;
    QMediaPlayer* mediaPlayer;
};
#endif // MUSICPLAYER_H
