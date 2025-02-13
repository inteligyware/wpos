// file      :  memstickinterface.h
// birth     :  7/27/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef MEM_STICK_INTERFACE_H
#define MEM_STICK_INTERFACE_H

class MemStickInterface {
    virtual void setOperationResult(bool) = 0; // ASYNC
};

#endif // MEM_STICK_INTERFACE_H
