package window;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import exception.BothFieldEnteredException;

public class EditCalendarAndSchduleFrame extends JFrame {

	/**
	 *
	 */
	private static final long serialVersionUID = -2838724568182074884L;
	private JTextField AddCalendarField, DeleteCalendarField;
	private JButton AddScheduleButton, SubmitButton;
	private JComboBox<String> colorcomboBox;

	public JButton getAddScheduleButton() {
		return AddScheduleButton;
	}

	public JButton getSubmitButton() {
		return SubmitButton;
	}

	public String[] getTextField() {
		String[] text = new String[3];
		text[0] = AddCalendarField.getText();
		text[1] = DeleteCalendarField.getText();
		text[2] = Integer.toString(colorcomboBox.getSelectedIndex());
		if (text[0].length() != 0 && text[1].length() != 0) {
			throw new BothFieldEnteredException("You cannot add and delete at the same time.");
		}
		else {
			return text;
		}
	}

	/**
	 * Initialize the contents of the frame.
	 */
	public EditCalendarAndSchduleFrame() {
		BorderLayout borderLayout = (BorderLayout) getContentPane().getLayout();
		borderLayout.setVgap(20);
		setBounds(100, 100, 328, 273);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JPanel panel = new JPanel();
		getContentPane().add(panel, BorderLayout.NORTH);
		panel.setLayout(new BorderLayout(0, 10));
		
		JPanel panel_2 = new JPanel();
		panel.add(panel_2, BorderLayout.NORTH);
		panel_2.setLayout(new BorderLayout(0, 0));
		
		JPanel panel_7 = new JPanel();
		panel_2.add(panel_7, BorderLayout.NORTH);
		
		JLabel lblNewLabel_1 = new JLabel("Add Calendar");
		lblNewLabel_1.setPreferredSize(new Dimension(90, 15));
		panel_7.add(lblNewLabel_1);
		
		JPanel panel_8 = new JPanel();
		panel_2.add(panel_8, BorderLayout.SOUTH);
		panel_8.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		JLabel lblNewLabel_2 = new JLabel("Name");
		lblNewLabel_2.setPreferredSize(new Dimension(50, 15));
		panel_8.add(lblNewLabel_2);
		
		AddCalendarField = new JTextField();
		panel_8.add(AddCalendarField);
		AddCalendarField.setColumns(10);
		
		String colors[] = {"Magenta","Pink","Red","Orange", "Yellow", "Green","Cyan", "Blue","Light Gray","Gray","Dark Gray"};
		colorcomboBox = new JComboBox<String>(colors);
		panel_8.add(colorcomboBox);
		
		JPanel panel_3 = new JPanel();
		panel.add(panel_3, BorderLayout.CENTER);
		panel_3.setLayout(new BorderLayout(0, 0));
		
		JPanel panel_5 = new JPanel();
		panel_3.add(panel_5, BorderLayout.NORTH);
		
		JLabel lblNewLabel_3 = new JLabel("Delete Calendar");
		lblNewLabel_3.setPreferredSize(new Dimension(100, 15));
		panel_5.add(lblNewLabel_3);
		
		JPanel panel_6 = new JPanel();
		panel_3.add(panel_6, BorderLayout.SOUTH);
		
		JLabel lblNewLabel_4 = new JLabel("Name");
		lblNewLabel_4.setPreferredSize(new Dimension(50, 15));
		panel_6.add(lblNewLabel_4);
		
		DeleteCalendarField = new JTextField();
		panel_6.add(DeleteCalendarField);
		DeleteCalendarField.setColumns(10);
		
		JPanel panel_4 = new JPanel();
		panel.add(panel_4, BorderLayout.SOUTH);
		panel_4.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		JLabel lblNewLabel = new JLabel("Add Schedule");
		lblNewLabel.setHorizontalAlignment(SwingConstants.CENTER);
		lblNewLabel.setPreferredSize(new Dimension(90, 15));
		panel_4.add(lblNewLabel);
		
		AddScheduleButton = new JButton("Cilck");
		panel_4.add(AddScheduleButton);
		
		JPanel panel_1 = new JPanel();
		getContentPane().add(panel_1, BorderLayout.CENTER);
		
		SubmitButton = new JButton("Submit");
		panel_1.add(SubmitButton);
	}

}
