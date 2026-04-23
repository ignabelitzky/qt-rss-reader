#pragma once

#include <QMainWindow>
#include "../core/rssfetcher.h"
#include "../models/rssmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    RssFetcher* m_fetcher;
    RssModel* m_model;
};
