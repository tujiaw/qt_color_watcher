#include "dialog.h"
#include "ui_dialog.h"
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &Dialog::slotDataChanged);
    this->setWindowTitle(tr("watcher clipboard color"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotDataChanged()
{
    qDebug() << "xxxxx";
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QString text;
    if (mimeData->hasImage()) {
        qDebug() << "image";
    } else if (mimeData->hasHtml()) {
        text = mimeData->html();
    } else if (mimeData->hasText()) {
        text = mimeData->text();
    } else {
        qDebug() << "Cannot display data";
    }

    if (!text.isEmpty()) {
        QString backgroundColor;
        QString labelColor;
        QColor color;

        text = text.toLower();
        int index1 = text.indexOf("rgb(");
        int index2 = text.indexOf("#");
        int index3 = text.indexOf("qcolor(");
        if (index1 >= 0) {
            int start = index1;
            int end = text.indexOf(")", start + 4);
            if (end > start) {
                backgroundColor = text.mid(start + 4, end - start - 4);
                QStringList strList = backgroundColor.split(",");
                if (strList.size() >= 3) {
                    color = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
                }
            }
        } else if (index2 >= 0) {
            int start = index2;
            if (text.length() - start >= 7) {
                int count = 0;
                for (int i=1; i<=6; i++) {
                    QChar c = text.at(start + i);
                    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')) {
                        ++count;
                    }
                }
                if (count == 6) {
                    color = QColor(text.mid(start, 7));
                }
            }
        } else if (index3 >= 0) {
            int start = index3;
            int end = text.indexOf(")", start + 7);
            if (end > start) {
                qDebug() << "end:" << end << ", start:" << start;
                backgroundColor = text.mid(start + 7, end - start - 7);
                QStringList strList = backgroundColor.split(",");
                if (strList.size() >= 3) {
                    color = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
                }
            }
        }

        if (color.isValid()) {
            //QColor inverseColor(255-color.red(), 255-color.green(), 255-color.blue());
            //this->setStyleSheet(QString("QDialog { background:%1; }").arg(inverseColor.name()));
            this->setStyleSheet(QString("QDialog { background:%1; }").arg(color.name()));
            ui->leColor1->setText(color.name());
            ui->leColor2->setText(QString("RGB(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
        } else {
            ui->leColor1->setText("");
            ui->leColor2->setText("");
        }
    }
}
