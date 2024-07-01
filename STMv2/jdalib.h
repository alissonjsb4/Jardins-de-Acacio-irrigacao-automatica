
#ifndef JDALIB_H
#define JDALIB_H





typedef struct {
	int active;
	int duration;
} Action;

/* Implemented types of command: instruction [arg1] [arg2] ...

    - turn [on/off] [duration] (<- optational, Standard: forever)
    //Turn on or off the system by the designated duration whithout pausing the RTC or erasing memory
    //Often used to cancel previous config during a certain period of rains

    - sche [timeUntilStart] [duration] [scheduleId] (<- optational, Standard: system generated)
    //Schedule an event at 'timeUntilStart' for 'duration'
    //'sche' instruction accumulates and 2 'sche' dont cancell each other

    - sdel [scheduleId]
    //Deletes a scheduled event

    - prog [timeUntilStart] [duration] [period]
    //Schedule a event at 'timeToStart' for 'duration' and repeat it every 'period'
*/

typedef struct {
    char instruction[5];
    int param1;
    int param2;
    int param3;
} Command;

typedef struct {
    int timeUntilStart;
    int duration;
    int scheduleId;
} Schedule;

typedef struct {
    int timeUntilStart;
    int duration;
    int period;
} Programated;

typedef struct {
    int active;                // 1 - On || 0 - Off
    int activeDuration;        // 0 - done | -1 - notDefined
    Schedule schedules[8];    // schedule[i].scheduleId == 0 - not scheduled
    Programated programated;  // period == 0 - not programated
} RoutineState;

Command processCommand(char* command);

int findFreeSchedule(Schedule* schedulesArray);

Schedule findScheduleById(Schedule* schedulesArray, int scheuduleId);

Schedule setSchedule(Command commandToApply);

Programated setProgram(Command commandToApply);

#endif // JDALIB_H
