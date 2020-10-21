/*
Copyright (C) 2019-2020 The CadexOS Project
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#ifndef MUTEX_H
#define MUTEX_H

#include "list.h"

struct mutex {
    int locked;
    struct list waitqueue;
};

#define MUTEX_INIT \
    { 0, LIST_INIT }

void mutex_lock(struct mutex *m);
void mutex_unlock(struct mutex *m);

#endif
