package scheduler;

import exception.*;

import java.awt.Color;
import java.util.ArrayList;
import java.util.Iterator;

public class Scheduler {
    private ArrayList<Calendar> calendars;

    /**
     * default constructor of Scheduler
     */
    public Scheduler() {
        calendars = new ArrayList<>();
        calendars.add(new Calendar("default",Color.LIGHT_GRAY));
    }
    
    /**
     * get names of calendar
     * @return name	names of calendar
     */
    public String[] get_name() {
    	ArrayList<String> name = new ArrayList<>();
    	for (Calendar c : calendars) {
    		name.add(c.getName());
    	}
    	return name.toArray(new String[name.size()]);
    }
    
    /**
     * get calendar
     * @param name  name of calendar to get
     * @exception NoNameMatchException          
     */
    public Calendar get_calendar(String name) {
    	for (Calendar c : calendars) {
    		if (name.equals(c.getName())) {
    			return c;
    		}
    	}
    	
    	throw new NoNameMatchException("The calendar with that name does not exist.");
    }

    /**
     * make new calendar
     * @param name  name of new calendar  
     * @exception AlreadyDefinedException   
     * @exception NoNameEnteredException    
     */
    public void add_calendar(String name, Color color) {
        if (name.length() == 0 || name == null) {
            throw new NoNameEnteredException("No name entered.");
        }

        for (Calendar c : calendars) {
            if (name.equals(c.getName())) {
                throw new AlreadyDefinedException("'" + name + "' Calendar is already defined.");
            }
        }

        Calendar c = new Calendar(name,color);
        calendars.add(c);
    }

    /**
     * remove calendar
     * @param name  name of calendar to delete
     * @exception NoNameEnteredException        
     * @exception NoNameMatchException          
     * @exception LastCalendarException         
     */
    public void remove_calendar(String name) {
        if (name.length() == 0 || name == null) {
            throw new NoNameEnteredException("No name entered.");
        }

        if (calendars.size() == 1) {
            throw new LastCalendarException("There is only one calendar left.");
        }

        Iterator<Calendar> iter = calendars.iterator();
        while(iter.hasNext()) {
            Calendar c = iter.next();
            if (name.equals(c.getName())) {
                iter.remove();
                return;
            }
        }

        throw new NoNameMatchException("There is no calendar : '" + name + "'");
    }

    /**
     * change name of calendar
     * @param old_name  old name of calendar
     * @param new_name  new name of calendar
     * @exception NoNameMatchException                 
     * @exception NoNameEnteredException             
     * @exception AlreadyDefinedException   		
     * @exception EmptyCalendarException             
     */
    public void modify_calendar(String old_name, String new_name) {
        if (old_name.length() == 0 || old_name == null || new_name.length() == 0 || new_name == null) {
            throw new NoNameEnteredException("No name entered.");
        }
        
        for (Calendar c : calendars) {
            if (new_name.equals(c.getName())) {
                throw new AlreadyDefinedException("Calendar names cannot be duplicated.");
            }
        }

        for (Calendar c : calendars) {
            if (old_name.equals(c.getName())) {
                c.setName(new_name);
                return;
            }
        }

        throw new NoNameMatchException("The calendar with that name does not exist.");
    }
}