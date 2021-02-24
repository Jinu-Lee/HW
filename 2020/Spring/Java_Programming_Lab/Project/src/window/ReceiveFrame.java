package window;

import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.SwingConstants;

public class ReceiveFrame extends JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2108844096560396184L;
	private JButton AddBtn, CancelBtn;
	private JComboBox<String> CalComboBox;
	
	public JButton getAddBtn() {
		return AddBtn;
	}
	
	public JButton getCancelBtn() {
		return CancelBtn;
	}

	public String getCalComboBox() {
		return (String) CalComboBox.getSelectedItem();
	}

	public ReceiveFrame(String[] name) {
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[]{100, 200};
		gridBagLayout.rowHeights = new int[]{100, 50};
		gridBagLayout.columnWeights = new double[]{0.0};
		gridBagLayout.rowWeights = new double[]{0};
		getContentPane().setLayout(gridBagLayout);
		
		JLabel CalendarLbl = new JLabel("Calendar");
		CalendarLbl.setFont(new Font(null, Font.PLAIN, 18));
		CalendarLbl.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_CalendarLbl = new GridBagConstraints();
		gbc_CalendarLbl.insets = new Insets(0, 0, 5, 5);
		gbc_CalendarLbl.gridx = 0;
		gbc_CalendarLbl.gridy = 0;
		getContentPane().add(CalendarLbl, gbc_CalendarLbl);
		
		CalComboBox = new JComboBox<String>(name);
		GridBagConstraints gbc_CalComboBox = new GridBagConstraints();
		gbc_CalComboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_CalComboBox.insets = new Insets(0, 0, 5, 0);
		gbc_CalComboBox.gridx = 1;
		gbc_CalComboBox.gridy = 0;
		getContentPane().add(CalComboBox, gbc_CalComboBox);
		
		AddBtn = new JButton("Add");
		AddBtn.setFont(new Font(null, Font.PLAIN, 18));
		GridBagConstraints gbc_AddBtn = new GridBagConstraints();
		gbc_AddBtn.insets = new Insets(0, 0, 5, 0);
		gbc_AddBtn.anchor = GridBagConstraints.WEST;
		gbc_AddBtn.gridx = 1;
		gbc_AddBtn.gridy = 1;
		getContentPane().add(AddBtn, gbc_AddBtn);

		pack();
	}

}
