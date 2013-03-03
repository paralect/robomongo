#include "robomongo/core/domain/MongoDatabase.h"

#include "robomongo/core/domain/MongoServer.h"
#include "robomongo/core/domain/MongoCollection.h"
#include "robomongo/core/events/MongoEvents.h"
#include "robomongo/core/AppRegistry.h"
#include "robomongo/core/EventBus.h"

using namespace Robomongo;

R_REGISTER_EVENT(MongoDatabase_CollectionListLoadedEvent)
R_REGISTER_EVENT(MongoDatabase_UsersLoadedEvent)

MongoDatabase::MongoDatabase(MongoServer *server, const QString &name) : QObject(),
    _system(false),
    _server(server),
    _bus(AppRegistry::instance().bus())
{
    _name = name;
    _client = _server->client();

    // Check that this is a system database
    _system = name == "admin" ||
              name == "local";
}

MongoDatabase::~MongoDatabase()
{
    clearCollections();
}

void MongoDatabase::loadCollections()
{
    _bus->send(_client, new LoadCollectionNamesRequest(this, _name));
}

void MongoDatabase::loadUsers()
{
    _bus->send(_client, new LoadUsersRequest(this, _name));
}

void MongoDatabase::createCollection(const QString &collection)
{
    _bus->send(_client, new CreateCollectionRequest(this, _name, collection));
}

void MongoDatabase::dropCollection(const QString &collection)
{
    _bus->send(_client, new DropCollectionRequest(this, _name, collection));
}

void MongoDatabase::renameCollection(const QString &collection, const QString &newCollection)
{
    _bus->send(_client, new RenameCollectionRequest(this, _name, collection, newCollection));
}

void MongoDatabase::createUser(const MongoUser &user, bool overwrite)
{
    _bus->send(_client, new CreateUserRequest(this, _name, user, overwrite));
}

void MongoDatabase::dropUser(const mongo::OID &id)
{
    _bus->send(_client, new DropUserRequest(this, _name, id));
}

void MongoDatabase::handle(LoadCollectionNamesResponse *loaded)
{
    if (loaded->isError())
        return;

    clearCollections();

    foreach(MongoCollectionInfo info, loaded->collectionInfos())    {
        MongoCollection *collection = new MongoCollection(this, info);
        addCollection(collection);
    }

    _bus->publish(new MongoDatabase_CollectionListLoadedEvent(this, _collections));
}

void MongoDatabase::handle(LoadUsersResponse *event)
{
    if (event->isError())
        return;

    _bus->publish(new MongoDatabase_UsersLoadedEvent(this, this, event->users()));
}

void MongoDatabase::clearCollections()
{
    qDeleteAll(_collections);
    _collections.clear();
}

void MongoDatabase::addCollection(MongoCollection *collection)
{
    _collections.append(collection);
}
