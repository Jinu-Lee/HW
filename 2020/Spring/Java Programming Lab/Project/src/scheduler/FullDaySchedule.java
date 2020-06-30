package scheduler;

import java.awt.Color;
import java.time.LocalDate;

public class FullDaySchedule extends Schedule{
    private LocalDate time;
    
    /**
     * default constructor of class FullDaySchedule
     */
    public FullDaySchedule() {
        super();
        time = LocalDate.now();
    }

    /**
     * Another constructor of class FullDaySchedule
     * @param n         name of schedule
     * @param isImp     is important or not
     * @param overlap   can be overlapped or not
     * @param m         memo about schedule
     * @param repeat    repeat type
     * @param t         time of schedule
     */
    public FullDaySchedule(String n, boolean isImp, String m, int repeat, LocalDate t, Color c) {
        super(n, isImp, m, repeat,c);
        time = t;
    }

    /**
     * getter of time
     * @return time of calendar
     */
    public LocalDate getTime() {
        return time;
    }

    /**
     * change value of each variable if it is not null
     * @param n         name of schedule
     * @param isImp     is important or not
     * @param m         memo about schedule
     * @param repeat    repeat type
     * @param t         time of Schedule
     */
    public void modify_schedule(String n, boolean isImp, String m, int repeat, LocalDate t, Color c) {
        super.modify_schedule(n, isImp, m, repeat, c);
        time = t;
    }
}