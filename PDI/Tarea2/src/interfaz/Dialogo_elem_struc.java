/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interfaz;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.HashSet;
import java.util.Set;
import javax.swing.*;

/**
 *
 * @author Marvin
 */
public class Dialogo_elem_struc extends javax.swing.JDialog {

    /**
     * A return status code - returned if Cancel button has been pressed
     */
    public static final boolean RET_CANCEL = false;
    /**
     * A return status code - returned if OK button has been pressed
     */
    public static final boolean RET_OK = true;

    public enum Tam {

        M3, M5, M7
    };
    Tam matrix_select;

    /**
     * Creates new form Dialogo_log
     */
    public Dialogo_elem_struc(JFrame parent, boolean modal) {
        super(parent, modal);
        initComponents();
        setLocationRelativeTo(parent);
        // Close the dialog when Esc is pressed
        String cancelName = "cancel";
        InputMap inputMap = getRootPane().getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), cancelName);
        ActionMap actionMap = getRootPane().getActionMap();
        actionMap.put(cancelName, new AbstractAction() {

            public void actionPerformed(ActionEvent e) {
                doClose(RET_CANCEL);
            }
        });
    }

    /**
     * @return the return status of this dialog - one of RET_OK or RET_CANCEL
     */
    public boolean getReturnStatus() {
        return returnStatus;
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        okButton = new javax.swing.JButton();
        cancelButton = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        m00 = new javax.swing.JTextField();
        m10 = new javax.swing.JTextField();
        m20 = new javax.swing.JTextField();
        m21 = new javax.swing.JTextField();
        m11 = new javax.swing.JTextField();
        m01 = new javax.swing.JTextField();
        m02 = new javax.swing.JTextField();
        m12 = new javax.swing.JTextField();
        m22 = new javax.swing.JTextField();
        radio_cruz = new javax.swing.JRadioButton();
        radio_cuadrado = new javax.swing.JRadioButton();
        radio_esquina = new javax.swing.JRadioButton();
        radio_horizontal = new javax.swing.JRadioButton();
        radio_vertical = new javax.swing.JRadioButton();
        radio_diag_izq = new javax.swing.JRadioButton();
        radio_diag_der = new javax.swing.JRadioButton();
        radio_user = new javax.swing.JRadioButton();

        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                closeDialog(evt);
            }
        });

        okButton.setText("OK");
        okButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okButtonActionPerformed(evt);
            }
        });

        cancelButton.setText("Cancel");
        cancelButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cancelButtonActionPerformed(evt);
            }
        });

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel1.setText("Elemento estructurante");

        m00.setEditable(false);
        m00.setText("1");
        m00.setMaximumSize(new java.awt.Dimension(6, 20));

        m10.setEditable(false);
        m10.setText("1");
        m10.setMaximumSize(new java.awt.Dimension(6, 20));

        m20.setEditable(false);
        m20.setText("1");
        m20.setMaximumSize(new java.awt.Dimension(6, 20));

        m21.setEditable(false);
        m21.setText("1");
        m21.setMaximumSize(new java.awt.Dimension(6, 20));

        m11.setEditable(false);
        m11.setText("1");
        m11.setMaximumSize(new java.awt.Dimension(6, 20));

        m01.setEditable(false);
        m01.setText("1");
        m01.setMaximumSize(new java.awt.Dimension(6, 20));

        m02.setEditable(false);
        m02.setText("1");
        m02.setMaximumSize(new java.awt.Dimension(6, 20));

        m12.setEditable(false);
        m12.setText("1");
        m12.setMaximumSize(new java.awt.Dimension(6, 20));

        m22.setEditable(false);
        m22.setText("1");
        m22.setMaximumSize(new java.awt.Dimension(6, 20));

        buttonGroup1.add(radio_cruz);
        radio_cruz.setText("Cruz");
        radio_cruz.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_cruzActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_cuadrado);
        radio_cuadrado.setSelected(true);
        radio_cuadrado.setText("Cuadrado");
        radio_cuadrado.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_cuadradoActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_esquina);
        radio_esquina.setText("Esquinas");
        radio_esquina.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_esquinaActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_horizontal);
        radio_horizontal.setText("Horizontal");
        radio_horizontal.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_horizontalActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_vertical);
        radio_vertical.setText("Vertical");
        radio_vertical.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_verticalActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_diag_izq);
        radio_diag_izq.setText("Diagonal izquierda");
        radio_diag_izq.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_diag_izqActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_diag_der);
        radio_diag_der.setText("Diagonal derecha");
        radio_diag_der.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_diag_derActionPerformed(evt);
            }
        });

        buttonGroup1.add(radio_user);
        radio_user.setText("Otro");
        radio_user.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radio_userActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(okButton, javax.swing.GroupLayout.PREFERRED_SIZE, 67, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cancelButton))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(30, 30, 30)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(radio_cuadrado)
                                            .addComponent(radio_vertical)
                                            .addComponent(radio_diag_der)
                                            .addComponent(radio_esquina))
                                        .addGap(18, 18, 18)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(radio_cruz)
                                            .addComponent(radio_horizontal)
                                            .addComponent(radio_diag_izq)
                                            .addComponent(radio_user)))
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(34, 34, 34)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addGroup(layout.createSequentialGroup()
                                                .addComponent(m00, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m10, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m20, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                                            .addGroup(layout.createSequentialGroup()
                                                .addComponent(m01, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m11, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m21, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                                            .addGroup(layout.createSequentialGroup()
                                                .addComponent(m02, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m12, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(m22, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))))))
                            .addGroup(layout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 158, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {cancelButton, okButton});

        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 29, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(m00, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m10, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m20, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(m01, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m11, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m21, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(m02, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m12, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(m22, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(11, 11, 11)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(radio_cuadrado)
                    .addComponent(radio_cruz))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(radio_horizontal)
                    .addComponent(radio_vertical))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(radio_diag_izq)
                    .addComponent(radio_diag_der))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(radio_esquina)
                    .addComponent(radio_user))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 29, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(cancelButton)
                    .addComponent(okButton))
                .addContainerGap())
        );

        getRootPane().setDefaultButton(okButton);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void okButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_okButtonActionPerformed
        doClose(RET_OK);
    }//GEN-LAST:event_okButtonActionPerformed

    private void cancelButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cancelButtonActionPerformed
        doClose(RET_CANCEL);
    }//GEN-LAST:event_cancelButtonActionPerformed

    /**
     * Closes the dialog
     */
    private void closeDialog(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_closeDialog
        doClose(RET_CANCEL);
    }//GEN-LAST:event_closeDialog

    private void radio_cruzActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_cruzActionPerformed
        m00.setText("0");
        m10.setText("1");
        m20.setText("0");
        m01.setText("1");
        m11.setText("1");
        m21.setText("1");
        m02.setText("0");
        m12.setText("1");
        m22.setText("0");
        act(false);
    }//GEN-LAST:event_radio_cruzActionPerformed

    private void radio_cuadradoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_cuadradoActionPerformed
        m00.setText("1");
        m10.setText("1");
        m20.setText("1");
        m01.setText("1");
        m11.setText("1");
        m21.setText("1");
        m02.setText("1");
        m12.setText("1");
        m22.setText("1");
        act(false);
    }//GEN-LAST:event_radio_cuadradoActionPerformed

    private void radio_esquinaActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_esquinaActionPerformed
        m00.setText("1");
        m10.setText("0");
        m20.setText("1");
        m01.setText("0");
        m11.setText("1");
        m21.setText("0");
        m02.setText("1");
        m12.setText("0");
        m22.setText("1");
        act(false);
    }//GEN-LAST:event_radio_esquinaActionPerformed

    private void radio_horizontalActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_horizontalActionPerformed
        m00.setText("0");
        m10.setText("0");
        m20.setText("0");
        m01.setText("1");
        m11.setText("1");
        m21.setText("1");
        m02.setText("0");
        m12.setText("0");
        m22.setText("0");
        act(false);
    }//GEN-LAST:event_radio_horizontalActionPerformed

    private void radio_verticalActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_verticalActionPerformed
        m00.setText("0");
        m10.setText("1");
        m20.setText("0");
        m01.setText("0");
        m11.setText("1");
        m21.setText("0");
        m02.setText("0");
        m12.setText("1");
        m22.setText("0");
        act(false);
    }//GEN-LAST:event_radio_verticalActionPerformed

    private void radio_diag_izqActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_diag_izqActionPerformed
        m00.setText("0");
        m10.setText("0");
        m20.setText("1");
        m01.setText("0");
        m11.setText("1");
        m21.setText("0");
        m02.setText("1");
        m12.setText("0");
        m22.setText("0");
        act(false);
    }//GEN-LAST:event_radio_diag_izqActionPerformed

    private void radio_diag_derActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_diag_derActionPerformed
        m00.setText("1");
        m10.setText("0");
        m20.setText("0");
        m01.setText("0");
        m11.setText("1");
        m21.setText("0");
        m02.setText("0");
        m12.setText("0");
        m22.setText("1");
        act(false);
    }//GEN-LAST:event_radio_diag_derActionPerformed

    private void radio_userActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radio_userActionPerformed
        // TODO add your handling code here:
        act(true);
    }//GEN-LAST:event_radio_userActionPerformed

    private void act(boolean asd) {
        m00.setEditable(asd);
        m10.setEditable(asd);
        m20.setEditable(asd);
        m01.setEditable(asd);
        m11.setEditable(asd);
        m21.setEditable(asd);
        m02.setEditable(asd);
        m12.setEditable(asd);
        m22.setEditable(asd);
    }

    private void doClose(boolean retStatus) {
        returnStatus = retStatus;
        setVisible(false);
        dispose();
    }

    public double[][] getElemento() {
        try{
        double p1 = Double.parseDouble(m00.getText());
        double p2 = Double.parseDouble(m10.getText());
        double p3 = Double.parseDouble(m20.getText());
        double p4 = Double.parseDouble(m01.getText());
        double p5 = Double.parseDouble(m11.getText());
        double p6 = Double.parseDouble(m21.getText());
        double p7 = Double.parseDouble(m02.getText());
        double p8 = Double.parseDouble(m12.getText());
        double p9 = Double.parseDouble(m22.getText());
        double[][] m = {{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}};
        return m;
        }catch (NumberFormatException nfe){
            JOptionPane.showMessageDialog(this, "Debe llenar correctamente todos los cuadros");
        }
        return null;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /*
         * Set the Nimbus look and feel
         */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /*
         * If Nimbus (introduced in Java SE 6) is not available, stay with the
         * default look and feel. For details see
         * http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Dialogo_log.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /*
         * Create and display the dialog
         */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                Dialogo_elem_struc dialog = new Dialogo_elem_struc(new javax.swing.JFrame(), true);
                dialog.addWindowListener(new java.awt.event.WindowAdapter() {

                    @Override
                    public void windowClosing(java.awt.event.WindowEvent e) {
                        System.exit(0);
                    }
                });
                dialog.setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.JButton cancelButton;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JTextField m00;
    private javax.swing.JTextField m01;
    private javax.swing.JTextField m02;
    private javax.swing.JTextField m10;
    private javax.swing.JTextField m11;
    private javax.swing.JTextField m12;
    private javax.swing.JTextField m20;
    private javax.swing.JTextField m21;
    private javax.swing.JTextField m22;
    private javax.swing.JButton okButton;
    private javax.swing.JRadioButton radio_cruz;
    private javax.swing.JRadioButton radio_cuadrado;
    private javax.swing.JRadioButton radio_diag_der;
    private javax.swing.JRadioButton radio_diag_izq;
    private javax.swing.JRadioButton radio_esquina;
    private javax.swing.JRadioButton radio_horizontal;
    private javax.swing.JRadioButton radio_user;
    private javax.swing.JRadioButton radio_vertical;
    // End of variables declaration//GEN-END:variables
    private boolean returnStatus = RET_CANCEL;
}
