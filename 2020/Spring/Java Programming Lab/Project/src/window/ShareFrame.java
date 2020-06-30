package window;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import scheduler.Calendar;
import scheduler.Scheduler;

public class ShareFrame extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 5556452884840007947L;
	private Scheduler scheduler;
	private JComboBox<String> SchComboBox, CalComboBox, UserComboBox;
	private JButton SendBtn;
	
	public String[] getComboBox() {
		String[] result = {(String) UserComboBox.getSelectedItem(), (String) CalComboBox.getSelectedItem(), (String) SchComboBox.getSelectedItem()};
		return result;
	}

	public JButton getSendBtn() {
		return SendBtn;
	}

	public ShareFrame(Scheduler sch, String[] names, String name) {
		scheduler = sch;

		getContentPane().setLayout(new BorderLayout(0, 0));
		
		JPanel panel = new JPanel();
		getContentPane().add(panel, BorderLayout.NORTH);
		GridBagLayout gbl_panel = new GridBagLayout();
		gbl_panel.columnWidths = new int[]{70, 100, 150, 80};
		gbl_panel.rowHeights = new int[]{100, 100, 100};
		gbl_panel.columnWeights = new double[]{1.0, 0.0, 0.0};
		gbl_panel.rowWeights = new double[]{0.0, 0};
		panel.setLayout(gbl_panel);
		
		JLabel UserLabel = new JLabel("User");
		UserLabel.setHorizontalAlignment(SwingConstants.CENTER);
		UserLabel.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_UserLabel = new GridBagConstraints();
		gbc_UserLabel.insets = new Insets(0, 0, 5, 5);
		gbc_UserLabel.gridx = 1;
		gbc_UserLabel.gridy = 0;
		panel.add(UserLabel, gbc_UserLabel);
		
		UserComboBox = new JComboBox<String>(names);
		UserComboBox.removeItem(name);
		GridBagConstraints gbc_textField = new GridBagConstraints();
		gbc_textField.fill = GridBagConstraints.HORIZONTAL;
		gbc_textField.insets = new Insets(0, 0, 5, 5);
		gbc_textField.gridx = 2;
		gbc_textField.gridy = 0;
		panel.add(UserComboBox, gbc_textField);
		
		JLabel CalLabel = new JLabel("Calendar");
		CalLabel.setHorizontalAlignment(SwingConstants.CENTER);
		CalLabel.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_CalLabel = new GridBagConstraints();
		gbc_CalLabel.insets = new Insets(0, 0, 5, 5);
		gbc_CalLabel.gridx = 1;
		gbc_CalLabel.gridy = 1;
		panel.add(CalLabel, gbc_CalLabel);
		
		String[] cal_name = sch.get_name();
		CalComboBox = new JComboBox<String>(cal_name);
		GridBagConstraints gbc_CalComboBox = new GridBagConstraints();
		gbc_CalComboBox.insets = new Insets(0, 0, 5, 5);
		gbc_CalComboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_CalComboBox.gridx = 2;
		gbc_CalComboBox.gridy = 1;
		CalComboBox.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Calendar cal = scheduler.get_calendar((String) (CalComboBox.getSelectedItem()));
				String[] sch = cal.get_name();
				SchComboBox.removeAllItems();
				for (String n : sch) {
					SchComboBox.addItem(n);
				}
			}
		});
		panel.add(CalComboBox, gbc_CalComboBox);
		
		JLabel SchLabel = new JLabel("Schedule");
		SchLabel.setHorizontalAlignment(SwingConstants.CENTER);
		SchLabel.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_SchLabel = new GridBagConstraints();
		gbc_SchLabel.insets = new Insets(0, 0, 0, 5);
		gbc_SchLabel.gridx = 1;
		gbc_SchLabel.gridy = 2;
		panel.add(SchLabel, gbc_SchLabel);
		
		Calendar cal = sch.get_calendar((String) (CalComboBox.getSelectedItem()));
		String[] sch_name = cal.get_name();
		SchComboBox = new JComboBox<String>(sch_name);
		GridBagConstraints gbc_SchComboBox = new GridBagConstraints();
		gbc_SchComboBox.insets = new Insets(0, 0, 0, 5);
		gbc_SchComboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_SchComboBox.gridx = 2;
		gbc_SchComboBox.gridy = 2;
		panel.add(SchComboBox, gbc_SchComboBox);
		
		JPanel panel_1 = new JPanel();
		getContentPane().add(panel_1, BorderLayout.SOUTH);
		panel_1.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		SendBtn = new JButton("Send");
		SendBtn.setFont(new Font(null, Font.PLAIN, 18));
		panel_1.add(SendBtn);
		
		pack();
	}
}
