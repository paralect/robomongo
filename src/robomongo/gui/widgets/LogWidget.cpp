#include "robomongo/gui/widgets/LogWidget.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QMenu>
#include <QTime>
#include <QAction>
#include <QPlainTextEdit>
#include "robomongo/core/utils/QtUtils.h"

namespace Robomongo
{
    LogWidget::LogWidget(QWidget* parent) 
        : BaseClass(parent), _logTextEdit(new QTextEdit(this))
    {
        _logTextEdit->setReadOnly(true);
        _logTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(_logTextEdit,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint &))));
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0,0,0,0);
        hlayout->addWidget(_logTextEdit);
        _clear = new QAction(this);
        VERIFY(connect(_clear, SIGNAL(triggered()),_logTextEdit, SLOT(clear())));
        setLayout(hlayout);      
        
        retranslateUI();
    }
    
    void LogWidget::retranslateUI()
    {
        _clear->setText(tr("Clear All"));
    }

    void LogWidget::showContextMenu(const QPoint &pt)
    {
        QMenu *menu = _logTextEdit->createStandardContextMenu();
        menu->addAction(_clear);
        _clear->setEnabled(!_logTextEdit->toPlainText().isEmpty());

        menu->exec(_logTextEdit->mapToGlobal(pt));
        delete menu;
    }

    void LogWidget::addMessage(const QString &message, mongo::LogLevel level)
    {
        QTime time = QTime::currentTime();
        //_logTextEdit->appendHtml(QString(level == mongo::LL_ERROR ? "<font color=red>%1 %2</font>" : "<font color=black>%1 %2</font>").arg(time.toString("h:mm:ss AP:")).arg(message.toHtmlEscaped()));
        _logTextEdit->setTextColor(level == mongo::LL_ERROR ? QColor(Qt::red):QColor(Qt::black));
        _logTextEdit->append(time.toString("h:mm:ss AP: ") + message);
        QScrollBar *sb = _logTextEdit->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
    
    void LogWidget::changeEvent(QEvent* event)
    {
        if (0 != event) {
            switch (event->type()) {
                // this event is send if a translator is loaded
            case QEvent::LanguageChange:
                retranslateUI();
//                Robomongo::LOG_MSG("QEvent::LanguageChange", mongo::LL_INFO);
                break;
                // this event is send, if the system, language changes
            case QEvent::LocaleChange:
//                Robomongo::LOG_MSG("QEvent::LocaleChange", mongo::LL_INFO);
                break;
            }
        }
        BaseClass::changeEvent(event);
    }
}
