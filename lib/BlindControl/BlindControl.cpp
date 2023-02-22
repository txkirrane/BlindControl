#include "BlindControl.h"

// Blinds

void Blinds::open(){
    // To be populated by inheritng function
};

void Blinds::close(){
    // To be populated by inheritng function
};

// TimeData
TimeData::TimeData(){
    // Noargs constructor, the hour field must be set to
    // an unreachable value due to the fact when the ScheduleEntry
    // array is initialized it uses these values.
    this->hour = 25;
    this->minute = 0;
};
TimeData::TimeData(int hour, int minute){
    // Normal constructor
    this->hour = hour;
    this->minute = minute;
};

int TimeData::getHour(){
    return this->hour;
};

int TimeData::getMinute(){
    return this->minute;
};

// ScheduleEntry
ScheduleEntry::ScheduleEntry(){
    this->trigger = TimeData();
    this->desiredState = OPEN;
};
ScheduleEntry::ScheduleEntry(TimeData trigger, BlindState desiredState){
    this->trigger = trigger;
    this->desiredState = desiredState;
};

// Schedule
Schedule::Schedule(){
    this->defaultState = CLOSED;
};
Schedule::Schedule(BlindState defaultState){
    this->defaultState = defaultState;
};

void Schedule::addEntry(ScheduleEntry entry){
    if(entryCount < MAX_SCHEDULE_ENTRIES){
        this->entries[this->entryCount] = entry;
        this->entryCount++;
    }
}

BlindState Schedule::getScheduledBlindState(TimeData currentTime){
    BlindState scheduledBlindState = this->defaultState;

    for(ScheduleEntry entry : this->entries){
        if(currentTime.getHour() >= entry.trigger.getHour()) {
            if(currentTime.getMinute() >= entry.trigger.getMinute()){
                scheduledBlindState = entry.desiredState;
            }
        }
    }

    return scheduledBlindState;
}