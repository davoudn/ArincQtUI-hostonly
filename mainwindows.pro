TEMPLATE      = subdirs
SUBDIRS       = menus

unix|win32: LIBS += -lpigpio
