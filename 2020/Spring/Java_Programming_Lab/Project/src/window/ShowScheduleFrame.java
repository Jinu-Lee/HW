package window;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import scheduler.FullDaySchedule;
import scheduler.NormalSchedule;
import scheduler.Schedule;

public class ShowScheduleFrame extends JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = -5729594962921484723L;
	private JButton DelButton;
	private JTextField NameField;
	private JTextField StartTimeField;
	private JTextField EndTimeField;
	private JTextField CalendarField;
	private JTextField RepeatField;
	private JTextField MemoField;
	private JCheckBox OverlapCheckBox, ImportantCheckBox, FullDayCheckBox;
	private ArrayList<Schedule> schedules;
	private int index;
	private String name;

	public JButton getDelButton() {
		return DelButton;
	}

	public String getSeletedScheduleName() {
		return schedules.get(index).getName();
	}

	public void loadSchedules() {
		if (index < 0 || index >= schedules.size()) {
			return;
		}
		Schedule s = schedules.get(index);
		if (s instanceof FullDaySchedule) {
			FullDaySchedule fds = (FullDaySchedule) s;
			StartTimeField.setText(fds.getTime().toString());
			EndTimeField.setText("");
			FullDayCheckBox.doClick();
		} else {
			NormalSchedule ns = (NormalSchedule) s;
			StartTimeField.setText(ns.getStartTime().toString());
			EndTimeField.setText(ns.getEndTime().toString());
			if (ns.getCanBeOverlapped()) {
				OverlapCheckBox.doClick();
			}
		}
		NameField.setText(s.getName());
		CalendarField.setText(name);
		MemoField.setText(s.getMemo());
		if (s.getIsImportant()) {
			ImportantCheckBox.doClick();
		}
		switch(s.getRepeatType()) {
			case 0:
				RepeatField.setText("None");
				break;
			case 1:
				RepeatField.setText("Every year");
				break;
			case 2:
				RepeatField.setText("Every month");
				break;
			case 3:
				RepeatField.setText("Every week");
		}
		NameField.setEditable(false);
		StartTimeField.setEditable(false);
		EndTimeField.setEditable(false);
		CalendarField.setEditable(false);
		RepeatField.setEditable(false);
		MemoField.setEditable(false);
		OverlapCheckBox.setEnabled(false);
		ImportantCheckBox.setEnabled(false);
		FullDayCheckBox.setEnabled(false);
	}
	
	public ShowScheduleFrame(ArrayList<Schedule> schedules, String name) {
		this.schedules = schedules;
		index = 0;
		this.name = name;
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getContentPane().setLayout(new BorderLayout(0, 0));
		
		JPanel ButtonPanel = new JPanel();
		getContentPane().add(ButtonPanel, BorderLayout.NORTH);
		ButtonPanel.setLayout(new GridLayout(0, 3, 10, 0));
		
		JButton PrevButton = new JButton("<");
		PrevButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (index == 0) {
					return;
				}
				index--;
				loadSchedules();
			}
		});
		ButtonPanel.add(PrevButton);

		DelButton = new JButton("Delete");
		ButtonPanel.add(DelButton);
		
		JButton NextButton = new JButton(">");
		NextButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (index == schedules.size() - 1) {
					return;
				}
				index++;
				loadSchedules();
			}
		});
		ButtonPanel.add(NextButton);

		JPanel SchedulePanel = new JPanel();
		getContentPane().add(SchedulePanel, BorderLayout.CENTER);
		GridBagLayout gbl_SchedulePanel = new GridBagLayout();
		gbl_SchedulePanel.columnWidths = new int[] {50, 100, 50, 200, 100};
		gbl_SchedulePanel.rowHeights = new int[] {50, 50, 50, 50, 50, 50, 50, 50, 100, 50};
		gbl_SchedulePanel.columnWeights = new double[]{0, 0, 0, 0};
		gbl_SchedulePanel.rowWeights = new double[] {0, 0, 0, 0, 0, 0, 0, 0, 0};
		SchedulePanel.setLayout(gbl_SchedulePanel);
			
		JLabel NameLbl = new JLabel("Name");
		NameLbl.setFont(new Font(null, Font.PLAIN, 18));
		NameLbl.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_NameLbl = new GridBagConstraints();
		gbc_NameLbl.fill = GridBagConstraints.BOTH;
		gbc_NameLbl.insets = new Insets(0, 0, 5, 5);
		gbc_NameLbl.gridx = 1;
		gbc_NameLbl.gridy = 0;
		SchedulePanel.add(NameLbl, gbc_NameLbl);
			
		NameField = new JTextField();
		GridBagConstraints gbc_NameField = new GridBagConstraints();
		gbc_NameField.fill = GridBagConstraints.BOTH;
		gbc_NameField.insets = new Insets(0, 0, 5, 5);
		gbc_NameField.gridx = 3;
		gbc_NameField.gridy = 0;
		SchedulePanel.add(NameField, gbc_NameField);
		NameField.setColumns(10);
		
		JLabel FDLbl = new JLabel("Full Day");
		FDLbl.setHorizontalAlignment(SwingConstants.CENTER);
		FDLbl.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_FDLbl = new GridBagConstraints();
		gbc_FDLbl.fill = GridBagConstraints.BOTH;
		gbc_FDLbl.insets = new Insets(0, 0, 5, 5);
		gbc_FDLbl.gridx = 1;
		gbc_FDLbl.gridy = 1;
		SchedulePanel.add(FDLbl, gbc_FDLbl);
		
		FullDayCheckBox = new JCheckBox("");
		FullDayCheckBox.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_FullDayCheckBox = new GridBagConstraints();
		gbc_FullDayCheckBox.insets = new Insets(0, 0, 5, 5);
		gbc_FullDayCheckBox.gridx = 3;
		gbc_FullDayCheckBox.gridy = 1;
		SchedulePanel.add(FullDayCheckBox, gbc_FullDayCheckBox);
		
		JLabel TimeLbl = new JLabel("Time");
		TimeLbl.setHorizontalAlignment(SwingConstants.CENTER);
		TimeLbl.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_TimeLbl = new GridBagConstraints();
		gbc_TimeLbl.fill = GridBagConstraints.BOTH;
		gbc_TimeLbl.insets = new Insets(0, 0, 5, 5);
		gbc_TimeLbl.gridx = 1;
		gbc_TimeLbl.gridy = 2;
		SchedulePanel.add(TimeLbl, gbc_TimeLbl);
		
		StartTimeField = new JTextField();
		GridBagConstraints gbc_StartTimeField = new GridBagConstraints();
		gbc_StartTimeField.insets = new Insets(0, 0, 5, 5);
		gbc_StartTimeField.fill = GridBagConstraints.BOTH;
		gbc_StartTimeField.gridx = 3;
		gbc_StartTimeField.gridy = 2;
		SchedulePanel.add(StartTimeField, gbc_StartTimeField);
		StartTimeField.setColumns(10);
		
		EndTimeField = new JTextField();
		GridBagConstraints gbc_EndTimeField = new GridBagConstraints();
		gbc_EndTimeField.insets = new Insets(0, 0, 5, 5);
		gbc_EndTimeField.fill = GridBagConstraints.BOTH;
		gbc_EndTimeField.gridx = 3;
		gbc_EndTimeField.gridy = 3;
		SchedulePanel.add(EndTimeField, gbc_EndTimeField);
		EndTimeField.setColumns(10);
		
		JLabel CalendarLbl = new JLabel("Calendar");
		CalendarLbl.setHorizontalAlignment(SwingConstants.CENTER);
		CalendarLbl.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_CalendarLbl = new GridBagConstraints();
		gbc_CalendarLbl.fill = GridBagConstraints.BOTH;
		gbc_CalendarLbl.insets = new Insets(0, 0, 5, 5);
		gbc_CalendarLbl.gridx = 1;
		gbc_CalendarLbl.gridy = 4;
		SchedulePanel.add(CalendarLbl, gbc_CalendarLbl);
		
		CalendarField = new JTextField();
		GridBagConstraints gbc_CalendarField = new GridBagConstraints();
		gbc_CalendarField.insets = new Insets(0, 0, 5, 5);
		gbc_CalendarField.fill = GridBagConstraints.BOTH;
		gbc_CalendarField.gridx = 3;
		gbc_CalendarField.gridy = 4;
		SchedulePanel.add(CalendarField, gbc_CalendarField);
		CalendarField.setColumns(10);
		
		JLabel OlLbl = new JLabel("Overlap");
		OlLbl.setFont(new Font(null, Font.PLAIN, 18));
		OlLbl.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_OlLbl = new GridBagConstraints();
		gbc_OlLbl.fill = GridBagConstraints.BOTH;
		gbc_OlLbl.insets = new Insets(0, 0, 5, 5);
		gbc_OlLbl.gridx = 1;
		gbc_OlLbl.gridy = 5;
		SchedulePanel.add(OlLbl, gbc_OlLbl);
		
		OverlapCheckBox = new JCheckBox("");
		GridBagConstraints gbc_OverlapCheckBox = new GridBagConstraints();
		gbc_OverlapCheckBox.insets = new Insets(0, 0, 5, 5);
		gbc_OverlapCheckBox.gridx = 3;
		gbc_OverlapCheckBox.gridy = 5;
		SchedulePanel.add(OverlapCheckBox, gbc_OverlapCheckBox);
		
		JLabel ImpLbl = new JLabel("Important");
		ImpLbl.setHorizontalAlignment(SwingConstants.CENTER);
		ImpLbl.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_ImpLbl = new GridBagConstraints();
		gbc_ImpLbl.fill = GridBagConstraints.BOTH;
		gbc_ImpLbl.insets = new Insets(0, 0, 5, 5);
		gbc_ImpLbl.gridx = 1;
		gbc_ImpLbl.gridy = 6;
		SchedulePanel.add(ImpLbl, gbc_ImpLbl);
		
		ImportantCheckBox = new JCheckBox("");
		GridBagConstraints gbc_ImportantCheckBox = new GridBagConstraints();
		gbc_ImportantCheckBox.insets = new Insets(0, 0, 5, 5);
		gbc_ImportantCheckBox.gridx = 3;
		gbc_ImportantCheckBox.gridy = 6;
		SchedulePanel.add(ImportantCheckBox, gbc_ImportantCheckBox);
		
		JLabel RepeatLbl = new JLabel("Repeat");
		RepeatLbl.setFont(new Font(null, Font.PLAIN, 18));
		RepeatLbl.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_RepeatLbl = new GridBagConstraints();
		gbc_RepeatLbl.fill = GridBagConstraints.BOTH;
		gbc_RepeatLbl.insets = new Insets(0, 0, 5, 5);
		gbc_RepeatLbl.gridx = 1;
		gbc_RepeatLbl.gridy = 7;
		SchedulePanel.add(RepeatLbl, gbc_RepeatLbl);
		
		RepeatField = new JTextField();
		GridBagConstraints gbc_RepeatField = new GridBagConstraints();
		gbc_RepeatField.insets = new Insets(0, 0, 5, 5);
		gbc_RepeatField.fill = GridBagConstraints.BOTH;
		gbc_RepeatField.gridx = 3;
		gbc_RepeatField.gridy = 7;
		SchedulePanel.add(RepeatField, gbc_RepeatField);
		RepeatField.setColumns(10);
		
		JLabel MemoLbl = new JLabel("Memo");
		MemoLbl.setHorizontalAlignment(SwingConstants.CENTER);
		MemoLbl.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_MemoLbl = new GridBagConstraints();
		gbc_MemoLbl.fill = GridBagConstraints.BOTH;
		gbc_MemoLbl.insets = new Insets(0, 0, 5, 5);
		gbc_MemoLbl.gridx = 1;
		gbc_MemoLbl.gridy = 8;
		SchedulePanel.add(MemoLbl, gbc_MemoLbl);
		
		MemoField = new JTextField();
		GridBagConstraints gbc_MemoField = new GridBagConstraints();
		gbc_MemoField.insets = new Insets(0, 0, 5, 5);
		gbc_MemoField.fill = GridBagConstraints.BOTH;
		gbc_MemoField.gridx = 3;
		gbc_MemoField.gridy = 8;
		SchedulePanel.add(MemoField, gbc_MemoField);
		MemoField.setColumns(10);

		pack();
		loadSchedules();
	}

}