
#include "Chart.h"

Chart::Chart() : strobes(new QVector<InteractiveStrobe*>)
{
    setMinimumHeight(0);

    layout()->setContentsMargins(0, 0, 0, 0);
    setBackgroundRoundness(0);
    setMargins(QMargins{ 0 , 0 ,0 , 0 });
}

Chart::~Chart()
{
}

