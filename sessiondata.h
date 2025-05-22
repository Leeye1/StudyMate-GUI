#ifndef SESSIONDATA_H
#define SESSIONDATA_H


#include <QString>

struct DistractionData {
    int action1 = 0;//行为1的分神次数
    int action2 = 0;
    int action3 = 0;
    int total_time = 0;//所有分神动作的累积分神时间
};

struct FocusSession {
    QString datetime;//日期+时间
    int duration_minutes = 0;//本次番茄钟时长
    int focus_minutes = 0;//本次专注时间等于durationtime-totaltime
    DistractionData distractions;//distractions: 嵌套了上面定义的 DistractionData。
};



#endif // SESSIONDATA_H
