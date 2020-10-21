import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ErrorMessage extends JDialog {
    private JPanel contentPane;
    public JButton buttonOK;
    public JLabel label;

    public ErrorMessage(String message) {
        label.setText(message);
        setContentPane(contentPane);
        setModal(true);
        getRootPane().setDefaultButton(buttonOK);
        setSize(200, 100);
        setLocationRelativeTo(null);

        buttonOK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onOK();
            }
        });
    }

    private void onOK() {
        setVisible(false);
        dispose();
    }

}
