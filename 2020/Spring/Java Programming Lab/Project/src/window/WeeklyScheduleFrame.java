package window;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.time.LocalDateTime;
import java.util.ArrayList;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.ScrollPaneConstants;
import scheduler.FullDaySchedule;
import scheduler.NormalSchedule;
import scheduler.Schedule;

public class WeeklyScheduleFrame extends JFrame {

	/**
	 *
	 */
	private static final long serialVersionUID = -113556207287252627L;
	private JPanel panel;
	private JPanel panel_1 = new JPanel();
	private JPanel panel_3;
	private JScrollPane scrollPane;
	private JPanel panel_4;
	private GridBagLayout gbl_panel_4;
	private GridBagConstraints first_gridbag;
	private JPanel first_panel;
	private JPanel[] panel_times;
	private GridBagConstraints[] times_constraints;
	private JPanel[] panel_dates;
	private GridBagConstraints[] dates_constraints; 
	
	public WeeklyScheduleFrame(ArrayList<ArrayList<Schedule>> schedule){
		setVisible(true);
		
		int maxnumofallday = 0;
		for(int i=0;i<7;i++) {
			int numofallday = 0;
			for (Schedule s : schedule.get(i)) {
				if(s instanceof FullDaySchedule) {
					numofallday++;
				}
			}
			if(numofallday>maxnumofallday) {
				maxnumofallday = numofallday;
			}
		}
		getContentPane().setMinimumSize(new Dimension(1430, 700));
		setMinimumSize(new Dimension(1430, 700));
		getContentPane().setSize(new Dimension(500, 500));
		setBounds(100, 100, 510, 500);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
		
		panel_1.setPreferredSize(new Dimension(1430, 500));
		getContentPane().add(panel_1);
		panel_1.setLayout(new BoxLayout(panel_1, BoxLayout.X_AXIS));
		
		panel_3 = new JPanel();
		panel_3.setMinimumSize(new Dimension(1400, 23));
		scrollPane = new JScrollPane(panel_3);
		scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		panel_1.add(scrollPane);
		panel_3.setLayout(new BoxLayout(panel_3, BoxLayout.X_AXIS));
		
		panel = new JPanel();
		panel_4 = panel;
		panel_3.add(panel_4);
		gbl_panel_4 = new GridBagLayout();
		panel_4.setLayout(gbl_panel_4);
		
		first_panel = new JPanel();
		first_gridbag = new GridBagConstraints();
		first_gridbag.gridx = 0;
		first_gridbag.gridy = 0;
		panel_4.add(first_panel,first_gridbag);
		
		panel_times = new JPanel[96];
		times_constraints = new GridBagConstraints[96];
		for(int i=0;i<96;i++) {
			panel_times[i] = new JPanel();
			times_constraints[i] = new GridBagConstraints();
			times_constraints[i].gridx = 0;
			times_constraints[i].gridy = i+3+maxnumofallday;
			times_constraints[i].gridheight = 1;
			times_constraints[i].gridwidth = 1;
			
			String temp = Integer.toString(i/4);
			if(i/4<10)
				temp = "0".concat(temp);
			temp = temp.concat(":");
			if(i%4==0)
				temp = temp.concat("0");
			temp = temp.concat(Integer.toString(i%4 *15));
			panel_times[i].add(new JLabel(temp));
			
			panel_4.add(panel_times[i],times_constraints[i]);
		}
		
		panel_dates = new JPanel[8];
		dates_constraints = new GridBagConstraints[8];
		for(int i=0;i<7;i++) {
			panel_dates[i] = new JPanel();
			dates_constraints[i] = new GridBagConstraints();
			dates_constraints[i].gridx = i*6+1;
			dates_constraints[i].gridy = 0;
			dates_constraints[i].gridwidth = 6;
			dates_constraints[i].gridheight = 1;
			
			panel_4.add(panel_dates[i],dates_constraints[i]);
		}
		panel_dates[0].add(new JLabel("                         Sun                         "));
		panel_dates[1].add(new JLabel("                         Mon                         "));
		panel_dates[2].add(new JLabel("                         Tue                         "));
		panel_dates[3].add(new JLabel("                         Wed                         "));
		panel_dates[4].add(new JLabel("                         Thu                         "));
		panel_dates[5].add(new JLabel("                         Fri                         "));
		panel_dates[6].add(new JLabel("                         Sat                         "));
		
		JSeparator sep = new JSeparator(JSeparator.HORIZONTAL);
		GridBagConstraints gridsep = new GridBagConstraints();
		gridsep.gridx = 1;
		gridsep.gridy = 1;
		gridsep.gridheight = 1;
		gridsep.gridwidth = 6*7;
		sep.setPreferredSize(new Dimension(1400,1));
		panel_4.add(sep,gridsep);
		
		JSeparator sep1 = new JSeparator(JSeparator.HORIZONTAL);
		GridBagConstraints gridsep1 = new GridBagConstraints();
		gridsep1.gridx = 1;
		gridsep1.gridy = 2+maxnumofallday;
		gridsep1.gridheight = 1;
		gridsep1.gridwidth = 6*7;
		sep1.setPreferredSize(new Dimension(1400,1));
		panel_4.add(sep1,gridsep1);
		
		for(int i=0;i<7;i++) {
			int alldayorder = 0;
			int[] order = new int[100];
			int orderindex = 0;
			int[][] overlap = new int[96][100];
			int index = 0;
			for(Schedule s : schedule.get(i)) {
				if(s instanceof FullDaySchedule) {
					showweeklyalldayschedule(panel,s.getName(),i,s.getColor(), alldayorder++);
				}else {
					NormalSchedule ns = (NormalSchedule) s;
					LocalDateTime starttime = ns.getStartTime();
					int intstarttime = starttime.getHour()*4+starttime.getMinute()/15;
					LocalDateTime endtime = ns.getEndTime();
					int intendtime = endtime.getHour()*4+endtime.getMinute()/15;
					
					boolean flag = true;
					
					for(orderindex=0;orderindex<100;orderindex++) {
						for(int k=intstarttime;k<intendtime;k++) {
							if(overlap[k][orderindex]!=0) {
								flag = false;
								break;
							}
						}
						if(flag==false) {
							flag = true;
							continue;
						}
						break;

					}
					for(int k=intstarttime;k<intendtime;k++) {
						overlap[k][orderindex] = 1;
					}					
					order[index++]=orderindex;
				}
			}
			orderindex = 0;
			for(Schedule s : schedule.get(i)) {
				if(s instanceof NormalSchedule) {
					NormalSchedule ns = (NormalSchedule) s;
					LocalDateTime starttime = ns.getStartTime();
					int intstarttime = starttime.getHour()*4+starttime.getMinute()/15;
					LocalDateTime endtime = ns.getEndTime();
					int intendtime = endtime.getHour()*4+endtime.getMinute()/15;

					int num = 1;
					for(int j=intstarttime;j<intendtime;j++) {
						int sum = 0;
						for(int k=0;k<100;k++) {
							sum+=overlap[j][k];
						}
						if(sum>num) {
							num = sum;
						}
					}
					showweeklyschedule(panel,s.getName(),i,intstarttime, intendtime, s.getColor(), num, order[orderindex++], maxnumofallday);
				}		
			}
		}

		pack();
	}


	/**
	 * panel : Gridbag panel
	 * name : name of schedule
	 * date : day of week : sun-0, mon-1, tue-2,..., sat-6
	 * starttime, endtime : hour*4+minute%15
	 * Color : color of calendar
	 * num : number of overlapped schedule
	 * order : order of this schedule among overlapped schedule
	 * numofallday : maximum number of allday schedule of the week
	 */
	static void showweeklyschedule(JPanel panel,String name,int date ,int starttime, int endtime, Color color,int num, int order, int numofallday) {

		JPanel paneltemp = new JPanel();
		
		paneltemp.setPreferredSize(new Dimension(210/num,(endtime-starttime)*26));
		paneltemp.setBackground(color);
		GridBagConstraints gridbagtemp = new GridBagConstraints();	
			
		gridbagtemp.gridy = 3+starttime+numofallday;
		gridbagtemp.gridheight = endtime-starttime;
		gridbagtemp.gridx = 1+date*6+order*(6/num);
		gridbagtemp.gridwidth = 6/num;		
		
		if (name.length()>8&&num>2){
			name = name.substring(0, 8)+"<br>"+name.substring(8,name.length());
		}
		name = "<html>"+name+"</html>";
		JLabel temp = new JLabel(name);
		temp.setFont(new Font(null,Font.PLAIN, 9));
		paneltemp.add(temp);
		panel.add(paneltemp,gridbagtemp);
		
	}
	/*
	 * panel : Gridbag panel
	 * name : name of schedule
	 * date : day of week : sun-0, mon-1, tue-2,..., sat-6
	 * Color : color of calendar
	 * order : order of this schedule among same day schedules
	 */
	static void showweeklyalldayschedule(JPanel panel,String name,int date ,Color color, int order) {
		JPanel paneltemp = new JPanel();
		
		paneltemp.setPreferredSize(new Dimension(210,30));
		paneltemp.setBackground(color);
		GridBagConstraints gridbagtemp = new GridBagConstraints();	
			
		gridbagtemp.gridy = 2+order;
		gridbagtemp.gridheight = 1;
		gridbagtemp.gridx = 1+date*6;
		gridbagtemp.gridwidth = 6;
		
		JLabel temp = new JLabel(name);
		temp.setFont(new Font(null,Font.PLAIN, 9));
		paneltemp.add(temp);
		panel.add(paneltemp,gridbagtemp);
	}
}
