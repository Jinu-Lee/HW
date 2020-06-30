package scheduler;

import java.awt.Color;
import java.time.LocalDateTime;

public class NormalSchedule extends Schedule {
    private LocalDateTime start_time;
    private LocalDateTime end_time;
    private boolean canBeOverlapped;
    
    /**
     * default constructor of class NormalSchedule
     */
    public NormalSchedule() {
        super();
        start_time = LocalDateTime.now();
        end_time = start_time;
        canBeOverlapped = false;
    }

    /**
     * Another constructor of class NormalSchedule
     * @param n         name of schedule
     * @param isImp     is important or not
     * @param overlap   can be overlapped or not
     * @param m         memo about schedule
     * @param repeat    repeat type
     * @param start     start time of schedule
     * @param end       end time of schedule
     */
    public NormalSchedule(String n, boolean isImp, boolean overlap, String m, int repeat, LocalDateTime start, LocalDateTime end, Color c) {
        super(n, isImp, m, repeat, c);
        start_time = start;
        end_time = end;
        canBeOverlapped = overlap;
    }

    /**
     * getter of start_time
     * @return start_time of calendar
     */
    public LocalDateTime getStartTime() {
        return start_time;
    }

    /**
     * getter of end_time
     * @return end_time of calendar
     */
    public LocalDateTime getEndTime() {
        return end_time;
    }

    /**
     * getter of canBeOverlapped
     * @return canBeOverlapped of calendar
     */
    public boolean getCanBeOverlapped() {
        return canBeOverlapped;
    }

    /**
     * change value of each variable if it is not null
     * @param n         name of schedule
     * @param isImp     is important or not
     * @param overlap   can be overlapped or not
     * @param m         memo about schedule
     * @param repeat    repeat type
     * @param start     start time of Schedule
     * @param end       end time of Schedule
     */
    public void modify_schedule(String n, boolean isImp, boolean overlap, String m, int repeat, LocalDateTime start, LocalDateTime end, Color c) {
        super.modify_schedule(n, isImp, m, repeat, c);
        canBeOverlapped = overlap; 
        start_time = start;
        end_time = end;
    }
}