#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonOpen_clicked();

    void on_horizontalSliderVolume_sliderMoved(int position);

    void on_durationChanged(qint64 duration);

    void on_positionCahnged(qint64 position);

    void on_pushButtonMute_clicked();  

    void on_horizontalSliderProgress_sliderMoved(int position);

    void on_current_index_changed(int position);

    void on_pushButtonClear_clicked();

    void on_pushButtonDelete_clicked();


    void on_radioButtonLoop_clicked();

    void on_radioButtonShuffle_clicked();

    void on_radioButtonNormal_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::Widget *ui;

    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;
    QStandardItemModel* m_playlist_model;

};
#endif // WIDGET_H
