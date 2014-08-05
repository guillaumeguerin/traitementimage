package aquarium.util;

import java.awt.Point;

import aquarium.gui.Aquarium;

public class RandomNumber 
{
	public static int randomValue(int min, int max) {
		return ((int)(Math.random() * (max - min + 1)) + min);
	}
	
	public static Point randomPoint(int min_x, int max_x, int min_y, int max_y) {
		return new Point(randomValue(min_x, max_x), randomValue(min_y, max_y));
	}
	
	public static Point RandomDest(){
		return RandomNumber.randomPoint( 0, Aquarium.SIZE_AQUA_X, 0, Aquarium.SIZE_AQUA_Y);
	}
}
