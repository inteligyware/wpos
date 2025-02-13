// file      :  reportinterface.h
// birth     :  5/11/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef REPORTINTERFACE_H
#define REPORTINTERFACE_H

#include <QString>
class ReportInterface{
    virtual void reportSlot(QString)=0; //ASYNC
};
#endif
