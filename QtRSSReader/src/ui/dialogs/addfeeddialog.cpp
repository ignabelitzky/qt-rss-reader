#include "addfeeddialog.h"
#include "ui_addfeeddialog.h"

AddFeedDialog::AddFeedDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFeedDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add Feed");

    ui->enabledCheckBox->setChecked(true);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

Feed AddFeedDialog::getFeed() const
{
    Feed feed;
    feed.url = ui->urlLineEdit->text();
    feed.name = ui->nameLineEdit->text();
    feed.enabled = ui->enabledCheckBox->isChecked();
    return feed;
}
