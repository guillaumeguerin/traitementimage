package aquarium.items;

import java.awt.Image;
import java.awt.Toolkit;
import java.net.URL;

public class Seaweed extends AquariumItem {

	private static final int MIN_WIDTH = 30, MAX_WIDTH = 60;
	private static URL u = ClassLoader.getSystemResource("image/algue.png");
	private static Image seaweedImage = Toolkit.getDefaultToolkit().createImage(u);

	public Seaweed(int width) {
		super(width, seaweedImage);
	}

	static public int getMaxWidth() {
		return MAX_WIDTH;
	}
	
	static public int getMinWidth() {
		return MIN_WIDTH;
	}
}
