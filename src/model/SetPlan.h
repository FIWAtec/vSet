#pragma once

#include "model/TrackItem.h"
#include <QList>
#include <QString>

struct SetPlan
{
    QString title;
    int targetBpm = 0;
    QList<TrackItem> tracks;
};
