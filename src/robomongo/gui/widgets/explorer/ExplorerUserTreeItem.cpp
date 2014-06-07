#include "robomongo/gui/widgets/explorer/ExplorerUserTreeItem.h"

#include <QAction>
#include <QMenu>

#include "robomongo/gui/dialogs/CreateUserDialog.h"
#include "robomongo/gui/utils/DialogUtils.h"
#include "robomongo/gui/GuiRegistry.h"

#include "robomongo/core/domain/MongoDatabase.h"
#include "robomongo/core/domain/MongoServer.h"
#include "robomongo/core/settings/ConnectionSettings.h"
#include "robomongo/core/utils/QtUtils.h"

namespace
{
    const char* tooltipTemplate = 
        "%s "
        "<table>"
        "<tr><td>ID:</td><td width=\"180\"><b>&nbsp;&nbsp;%s</b></td></tr>"
        "</table>";

    std::string buildToolTip(const Robomongo::MongoUser &user)
    {
        char buff[2048]={0};
        sprintf(buff,tooltipTemplate,user.name().c_str(),user.id().toString().c_str());
        return buff;
    }
}

namespace Robomongo
{
    ExplorerUserTreeItem::ExplorerUserTreeItem(QTreeWidgetItem *parent,MongoDatabase *const database, const MongoUser &user) :
        BaseClass(parent),_user(user),_database(database)
    {
        _dropUserAction = new QAction(this);
        VERIFY(connect(_dropUserAction, SIGNAL(triggered()), SLOT(ui_dropUser())));

        _editUserAction = new QAction(this);
        VERIFY(connect(_editUserAction, SIGNAL(triggered()), SLOT(ui_editUser())));

        BaseClass::_contextMenu->addAction(_editUserAction);
        BaseClass::_contextMenu->addAction(_dropUserAction);

        setText(0, QtUtils::toQString(_user.name()));
        setIcon(0, GuiRegistry::instance().userIcon());
        setExpanded(false);
        //setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        setToolTip(0, QtUtils::toQString(buildToolTip(user)));
        
        retranslateUI();
    }
    
    void ExplorerUserTreeItem::retranslateUI()
    {
        _dropUserAction->setText(tr("Drop User"));
        _editUserAction->setText(tr("Edit User"));
    }

    void ExplorerUserTreeItem::ui_dropUser()
    {
        // Ask user
        int answer = utils::questionDialog(treeWidget(), tr("Drop"), tr("User"), QtUtils::toQString(_user.name()));

        if (answer == QMessageBox::Yes) {
            _database->dropUser(_user.id());
            _database->loadUsers(); // refresh list of users
        }
    }

    void ExplorerUserTreeItem::ui_editUser()
    {
        float version = _user.version();
        CreateUserDialog *dlg = NULL;

        if (version < MongoUser::minimumSupportedVersion) {
            dlg = new CreateUserDialog(QtUtils::toQString(_database->server()->connectionRecord()->getFullAddress()), QtUtils::toQString(_database->name()), _user, treeWidget());
        }
        else {
           dlg = new CreateUserDialog(_database->server()->getDatabasesNames(), QtUtils::toQString(_database->server()->connectionRecord()->getFullAddress()), QtUtils::toQString(_database->name()), _user, treeWidget());
        }
        
        dlg->setWindowTitle(tr("Edit User"));
        dlg->setUserPasswordLabelText(tr("New Password:"));
        int result = dlg->exec();

        if (result == QDialog::Accepted) {

            MongoUser user = dlg->user();
            _database->createUser(user, true);

            // refresh list of users
            _database->loadUsers();
        }
        delete dlg;
    }
}
