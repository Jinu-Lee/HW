package scheduler;

import exception.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.awt.Color;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;

public class Calendar {
	private ArrayList<Schedule> schedules;
	private String name;
	private Color color;

	/**
	 * Default constructor for class Calendar
	 */
	public Calendar() {
		name = "";
		schedules = new ArrayList<>();
	}

	/**
	 * Another constructor for class Calendar
	 * @param name name of calendar
	 */
	public Calendar(String name, Color color) {
		this.name = name;
		schedules = new ArrayList<>();
		this.color = color;
	}

	/**
	 * getter of name
	 * @return name of calendar
	 */
	public String getName() {
		return name;
	}
	
	/**
	 * getter of color
	 * @return color of calendar
	 */
	public Color getColor() {
		return color;
	}

	/**
	 * setter of name
	 * @param name new name of calendar
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * get names of schedules
	 * @return names of schedules
	 */
	public String[] get_name() {
    	ArrayList<String> name = new ArrayList<>();
    	for (Schedule s : schedules) {
    		name.add(s.getName());
    	}
    	return name.toArray(new String[name.size()]);
    }

	/**
     * add new schedule
	 * time[year, month, date, hour, minute]
     * @param   all variables in class Schedule
     * @exception AlreadyDefinedException			
	 * @exception TooLongNameEnteredException		
     * @exception NoNameEnteredException			
     * @exception TimeAlreadyFullException		   
     * @exception StartTimeAfterEndTimeException	
	 * @exception NotRepeatableException		 	
     */
    public void add_schedule(String name, int[] start, int[] end, boolean isImp, boolean overlap, String memo, int repeat, boolean isAllDay, Color color) {
		LocalDateTime start_time, end_time;
		LocalDate time;
		Schedule new_schedule;
		
		if(name.length() == 0) {
    		throw new NoNameEnteredException("No name entered.");
		}
		else if (name.length() > 16) {
			throw new TooLongNameEnteredException("Name is too long. (limit: 16)");
		}

		if (isAllDay) {
			time = LocalDate.of(start[0], start[1], start[2]);
			new_schedule = new FullDaySchedule(name, isImp, memo, repeat, time,color);
		} 
		else {
			start_time = LocalDateTime.of(start[0], start[1], start[2], start[3], start[4], 0, 0);
			end_time = LocalDateTime.of(end[0], end[1], end[2], end[3], end[4], 0, 0);
			if(start_time.isEqual(end_time)) {
				throw new SameTimeException("Start and end times cannot be same.");
			}
			if(start_time.isAfter(end_time)) {
				throw new StartTimeAfterEndTimeException("Start time must be earlier than end time.");
			}
			switch (repeat) {
				case 1:
					if (ChronoUnit.YEARS.between(start_time, end_time) >= 1) {
						throw new NotRepeatableException("It cannot be repeated.");
					}
					break;
				case 2:
					if (ChronoUnit.MONTHS.between(start_time, end_time) >= 1) {
						throw new NotRepeatableException("It cannot be repeated.");
					}
					break;
				case 3:
					if (ChronoUnit.WEEKS.between(start_time, end_time) >= 1) {
						throw new NotRepeatableException("It cannot be repeated.");
					}
					break;
			}
			for (Schedule s : schedules) {
				if (s instanceof FullDaySchedule) {
					continue;
				}

				NormalSchedule temp = (NormalSchedule) s;

				if(name.equals(temp.getName())) {
					throw new AlreadyDefinedException("Schedule names cannot be duplicated.");
				}
				
				if(temp.getCanBeOverlapped() && overlap) {
					continue;
				}

				if(start_time.equals(temp.getStartTime()) || end_time.equals(temp.getEndTime())) {
					throw new TimeAlreadyFullException("There is already another schedule at that time.");
				}
	
				if(temp.getStartTime().isAfter(start_time)) {
					if(end_time.isAfter(temp.getStartTime())) {
						throw new TimeAlreadyFullException("There is already another schedule at that time.");
					}
				}
				else {
					if(temp.getEndTime().isAfter(start_time)) {
						throw new TimeAlreadyFullException("There is already another schedule at that time.");
					}
				}
			}
			new_schedule = new NormalSchedule(name, isImp, overlap, memo, repeat, start_time, end_time,color);
		}

    	schedules.add(new_schedule);
	}
	
	/**
	 * get schedule
	 * @param name name of schedule to delete
	 * @exception NoNameEnteredException 
	 * @exception NoNameMatchException   
	 */
	public Schedule get_Schedule(String name) {
		if (name == null) {
			throw new NoNameEnteredException("No name entered.");
		}

		Iterator<Schedule> iter = schedules.iterator();
		while (iter.hasNext()) {
			Schedule s = iter.next();
			if (name.equals(s.getName())) {
				return s;
			}
		}

		throw new NoNameMatchException("The schedule with that name does not exist.");
	}

	/**
	 * get all schedules
	 * @return list of schedules  
	 */
	public ArrayList<Schedule> get_Schedules() {
		return schedules;
	}


	/**
	 * delete schedule
	 * @param name name of schedule to delete
	 * @exception NoNameEnteredException 
	 * @exception NoNameMatchException   
	 */
	public void remove_schedule(String name) {
		if (name == null) {
			throw new NoNameEnteredException("No name entered.");
		}

		Iterator<Schedule> iter = schedules.iterator();
		while (iter.hasNext()) {
			Schedule s = iter.next();
			if (name.equals(s.getName())) {
				iter.remove();
				return;
			}
		}

		throw new NoNameMatchException("The schedule with that name does not exist.");
	}

	/**
	 * change details of schedule
	 * @param all variables in class Schedule & new name of schedule
	 * @exception NoNameEnteredException        	
	 * @exception AlreadyDefinedException        
	 * @exception TimeAlreadyFullException       	
	 * @exception StartTimeAfterEndTimeException 	
	 * @exception NoNameMatchException           	
	 * @exception NotRepeatableException		 
	 */
	public void modify_schedule(String name, String new_name, int[] start, int[] end, boolean isImp, boolean overlap,
			String memo, int repeat, boolean isAllDay, Color color) {
		if (new_name == null) {
			throw new NoNameEnteredException("No name entered.");
		}

		remove_schedule(name);
		add_schedule(new_name, start, end, isImp, overlap, memo, repeat, isAllDay, color);
	}

	/**
	 * read schedule from arraylist 'schedules' on specific date
	 * @param today	
	 * @return			
	 */
	public ArrayList<Schedule> read_schedule(LocalDate today) {
		ArrayList<Schedule> schedule_of_day = new ArrayList<>();

		for (Schedule s : schedules) {
			boolean flag = false;
			switch (s.getRepeatType()) {
				case 0:
					if (s instanceof NormalSchedule) {
						LocalDate start_time = ((NormalSchedule) s).getStartTime().toLocalDate();
						LocalDate end_time = ((NormalSchedule) s).getEndTime().toLocalDate();
						
						if (start_time.equals(today) || end_time.equals(today) || (start_time.isBefore(today) && end_time.isAfter(today))) {
							flag = true;
						}
					} else {
						LocalDate time = ((FullDaySchedule) s).getTime();

						if (time.equals(today)) {
							flag = true;
						}
					}
					break;
				case 1:
					if (s instanceof NormalSchedule) {
						LocalDate start_time = ((NormalSchedule) s).getStartTime().toLocalDate();
						LocalDate end_time = ((NormalSchedule) s).getEndTime().toLocalDate();
						int start_month = start_time.getMonthValue(), start_day = start_time.getDayOfMonth();
						int end_month = end_time.getMonthValue(), end_day = end_time.getDayOfMonth();
						int today_month = today.getMonthValue(), today_day = today.getDayOfMonth();

						if (start_month > end_month) {
							if (today_month < end_month) {
								today_month += 12;
							}
							end_month += 12;
						}
						if (start_month == end_month) {
							if (start_day <= today_day && today_day <= end_day) {
								flag = true;
							}
						}
						else {
							if (start_month == today_month && start_day <= today_day) {
								flag = true;
							} else if (end_month == today_month && today_day <= end_day) {
								flag = true;
							} else if (start_month < today_month && today_month < end_month) {
								flag = true;
							}
						}
					} else {
						LocalDate time = ((FullDaySchedule) s).getTime();
						int time_month = time.getMonthValue(), time_day = time.getDayOfMonth();
						int today_month = today.getMonthValue(), today_day = today.getDayOfMonth();

						if (today_month == time_month && today_day == time_day) {
							flag = true;
						}
					}
					break;
				case 2:
					if (s instanceof NormalSchedule) {
						LocalDate start_time = ((NormalSchedule) s).getStartTime().toLocalDate();
						LocalDate end_time = ((NormalSchedule) s).getEndTime().toLocalDate();
						int start_day = start_time.getDayOfMonth(), end_day = end_time.getDayOfMonth(), today_day = today.getDayOfMonth();

						if (start_day > end_day) {
							if (today_day <= end_day || start_day <= today_day) {
								flag = true;
							}
						}
						else {
							if (start_day <= today_day && today_day <= end_day) {
								flag = true;
							}
						}
					} else {
						LocalDate time = ((FullDaySchedule) s).getTime();
						int time_day = time.getDayOfMonth(), today_day = today.getDayOfMonth();

						if (time_day == today_day) {
							flag = true;
						}
					}
					break;
				case 3:
					if (s instanceof NormalSchedule) {
						LocalDate start_time = ((NormalSchedule) s).getStartTime().toLocalDate();
						LocalDate end_time = ((NormalSchedule) s).getEndTime().toLocalDate();
						int start_day_week = start_time.getDayOfWeek().getValue(), end_day_week = end_time.getDayOfWeek().getValue(), today_day_week = today.getDayOfWeek().getValue();

						if (start_day_week > end_day_week) {
							if (today_day_week <= end_day_week || start_day_week <= today_day_week) {
								flag = true;
							}
						} else {
							if (start_day_week <= today_day_week && today_day_week <= end_day_week) {
								flag = true;
							}
						}
					} else {
						LocalDate time = ((FullDaySchedule) s).getTime();
						int time_day_week = time.getDayOfWeek().getValue(), today_day_week = today.getDayOfWeek().getValue();

						if (time_day_week == today_day_week) {
							flag = true;
						}
					}
					break;
			}
			if (flag) {
				schedule_of_day.add(s);
			}
		}

		return schedule_of_day;
	}
}
