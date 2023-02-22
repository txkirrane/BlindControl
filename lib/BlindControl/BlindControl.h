#define BlindControl_h

#define MAX_SCHEDULE_ENTRIES 8

enum BlindState {
    CLOSED,
    OPEN,
};

class Blinds {
    public:
        void open();
        void close();
};

class TimeData {
    private:
        int hour;
        int minute;
    public:
        int getHour();
        int getMinute();

        TimeData();
        TimeData(int hour, int minute);
};

class ScheduleEntry {
    public:
        TimeData trigger;
        BlindState desiredState;

        ScheduleEntry();
        ScheduleEntry(TimeData trigger, BlindState desiredState);
};

class Schedule {
    private:
        BlindState defaultState;
        ScheduleEntry entries [MAX_SCHEDULE_ENTRIES];
        int entryCount;
    public:
        BlindState getScheduledBlindState(TimeData currentTime);
        void addEntry(ScheduleEntry entry);

        Schedule();
        Schedule(BlindState defaultState);

};