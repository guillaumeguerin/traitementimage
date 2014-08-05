package aquarium.gui;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

/**
 * @author ccassagn
 */
public class Animation extends JFrame {

	private static final long serialVersionUID = 1L;
	private Aquarium aquarium = null;
	private final int MARGE = 10;
	
	public Animation(Aquarium aquarium) {
		this.aquarium = aquarium;
	}

	/**
	 * La fonction d'affichage utilisée dans start
	 */
	public void afficher(boolean server) {
		add(aquarium);
		// size
		setSize(aquarium.getSizeX()+MARGE, aquarium.getSizeY()+MARGE);
		
		// Optional: Ask for window decorations provided by the look and feel.
		setDefaultLookAndFeelDecorated(true);
		
		//2. Optional: What happens when the frame closes?
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		if(server)
			setTitle("Server");
		else
			setTitle("Client");
		
		//public abstract class WindowAdapter
		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				//super.windowClosing(e);
				int reponse = JOptionPane.showConfirmDialog(Animation.this, "êtes-vous sûr de vouloir quitter ?", "aquarium online", JOptionPane.YES_NO_OPTION);
				if (reponse == JOptionPane.OK_OPTION) {
					Animation.this.dispose();
					System.exit(0);
				}
			}
		});		
		
		//4. Size the frame.
		//pack();
		//5. Show it.
		setVisible(true);

		while(true) {
			aquarium.go();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {};
		}
	}
}
