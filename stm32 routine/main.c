#include <stdio.h>
#include <string.h>
#include "jdalib.h"



RoutineState routine;

int main() {

    while(1) {
        //ROUTINE I - LISTENING TO CHANGES
        char rawCommand[100];                                //Maximum buffer
        fgets(rawCommand, sizeof(rawCommand), stdin);        //Code to receive Serial UART *
        Command commandToApply = processCommand(rawCommand); //Command parsed to struct Command

        if(!strcmp(commandToApply.instruction, "turn")) {
            //If both params are null, standard: Off for undeterminated time
            routine.active = (commandToApply.param1 > 0) ? commandToApply.param1 : 0;
            routine.activeDuration = (commandToApply.param2 != NULL) ? commandToApply.param2 : -1;

        } else if(!strcmp(commandToApply.instruction, "sche")) {
            //Lack of some constraints
            int index = findFreeSchedule(routine.schedules);
            if(index != -1) {
                Schedule scheduleToApply = setSchedule(commandToApply);
                routine.schedules[index] = scheduleToApply;

                printf("New schedule ID: %d \nTimeUntilStart: %d \nDuration: %d \n allocatedAt: %d", 
                routine.schedules[index].scheduleId,
                routine.schedules[index].timeUntilStart,
                routine.schedules[index].duration, index);
            } else {
                //ERROR: SCHEDULES FULL!
            }

        } else if(!strcmp(commandToApply.instruction, "sdel")) {
            //Lack of some constraints
            int indexToDelete = findScheduleIndexById(routine.schedules, commandToApply.param1);
            if(indexToDelete >= 0) {
                routine.schedules[indexToDelete].duration = 0;
                routine.schedules[indexToDelete].scheduleId = 0;
                routine.schedules[indexToDelete].timeUntilStart = 0;
            } else {
                //ERROR: SCHEDULE NOT FOUND!
            }
            
        } else if(!strcmp(commandToApply.instruction, "prog")) {
            //Lack of some constraints
            Programated newProgram = setProgram(commandToApply);
            routine.programated.duration = newProgram.duration;
            routine.programated.period = newProgram.period;
            routine.programated.timeUntilStart = newProgram.timeUntilStart;

        } else {
            //ERROR: COMMAND DONT EXIST!
        }

        //ROUTINE II - RESPONSE HANDLING
        //RESPOSE CODE
        
        //ROUTINE III - HANDLING TIMING
        if(routine.active == 0 && routine.activeDuration == 0)
            routine.active = 1;

        //ROUTINE IV - EXECUTING CURRENT ROUTINE SETTINGS
        if(routine.active == 0)
            continue;
            
    }

    return 0;
}