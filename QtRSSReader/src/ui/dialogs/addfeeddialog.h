#pragma once

#include <QDialog>
#include "src/models/feed.h"

namespace Ui {
class AddFeedDialog;
}

class AddFeedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFeedDialog(QWidget *parent = nullptr);
    ~AddFeedDialog() = default;

    Feed getFeed() const;

private:
    Ui::AddFeedDialog *ui;
};
