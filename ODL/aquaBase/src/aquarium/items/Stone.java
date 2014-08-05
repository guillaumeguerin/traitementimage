package aquarium.items;

import java.awt.Image;
import java.awt.Toolkit;
import java.net.URL;

public class Stone extends AquariumItem {

	private static final int MIN_WIDTH = 30, MAX_WIDTH = 60;
	private static URL u = ClassLoader.getSystemResource("image/pierre.png");
	private static Image stoneImage = Toolkit.getDefaultToolkit().createImage(u);

	public Stone(int width) {
		super(width, stoneImage);
	}

	static public int getMaxWidth() {
		return MAX_WIDTH;
	}

	static public int getMinWidth() {
		return MIN_WIDTH;
	}
	
}
