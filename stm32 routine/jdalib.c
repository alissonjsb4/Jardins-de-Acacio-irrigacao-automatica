#include <string.h>
#include <stdlib.h>
#include "jdalib.h"

Command processCommand(char* command) {
    Command refinedCommand = {"", "", "", ""};  // Initialize with empty strings

    char* strCopy = strdup(command);
    if (!strCopy) {
        return refinedCommand;  // Memory allocation failed
    }

    char* token = strtok(strCopy, " ");
    if (token == NULL || strlen(token) > 4) {
        free(strCopy);
        return refinedCommand;
    }
    strcpy(refinedCommand.instruction, token);

    token = strtok(NULL, " ");
    if (token == NULL || strlen(token) > 8) {
        free(strCopy);
        return refinedCommand;
    }
    refinedCommand.param1 = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL || strlen(token) > 8) {
        free(strCopy);
        return refinedCommand;
    }
    refinedCommand.param2 = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL || strlen(token) > 8) {
        free(strCopy);
        return refinedCommand;
    }
    refinedCommand.param3 = atoi(token);

    free(strCopy);
    return refinedCommand;
}

int findFreeSchedule(Schedule* schedulesArray) {
    for(int i = 0; i < 8; i++) {
        if(schedulesArray[i].scheduleId == 0)
            return i;
    }
    return -1;
}

int findScheduleIndexById(Schedule* schedulesArray, int scheuduleId) {
    for(int i = 0; i < 8; i++) {
        if(schedulesArray[i].scheduleId == scheuduleId)
            return i;
    }
    return -1;
}

Schedule setSchedule(Command commandToApply) {
    Schedule newSchedule = {commandToApply.param1, commandToApply.param2, commandToApply.param3};
    return newSchedule;
}

Programated setProgram(Command commandToApply) {
    Programated newProgram = {commandToApply.param1, commandToApply.param2, commandToApply.param3};
    return newProgram;
}