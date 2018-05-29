TEMPLATE = subdirs

SUBDIRS += \
    Core \
#    AccessController \
    Client \
    SalesService

AccessController.depends = Core
Client.depends = Core
