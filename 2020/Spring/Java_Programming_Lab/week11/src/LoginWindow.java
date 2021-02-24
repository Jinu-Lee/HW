import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class LoginWindow {
    public static void main(String[] args) {
        MyFrame mf = new MyFrame();
    }
}

class MyFrame extends JFrame {
    String id = "test";
    String password = "12345678";
    JTextField textField1;
    JPasswordField textField2;

    MyFrame() {
        setTitle("Login");

        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new GridLayout(3, 1));

        textField1 = new JTextField();
        textField2 = new JPasswordField();
        JButton button = new JButton("login");
        JLabel label1 = new JLabel("Id");
        JLabel label2 = new JLabel("Password");

        button.addActionListener(new ButtonClickListener());
        label1.setPreferredSize(new Dimension(80, 20));     
        label2.setPreferredSize(new Dimension(80, 20));   
        textField1.setPreferredSize(new Dimension(160, 20));
        textField2.setPreferredSize(new Dimension(160, 20));

        JPanel panel1 = new JPanel();
        JPanel panel2 = new JPanel();
        
        panel1.add(label1);
        panel1.add(textField1);
        panel2.add(label2);
        panel2.add(textField2);

        Container c = getContentPane();
        
        c.add(panel1);
        c.add(panel2);
        c.add(button);
        pack();
        
        setVisible(true);
    }

    class ButtonClickListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            String tempId = textField1.getText();
            String tempPassword = textField2.getText();

            if(tempId.equals(id) && tempPassword.contentEquals(password)) {
                JOptionPane.showMessageDialog(null, "Success");
            } else {
                JOptionPane.showMessageDialog(null, "Fail");
            }
        }
    }
}