<<<<<<< HEAD
package window;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JTextField;
import javax.swing.SpinnerModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import exception.AlreadyDefinedException;
import exception.NoNameEnteredException;
import scheduler.Calendar;
import scheduler.FullDaySchedule;
import scheduler.NormalSchedule;
import scheduler.Schedule;
import scheduler.Scheduler;

public class MainFrame extends JFrame {
	private static final long serialVersionUID = 26869752226168311L;

	private Scheduler scheduler = new Scheduler();
	private Color[] colors = { Color.MAGENTA, Color.PINK, Color.RED, Color.ORANGE, Color.YELLOW, Color.GREEN,
			Color.CYAN, Color.BLUE, Color.LIGHT_GRAY, Color.GRAY, Color.DARK_GRAY, Color.LIGHT_GRAY };
	private JComboBox<String> calendarComboBox;
	private JSpinner yearSpinner, monthSpinner;
	private ArrayList<JButton> WeekButton = new ArrayList<>();
	private ArrayList<JButton> DayButton = new ArrayList<>();
	private String name = "";
	private Socket socket;
	private Sender sender;
	private ReceiveThread receive;
	private String[] user_names;

	public MainFrame() {
		try {
			socket = new Socket("localhost", 5000);
		} catch (IOException e1) {
		}

		getContentPane().setLayout(new BorderLayout(0, 20));
		setTitle("Calendar");

		JPanel ButtonPanel = new JPanel();
		getContentPane().add(ButtonPanel, BorderLayout.NORTH);
		ButtonPanel.setBorder(BorderFactory.createEmptyBorder(20, 0, 0, 0));
		GridBagLayout gbl_ButtonPanel = new GridBagLayout();
		gbl_ButtonPanel.columnWidths = new int[] { 50, 150, 200, 150, 50, 50, 50, 50 };
		gbl_ButtonPanel.rowHeights = new int[] { 50, 20, 50 };
		gbl_ButtonPanel.columnWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		gbl_ButtonPanel.rowWeights = new double[] { 0.0, 0.0 };
		ButtonPanel.setLayout(gbl_ButtonPanel);

		calendarComboBox = new JComboBox<String>(scheduler.get_name());
		GridBagConstraints gbc_comboBox = new GridBagConstraints();
		gbc_comboBox.insets = new Insets(0, 0, 0, 5);
		gbc_comboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_comboBox.gridx = 1;
		gbc_comboBox.gridy = 2;
		calendarComboBox.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(calendarComboBox, gbc_comboBox);

		LocalDate today = LocalDate.now();
		SpinnerModel yearModel = new SpinnerNumberModel(today.getYear(), null, null, 1);
		yearSpinner = new JSpinner(yearModel);
		yearSpinner.setEditor(new JSpinner.NumberEditor(yearSpinner, "####"));
		yearSpinner.setFont(new Font(null, Font.PLAIN, 30));
		JSpinner.DefaultEditor yearSpinnerEditor = (JSpinner.DefaultEditor) yearSpinner.getEditor();
		yearSpinnerEditor.getTextField().setHorizontalAlignment(JTextField.CENTER);
		yearSpinnerEditor.getTextField().setEditable(false);
		GridBagConstraints gbc_yearSpinner = new GridBagConstraints();
		gbc_yearSpinner.insets = new Insets(0, 0, 0, 5);
		gbc_yearSpinner.fill = GridBagConstraints.BOTH;
		gbc_yearSpinner.gridx = 2;
		gbc_yearSpinner.gridy = 0;
		yearSpinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(yearSpinner, gbc_yearSpinner);

		SpinnerModel monthModel = new SpinnerNumberModel(today.getMonthValue(), 1, 12, 1);
		monthSpinner = new JSpinner(monthModel);
		monthSpinner.setEditor(new JSpinner.NumberEditor(monthSpinner, "00"));
		monthSpinner.setFont(new Font(null, Font.PLAIN, 30));
		JSpinner.DefaultEditor monthSpinnerEditor = (JSpinner.DefaultEditor) monthSpinner.getEditor();
		monthSpinnerEditor.getTextField().setHorizontalAlignment(JTextField.CENTER);
		monthSpinnerEditor.getTextField().setEditable(false);
		GridBagConstraints gbc_monthSpinner = new GridBagConstraints();
		gbc_monthSpinner.insets = new Insets(0, 0, 0, 5);
		gbc_monthSpinner.fill = GridBagConstraints.BOTH;
		gbc_monthSpinner.gridx = 3;
		gbc_monthSpinner.gridy = 0;
		monthSpinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(monthSpinner, gbc_monthSpinner);

		JButton ShareButton = new JButton("Share");
		GridBagConstraints gbc_ShareButton = new GridBagConstraints();
		gbc_ShareButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ShareButton.insets = new Insets(0, 0, 0, 5);
		gbc_ShareButton.gridx = 4;
		gbc_ShareButton.gridy = 2;
		ButtonPanel.add(ShareButton, gbc_ShareButton);
		ShareButton.addActionListener(new ShareButtonClickListener());

		JButton ExportButton = new JButton("Export");
		GridBagConstraints gbc_ExportButton = new GridBagConstraints();
		gbc_ExportButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ExportButton.insets = new Insets(0, 0, 0, 5);
		gbc_ExportButton.gridx = 5;
		gbc_ExportButton.gridy = 2;
		ButtonPanel.add(ExportButton, gbc_ExportButton);
		ExportButton.addActionListener(new ExportButtonClickListener());

		JButton ImportButton = new JButton("Import");
		GridBagConstraints gbc_ImportButton = new GridBagConstraints();
		gbc_ImportButton.insets = new Insets(0, 0, 0, 5);
		gbc_ImportButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ImportButton.gridx = 6;
		gbc_ImportButton.gridy = 2;
		ImportButton.addActionListener(new ImportButtonClickListener());
		ButtonPanel.add(ImportButton, gbc_ImportButton);

		JButton AddButton = new JButton("Add");
		GridBagConstraints gbc_AddButton = new GridBagConstraints();
		gbc_AddButton.insets = new Insets(0, 0, 0, 5);
		gbc_AddButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_AddButton.gridx = 7;
		gbc_AddButton.gridy = 2;
		AddButton.addActionListener(new AddButtonClickListener());
		ButtonPanel.add(AddButton, gbc_AddButton);

		JPanel CalendarPanel = new JPanel();
		getContentPane().add(CalendarPanel, BorderLayout.CENTER);
		GridBagLayout gbl_CalendarPanel = new GridBagLayout();
		gbl_CalendarPanel.columnWidths = new int[] { 50, 100, 100, 100, 100, 100, 100, 100 };
		gbl_CalendarPanel.rowHeights = new int[] { 50, 100, 100, 100, 100, 100, 100 };
		gbl_CalendarPanel.columnWeights = new double[] { 0.0 };
		gbl_CalendarPanel.rowWeights = new double[] { 0.0 };
		CalendarPanel.setLayout(gbl_CalendarPanel);

		String[] Day = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
		for (int i = 0; i < 7; i++) {
			JLabel Label = new JLabel(Day[i]);
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 0, 5, 5);
			gbc.gridx = i + 1;
			gbc.gridy = 0;
			Label.setHorizontalAlignment(JLabel.CENTER);
			Label.setVerticalAlignment(JLabel.CENTER);
			CalendarPanel.add(Label, gbc);
		}

		for (int i = 1; i <= 6; i++) {
			JButton Button = new JButton("" + i);
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 0, 5, 5);
			gbc.gridx = 0;
			gbc.gridy = i;
			Button.setHorizontalAlignment(JLabel.CENTER);
			Button.setVerticalAlignment(JLabel.CENTER);
			Button.addActionListener(new WeekButtonClickListener());
			WeekButton.add(Button);
			CalendarPanel.add(Button, gbc);
		}

		for (int i = 1; i <= 6; i++) {
			for (int j = 1; j <= 7; j++) {
				JButton Button = new JButton("");
				GridBagConstraints gbc = new GridBagConstraints();
				gbc.fill = GridBagConstraints.BOTH;
				gbc.insets = new Insets(0, 0, 5, 5);
				gbc.gridx = j;
				gbc.gridy = i;
				Button.addActionListener(new DayButtonClickListener());
				DayButton.add(Button);
				CalendarPanel.add(Button, gbc);
			}
		}
		loadCalendar();

		while (true) {
			try {
				read_name();
				sender = new Sender();
				break;
			} catch (Exception e) {
				JOptionPane.showMessageDialog(this, e.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
			}
		}

		JLabel NameLabel = new JLabel("User: " + name);
		NameLabel.setHorizontalAlignment(SwingConstants.CENTER);
		NameLabel.setFont(new Font(null, Font.PLAIN, 16));
		GridBagConstraints gbc_NameLabel = new GridBagConstraints();
		gbc_NameLabel.insets = new Insets(0, 0, 5, 5);
		gbc_NameLabel.gridx = 1;
		gbc_NameLabel.gridy = 1;
		ButtonPanel.add(NameLabel, gbc_NameLabel);
		pack();
		setVisible(true);

		receive = new ReceiveThread();
		receive.start();
	}

	class WeekButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
			String selectedcal = calendarComboBox.getSelectedItem().toString();
			ArrayList<ArrayList<Schedule>> schedule = new ArrayList<>();
			if (selectedcal.equals("default")) {
				for (int k = 0; k < scheduler.get_name().length; k++) {
					Calendar cal = scheduler.get_calendar((scheduler.get_name()[k]));
					JButton button = (JButton) e.getSource();
					int i = Integer.parseInt(button.getText()) - 1;

					for (int j = 0; j < 7; j++) {
						schedule.add(new ArrayList<Schedule>());
					}

					for (int j = 0; j < 7; j++) {
						JButton db = DayButton.get(7 * i + j);
						if (db.getText().isEmpty()) {
							continue;
						}
						String day = db.getText().substring(10, 12);
						if (day.charAt(1) == '<') {
							day = day.substring(0, 1);
						}

						LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
						schedule.get(j).addAll(cal.read_schedule(today));
					}
				}
			} else {
				Calendar cal = scheduler.get_calendar(selectedcal);
				JButton button = (JButton) e.getSource();
				int i = Integer.parseInt(button.getText()) - 1;

				for (int j = 0; j < 7; j++) {
					JButton db = DayButton.get(7 * i + j);
					if (db.getText().isEmpty()) {
						schedule.add(new ArrayList<Schedule>());
						continue;
					}
					String day = db.getText().substring(10, 12);
					if (day.charAt(1) == '<') {
						day = day.substring(0, 1);
					}
					LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
					schedule.add(cal.read_schedule(today));
				}
			}
			WeeklyScheduleFrame wsf = new WeeklyScheduleFrame(schedule);
			wsf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			wsf.setVisible(true);

		}
	}

	class DayButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			JButton button = (JButton) e.getSource();

			if (button.getText().isEmpty()) {
				return;
			}
			String day = button.getText().substring(10, 12);
			if (day.charAt(1) == '<') {
				day = day.substring(0, 1);
			}

			LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
			ArrayList<Schedule> s = cal.read_schedule(today);
			if (s.size() == 0) {
				return;
			}

			ShowScheduleFrame ssf = new ShowScheduleFrame(s, cal.getName());
			ssf.getDelButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					cal.remove_schedule(ssf.getSeletedScheduleName());
					ssf.setVisible(false);
					ssf.dispose();
					loadCalendar();
				}
			});
			ssf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			ssf.setVisible(true);
		}
	}

	class AddButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			EditCalendarAndSchduleFrame ecsf = new EditCalendarAndSchduleFrame();
			ecsf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			ecsf.getAddScheduleButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					String[] names = scheduler.get_name();
					AddScheduleFrame asf = new AddScheduleFrame(names, calendarComboBox.getSelectedItem().toString());
					asf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
					asf.getSubmitButton().addActionListener(new ActionListener() {
						public void actionPerformed(ActionEvent e) {
							Calendar c = scheduler.get_calendar(asf.getCalendarName());
							JCheckBox[] cb = asf.getCheckBox();
							try {
								c.add_schedule(asf.getName(), asf.getTime(false), asf.getTime(true), cb[3].isSelected(),
										cb[2].isSelected(), asf.getMemo(), asf.getRepeat(), cb[1].isSelected(),
										c.getColor());
								asf.setVisible(false);
								asf.dispose();
								loadCalendar();
							} catch (RuntimeException e1) {
								JOptionPane.showMessageDialog(asf, e1.getMessage(), "Exception",
										JOptionPane.ERROR_MESSAGE);
							}
						}
					});
					asf.setVisible(true);
				}
			});
			ecsf.getSubmitButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					try {
						String[] textField = ecsf.getTextField();
						int selected_col = Integer.parseInt(textField[2]);
						Color calcolor;
						if (0 <= selected_col && selected_col <= 10) {
							calcolor = colors[selected_col];
						} else {
							calcolor = colors[11];
						}
						if (textField[0].length() != 0 && textField[1].length() == 0) {
							scheduler.add_calendar(textField[0], calcolor);
							calendarComboBox.addItem(textField[0]);
						} else if (textField[0].length() == 0 && textField[1].length() != 0) {
							scheduler.remove_calendar(textField[1]);
							calendarComboBox.removeItem(textField[1]);
						}
						ecsf.setVisible(false);
						ecsf.dispose();
					} catch (RuntimeException e1) {
						JOptionPane.showMessageDialog(ecsf, e1.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
					}
				}
			});
			ecsf.setVisible(true);
		}
	}

	class ShareButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			ShareFrame sf = new ShareFrame(scheduler, user_names, name);
			sf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			sf.getSendBtn().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					String[] selected = sf.getComboBox();
					Calendar cal = scheduler.get_calendar(selected[1]);
					Schedule sch = cal.get_Schedule(selected[2]);
					try {
						sender.sendSchedule(sch, selected[0]);
					} catch (Exception e2) {
					}

					sf.setVisible(false);
					sf.dispose();
				}
			});
			sf.setVisible(true);
		}
	}

	class ExportButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			OutputStream fos = null;
			try {
				fos = new FileOutputStream(calendarComboBox.getSelectedItem().toString() + "Calendar.txt");
			} catch (FileNotFoundException e1) {
			}
			OutputStreamWriter osw = new OutputStreamWriter(fos);
			BufferedWriter bw = new BufferedWriter(osw);

			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			ArrayList<Schedule> schedules = cal.get_Schedules();
			try {
				for (Schedule s : schedules) {
					bw.write(s.getName() + ",");
					if (s.getIsImportant()) {
						bw.write("True,");
					} else {
						bw.write("False,");
					}
					bw.write(s.getMemo() + ",");
					bw.write(s.getRepeatType() + ",");
					bw.write(s.getColor().getRGB() + ",");
					if (s instanceof FullDaySchedule) {
						LocalDate time = ((FullDaySchedule) s).getTime();
						int[] start = { time.getYear(), time.getMonthValue(), time.getDayOfMonth() };
						bw.write("FullDay,");
						for (int i = 0; i < 3; i++) {
							bw.write(start[i] + ",");
						}
					} else {
						LocalDateTime start = ((NormalSchedule) s).getStartTime();
						LocalDateTime end = ((NormalSchedule) s).getEndTime();
						int[] start_time = { start.getYear(), start.getMonthValue(), start.getDayOfMonth(),
								start.getHour(), start.getMinute() };
						int[] end_time = { end.getYear(), end.getMonthValue(), end.getDayOfMonth(), end.getHour(),
								end.getMinute() };
						bw.write("Normal" + ",");
						for (int i = 0; i < 5; i++) {
							bw.write(start_time[i] + ",");
							bw.write(end_time[i] + ",");
						}
						if (((NormalSchedule) s).getCanBeOverlapped()) {
							bw.write("True");
						} else {
							bw.write("False");
						}
					}
					bw.newLine();
				}
				bw.flush();
			} catch (Exception e2) {
			}
			try {
				bw.close(); osw.close(); fos.close();
			} catch (Exception e2) {
			}
		}
	}

	class ImportButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			InputStream fis = null;
			try {
				fis = new FileInputStream(calendarComboBox.getSelectedItem().toString() + "Calendar.txt");
			} catch (FileNotFoundException e1) {
				return;
			}
        	InputStreamReader isr = new InputStreamReader(fis);
        	BufferedReader br = new BufferedReader(isr);

			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			try {
				String s = new String();
        		while((s = br.readLine()) != null) {
					String[] input = s.split(",");
					String name = input[0];
					boolean isImp;
					if (input[1] == "True") {
						isImp = true;
					} else {
						isImp = false;
					}
					String memo = input[2];
					int RepeatType = Integer.parseInt(input[3]);
					Color color = new Color(Integer.parseInt(input[4]));
					int[] start = null;
					int[] end = null;
					boolean canBeOverlapped = false;
					if (input[5] == "FullDay") {
						start = new int[3];
						for (int i = 0; i < 3; i++) {
							start[i] = Integer.parseInt(input[6 + i]);
						}
					} else {
						start = new int[5];
						end = new int[5];
						for (int i = 0; i < 5; i++) {
							start[i] = Integer.parseInt(input[6 + 2 * i]);
							end[i] = Integer.parseInt(input[7 + 2 * i]);
						}
						if (input[16] == "True") {
							canBeOverlapped = true;
						} else {
							canBeOverlapped = false;
						}
					}
					try {
						if (input[5] == "FullDay") {
							cal.add_schedule(name, start, start, isImp, true, memo, RepeatType, true,
									color);
						} else {
							cal.add_schedule(name, start, end, isImp, canBeOverlapped, memo, RepeatType,
									false, color);
						}
					} catch (Exception e1) {
						JOptionPane.showMessageDialog(null, e1.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
					}
        		}
			} catch (Exception e2) {
			}
			try {
				fis.close(); isr.close(); br.close();
			} catch (Exception e2) {
			}
			loadCalendar();
		}
	}

	public void loadCalendar() {
		int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
		String selectedItem = calendarComboBox.getSelectedItem().toString();
		if (selectedItem.equals("default")) {
			LocalDate date = LocalDate.of(year, month, 1);
			int start_pos = date.getDayOfWeek().getValue();

			for (JButton b : DayButton) {
				b.setText("");
			}

			start_pos %= 7;

			for (int i = 0; i < date.lengthOfMonth(); i++) {
				JButton button = DayButton.get(start_pos + i);
				button.setHorizontalAlignment(SwingConstants.LEFT);
				button.setVerticalAlignment(SwingConstants.TOP);
				ArrayList<Schedule> schedules = new ArrayList<Schedule>();
				for (int j = 0; j < scheduler.get_name().length; j++) {
					schedules.addAll(scheduler.get_calendar(scheduler.get_name()[j]).read_schedule(date.plusDays(i)));
				}
				if (schedules.size() == 0) {
					button.setText("<html><h2>" + (i + 1) + "</h2></html>");
				} else if (schedules.size() == 1) {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " thing</html>");
				} else {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " things</html>");
				}
			}
		} else {
			Calendar cal = scheduler.get_calendar(selectedItem);
			LocalDate date = LocalDate.of(year, month, 1);
			int start_pos = date.getDayOfWeek().getValue();

			for (JButton b : DayButton) {
				b.setText("");
			}

			start_pos %= 7;

			for (int i = 0; i < date.lengthOfMonth(); i++) {
				JButton button = DayButton.get(start_pos + i);
				button.setHorizontalAlignment(SwingConstants.LEFT);
				button.setVerticalAlignment(SwingConstants.TOP);
				ArrayList<Schedule> schedules = cal.read_schedule(date.plusDays(i));
				if (schedules.size() == 0) {
					button.setText("<html><h2>" + (i + 1) + "</h2></html>");
				} else if (schedules.size() == 1) {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " thing</html>");
				} else {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " things</html>");
				}
			}
		}
	}

	public void read_name() {
		name = JOptionPane.showInputDialog("Enter your name.");
		if (name.length() == 0) {
			throw new NoNameEnteredException("No name entered.");
		}
	}

	class Sender {
		DataOutputStream dos;
		DataInputStream dis;

		public Sender() throws IOException {
			dos = new DataOutputStream(socket.getOutputStream());
			dis = new DataInputStream(socket.getInputStream());

			dos.writeUTF(name);
			dos.flush();
			boolean isValid = dis.readBoolean();
			if (!isValid) {
				throw new AlreadyDefinedException("User names cannot be duplicated.");
			}
		}

		public void sendSchedule(Schedule s, String name) {
			try {
				dos.writeUTF(name);
				dos.writeUTF(s.getName());
				dos.writeBoolean(s.getIsImportant());
				dos.writeUTF(s.getMemo());
				dos.writeInt(s.getRepeatType());
				dos.writeInt(s.getColor().getRGB());

				if (s instanceof FullDaySchedule) {
					LocalDate time = ((FullDaySchedule) s).getTime();
					int[] start = { time.getYear(), time.getMonthValue(), time.getDayOfMonth() };
					dos.writeBoolean(true);
					for (int i = 0; i < 3; i++) {
						dos.writeInt(start[i]);
					}
				} else {
					LocalDateTime start = ((NormalSchedule) s).getStartTime();
					LocalDateTime end = ((NormalSchedule) s).getEndTime();
					int[] start_time = { start.getYear(), start.getMonthValue(), start.getDayOfMonth(), start.getHour(),
							start.getMinute() };
					int[] end_time = { end.getYear(), end.getMonthValue(), end.getDayOfMonth(), end.getHour(),
							end.getMinute() };
					dos.writeBoolean(false);
					for (int i = 0; i < 5; i++) {
						dos.writeInt(start_time[i]);
						dos.writeInt(end_time[i]);
					}
					dos.writeBoolean(((NormalSchedule) s).getCanBeOverlapped());
				}
				dos.flush();
			} catch (Exception e) {

			}
		}
	}

	class ReceiveThread extends Thread {
		DataInputStream dis;
		DataOutputStream dos;
		Boolean isImp;
		String memo;
		int RepeatType;
		boolean isFullDay;
		int[] start, end;
		Color color;
		boolean canBeOverlapped;

		public ReceiveThread() {
			try {
				dis = new DataInputStream((socket.getInputStream()));
				dos = new DataOutputStream(socket.getOutputStream());
			} catch (IOException e) {
			}
		}

		public void run() {
			while (true) {
				try {
					int mode = dis.readInt();
					if (mode == 0) {
						int size = dis.readInt();
						user_names = new String[size];
						for (int i = 0; i < size; i++) {
							user_names[i] = dis.readUTF();
						}
					} else if (mode == 1) {
						name = dis.readUTF();
						isImp = dis.readBoolean();
						memo = dis.readUTF();
						RepeatType = dis.readInt();
						color = new Color(dis.readInt());
						isFullDay = dis.readBoolean();
						if (isFullDay) {
							start = new int[3];
							for (int i = 0; i < 3; i++) {
								start[i] = dis.readInt();
							}
						} else {
							start = new int[5];
							end = new int[5];
							for (int i = 0; i < 5; i++) {
								start[i] = dis.readInt();
								end[i] = dis.readInt();
							}
							canBeOverlapped = dis.readBoolean();
						}
						ReceiveFrame rf = new ReceiveFrame(scheduler.get_name());
						rf.setDefaultCloseOperation(2);
						rf.getAddBtn().addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent e) {
								String selected_name = rf.getCalComboBox();
								Calendar cal = scheduler.get_calendar(selected_name);
								try {
									if (isFullDay) {
										cal.add_schedule(name, start, start, isImp, true, memo, RepeatType, true,
												cal.getColor());
									} else {
										cal.add_schedule(name, start, end, isImp, canBeOverlapped, memo, RepeatType,
												false, cal.getColor());
									}
								} catch (Exception e1) {
									JOptionPane.showMessageDialog(rf, e1.getMessage(), "Exception",
											JOptionPane.ERROR_MESSAGE);
								}
								rf.setVisible(false);
								rf.dispose();
								loadCalendar();
							}
						});
						rf.setVisible(true);
					}
				} catch (Exception e) {
				}
			}
		}
	}
}
=======
package window;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JTextField;
import javax.swing.SpinnerModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import exception.AlreadyDefinedException;
import exception.NoNameEnteredException;
import scheduler.Calendar;
import scheduler.FullDaySchedule;
import scheduler.NormalSchedule;
import scheduler.Schedule;
import scheduler.Scheduler;

public class MainFrame extends JFrame {
	private static final long serialVersionUID = 26869752226168311L;

	private Scheduler scheduler = new Scheduler();
	private Color[] colors = { Color.MAGENTA, Color.PINK, Color.RED, Color.ORANGE, Color.YELLOW, Color.GREEN,
			Color.CYAN, Color.BLUE, Color.LIGHT_GRAY, Color.GRAY, Color.DARK_GRAY, Color.LIGHT_GRAY };
	private JComboBox<String> calendarComboBox;
	private JSpinner yearSpinner, monthSpinner;
	private ArrayList<JButton> WeekButton = new ArrayList<>();
	private ArrayList<JButton> DayButton = new ArrayList<>();
	private String name = "";
	private Socket socket;
	private Sender sender;
	private ReceiveThread receive;
	private String[] user_names;

	public MainFrame() {
		try {
			socket = new Socket("localhost", 5000);
		} catch (IOException e1) {
		}

		getContentPane().setLayout(new BorderLayout(0, 20));
		setTitle("Calendar");

		JPanel ButtonPanel = new JPanel();
		getContentPane().add(ButtonPanel, BorderLayout.NORTH);
		ButtonPanel.setBorder(BorderFactory.createEmptyBorder(20, 0, 0, 0));
		GridBagLayout gbl_ButtonPanel = new GridBagLayout();
		gbl_ButtonPanel.columnWidths = new int[] { 50, 150, 200, 150, 50, 50, 50, 50 };
		gbl_ButtonPanel.rowHeights = new int[] { 50, 20, 50 };
		gbl_ButtonPanel.columnWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		gbl_ButtonPanel.rowWeights = new double[] { 0.0, 0.0 };
		ButtonPanel.setLayout(gbl_ButtonPanel);

		calendarComboBox = new JComboBox<String>(scheduler.get_name());
		GridBagConstraints gbc_comboBox = new GridBagConstraints();
		gbc_comboBox.insets = new Insets(0, 0, 0, 5);
		gbc_comboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_comboBox.gridx = 1;
		gbc_comboBox.gridy = 2;
		calendarComboBox.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(calendarComboBox, gbc_comboBox);

		LocalDate today = LocalDate.now();
		SpinnerModel yearModel = new SpinnerNumberModel(today.getYear(), null, null, 1);
		yearSpinner = new JSpinner(yearModel);
		yearSpinner.setEditor(new JSpinner.NumberEditor(yearSpinner, "####"));
		yearSpinner.setFont(new Font(null, Font.PLAIN, 30));
		JSpinner.DefaultEditor yearSpinnerEditor = (JSpinner.DefaultEditor) yearSpinner.getEditor();
		yearSpinnerEditor.getTextField().setHorizontalAlignment(JTextField.CENTER);
		yearSpinnerEditor.getTextField().setEditable(false);
		GridBagConstraints gbc_yearSpinner = new GridBagConstraints();
		gbc_yearSpinner.insets = new Insets(0, 0, 0, 5);
		gbc_yearSpinner.fill = GridBagConstraints.BOTH;
		gbc_yearSpinner.gridx = 2;
		gbc_yearSpinner.gridy = 0;
		yearSpinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(yearSpinner, gbc_yearSpinner);

		SpinnerModel monthModel = new SpinnerNumberModel(today.getMonthValue(), 1, 12, 1);
		monthSpinner = new JSpinner(monthModel);
		monthSpinner.setEditor(new JSpinner.NumberEditor(monthSpinner, "00"));
		monthSpinner.setFont(new Font(null, Font.PLAIN, 30));
		JSpinner.DefaultEditor monthSpinnerEditor = (JSpinner.DefaultEditor) monthSpinner.getEditor();
		monthSpinnerEditor.getTextField().setHorizontalAlignment(JTextField.CENTER);
		monthSpinnerEditor.getTextField().setEditable(false);
		GridBagConstraints gbc_monthSpinner = new GridBagConstraints();
		gbc_monthSpinner.insets = new Insets(0, 0, 0, 5);
		gbc_monthSpinner.fill = GridBagConstraints.BOTH;
		gbc_monthSpinner.gridx = 3;
		gbc_monthSpinner.gridy = 0;
		monthSpinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				loadCalendar();
			}
		});
		ButtonPanel.add(monthSpinner, gbc_monthSpinner);

		JButton ShareButton = new JButton("Share");
		GridBagConstraints gbc_ShareButton = new GridBagConstraints();
		gbc_ShareButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ShareButton.insets = new Insets(0, 0, 0, 5);
		gbc_ShareButton.gridx = 4;
		gbc_ShareButton.gridy = 2;
		ButtonPanel.add(ShareButton, gbc_ShareButton);
		ShareButton.addActionListener(new ShareButtonClickListener());

		JButton ExportButton = new JButton("Export");
		GridBagConstraints gbc_ExportButton = new GridBagConstraints();
		gbc_ExportButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ExportButton.insets = new Insets(0, 0, 0, 5);
		gbc_ExportButton.gridx = 5;
		gbc_ExportButton.gridy = 2;
		ButtonPanel.add(ExportButton, gbc_ExportButton);
		ExportButton.addActionListener(new ExportButtonClickListener());

		JButton ImportButton = new JButton("Import");
		GridBagConstraints gbc_ImportButton = new GridBagConstraints();
		gbc_ImportButton.insets = new Insets(0, 0, 0, 5);
		gbc_ImportButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_ImportButton.gridx = 6;
		gbc_ImportButton.gridy = 2;
		ImportButton.addActionListener(new ImportButtonClickListener());
		ButtonPanel.add(ImportButton, gbc_ImportButton);

		JButton AddButton = new JButton("Add");
		GridBagConstraints gbc_AddButton = new GridBagConstraints();
		gbc_AddButton.insets = new Insets(0, 0, 0, 5);
		gbc_AddButton.fill = GridBagConstraints.HORIZONTAL;
		gbc_AddButton.gridx = 7;
		gbc_AddButton.gridy = 2;
		AddButton.addActionListener(new AddButtonClickListener());
		ButtonPanel.add(AddButton, gbc_AddButton);

		JPanel CalendarPanel = new JPanel();
		getContentPane().add(CalendarPanel, BorderLayout.CENTER);
		GridBagLayout gbl_CalendarPanel = new GridBagLayout();
		gbl_CalendarPanel.columnWidths = new int[] { 50, 100, 100, 100, 100, 100, 100, 100 };
		gbl_CalendarPanel.rowHeights = new int[] { 50, 100, 100, 100, 100, 100, 100 };
		gbl_CalendarPanel.columnWeights = new double[] { 0.0 };
		gbl_CalendarPanel.rowWeights = new double[] { 0.0 };
		CalendarPanel.setLayout(gbl_CalendarPanel);

		String[] Day = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
		for (int i = 0; i < 7; i++) {
			JLabel Label = new JLabel(Day[i]);
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 0, 5, 5);
			gbc.gridx = i + 1;
			gbc.gridy = 0;
			Label.setHorizontalAlignment(JLabel.CENTER);
			Label.setVerticalAlignment(JLabel.CENTER);
			CalendarPanel.add(Label, gbc);
		}

		for (int i = 1; i <= 6; i++) {
			JButton Button = new JButton("" + i);
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 0, 5, 5);
			gbc.gridx = 0;
			gbc.gridy = i;
			Button.setHorizontalAlignment(JLabel.CENTER);
			Button.setVerticalAlignment(JLabel.CENTER);
			Button.addActionListener(new WeekButtonClickListener());
			WeekButton.add(Button);
			CalendarPanel.add(Button, gbc);
		}

		for (int i = 1; i <= 6; i++) {
			for (int j = 1; j <= 7; j++) {
				JButton Button = new JButton("");
				GridBagConstraints gbc = new GridBagConstraints();
				gbc.fill = GridBagConstraints.BOTH;
				gbc.insets = new Insets(0, 0, 5, 5);
				gbc.gridx = j;
				gbc.gridy = i;
				Button.addActionListener(new DayButtonClickListener());
				DayButton.add(Button);
				CalendarPanel.add(Button, gbc);
			}
		}
		loadCalendar();

		while (true) {
			try {
				read_name();
				sender = new Sender();
				break;
			} catch (Exception e) {
				JOptionPane.showMessageDialog(this, e.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
			}
		}

		JLabel NameLabel = new JLabel("User: " + name);
		NameLabel.setHorizontalAlignment(SwingConstants.CENTER);
		NameLabel.setFont(new Font(null, Font.PLAIN, 16));
		GridBagConstraints gbc_NameLabel = new GridBagConstraints();
		gbc_NameLabel.insets = new Insets(0, 0, 5, 5);
		gbc_NameLabel.gridx = 1;
		gbc_NameLabel.gridy = 1;
		ButtonPanel.add(NameLabel, gbc_NameLabel);
		pack();
		setVisible(true);

		receive = new ReceiveThread();
		receive.start();
	}

	class WeekButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
			String selectedcal = calendarComboBox.getSelectedItem().toString();
			ArrayList<ArrayList<Schedule>> schedule = new ArrayList<>();
			if (selectedcal.equals("default")) {
				for (int k = 0; k < scheduler.get_name().length; k++) {
					Calendar cal = scheduler.get_calendar((scheduler.get_name()[k]));
					JButton button = (JButton) e.getSource();
					int i = Integer.parseInt(button.getText()) - 1;

					for (int j = 0; j < 7; j++) {
						schedule.add(new ArrayList<Schedule>());
					}

					for (int j = 0; j < 7; j++) {
						JButton db = DayButton.get(7 * i + j);
						if (db.getText().isEmpty()) {
							continue;
						}
						String day = db.getText().substring(10, 12);
						if (day.charAt(1) == '<') {
							day = day.substring(0, 1);
						}

						LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
						schedule.get(j).addAll(cal.read_schedule(today));
					}
				}
			} else {
				Calendar cal = scheduler.get_calendar(selectedcal);
				JButton button = (JButton) e.getSource();
				int i = Integer.parseInt(button.getText()) - 1;

				for (int j = 0; j < 7; j++) {
					JButton db = DayButton.get(7 * i + j);
					if (db.getText().isEmpty()) {
						schedule.add(new ArrayList<Schedule>());
						continue;
					}
					String day = db.getText().substring(10, 12);
					if (day.charAt(1) == '<') {
						day = day.substring(0, 1);
					}
					LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
					schedule.add(cal.read_schedule(today));
				}
			}
			WeeklyScheduleFrame wsf = new WeeklyScheduleFrame(schedule);
			wsf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			wsf.setVisible(true);

		}
	}

	class DayButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			JButton button = (JButton) e.getSource();

			if (button.getText().isEmpty()) {
				return;
			}
			String day = button.getText().substring(10, 12);
			if (day.charAt(1) == '<') {
				day = day.substring(0, 1);
			}

			LocalDate today = LocalDate.of(year, month, Integer.parseInt(day));
			ArrayList<Schedule> s = cal.read_schedule(today);
			if (s.size() == 0) {
				return;
			}

			ShowScheduleFrame ssf = new ShowScheduleFrame(s, cal.getName());
			ssf.getDelButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					cal.remove_schedule(ssf.getSeletedScheduleName());
					ssf.setVisible(false);
					ssf.dispose();
					loadCalendar();
				}
			});
			ssf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			ssf.setVisible(true);
		}
	}

	class AddButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			EditCalendarAndSchduleFrame ecsf = new EditCalendarAndSchduleFrame();
			ecsf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			ecsf.getAddScheduleButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					String[] names = scheduler.get_name();
					AddScheduleFrame asf = new AddScheduleFrame(names, calendarComboBox.getSelectedItem().toString());
					asf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
					asf.getSubmitButton().addActionListener(new ActionListener() {
						public void actionPerformed(ActionEvent e) {
							Calendar c = scheduler.get_calendar(asf.getCalendarName());
							JCheckBox[] cb = asf.getCheckBox();
							try {
								c.add_schedule(asf.getName(), asf.getTime(false), asf.getTime(true), cb[3].isSelected(),
										cb[2].isSelected(), asf.getMemo(), asf.getRepeat(), cb[1].isSelected(),
										c.getColor());
								asf.setVisible(false);
								asf.dispose();
								loadCalendar();
							} catch (RuntimeException e1) {
								JOptionPane.showMessageDialog(asf, e1.getMessage(), "Exception",
										JOptionPane.ERROR_MESSAGE);
							}
						}
					});
					asf.setVisible(true);
				}
			});
			ecsf.getSubmitButton().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					try {
						String[] textField = ecsf.getTextField();
						int selected_col = Integer.parseInt(textField[2]);
						Color calcolor;
						if (0 <= selected_col && selected_col <= 10) {
							calcolor = colors[selected_col];
						} else {
							calcolor = colors[11];
						}
						if (textField[0].length() != 0 && textField[1].length() == 0) {
							scheduler.add_calendar(textField[0], calcolor);
							calendarComboBox.addItem(textField[0]);
						} else if (textField[0].length() == 0 && textField[1].length() != 0) {
							scheduler.remove_calendar(textField[1]);
							calendarComboBox.removeItem(textField[1]);
						}
						ecsf.setVisible(false);
						ecsf.dispose();
					} catch (RuntimeException e1) {
						JOptionPane.showMessageDialog(ecsf, e1.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
					}
				}
			});
			ecsf.setVisible(true);
		}
	}

	class ShareButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			ShareFrame sf = new ShareFrame(scheduler, user_names, name);
			sf.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			sf.getSendBtn().addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					String[] selected = sf.getComboBox();
					Calendar cal = scheduler.get_calendar(selected[1]);
					Schedule sch = cal.get_Schedule(selected[2]);
					try {
						sender.sendSchedule(sch, selected[0]);
					} catch (Exception e2) {
					}

					sf.setVisible(false);
					sf.dispose();
				}
			});
			sf.setVisible(true);
		}
	}

	class ExportButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			OutputStream fos = null;
			try {
				fos = new FileOutputStream(calendarComboBox.getSelectedItem().toString() + "Calendar.txt");
			} catch (FileNotFoundException e1) {
			}
			OutputStreamWriter osw = new OutputStreamWriter(fos);
			BufferedWriter bw = new BufferedWriter(osw);

			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			ArrayList<Schedule> schedules = cal.get_Schedules();
			try {
				for (Schedule s : schedules) {
					bw.write(s.getName() + ",");
					if (s.getIsImportant()) {
						bw.write("True,");
					} else {
						bw.write("False,");
					}
					bw.write(s.getMemo() + ",");
					bw.write(s.getRepeatType() + ",");
					bw.write(s.getColor().getRGB() + ",");
					if (s instanceof FullDaySchedule) {
						LocalDate time = ((FullDaySchedule) s).getTime();
						int[] start = { time.getYear(), time.getMonthValue(), time.getDayOfMonth() };
						bw.write("FullDay,");
						for (int i = 0; i < 3; i++) {
							bw.write(start[i] + ",");
						}
					} else {
						LocalDateTime start = ((NormalSchedule) s).getStartTime();
						LocalDateTime end = ((NormalSchedule) s).getEndTime();
						int[] start_time = { start.getYear(), start.getMonthValue(), start.getDayOfMonth(),
								start.getHour(), start.getMinute() };
						int[] end_time = { end.getYear(), end.getMonthValue(), end.getDayOfMonth(), end.getHour(),
								end.getMinute() };
						bw.write("Normal" + ",");
						for (int i = 0; i < 5; i++) {
							bw.write(start_time[i] + ",");
							bw.write(end_time[i] + ",");
						}
						if (((NormalSchedule) s).getCanBeOverlapped()) {
							bw.write("True");
						} else {
							bw.write("False");
						}
					}
					bw.newLine();
				}
				bw.flush();
			} catch (Exception e2) {
			}
			try {
				bw.close(); osw.close(); fos.close();
			} catch (Exception e2) {
			}
		}
	}

	class ImportButtonClickListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			InputStream fis = null;
			try {
				fis = new FileInputStream(calendarComboBox.getSelectedItem().toString() + "Calendar.txt");
			} catch (FileNotFoundException e1) {
				return;
			}
        	InputStreamReader isr = new InputStreamReader(fis);
        	BufferedReader br = new BufferedReader(isr);

			Calendar cal = scheduler.get_calendar(calendarComboBox.getSelectedItem().toString());
			try {
				String s = new String();
        		while((s = br.readLine()) != null) {
					String[] input = s.split(",");
					String name = input[0];
					boolean isImp;
					if (input[1] == "True") {
						isImp = true;
					} else {
						isImp = false;
					}
					String memo = input[2];
					int RepeatType = Integer.parseInt(input[3]);
					Color color = new Color(Integer.parseInt(input[4]));
					int[] start = null;
					int[] end = null;
					boolean canBeOverlapped = false;
					if (input[5] == "FullDay") {
						start = new int[3];
						for (int i = 0; i < 3; i++) {
							start[i] = Integer.parseInt(input[6 + i]);
						}
					} else {
						start = new int[5];
						end = new int[5];
						for (int i = 0; i < 5; i++) {
							start[i] = Integer.parseInt(input[6 + 2 * i]);
							end[i] = Integer.parseInt(input[7 + 2 * i]);
						}
						if (input[16] == "True") {
							canBeOverlapped = true;
						} else {
							canBeOverlapped = false;
						}
					}
					try {
						if (input[5] == "FullDay") {
							cal.add_schedule(name, start, start, isImp, true, memo, RepeatType, true,
									color);
						} else {
							cal.add_schedule(name, start, end, isImp, canBeOverlapped, memo, RepeatType,
									false, color);
						}
					} catch (Exception e1) {
						JOptionPane.showMessageDialog(null, e1.getMessage(), "Exception", JOptionPane.ERROR_MESSAGE);
					}
        		}
			} catch (Exception e2) {
			}
			try {
				fis.close(); isr.close(); br.close();
			} catch (Exception e2) {
			}
			loadCalendar();
		}
	}

	public void loadCalendar() {
		int year = (int) yearSpinner.getValue(), month = (int) monthSpinner.getValue();
		String selectedItem = calendarComboBox.getSelectedItem().toString();
		if (selectedItem.equals("default")) {
			LocalDate date = LocalDate.of(year, month, 1);
			int start_pos = date.getDayOfWeek().getValue();

			for (JButton b : DayButton) {
				b.setText("");
			}

			start_pos %= 7;

			for (int i = 0; i < date.lengthOfMonth(); i++) {
				JButton button = DayButton.get(start_pos + i);
				button.setHorizontalAlignment(SwingConstants.LEFT);
				button.setVerticalAlignment(SwingConstants.TOP);
				ArrayList<Schedule> schedules = new ArrayList<Schedule>();
				for (int j = 0; j < scheduler.get_name().length; j++) {
					schedules.addAll(scheduler.get_calendar(scheduler.get_name()[j]).read_schedule(date.plusDays(i)));
				}
				if (schedules.size() == 0) {
					button.setText("<html><h2>" + (i + 1) + "</h2></html>");
				} else if (schedules.size() == 1) {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " thing</html>");
				} else {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " things</html>");
				}
			}
		} else {
			Calendar cal = scheduler.get_calendar(selectedItem);
			LocalDate date = LocalDate.of(year, month, 1);
			int start_pos = date.getDayOfWeek().getValue();

			for (JButton b : DayButton) {
				b.setText("");
			}

			start_pos %= 7;

			for (int i = 0; i < date.lengthOfMonth(); i++) {
				JButton button = DayButton.get(start_pos + i);
				button.setHorizontalAlignment(SwingConstants.LEFT);
				button.setVerticalAlignment(SwingConstants.TOP);
				ArrayList<Schedule> schedules = cal.read_schedule(date.plusDays(i));
				if (schedules.size() == 0) {
					button.setText("<html><h2>" + (i + 1) + "</h2></html>");
				} else if (schedules.size() == 1) {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " thing</html>");
				} else {
					button.setText("<html><h2>" + (i + 1) + "</h2><br>" + schedules.size() + " things</html>");
				}
			}
		}
	}

	public void read_name() {
		name = JOptionPane.showInputDialog("Enter your name.");
		if (name.length() == 0) {
			throw new NoNameEnteredException("No name entered.");
		}
	}

	class Sender {
		DataOutputStream dos;
		DataInputStream dis;

		public Sender() throws IOException {
			dos = new DataOutputStream(socket.getOutputStream());
			dis = new DataInputStream(socket.getInputStream());

			dos.writeUTF(name);
			dos.flush();
			boolean isValid = dis.readBoolean();
			if (!isValid) {
				throw new AlreadyDefinedException("User names cannot be duplicated.");
			}
		}

		public void sendSchedule(Schedule s, String name) {
			try {
				dos.writeUTF(name);
				dos.writeUTF(s.getName());
				dos.writeBoolean(s.getIsImportant());
				dos.writeUTF(s.getMemo());
				dos.writeInt(s.getRepeatType());
				dos.writeInt(s.getColor().getRGB());

				if (s instanceof FullDaySchedule) {
					LocalDate time = ((FullDaySchedule) s).getTime();
					int[] start = { time.getYear(), time.getMonthValue(), time.getDayOfMonth() };
					dos.writeBoolean(true);
					for (int i = 0; i < 3; i++) {
						dos.writeInt(start[i]);
					}
				} else {
					LocalDateTime start = ((NormalSchedule) s).getStartTime();
					LocalDateTime end = ((NormalSchedule) s).getEndTime();
					int[] start_time = { start.getYear(), start.getMonthValue(), start.getDayOfMonth(), start.getHour(),
							start.getMinute() };
					int[] end_time = { end.getYear(), end.getMonthValue(), end.getDayOfMonth(), end.getHour(),
							end.getMinute() };
					dos.writeBoolean(false);
					for (int i = 0; i < 5; i++) {
						dos.writeInt(start_time[i]);
						dos.writeInt(end_time[i]);
					}
					dos.writeBoolean(((NormalSchedule) s).getCanBeOverlapped());
				}
				dos.flush();
			} catch (Exception e) {

			}
		}
	}

	class ReceiveThread extends Thread {
		DataInputStream dis;
		DataOutputStream dos;
		Boolean isImp;
		String memo;
		int RepeatType;
		boolean isFullDay;
		int[] start, end;
		Color color;
		boolean canBeOverlapped;

		public ReceiveThread() {
			try {
				dis = new DataInputStream((socket.getInputStream()));
				dos = new DataOutputStream(socket.getOutputStream());
			} catch (IOException e) {
			}
		}

		public void run() {
			while (true) {
				try {
					int mode = dis.readInt();
					if (mode == 0) {
						int size = dis.readInt();
						user_names = new String[size];
						for (int i = 0; i < size; i++) {
							user_names[i] = dis.readUTF();
						}
					} else if (mode == 1) {
						name = dis.readUTF();
						isImp = dis.readBoolean();
						memo = dis.readUTF();
						RepeatType = dis.readInt();
						color = new Color(dis.readInt());
						isFullDay = dis.readBoolean();
						if (isFullDay) {
							start = new int[3];
							for (int i = 0; i < 3; i++) {
								start[i] = dis.readInt();
							}
						} else {
							start = new int[5];
							end = new int[5];
							for (int i = 0; i < 5; i++) {
								start[i] = dis.readInt();
								end[i] = dis.readInt();
							}
							canBeOverlapped = dis.readBoolean();
						}
						ReceiveFrame rf = new ReceiveFrame(scheduler.get_name());
						rf.setDefaultCloseOperation(2);
						rf.getAddBtn().addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent e) {
								String selected_name = rf.getCalComboBox();
								Calendar cal = scheduler.get_calendar(selected_name);
								try {
									if (isFullDay) {
										cal.add_schedule(name, start, start, isImp, true, memo, RepeatType, true,
												cal.getColor());
									} else {
										cal.add_schedule(name, start, end, isImp, canBeOverlapped, memo, RepeatType,
												false, cal.getColor());
									}
								} catch (Exception e1) {
									JOptionPane.showMessageDialog(rf, e1.getMessage(), "Exception",
											JOptionPane.ERROR_MESSAGE);
								}
								rf.setVisible(false);
								rf.dispose();
								loadCalendar();
							}
						});
						rf.setVisible(true);
					}
				} catch (Exception e) {
				}
			}
		}
	}
}
>>>>>>> eb8d12e5d375f0e3816d618468d42c1f2a30dda4
