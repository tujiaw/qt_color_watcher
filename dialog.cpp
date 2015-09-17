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

        text = text.toLower();
        int start = text.indexOf("rgb(");
        int end = text.lastIndexOf(")");
        if (end - start > 5) {
            backgroundColor = text.mid(start, end - start + 1);
            // 计算反色
            QString labelColor = text.mid(start + 4, end - start - 4);
            qDebug() << labelColor;
            QStringList strList = labelColor.split(",");
            if (strList.size() >= 3) {
                int red = 255 - strList[0].toInt();
                int green = 255 - strList[1].toInt();
                int blue = 255 - strList[2].toInt();
                labelColor = QString("rgb(%1, %2, %3)").arg(red).arg(green).arg(blue);
            }
        } else {
            start = text.indexOf("#");
            if (text.length() - start >= 7) {
                int count = 0;
                for (int i=1; i<=6; i++) {
                    QChar c = text.at(start + i);
                    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')) {
                        ++count;
                    }
                }
                if (count == 6) {
                    backgroundColor = text.mid(start, 7);
                    QColor clr(backgroundColor);
                    labelColor = QString("rgb(%1, %2, %3)").arg(255-clr.red()).arg(255-clr.green()).arg(255-clr.blue());
                }
            }
        }

        this->setStyleSheet(QString("QDialog { background:%1; }").arg(backgroundColor));
        ui->labelColor->setText(backgroundColor);
        ui->labelColor->setStyleSheet(QString("QLabel { background:transparent; color:%1}").arg(labelColor));
    }
}
